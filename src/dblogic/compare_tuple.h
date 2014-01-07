/******************************************************************************/
/**
@file		compare_tuple.h
@author		Graeme Douglas
@brief		Code to compare and sort tuples absolutely.
@details	This code allows us to compare two tuples to determine their
		order, assuming they belong to the same relation.  It also
		provides code needed to do a tuple-at-a-time sort (read:
		insertion sort).
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
@todo		Implement better sorting algorithm, perhaps Tyler C's.
*/
/******************************************************************************/

#ifndef COMPARE_TUPLE_H
#define COMPARE_TUPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "../ref.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include "../dbops/db_ops_types.h"
#include "../dbops/db_ops.h"
#include "eet.h"

/* Used to determine whether to sort tuples by max or minimum first. */
/**
@enum		db_tupler_order_t
@brief		Enumerated values for values for tuple order.
*/
typedef enum
{
	DB_TUPLE_ORDER_ASC = 0,		/**< The tuples are to be compared
					     and/or sorted in ascending order.
					*/
	DB_TUPLE_ORDER_DESC,		/**< The tuples are to be compared
					     and/or sorted in descending order.
					*/
	DB_TUPLE_ORDER_COUNT		/**< The number of values for this
					     type. */
} db_tuple_order_t;

/* Compares two tuples _FROM THE SAME RELATION_ to determine which comes
   before the other, in either minimum or maximum order. */
/**
@brief		Compares two tuples of the same relation to determine their
		order.
@details	This comparison first compares the tuples based on the order
		expressions.  Then, if the order expressions do not determine
		the order of the tuples and strictComp != 1, the tuples'
		attributes are used to determine the order, so as to give a
		"total ordering".
@param		a		Pointer to the first tuple to be compared.
@param		b		Pointer to the second tuple to be compared.
@param		hp_a		The relation header (schema) for the relation
				@p a belongs to.
@param		hp_b		The relation header (schema) for the relation
				@p a belongs to.  Set pointer to @p hp_a if
				@p b is from the same relation as @p a.
@param		order_exprs_a	Array of order-determining expressions for
				@p a.
@param		order_exprs_b	Array of order-determining expressions for
				@p b.  Set pointer to @p order_exprs_a if
				@p b is from the same relation as @p a.
@param		num_expr	The number of elements in @p order_exprs
				and @p order.
@param		order		Array where each element's value is either
				@c DB_TUPLE_ORDER_ASC or @c DB_TUPLE_ORDER_DESC.
				Element i of this array determines the order
				which element i in @p order_exprs is compared
				and/or sorted on.
@param		strictComp	If this value is 1, @p a and @p b are not
				compared based on their attributes, but
				insttead ONLY compared on @p order_exprs.
				Otherwise, @p a and @p b are compared
				on their attributes to break a tie.
@param		mmp		A pointer to the memory manager pointer
				used to make memory allocations.
@returns	@c -1 if @p a comes before @p b, 0 if @p a is equivalent
		to @p b, and @c 1 if @p a comes after @p b.
*/
db_int8 cmp_tuple(db_tuple_t* a,
		db_tuple_t* b,
		relation_header_t *hp_a,
		relation_header_t *hp_b,
		db_eet_t *order_exprs_a,
		db_eet_t *order_exprs_b,
		db_int num_expr,
		db_uint8 *order,
		db_uint8 strictComp,
		db_query_mm_t *mmp);

/* Return the next tuple in sorted order. */
/**
@brief		Given a previously selected tuple, returns the next sorted
		tuple.
@param		next_tp		Pointer to the tuple variable to place the next
				tuple in.
@param		previous_tp	Pointer to the last tuple returned.  If it is
				desired to retrieve the first in-order tuple,
				this parameter should be set to @c NULL.
@param		next_count	A pointer to the variable in which to store the
				number of tuples that are identical to
				@p next_tp.
@param		order_exprs	Array of order-determining expressions.
@param		src_op		Pointer to operator that will be providing
				tuples to be sorted.  It can be thought of
				as the relation that is to be sorted.
@param		num_expr	The number of elements in @p order_exprs
				and @p order.
@param		order		Array where each element's value is either
				@c DB_TUPLE_ORDER_ASC or @c DB_TUPLE_ORDER_DESC.
				Element i of this array determines the order
				which element i in @p order_exprs is compared
				and/or sorted on.
@param		mmp		A pointer to the memory manager pointer
				used to make memory allocations.
@returns	@c -1 if an error occurs, @c 0 if there are no more tuples
		to return, and @c 1 otherwise.
*/
db_int next_inorder_tuple(db_tuple_t *next_tp,
		db_tuple_t *previous_tp,
		db_int *next_count,
		db_op_base_t *src_op,
		db_eet_t *order_exprs,
		db_uint8 num_expr,
		db_uint8 *order,
		db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
