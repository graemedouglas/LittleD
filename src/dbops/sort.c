/******************************************************************************/
/**
@file		sort.c
@author		Graeme Douglas
@brief		The implementation for the sort operator.
@see		For more information, see @ref sort.h.
@details
@copyright	Copyright 2013 Graeme Douglas
@license	Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at
			http://www.apache.org/licenses/LICENSE-2.0

@par
		Unless required by applicable law or agreed to in writing,
		software distributed under the License is distributed on an
		"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
		either express or implied. See the License for the specific
		language governing permissions and limitations under the
		License.
*/
/******************************************************************************/

#include "sort.h"
#include "db_ops.h"
#include "../dblogic/compare_tuple.h"

/* Initialize the sort operator. */
db_int init_sort(sort_t *sp, db_op_base_t *child, db_eet_t *sort_exprs,
		db_uint8 num_expr, db_uint8 *order, db_query_mm_t *mmp)
{
	sp->base.type = DB_SORT;
	sp->base.header = child->header;
	
	/* Allocate memory for the previous tuple. */
	sp->previous_tp = NULL;
	
	sp->next_count = 0;
	sp->child = child;
	sp->sort_exprs = sort_exprs;
	sp->num_expr = num_expr;
	sp->order = order;
	return 1;
}

/* Rewind the sort operator. */
db_int rewind_sort(sort_t *sp, db_query_mm_t *mmp)
{
	switch (rewind_dbop(sp->child, mmp))
	{
		case 1:
			if (sp->previous_tp != NULL)
			{
				close_tuple(sp->previous_tp, mmp);
				DB_QMM_BFREE(mmp, sp->previous_tp);
			}
			sp->previous_tp = NULL;
			return 1;
		case 0:
			return 0;
		default:
			return -1;	/* Error out. */
	}
}

/* Return the next tuple from the sort operator. */
db_int next_sort(sort_t *sp, tuple_t *tp, db_query_mm_t *mmp)
{
	if (sp == NULL)
		return -1;
	if (tp == NULL)
		return -1;
	if (tp->bytes == NULL)
		return -1;
	
	db_int i;
	if (sp->next_count > 0)
	{
		if (sp->previous_tp == NULL)
			return -1;
		if (sp->previous_tp->bytes == NULL)
			return -1;
		
		/* Copy out previous's bytes to next. */
		for (i = 0; i < (db_int)(sp->base.header->tuple_size); ++i)
		{
			tp->bytes[i] = sp->previous_tp->bytes[i];
		}
		for (i = 0; i < (db_int)(sp->base.header->num_attr); ++i)
		{
			tp->isnull[i/8] = sp->previous_tp->isnull[i/8];
		}
		sp->next_count--;
		return 1;
	}
	switch (next_inorder_tuple(tp, sp->previous_tp, &(sp->next_count),
		sp->child, (sp->sort_exprs), sp->num_expr, sp->order, mmp))
	{
		case 1:
			if (sp->previous_tp == NULL)
			{
				sp->previous_tp = DB_QMM_BALLOC(mmp, sizeof(tuple_t));
				init_tuple(sp->previous_tp,
					sp->base.header->tuple_size,
					sp->base.header->num_attr, mmp);
			}
			/* Decrement the count by 1. */
			(sp->next_count)--;
			/* Copy out next's bytes to previous. */
			for (i = 0; i < (db_int)sp->base.header->tuple_size; ++i)
			{
				sp->previous_tp->bytes[i] = tp->bytes[i];
			}
			for (i = 0; i < (db_int)(sp->base.header->num_attr); ++i)
			{
				sp->previous_tp->isnull[i/8] = tp->isnull[i/8];
			}
			return 1;
		case 0:
			/* Decrement the count by 1. */
			(sp->next_count)--;
			return 0;
		default:
			/* Decrement the count by 1. */
			(sp->next_count)--;
			return -1;	/* Error out. */
	}
}

/* Close the sort operator. */
db_int close_sort(sort_t *sp, db_query_mm_t *mmp)
{
	/* Free memory used by previous tuple, if it was initialized. */
	if (sp->previous_tp != NULL)
	{
		close_tuple(sp->previous_tp, mmp);
		DB_QMM_BFREE(mmp, sp->previous_tp);
	}
	return 1;
}
