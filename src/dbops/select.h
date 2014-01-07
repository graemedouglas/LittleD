/******************************************************************************/
/**
@file		select.h
@author		Graeme Douglas
@brief		The relational selection operator.
@details	Selection (WHERE) is the part of the query that filters results
		that do not involve any form of aggregation/grouping.
		In other words, we choose tuples that meet some condition.
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

#ifndef SELECT_H
#define SELECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "db_ops_types.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"

/* Initialize the selection operator. */
/**
@brief		Intialize a selection operator.
@param		sp		A pointer to the selection operator that is to
				be initialized.
@param		ep		A pointer to the bytecode expression that will
				be used to filter results.
@param		child		A pointer to the relational operator that will
				be passing tuples into the selection.
@param		mmp		A pointer to the per-query memory manager that
				will be used to allocate memory for this query.
*/
db_int init_select(select_t *sp,
		db_eet_t *ep,
		db_op_base_t *child,
		db_query_mm_t *mmp);

/* Find next tuple that passes the selection condition. */
/**
@brief		Retrieve the next tuple from a selection operator.
@details	This assumes that the filtering expression will always
		reduce to either @c 0 or @c 1 (@c false / @c true).
@see		For more information, reference @ref next.
*/
db_int next_select(select_t *sp, db_tuple_t *next_tp, db_query_mm_t *mmp);

/* Rewind the selection operator. */
/**
@brief		Rewind the selection operator.
@see		For more information, reference @ref rewind_dbop.
*/
db_int rewind_select(select_t *sp, db_query_mm_t *mmp);

/* Cclose the selection operator. */
/**
@brief		Safely deconstruct the selection operator.
@see		For more information, reference @ref close.
*/
db_int close_select(select_t *sp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
