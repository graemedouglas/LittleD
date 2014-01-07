/*****
******************************************************************************
Author: Graeme Douglas

Implementation of db_ops.h interface.  See db_ops.h for documentation,
notes.
******************************************************************************
*/

#include "db_ops.h"
#include "../db_ctconf.h"

db_int8 findindexon(scan_t *sp, db_eetnode_attr_t *attrp)
{
	db_int8 i = 0;
	db_uint8 found = 1;
	for (; i < sp->idx_meta_data.num_idx; ++i)
	{
		if (sizeof(db_eetnode_attr_t) != sp->idx_meta_data.exprs[i]->size)
			continue;
		
		// FIXME: This only works for attribute to attribute indexing.
		if (attrp->pos != ((db_eetnode_attr_t*)(sp->idx_meta_data.exprs[i]->nodes))->pos)
		{
			found = 0;
		}
		if (found)
			return i;
		else
			found = 1;
	}
	
	return -1;
}

/* A generic next method that can be called on any operator. */
db_int next(db_op_base_t *op, db_tuple_t *next_tp, db_query_mm_t *mmp)
{
	if (op->type == DB_SCAN)
	{
		return next_scan((scan_t*)op, next_tp, mmp);
	}
	else if (op->type == DB_PROJECT)
	{
		return next_project((project_t*)op, next_tp, mmp);
	}
	else if (op->type == DB_SELECT)
	{
		return next_select((select_t*)op, next_tp, mmp);
	}
	else if (op->type == DB_NTJOIN)
	{
		return next_ntjoin((ntjoin_t*)op, next_tp, mmp);
	}
	else if (op->type == DB_OSIJOIN)
	{
		return next_osijoin((osijoin_t*)op, next_tp, mmp);
	}
	else if (op->type == DB_SORT)
	{
		return next_sort((sort_t*)op, next_tp, mmp);
	}
#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1
	else if (op->type == DB_AGGREGATE)
	{
		return next_aggregate((aggregate_t*)op, next_tp, mmp);
	}
#endif
#endif
	else
		return -1;
}

/* A generic rewind method. */
db_int rewind_dbop(db_op_base_t *op, db_query_mm_t *mmp)
{
	if (op->type == DB_SCAN)
	{
		return rewind_scan((scan_t*)op, mmp);
	}
	else if (op->type == DB_PROJECT)
	{
		return rewind_project((project_t*)op, mmp);
	}
	else if (op->type == DB_SELECT)
	{
		return rewind_select((select_t*)op, mmp);
	}
	else if (op->type == DB_NTJOIN)
	{
		return rewind_ntjoin((ntjoin_t*)op, mmp);
	}
	else if (op->type == DB_OSIJOIN)
	{
		return rewind_osijoin((osijoin_t*)op, mmp);
	}
	else if (op->type == DB_SORT)
	{
		return rewind_sort((sort_t*)op, mmp);
	}
#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1
	else if (op->type == DB_AGGREGATE)
	{
		return rewind_aggregate((aggregate_t*)op, mmp);
	}
#endif
#endif
	else
		return -1;
}

/* A generic close method. */
void close(db_op_base_t *op, db_query_mm_t *mmp)
{
	/* TODO In the future, some sort of check on types would be good. */
	if (op->type == DB_SCAN)
	{
		close_scan((scan_t*)op, mmp);
	}
	else if (op->type == DB_PROJECT)
	{
		close_project((project_t*)op, mmp);
	}
	else if (op->type == DB_SELECT)
	{
		close_select((select_t*)op, mmp);
	}
	else if (op->type == DB_NTJOIN)
	{
		close_ntjoin((ntjoin_t*)op, mmp);
	}
	else if (op->type == DB_OSIJOIN)
	{
		close_osijoin((osijoin_t*)op, mmp);
	}
	else if (op->type == DB_SORT)
	{
		close_sort((sort_t*)op, mmp);
	}
#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1
	else if (op->type == DB_AGGREGATE)
	{
		close_aggregate((aggregate_t*)op, mmp);
	}
#endif
#endif
}

/* Get the number of childrem an operator has. */
db_int numopchildren(db_op_base_t *op)
{
	if (op->type == DB_SCAN)
	{
		return 0;
	}
	else if (DB_PROJECT == op->type ||
	         DB_SELECT == op->type ||
	         DB_SORT == op->type ||
	         DB_AGGREGATE == op->type)
	{
		return 1;
	}
	else if (DB_NTJOIN == op->type || DB_OSIJOIN == op->type)
	{
		return 2;
	}
	else
	{
		return -1;
	}
}

/* Close an entire execution tree recursively. */
db_int closeexecutiontree(db_op_base_t *op, db_query_mm_t *mmp)
{
	if (NULL == op)
	{
		return 1;
	}
	else if (DB_NTJOIN == op->type || DB_OSIJOIN == op->type)
	{
		switch (closeexecutiontree(((ntjoin_t*)op)->lchild, mmp)) {
		case 1:
			break;
		default:
			return -1;
		}
		switch (closeexecutiontree(((ntjoin_t*)op)->rchild, mmp)) {
		case 1:
			break;
		default:
			return -1;
		}
	}
	else if (1 == numopchildren(op))
	{
		switch (closeexecutiontree(((db_op_onechild_t*)op)->child, mmp)) {
		case 1:
			break;
		default:
			return -1;
		}
	}
	else if (0 == numopchildren(op))
	{}
	else
	{
		return -1;
	}
	close(op, mmp);
	return 1;
}
