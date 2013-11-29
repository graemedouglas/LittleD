/******************************************************************************/
/**
@file		aggregate.h
@author		Graeme Douglas
@brief		The relational aggregate (GROUP BY) operator.
@details	The projecting expressions are those that are parsed from the
		SELECT clause.  Some assumptions are made.  The most important
		of these is that we assume that expressions are never larger
		than DB_INT8_MAX.  We also assume that no expressions can have
		non-grouping or non-aggregated expressions.
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

#ifndef AGGREGATE_H
#define AGGREGATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "db_ops.h"
#include "../dbobjects/relation.h"
#include "../dblogic/eet.h"
#include "../dblogic/compare_tuple.h"
#include <limits.h>

#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1

/* Initialize the aggregate operator. */
/**
@brief		Initialize an aggregate operator.
@param		ap			Pointer to the aggregate operator that
					is to be initialized.
@param		child			Pointer to the operator immediately
					below @p ap in the execution tree. That
					is, the child.
@param		exprs			Array of bytecode projecting
					expressions.
@param		num_expr		The num of elements in @p exprs.
@param		groupby_exprs		The set of expressions that determines
					how the tuples are grouped during
					aggregation.
@param		num_groupby_exprs	The number of elements in
					@p groupby_exprs.
@param		having_expr		The bytecode expression generated
					from the HAVING clause.
@param		mmp			The per-query memory manager being used
					to allocate memory for this query.
@returns	@c 1 if the operator was initialized, @c -1 if an error occured,
		@c 0 otherwise.
*/
db_int init_aggregate(aggregate_t *ap,
		db_op_base_t *child,
		db_eet_t *exprs,
		db_uint8 num_expr,
		db_eet_t *groupby_exprs,
		db_uint8 num_groupby_expr,
		db_eet_t *having_expr,
		db_query_mm_t *mmp);

/* Rewind the aggregate operator. */
/**
@brief		Rewind the aggregate operator.
@see		rewind_dbop
*/
db_int rewind_aggregate(aggregate_t *ap, db_query_mm_t *mmp);

/* Return the next tuple from the aggregate operator. */
/**
@brief		Get the next tuple from an aggregate operator.
@see		next
*/
db_int next_aggregate(aggregate_t *ap, tuple_t *tp, db_query_mm_t *mmp);

/* Close the aggregate operator. */
/**
@brief		Safely deconstruct the aggregate operator.
*/
db_int close_aggregate(aggregate_t *ap, db_query_mm_t *mmp);

#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
