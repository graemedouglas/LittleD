/******************************************************************************/
/**
@file		ntjoin.h
@author		Graeme Douglas
@brief		The relational join, specfically a binary nested-tuple join.
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

#ifndef NTJOIN_H
#define NTJOIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "db_ops_types.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include <stdio.h>

/* Initialize the selection operator. */
/**
@brief		Initialize a nested-tuple join oeprator.
@param		jp		A pointer to the nested-tuple join that is
				to be initialized.
@param		ep		A pointer to the bytecode expression to be used
				to filter results by, or @c NULL if no filtering
				is to occur.
@param		lchild		A pointer to the operator that is passing tuples
				to the left side of the join.
@param		rchild		A pointer to the operator that is passing tuples
				to the right side of the join.
@param		mmp		A pointer to the per-query memory manager that
				is allocating space for this query.
*/
db_int init_ntjoin(ntjoin_t *jp,
		db_eet_t *ep,
		db_op_base_t *lchild,
		db_op_base_t *rchild,
		db_query_mm_t *mmp);

/* Re-start the operator from the beginning. This assumes the operator has
   been initialized. */
/**
@brief		Rewind a nested-tuple join operator.
@see		For more information, please reference @ref rewind_dbop.
*/
db_int rewind_ntjoin(ntjoin_t *jp, db_query_mm_t *mmp);

/* Find next tuple that passes the join condition. */
/**
@brief		Produce the next tuple from a nested-tuple join operator.
@see		For more information, please reference @ref next.
*/
db_int next_ntjoin(ntjoin_t *jp, tuple_t *next_tp, db_query_mm_t *mmp);

/* Close the selection operator. */
/**
@brief		Cleanly deconstruct a  nested-tuple join operator.
@see		For more information, please reference @ref close.
*/
db_int close_ntjoin(ntjoin_t *jp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
