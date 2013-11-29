/******************************************************************************/
/**
@file		project.h
@author		Graeme Douglas
@brief		The relational projection operator.
@details	This assumes a tuple at a time approach.
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

#ifndef PROJECT_H
#define PROJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "db_ops_types.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include "../dblogic/eet.h"
#include <stdio.h>
#include <string.h>

/* Initialize the projection operator. */
/**
@brief		Initialize a projection operator.
@param		pp		A pointer to a projection operator to be
				intialized.
@param		childop_p	A pointer to the operator that will pass
				tuples into the projection operator.
@param		exprs		The array of projecting bytecode expressions.
@param		num_exprs	The number of expressions in @p exprs.
@param		mmp		Pointer to the per-query memory manager that is
				being used to allocate memory for the query.
*/
db_int init_project(project_t *pp,
		db_op_base_t *childop_p,
		db_eet_t *exprs,
		db_uint8 num_exprs,
		db_query_mm_t *mmp);

/* This method assumes the scan is already initialized */
/**
@brief		Rewind the projection operator.
@see		Reference @ref rewind_dbop for documentation.
*/
db_int rewind_project(project_t *pp, db_query_mm_t *mmp);

/* Retrieve next tuple. */
/**
@brief		Retrieve the next tuple from the projection operator.
@see		Reference @ref next for documentation.
*/
db_int next_project(project_t *pp, tuple_t *next_tp, db_query_mm_t *mmp);

/* Close the projection operator. */
/**
@brief		Safely destroy a projection operator.
@see		Reference @ref close for documentation.
*/
void close_project(project_t *pp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
