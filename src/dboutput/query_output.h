/******************************************************************************/
/**
@file		query_output.h
@author		Graeme Douglas
@brief		Output queries to string or standard output.
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

#ifndef QUERY_OUTPUT_H
#define QUERY_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../dbmm/db_query_mm.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include "../dbops/db_ops.h"
#include "../ref.h"
#include <stdio.h>
#include <math.h>

/**
@brief		Print a query to standard out.
@details	The query will be formatted into a string and then printed
		out via the standard output.  If the target system does
		not have a large enough section of memory to stringize the
		results, don't call the method.
@param		op	A pointer to the root operator for the query's
			execution tree.
@param		mmp	A pointer to the per-query memory manager used
			to allocate memory for the query.
*/
void printQuery(db_op_base_t *op,
		db_query_mm_t *mmp);

/**
@brief		Format the query results into a stirng.
@details	If your system does not have enough memory to format the
		results into a string, do not call this method.  The result
		must be freed!
@param		op	A pointer to the root operator for the query's
			execution tree.
@param		mmp	A pointer to the per-query memory manager used
			to allocate memory for the query.
@returns	The formatted query.
*/
char* formatQuery(db_op_base_t *op,
		db_query_mm_t *mmp);

/**
@brief		Print a query to standard out.
@param		op	A pointer to the root operator for the query's
			execution tree.
@param		next_t	A pointer to an initialized tuple corresponding
			to @p op.
@param		mmp	A pointer to the per-query memory manager used
			to allocate memory for the query.
@returns	The size of the string representing the query.
*/
db_int sizeQuery(db_op_base_t *op,
		db_tuple_t *next_t,
		db_query_mm_t *mmp);

/**
@brief		Format a tuple.
@details	The result must be free!
@param		toprint	The tuple to be formatted.
@param		op	A pointer to the root operator for the query's
			execution tree.
@param		widths	The width of each formatted column in the
			query results.
@returns	The formatted tuple.
*/
char* formatTuple(db_tuple_t *toprint,
		db_op_base_t *op,
		db_int *widths);

/**
@brief		Print a tuple.
@param		toprint	The tuple to be printed.
@param		op	A pointer to the root operator for the query's
			execution tree.
@param		widths	The width of each formatted column in the
			query results.
*/
void printTuple(db_tuple_t *toprint,
		db_op_base_t *op,
		db_int *widths);

/**
@brief		Print a row separator.
@param		op	A pointer to the root operator in the query
			execution tree.
@param		widths	The widths of each formatted column in the
			query results.
*/
void printRowSeparator(db_op_base_t *op,
		db_int *widths);

/**
@brief		Format a row separator.
@details	This must be freed!
@param		op	A pointer to the root operator in the query
			execution tree.
@param		widths	The widths of each formatted column in the
			query results.
@returns	A formatted row separator for the query results.
*/
char* formatRowSeparator(db_op_base_t *op,
			db_int *widths);

/**
@brief		Get the size of the string representation of a row
		in the query results.
@param		op	A pointer to the root operator in the query
			execution tree.
@param		widths	The widths of each formatted column in the
			query results.
@returns	The size of the string representation of a row in
		the query results.
*/
db_int sizeOfRow(db_op_base_t *op,
		db_int *widths);

/**
@brief		Compute the widths of each column in the formatted
		query results.
@details	Allocated space for @p widths must be freed!
@param		op	A pointer to the root operator in the query
			execution tree.
@param		widths	A pointer that will be set to an array
			of @c db_int values, one for each
			column in the query output.
*/
void computeWidths(db_op_base_t *op,
		db_int *widths);

#ifdef __cplusplus
}
#endif

#endif
