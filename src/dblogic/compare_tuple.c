/******************************************************************************/
/**
@file		compare_tuple.c
@author		Graeme Douglas
@brief		Implementation of tuple comparison library.
@details	The algorithm used to return a tuple is the following:
		
		previous <- next // previous min/max, or null on first call
		next <- first tuple in the relation
		for each tuple t in T
			if t follows previous and precedes next
				next <- t
		return next
		
		@c NULL is always less than all other values.
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
@see		For more information, see @ref compare_tuple.h.
*/
/******************************************************************************/

#include <stdio.h>
#include "../dbobjects/tuple.h"
#include "../dbops/db_ops_types.h"
#include "compare_tuple.h"

/* Compares two tuples _FROM THE SAME RELATION_ to determine which comes
 * before the other, in either minimum or maximum order.*/
db_int8 cmp_tuple(db_tuple_t* a, db_tuple_t* b, relation_header_t *hp_a,
		relation_header_t *hp_b, db_eet_t *order_exprs_a,
		db_eet_t *order_exprs_b, db_int num_expr, db_uint8 *order,
		db_uint8 strictComp, db_query_mm_t *mmp)
{
	/* We check each attribute in the order specified by sort_attr_pos
	 * array.  We only need to compare a.attr to b.attr for each
	 * ordering attribute until we find the first attribute where
	 * a.attr != b.attr, and this will determine the order. */
	db_int cmp_result = 0;
	db_int i;
	db_uint8 order_used = 0;	/* What this variable does is track
					   which ordering we are actually
					   based on how this code executes.
					   Since we know the result of a
					   comparison as soon as one
					   expression comparison results in
					   some non-zero value, we simply
					   record the ordering used for that
					   variable, or simply ASC if
					   ordering determined by attributes
					   instead of expression. */
	db_int type;
	for (i = 0; NULL != order_exprs_a && NULL != order_exprs_b && i < num_expr; i++)
	{
		type = evaluate_eet(&order_exprs_a[i], NULL, NULL, &hp_a, 0, mmp);
		if (order_exprs_a != order_exprs_b && evaluate_eet(&order_exprs_b[i], NULL, NULL, &hp_b, 0, mmp) != type)
			exit(-1);
		
		if (DB_EETNODE_CONST_DBINT == type)
		{
			db_int a_int, b_int, a_retval, b_retval;
			a_retval = evaluate_eet(&order_exprs_a[i], &a_int, &a, &hp_a, 0, mmp);
			b_retval = evaluate_eet(&order_exprs_b[i], &b_int, &b, &hp_b, 0, mmp);
			
			if (-1 == a_retval || -1 == b_retval)
			{
				exit(-1);
			}
			/* Case where both are NULL. */
			else if (2 == a_retval && 2 == b_retval)
			{
				/* Do nothing. */
			}
			/* Case a is NULL, b is not NULL. */
			else if (2 == a_retval && 2 != b_retval)
			{
				cmp_result = -1;
			}
			/* Case a is not NULL, b is NULL. */
			else if (2 != a_retval && 2 == b_retval)
			{
				cmp_result = 1;
			}
			/* Case where neither are NULL. */
			else
			{
				cmp_result = a_int - b_int;
			}
		}
		else if (DB_EETNODE_CONST_DBSTRING == type)
		{
			char *a_string = NULL, *b_string = NULL;
			db_int a_retval, b_retval;
			a_retval = evaluate_eet(&order_exprs_a[i], &a_string, &a, &hp_a, 0, mmp);
			b_retval = evaluate_eet(&order_exprs_b[i], &b_string, &b, &hp_b, 0, mmp);
			
			if (-1 == a_retval || -1 == b_retval)
			{
				exit(-1);
			}
			/* Case where both are NULL. */
			else if (2 == a_retval && 2 == b_retval)
			{
				/* Do nothing. */
			}
			/* Case a is NULL, b is not NULL. */
			else if (2 == a_retval && 2 != b_retval)
			{
				cmp_result = -1;
			}
			/* Case a is not NULL, b is NULL. */
			else if (2 != a_retval && 2 == b_retval)
			{
				cmp_result = 1;
			}
			/* Case where neither are NULL. */
			else
			{
				cmp_result = strcmp(a_string, b_string);
			}
		}
		/* TODO: Implement future types here. */
		if (cmp_result != 0)
		{
			/* If we have determined an ordering on this attribute.
			*/
			order_used = order[i];
			break;
		}
	}
	/* This section of code compares the tuples on attributes.  If
	   this isn't done, it is extremely dificult (if not impossible) to
	   properly sort / group tuples correctly, since we count all tuples
	   with the same ordering key, regardless if they are totally
	   identical or not. Also note that since we have to scan the entire
	   relation each time we find the next element to return, this
	   esentially costs us nothing. */
	if (cmp_result == 0 && hp_a == hp_b && 1 != strictComp)
	{
		/* For each attribute... */
		for (i = 0; i < (db_int)hp_a->num_attr; ++i)
		{
			// TODO: Perhaps move this comparison to tuple.c?
			switch((attr_type)hp_a->types[i])
			{
				case DB_INT:
					cmp_result = getintbypos(a, i, hp_a)
						- getintbypos(b, i, hp_a);
					break;
				case DB_STRING:
					cmp_result =
						strcmp(getstringbypos(a, i, hp_a),
								getstringbypos(b, i, hp_a));
					break;
				default:
					break;
			}
			if (0 != cmp_result)
				break;
		}
	}
	
	/* Normalize. */
	if (cmp_result > 0)
	{
		cmp_result = 1;
	}
	else if (cmp_result < 0)
	{
		cmp_result = -1;
	}
	/* This is a trick I have employed.  We do all comparisons by assuming
	 * we want ascending ordering. If we really want descending ordering,
	 * we simply multiply our result by -1.  This drastically reduces our
	 * code-space footprint for this section and reduces the amount of
	 * code to maintain as well. */
	if (order_used == (db_int8)DB_TUPLE_ORDER_ASC)
	{
		return ((db_int8)cmp_result);
	}
	else if (order_used == (db_int8)DB_TUPLE_ORDER_DESC)
	{
		return (-1)*((db_int8)cmp_result);
	}
	else
	{
		exit(-1);	/* Error out. */
	}
}

/* Return the next tuple in sorted order. */
db_int next_inorder_tuple(db_tuple_t *next_tp, db_tuple_t *previous_tp, db_int *next_count,
		db_op_base_t *src_op, db_eet_t *order_exprs,
		db_uint8 num_expr, db_uint8 *order, db_query_mm_t *mmp)
{
	db_tuple_t t;	/* Temporary tuple used to run through the operator. */
	init_tuple(&t, src_op->header->tuple_size, src_op->header->num_attr, mmp);
	//*previous_tp = *next_tp;
	rewind_dbop(src_op, mmp);
	/* Set next to the first tuple from the source operator. */
	if (1 == next(src_op, next_tp, mmp))
	{
		(*(next_count)) = 1;
		/* If next comes before previous */
		if (previous_tp!=NULL &&
			cmp_tuple(next_tp, previous_tp, src_op->header, src_op->header,
				order_exprs, order_exprs, num_expr, order, 0, mmp) < 0)
		{
			/* next cannot come before previous, so copy
			   previous into next. */
			db_uint8 i;
			for (i = 0; i < src_op->header->tuple_size; ++i)
			{
				next_tp->bytes[(db_int)i] = 
					previous_tp->bytes[(db_int)i];
			}
			for (i = 0; i < src_op->header->num_attr; ++i)
			{
				next_tp->isnull[((db_int)i)/8] = 
					previous_tp->isnull[((db_int)i)/8];
			}
		}
	}
	else
	{
		return 0;
	}
	while (1 == next(src_op, &t, mmp))
	{
		//printf("First db_int of next: %d\n", getintbypos(next_tp, 0, src_op->header));
		//printf("First db_int of t: %d\n", getintbypos(&t, 0, src_op->header));
		/* If t comes after previous */
		if (previous_tp==NULL ||
			cmp_tuple(&t, previous_tp, src_op->header, src_op->header,
				order_exprs, order_exprs, num_expr, order, 0, mmp) > 0)
		{
			switch (cmp_tuple(&t, next_tp, src_op->header, src_op->header,
				order_exprs, order_exprs, num_expr, order, 0, mmp))
			{
				case 0:		/* t == next */
					(*(next_count))++;
					break;
				case -1:	/* t comes before next */
					(*(next_count)) = 1;
					/* Copy out all of tuples bytes. */
					db_uint8 i;
					for (i = 0;
						i < src_op->header->tuple_size;
						++i)
					{
						next_tp->bytes[(db_int)i] =
							t.bytes[(db_int)i];
					}
					for (i = 0; i < src_op->header->
								num_attr; ++i)
					{
						next_tp->isnull[((db_int)i)/8]=
							t.isnull[((db_int)i)/8];
					}
					break;
				case 1:		/* t comes after next. */
					/* If next == previous */
					if (previous_tp!=NULL &&
						cmp_tuple(next_tp, previous_tp,
						src_op->header, src_op->header, order_exprs, order_exprs,
						num_expr, order, 0, mmp) == 0)
					{
						/* special case. */
						db_uint8 i;
						for (i = 0; i < src_op->
							header->tuple_size;
							++i)
						{
							next_tp->bytes[(db_int)i] = 
								t.bytes[(db_int)i];
						}
						for (i = 0; i < src_op->header->
									num_attr; ++i)
						{
							next_tp->isnull[((db_int)i)/8]=
								t.isnull[((db_int)i)/8];
						}
					}
					break;
				default:
					close_tuple(&t, mmp);
					return -1;
			}
		}
	}
	//printf("&t: %p\n", &t);
	//printf("t.bytes: %p\n", t.bytes);
	close_tuple(&t, mmp);
	/* Next is initially set to the first tuple in the source operator,
	   or to the contents of previous_tp (provided its not null).
	   If we never change that value (and previous isn't null), then
	   there are no more values to process.  We can check this by
	   checking that next comes after previous. */
	return (db_int)(previous_tp == NULL || (cmp_tuple(next_tp, previous_tp,
		src_op->header, src_op->header, order_exprs, order_exprs, num_expr, order, 0, mmp) > 0));
}
