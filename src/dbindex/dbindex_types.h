/******************************************************************************/
/**
@file		dbindex_types.h
@author		Graeme Douglas
@brief		A file storing index types that need to be referenced
		in ways that may lead to circular dependencies.
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

#ifndef DBINDEX_TYPES_H
#define DBINDEX_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief		In-memory metadata for all indexing information of a
		relation.
*/
typedef struct db_index_meta
{
	db_uint8	num_idx;	/**< Number of indexes belonging to
					     a relation. */
	db_uint8	*len_names;	/**< Length of each index name. */
	char		**names;	/**< The names of each index. */
	db_uint8	*num_expr;	/**< The number of expressions in
					     each index. */
	db_eet_t	**exprs;	/**< Array of expressions for each
					     index. */
} db_index_meta_t;

#ifdef __cplusplus
}
#endif

#endif
