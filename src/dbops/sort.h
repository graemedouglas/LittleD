/******************************************************************************/
/**
@file		sort.h
@author		Graeme Douglas
@brief		The relational sort operator.
@details	This is the relational operator behind the ORDER BY clause.
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

#ifndef SORT_H
#define SORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include "../ref.h"
#include "db_ops_types.h"

/* Initialize the sort operator. */
/**
@brief		Initialize a relational sort operator.
@param		sp		A pointer to the sort operator to be
				initialized.
@param		child		A pointer to the operator that will
				pass tuples into the sort operator.
@param		sort_exprs	An array of bytecode expressions that
				determine how to order the tuples.
@param		num_expr	The number of expressions in @p sort_exprs.
@param		order		An array of integers determining what order to
				sort the tuples for each expression.  This
				should have exactly @p num_expr integers.
@param		mmp		A pointer to the memory manager pointer to be
				used to allocate memory for this query.
*/
db_int init_sort(sort_t *sp,
		db_op_base_t *child,
		db_eet_t *sort_exprs,
		db_uint8 num_expr,
		db_uint8 *order,
		db_query_mm_t *mmp);

/* Rewind the sort operator. */
/**
@brief		Rewind a sort operator.
@see		For more information, reference @ref rewind_dbop.
*/
db_int rewind_sort(sort_t *sp, db_query_mm_t *mmp);

/* Return the next tuple from the sort operator. */
/**
@brief		Retrieve the next sorted tuple from a sort operator.
@see		For more information, reference @ref next.
*/
db_int next_sort(sort_t *sp, db_tuple_t *tp, db_query_mm_t *mmp);

/* Close the sort operator. */
/**
@brief		Cleanly deconstruct a sort operator.
@see		For more information, reference @ref close.
*/
db_int close_sort(sort_t *sp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
