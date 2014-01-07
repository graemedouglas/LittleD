/******************************************************************************/
/**
@file		aggregate.c
@author		Graeme Douglas
@brief		Implementation for the relational aggregate operator.
@see		For external function definitions, reference header
		@ref aggregate.h.
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

#include "aggregate.h"
#include "../db_ctconf.h"

#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1

/* Initialize the aggregate operator. */
db_int init_aggregate(aggregate_t *ap, db_op_base_t *child, db_eet_t *exprs,
		db_uint8 num_expr, db_eet_t *groupby_exprs,
		db_uint8 num_groupby_expr, db_eet_t *having_expr,
		db_query_mm_t *mmp)
{
	db_int i, j;
	
	/** Determine the aggregate locations, build array. **/
	/* Count the number of aggregates in each exprs[i]. */
	db_uint8 num_aggr[(db_int)num_expr];
	for (i = 0; i < (db_int)num_expr; ++i)
	{
		num_aggr[i] = 0;
		db_eetnode_t *np = exprs[i].nodes;
		while ((db_uint8)DB_EETNODE_COUNT != np->type)
		{
			if ((db_uint8)(DB_EETNODE_AGGR_TEMP) == np->type)
			{
				num_aggr[i]++;
			}
			advanceeetnodepointer(&np, 1);
		}
	}
	/* Build aggr_locs array. */
	ap->aggr_locs = DB_QMM_BALLOC(mmp, (db_int)num_expr * sizeof(db_uint8*));
	for (i = 0; i < (db_int)num_expr; ++i)
	{
		if (num_aggr[i] == 0)
		{
			ap->aggr_locs[i] = NULL;
		}
		else
		{
			j = 0;
			db_int pos = 0;
			ap->aggr_locs[i] = DB_QMM_BALLOC(mmp, (db_int)(num_aggr[i]) + 1);
			db_eetnode_t *np = exprs[i].nodes;
			while ((db_uint8)DB_EETNODE_COUNT != np->type)
			{
				if ((db_uint8)DB_EETNODE_AGGR_TEMP == np->type)
				{
					ap->aggr_locs[i][j] = pos;
					j++;
				}
				advanceeetnodepointer(&np, 1);
				pos++;
			}
			ap->aggr_locs[i][j] = -1;	/* The end marker. */	//TODO: This is clearly broken for uint8.
		}
	}
	
	/* We will pre-compute types and jump_p's for all of the aggregate sub expressions.
	*/
	for (i = 0; i < (db_int)num_expr; ++i)
	{
		for (j = 0;; ++j)
		{
			if (ap->aggr_locs[i] != NULL && ap->aggr_locs[i][j] > -1)
			{
				db_eetnode_t *np = geteetnodebypos(&exprs[i], (db_int)(ap->aggr_locs[i][j]));
				if (np->type != (db_uint8)DB_EETNODE_AGGR_TEMP)
				{
					return -1;
				}
				np = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)np)+1));
				if (1 != subexpr_type_addr(np, &(((((db_eetnode_aggr_temp_t*)np)-1))->subexpr_type),
							&((((db_eetnode_aggr_temp_t*)np)-1)->jump_p),
							(exprs[i].size / sizeof(db_eetnode_t*)) + 1,
							&(child->header), mmp))
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}
	}

	ap->base.type = DB_AGGREGATE;
	ap->base.header = DB_QMM_BALLOC(mmp, sizeof(relation_header_t));
	switch (createnewheader(ap->base.header,
		child->header, exprs, num_expr, mmp))
	{
		case 0:
			return 0;
		case 1:
			break;
		default:
			return -1;

	}
	ap->previous_tp = NULL;
	
	ap->next_count = 0;
	
	ap->child = child;
	ap->exprs = exprs;
	ap->num_expr = num_expr;
	ap->groupby_exprs = groupby_exprs;
	ap->num_groupby_expr = num_groupby_expr;
	ap->having_expr = having_expr;
	
	return 1;
}

/* Rewind the aggregate operator. */
db_int rewind_aggregate(aggregate_t *ap, db_query_mm_t *mmp)
{
	switch (rewind_dbop(ap->child, mmp))
	{
		case 1:
			if (ap->previous_tp != NULL)
			{
				close_tuple(ap->previous_tp, mmp);
				DB_QMM_BFREE(mmp, ap->previous_tp);
			}
			ap->previous_tp = NULL;
			ap->next_count = 0;
			return 1;
		case 0:
			return 0;
		default:
			return -1;	/* Error out. */
	}
}

/* Prepares expressions to be aggregated over, and evaluate all those
   expressions that do not have aggregates in them. It might seem strange
   that we are not just passing in the whole aggregate pointer only,
   but here we are attempting to abstract the process for both
   projecting expressions and HAVING clauses. */
db_int aggr_prepare_expressions(aggregate_t *ap, db_eet_t *exprs, db_int8 **aggr_locs, db_uint8 num_expr, db_tuple_t *src_tp, db_tuple_t *tp, db_query_mm_t *mmp)
{
	db_int i, j;
	/* Evaluate non-aggregate values and setup temporary values for aggregate functions. */
	for (i = 0; i < (db_int)num_expr; ++i)
	{
		for (j = 0;; ++j)
		{
			if (i < 0 || i > (db_int)num_expr || (aggr_locs[i] != NULL && aggr_locs[i][j] < -1))
			{
				return -1;
			}
			/* If there isn't an aggregate in this expression... */
			else if (NULL == aggr_locs[i])
			{
				db_uint8 node_type = exprs[i].nodes->type;
				if ((node_type == DB_EETNODE_ATTR && gettypebypos(ap->child->header,
						((db_eetnode_attr_t*)(exprs[i].nodes))->pos) == DB_INT) ||
					 node_type == DB_EETNODE_CONST_DBINT ||
					  (node_type >= DB_EETNODE_OP_NOT &&
					   node_type <= DB_EETNODE_FUNC_LENGTH_DBSTRING))
				{
					db_int result;
					switch (evaluate_eet(&(exprs[i]), &result, &src_tp, &(ap->child->header), 0, mmp))
					{
						case 1:
							/* Make sure this attributes isnull bit is 0. */
							tp->isnull[(i / 8)] &= ~(1 << (i % 8));
							*((db_int*)(&(tp->bytes[(db_int)(ap->base.header->offsets[i])]))) = result;
							break;
						case 2:
							/* Set this attributes isnull bit to 1. */
							tp->isnull[(i / 8)] |= (1 << (i % 8));
							break;
						case -1:
						default:
							return -1;
							// TODO: Might need to do more things here to prevent memory leaks?!?
					}
				}
				else if ((node_type == DB_EETNODE_ATTR && gettypebypos(ap->child->header, 
						((db_eetnode_attr_t*)(exprs[i].nodes))->pos) == DB_STRING) ||
					  node_type == DB_EETNODE_CONST_DBSTRING)
				{
					char *result;
					db_int retval = evaluate_eet(&(exprs[i]), &result, &src_tp, &(ap->child->header), 0, mmp);
					
					if (2 != retval && -1 != retval)
					{
						/* Copy out string to tuple. */
						db_int j, min;
						if (strlen(result) < (db_int)(ap->base.header->sizes[i]))
							min = strlen(result);
						else
							min = (db_int)(ap->base.header->sizes[i]);
						for (j = 0; j < min; ++j)
						{
							tp->bytes[(db_int)(ap->base.header->offsets[i]) + j] = result[j];
						}
						for (; j < (db_int)ap->base.header->sizes[i]; j++)
							tp->bytes[(db_int)(ap->base.header->offsets[i]) + j] = '\0';
						
						/* Make sure this attributes isnull bit is 0. */
						tp->isnull[(i / 8)] &= ~(1 << (i % 8));
					}
					else if (2 == retval)
					{
						/* Set this tuples attribute null bit to 1. */
						tp->isnull[i / 8] |= (1 << (i % 8));
					}
					else
					{
						// TODO: Gracefully exit.
					}
				}
				/* Break out of j-based loop. */
				break;
			}
			else if (aggr_locs[i][j] == -1)
			{
				/* Break out of j-based loop. */
				break;
			}
			/* If there is an aggregate in this expression... */
			else
			{
				if (DB_EETNODE_AGGR_TEMP != geteetnodetypebypos(&(exprs[i]), (db_int)(aggr_locs[i][j])))
				{
					/* Location of aggregate function does match
					   actual location. */
					db_int k, l;
					for (k = 0; k < i; ++k)
					{
						for (l = 0; ; ++l)
						{
							if (aggr_locs[k] != NULL)
							{
								if ((k < i || (k == i && l < j)) && aggr_locs[k][l] > -1)
								{
									DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(exprs[k]),
										(db_int)(aggr_locs[k][l])))->value_p);
									((db_eetnode_aggr_temp_t*)geteetnodebypos(&(exprs[k]),
										(db_int)(aggr_locs[k][l])))->value_p = NULL;
								}
								else
								{
									break;
								}
							}
						}
					}
					return -1;
				}
				
				db_eetnode_aggr_temp_t *aggr_np = (db_eetnode_aggr_temp_t*)geteetnodebypos(&(exprs[i]), (db_int)aggr_locs[i][j]);
				/* All aggregates are assumed to result
				   result in NULL until we know
				   otherwise. */
				aggr_np->aggr_isnull = 1;
				/* Do not need to check if aggr_type >= 0 since always positive. */
				if (aggr_np->aggr_type <= DB_AGGR_LAST)
				{
					if ((db_uint8)DB_EETNODE_CONST_DBINT == aggr_np->subexpr_type)
					{
						aggr_np->value_p = DB_QMM_BALLOC(mmp, sizeof(db_int));
						*((db_int*)(aggr_np->value_p)) = 0;
						
						/* Special cases for initial value. */
						if (aggr_np->aggr_type == DB_AGGR_MAX)
						{
							*((db_int*)(aggr_np->value_p)) = DB_INT_MIN;
						}
						else if (aggr_np->aggr_type == DB_AGGR_MIN || aggr_np->aggr_type == DB_AGGR_BAND)
						{
							*((db_int*)(aggr_np->value_p)) = DB_INT_MAX;
						}
					}
				}
				else
				{
					/* TODO: DO SOMETHING HERE! */
				}
				/* TODO: Implement future types here. */
			}
		}
	}
	return 1;
}

/* Return the next tuple from the aggregate operator. */
db_int next_aggregate(aggregate_t *ap, db_tuple_t *tp, db_query_mm_t *mmp)
{
	if (ap == NULL)
		return -1;
	if (tp == NULL)
		return -1;
	if (tp->bytes == NULL)
		return -1;
	
	db_int i;
	db_uint8 orderings[(db_int)(ap->num_groupby_expr)];
	for (i = 0; i < (db_int)(ap->num_groupby_expr); ++i)
		orderings[i] = (db_uint8)DB_TUPLE_ORDER_ASC;
	
	//printf("next_count: %d\n", ap->next_count);
	/* If the next_count is less than 0, we know there are no more tuples
	   from the child operator.  This means we have no moer tuples to give.
	*/
	if (ap->next_count < 0)
	{
		return 0;
	}
	
	/* Create a temporary tuple for to store child's next tuple in. */
	db_tuple_t src_t;
	init_tuple(&src_t, ap->child->header->tuple_size, ap->child->header->num_attr, mmp);
	
	/* Initially, mark all attributes as NULL in tuple to return.  This is
	   to accomodate for the way most RDBMS's work with aggregation.  That
	   is, NULLs are neglected when they are grouped with actual values,
	   but are otherwise returned.  In other words:
	   
	    sum(NULL, 1) = 1
	    sum(NULL) = NULL
	   
	   Got it? ;)
	*/
	for (i = 0; i < ((db_int)(ap->base.header->num_attr)); ++i)
	{
		// TODO: This works, but its pretty darn ugly.  There is definitely a better approach to this.
		tp->isnull[i/8] |= (1 << (i % 8));
	}
	
	db_int looper = 0;
	/* Compute next tuple to return. */
	do {
		//printf("looper: %d\n", looper);
		/* Note: looper is guaranteed to be 0 on the first iteration
		         and one 1 on every other iteration. */
		/* Copy out previous's bytes to src_t. */
		if (ap->previous_tp != NULL && looper == 0)
		{
			for (i = 0; i < (db_int)ap->child->header->tuple_size; ++i)
			{
				src_t.bytes[i] = ap->previous_tp->bytes[i];
			}
			// TODO: This works, but its pretty darn ugly.  There is definitely a better approach to this.
			for (i = 0; i < (db_int)ap->child->header->num_attr; ++i)
				src_t.isnull[i / 8] = ap->previous_tp->isnull[i / 8];
		}
		if (ap->previous_tp == NULL || looper == 0)
		{
			/* Pull in the first value. */
			switch (next_inorder_tuple(&src_t, ap->previous_tp, &(ap->next_count),
				ap->child, (ap->groupby_exprs), ap->num_groupby_expr, orderings, mmp))
			{
				case -1:
					close_tuple(&src_t, mmp);
					return -1;
				case 0:
					close_tuple(&src_t, mmp);
					return 0;
				case 1:
					/* Keep going, there is work to do! */
					break;
			}
			if (ap->previous_tp != NULL)
			{
				for (i = 0; i < (db_int)ap->child->header->tuple_size; ++i)
				{
					ap->previous_tp->bytes[i] = src_t.bytes[i];
				}
				// TODO: This works, but its pretty darn ugly.  There is definitely a better approach to this.
				for (i = 0; i < (db_int)ap->child->header->num_attr; ++i)
					ap->previous_tp->isnull[i / 8] = src_t.isnull[i / 8];
			}
		}
		else if (1 == looper)
		{
			/* Copy src_t's bytes into previous_tp's.*/
			for (i = 0; i < (db_int)ap->child->header->tuple_size; ++i)
			{
				ap->previous_tp->bytes[i] = src_t.bytes[i];
			}
			// TODO: This works, but its pretty darn ugly.  There is definitely a better approach to this.
			for (i = 0; i < (db_int)ap->child->header->num_attr; ++i)
				ap->previous_tp->isnull[i / 8] = src_t.isnull[i / 8];
		}
		//printf("First db_int in src_t: %d\n", getintbypos(&src_t, 0, ap->child->header));
		
		/* Evaluate non-aggregate values and setup temporary values for aggregate functions. */
		// TODO: Do something similar for HAVING clause only when ap->having_aggr_locs == NULL.
		if (1 == aggr_prepare_expressions(ap, ap->exprs, ap->aggr_locs, ap->num_expr, &src_t, tp, mmp))
		{
			/* Continue on. */
		}
		else
		{
			close_tuple(&src_t, mmp);
			return -1;
		}
		/* Abstraction point number 2: aggregation.  Only needs to be called on HAVING clause if it has aggregates. */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* While the previous tuple shares the same groupby expression values
		 * as returned tuple. */
		while (ap->next_count >= 0 && (ap->previous_tp == NULL ||
		       (0 == cmp_tuple(ap->previous_tp, &src_t, ap->child->header, ap->child->header,
				ap->groupby_exprs, ap->groupby_exprs, ap->num_groupby_expr, orderings, 1, mmp))))
		{
			/* For each attribute... */
			for (i = 0; i < ap->num_expr; i++)
			{
				if (ap->aggr_locs[i] != NULL)
				{
					db_int j = 0;
					for (; ap->aggr_locs[i][j] > -1; ++j)
					{
						void *temp_p = NULL;
						
						/* Get the type of the expression just after the aggregate, malloc appropriately. */
						db_eetnode_aggr_temp_t *aggr_np = (db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[i]), (db_int)(ap->aggr_locs[i][j]));
						if ((db_uint8)DB_EETNODE_CONST_DBINT == aggr_np->subexpr_type)
						{
							temp_p = DB_QMM_BALLOC(mmp, sizeof(db_int));
						}
						else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == aggr_np->subexpr_type)
						{
							/* Do nothing, we will just use pointer instead. */
						}
						
						//printf("starting eet evaluation from: %d\n", (db_int)(ap->aggr_locs[i][j])+1);
						db_tuple_t *src_tp = &src_t;
						db_int retval = evaluate_eet(&(ap->exprs[i]), temp_p, &src_tp, &(ap->child->header), (db_uint8)(ap->aggr_locs[i][j]+1), mmp);
						
						if (-1 == retval)
						{
							db_int k, l;
							for (k = 0; k < i; ++k)
							{
								for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
								{
									DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
										(db_int)(ap->aggr_locs[k][l])))->value_p);
									((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
										(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
								}
							}
							close_tuple(&src_t, mmp);
							return -1;
						}
						else if (2 == retval)
						{
							/* Subtract the current count,
							   as it will be added back
							   later. This is needed
							   to correctly calculate
							   aggregates that need acurate
							   counts, like AVG(). Notice
							   that this doesn't end up
							   messing up COUNT(), since
							   we just add the count to
							   to it's aggregate value. */
							ap->tuples_seen -= ap->next_count;
							if (aggr_np->aggr_type == DB_AGGR_COUNTROWS)
							{
								/* Force this attributes isnull bit to be 0 */
								aggr_np->aggr_isnull = 0;
								*((db_int*)(aggr_np->value_p)) = *((db_int*)(aggr_np->value_p)) + ((db_int)(ap->next_count));
							}
						}
						else if (1 == retval)
						{
							aggr_np->aggr_isnull = 0;
							/* Set isnull bit to 0 for
							   this attribute. */
							if (aggr_np->aggr_type == DB_AGGR_COUNTROWS)
							{
								*((db_int*)(aggr_np->value_p)) = *((db_int*)(aggr_np->value_p)) + ((db_int)(ap->next_count));
							}
							else if ((db_uint8)DB_EETNODE_CONST_DBINT == aggr_np->subexpr_type)
							{
								if (aggr_np->aggr_type == DB_AGGR_BAND)
								{
									*((db_int*)(aggr_np->value_p)) = *((db_int*)(aggr_np->value_p)) & (*((db_int*)temp_p));
								}
								else if (aggr_np->aggr_type == DB_AGGR_BOR)
								{
									*((db_int*)(aggr_np->value_p)) = *((db_int*)(aggr_np->value_p)) | (*((db_int*)temp_p));
								}
								else if (aggr_np->aggr_type == DB_AGGR_BXOR)
								{
									/* If this is the first iteration for this group... */
									if (0 == looper)
										*((db_int*)(aggr_np->value_p)) = (*((db_int*)temp_p));
									else
									{
										db_int k;
										for (k = 0; k < ap->next_count; k++)
										{
											*((db_int*)(aggr_np->value_p)) = *((db_int*)(aggr_np->value_p)) ^ (*((db_int*)temp_p));
										}
									}
								}
								else if (aggr_np->aggr_type == DB_AGGR_SUM)
								{
									//printf("Old sum value: %d\n", *((db_int*)(aggr_np->value_p)));
									//printf("Added temp value: %d\n", (*((db_int*)temp_p)));
									//printf("Added temp count: %d\n", ap->next_count);
									*((db_int*)(aggr_np->value_p)) = *((db_int*)(aggr_np->value_p)) +
														(((db_int)(ap->next_count)) *
														(*((db_int*)temp_p)));
									//printf("New sum value: %d\n", *((db_int*)(aggr_np->value_p)));
								}
								else if (aggr_np->aggr_type == DB_AGGR_MAX)
								{
									*((db_int*)(aggr_np->value_p)) =
										(*((db_int*)(aggr_np->value_p)) >=
											(*((db_int*)temp_p)))
										 ? *((db_int*)(aggr_np->value_p)) : (*((db_int*)temp_p));
								}
								else if (aggr_np->aggr_type == DB_AGGR_MIN)
								{
									*((db_int*)(aggr_np->value_p)) =
										(*((db_int*)(aggr_np->value_p)) <=
											(*((db_int*)temp_p)))
										 ? *((db_int*)(aggr_np->value_p)) : (*((db_int*)temp_p));
								}
								else if (aggr_np->aggr_type == DB_AGGR_FIRST)
								{
									/* Since looper is always 0 on
									   the first iteration and 1
									   on every other iteration,
									   we simply check if looper is
									   0, and if it is, we set the
									   temporary value.  Otherwise,
									   we don't. */
									if (0 == looper)
									{
										//printf("First value: %d\n", (*((db_int*)temp_p)));
										*((db_int*)(aggr_np->value_p)) = (*((db_int*)temp_p));
									}
								}
								else if (aggr_np->aggr_type == DB_AGGR_LAST)
								{
									*((db_int*)(aggr_np->value_p)) = (*((db_int*)temp_p));
								}
								else
								{
									db_int k, l;
									for (k = 0; k < i; ++k)
									{
										for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
										{
											DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
												(db_int)(ap->aggr_locs[k][l])))->value_p);
											((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
												(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
										}
									}
									close_tuple(&src_t, mmp);
									return -1;
								}
							}
						}
						 
						/* Free the temporary variable, if necessary. */
						if (temp_p != NULL)
						{
							DB_QMM_BFREE(mmp, temp_p);
							temp_p = NULL;
						}
					}
				}
			}
			ap->tuples_seen += ap->next_count;
			ap->next_count = 0;
			
			if (ap->previous_tp == NULL)
			{
				ap->previous_tp = DB_QMM_BALLOC(mmp, sizeof(db_tuple_t));
				init_tuple(ap->previous_tp,
					ap->child->header->tuple_size,
					ap->child->header->num_attr, mmp);
			}
			/* Copy out src_t to previous. */
			for (i = 0; i < (db_int)(ap->child->header->tuple_size); ++i)
			{
				ap->previous_tp->bytes[i] = src_t.bytes[i];
			}
			for (i = 0; i < (db_int)(ap->child->header->num_attr); ++i)
			{
				// FIXME: There has to be a better way to do this.  Involving %'s and division.
				ap->previous_tp->isnull[i/8] = src_t.isnull[i/8];
			}
			
			/* FIXME: It would be better if we didn't have to create two variables that are hopefully never used.... */
			/* Get the next tuple. */
			db_int k, l;
			switch ((next_inorder_tuple(&src_t, ap->previous_tp, &(ap->next_count),
				ap->child, (ap->groupby_exprs), ap->num_groupby_expr, orderings, mmp)))
			{
				case 1:
					break;
				case 0:
					/* Idea here is to signal that there is no more tuples to be pulled in. */
					ap->next_count = -1;
					break;
				default:
					for (k = 0; k < i; ++k)
					{
						for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
						{
							DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
								(db_int)(ap->aggr_locs[k][l])))->value_p);
							((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
								(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
						}
					}
					close_tuple(&src_t, mmp);
					return -1;	/* Error out. */
			}
			//printf("First db_int in src_t: %d\n", getintbypos(&src_t, 0, ap->child->header));
			//printf("First db_int in previous_tp: %d\n", getintbypos(ap->previous_tp, 0, ap->child->header));
			/* So I lied.... looper remains 0 through the entire
			   first iteration.  To make FIRST work, I will set
			   looper to 1 here.  If your eyes are beginning to
			   bleed, just chill, I am fairly certain I know what
			   I am doing.
			*/
			looper = 1;
		}
		//puts("STILL LOOPING");
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/* Abstraction point number 3: do final expression evaluation. Call this on having if it has aggregate, otherwise
		   just eval it (but do all of that where we currently eval HAVING clause. */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* For each expresion... */
		for (i = 0; i < ap->num_expr; ++i)
		{
			if (ap->aggr_locs[i] != NULL)
			{
				db_uint8 node_type = geteetnodetypebypos(&(ap->exprs[i]), 0);
				if ((db_uint8)(DB_EETNODE_AGGR_TEMP) != node_type)
				{
					if (node_type == DB_EETNODE_CONST_DBINT ||
					    node_type == DB_EETNODE_CONST_DBSTRING ||
					     node_type == DB_EETNODE_ATTR)	/* TODO: Implement future types here. */
					{
						/* Error out.  We have constructed a clause that does not make sense. */
						db_int k, l;
						for (k = 0; k < i; ++k)
						{
							for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
							{
								DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
									(db_int)(ap->aggr_locs[k][l])))->value_p);
								((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
									(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
							}
						}
						close_tuple(&src_t, mmp);
						return -1;
					}
					else if (node_type >= DB_EETNODE_OP_NOT &&
						 (node_type <= DB_EETNODE_FUNC_LENGTH_DBSTRING))
					{
						db_int result, retval;
						db_tuple_t *src_tp = &src_t;
						retval = evaluate_eet(&(ap->exprs[i]), &result, &src_tp, &(ap->child->header), 0, mmp);
						if (-1 == retval)
						{
							db_int k, l;
							for (k = 0; k < i; ++k)
							{
								for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
								{
									DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
										(db_int)(ap->aggr_locs[k][l])))->value_p);
									((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
										(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
								}
							}
							close_tuple(&src_t, mmp);
							return -1;
						}
						else if (2 == retval)
						{
							/* Set this tuples attribute null bit to 1. */
							//puts("result: NULL");
							tp->isnull[i / 8] |= (1 << (i % 8));
						}
						else
						{
							tp->isnull[i/8] &= ~(1 << (i % 8));
							//printf("result: %d\n", result);
							*((db_int*)(&(tp->bytes[ap->base.header->offsets[i]]))) = result;
						}
					}
				}
				else if (ap->aggr_locs[i] != NULL && (db_uint8)(DB_EETNODE_AGGR_TEMP) == node_type)
				{
					db_eetnode_aggr_temp_t* aggr_np = (db_eetnode_aggr_temp_t*)((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[i]),
										(db_int)0));
					if (aggr_np->aggr_type <= DB_AGGR_LAST)
					{
						db_int result, retval;
						db_tuple_t *src_tp = &src_t;
						retval = evaluate_eet(&(ap->exprs[i]), &result, &src_tp, &(ap->child->header), 0, mmp);
						if (-1 == retval)
						{
							db_int k, l;
							for (k = 0; k < i; ++k)
							{
								for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
								{
									DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
										(db_int)(ap->aggr_locs[k][l])))->value_p);
									((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
										(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
								}
							}
							close_tuple(&src_t, mmp);
							return -1;
						}
						else if (2 == retval)
						{
							/* Set this tuples attribute null bit to 1. */
							//puts("result: NULL");
							tp->isnull[i / 8] |= (1 << (i % 8));
						}
						else
						{
							tp->isnull[i/8] &= ~(1 << (i % 8));
							//printf("result: %d\n", result);
							*((db_int*)(&(tp->bytes[ap->base.header->offsets[i]]))) = result;
						}
					}
				}
				/* Otherwise, do nothing. These attributes have already been handled. */
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/* Check the HAVING clause. */
		db_int retval = evaluate_eet(ap->having_expr, &looper, &tp, &(ap->base.header), 0, mmp);
		//printf("ap->next_count: %d\n", ap->next_count);
		//printf("retval: %d\n", retval);
		//printf("looper: %d\n", looper);
		//fflush(stdout);
		if (2 == retval || 0 == looper)
		{
			/* Free all the created temporary values. */
			db_int k, l;
			for (k = 0; k < i; ++k)
			{
				for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
				{
					DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
						(db_int)(ap->aggr_locs[k][l])))->value_p);
					((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
						(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
				}
			}
			if (ap->next_count < 0)
			{
				close_tuple(&src_t, mmp);
				return 0;
			}
			else
				looper = 1;
		}
		else if (-1 == retval)
		{
			db_int k, l;
			for (k = 0; k < i; ++k)
			{
				for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
				{
					DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
						(db_int)(ap->aggr_locs[k][l])))->value_p);
					((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
						(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
				}
			}
			close_tuple(&src_t, mmp);
			return -1;
		}
		else
			looper = 0;
		/* If looper is 1, the currently computed aggregates pass the
		   HAVING clause.  Return. */
	} while (looper);
	db_int k, l;
	for (k = 0; k < i; ++k)
	{
		for (l = 0; ap->aggr_locs[k] != NULL && ap->aggr_locs[k][l] > -1; ++l)
		{
			DB_QMM_BFREE(mmp, ((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
				(db_int)(ap->aggr_locs[k][l])))->value_p);
			((db_eetnode_aggr_temp_t*)geteetnodebypos(&(ap->exprs[k]),
				(db_int)(ap->aggr_locs[k][l])))->value_p = NULL;
		}
	}
	close_tuple(&src_t, mmp);
	
	return 1;
}

/* Close the aggregate operator. */
db_int close_aggregate(aggregate_t *ap, db_query_mm_t *mmp)
{
	/* Free memory used by previous tuple, if it was initialized. */
	if (ap->previous_tp != NULL)
	{
		close_tuple(ap->previous_tp, mmp);
		DB_QMM_BFREE(mmp, ap->previous_tp);
	}
	db_int i;
	for (i = 0; i < (db_int)(ap->num_expr); ++i)
	{
		if (ap->aggr_locs[i] != NULL)
			DB_QMM_BFREE(mmp, ap->aggr_locs[i]);
	}
	DB_QMM_BFREE(mmp, ap->aggr_locs);
	DB_QMM_BFREE(mmp, ap->base.header->size_name);
	DB_QMM_BFREE(mmp, ap->base.header->names);
	DB_QMM_BFREE(mmp, ap->base.header->types);
	DB_QMM_BFREE(mmp, ap->base.header->offsets);
	DB_QMM_BFREE(mmp, ap->base.header->sizes);
	DB_QMM_BFREE(mmp, ap->base.header);
	return 1;
}

#endif
#endif
