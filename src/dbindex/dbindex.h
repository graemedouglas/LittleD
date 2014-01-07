/******************************************************************************/
/**
@file		dbindex.h
@author		Graeme Douglas
@brief		A generic relation index interface.
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

#ifndef DBINDEX_H
#define DBINDEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../ref.h"
#include "../dblogic/eet.h"
#include "../dblogic/compare_tuple.h"
#include "dbindex_types.h"
#include "../dbops/db_ops_types.h"

/**
@brief		Database index search modes.
@details	Since search routines usually only need to be slightly
		modified for various structure operations, we will
		maximize code reuse by leveraging search modes.
*/
typedef enum
{
	DB_INDEX_SEARCHMODE_EXACT = 0,		/**< Find an exact match. */
	DB_INDEX_SEARCHMODE_PREVIOUS = 1,	/**< If no exact match can
						     be found, then we return
						     the closest predecessor.
						*/
	DB_INDEX_SEARCHMODE_COUNT		/**< The number of elements
						     in this enumerated type.
						*/
} db_index_searchmode_t;

/**
@brief		Database relation index types.
*/
typedef enum
{
	DB_INDEX_TYPE_NONE = 0,		/**< No index. */
	DB_INDEX_TYPE_INLINE = 1,	/**< Inline index (sorted as it comes in). */
	DB_INDEX_TYPE_SKIPLIST,		/**< Skip list index. */
	DB_INDEX_TYPE_COUNT		/**< Count of all index types. */
} db_index_type_t;

/**
@brief		The generic index type.
*/
typedef struct db_index
{
	db_uint8	type;		/**< Type of the index. */
	db_fileref_t	indexref;	/**< Reference to the file containing
					     index data. */
} db_index_t;

/**
@brief		An index offset type.
*/
typedef long db_index_offset_t;

/**
@brief		Find the next tuple from an indexed scan based on another
		tuple.
@param		sp		The indexed scan operator to search.
@param		indexon		Which index to use.
@param		searchfor	The expression to search against.
@param		comparator_tp	A pointer to the tuple to evaluate the
				expression over.
@param		comparator_hp	A pointer to the relation header info for
				the relation the tuple pointed at by
				@p comparator_tp.
@param		mmp		A pointer to the memory manager instance being
				used to allocate memory for this query.
@returns	@c 1 if a tuple is found, @c 0 otherwise.
*/
db_int scan_find(scan_t *sp,
		db_uint8 indexon,
		db_eet_t *searchfor,
		db_tuple_t *comparator_tp,
		relation_header_t *comparator_hp,
		db_query_mm_t *mmp);

/**
@brief		Find the offset of a tuple in an indexed relation matching the
		search criterion.
@param		sp		The indexed scan operator to search.
@param		indexon		Which index to use.
@param		searchfor	The expression to search against.
@param		comparator_tp	A pointer to the tuple to evaluate the
				expression over.
@param		comparator_hp	A pointer to the relation header info for
				the relation the tuple pointed at by
				@p comparator_tp.
@param		mmp		A pointer to the memory manager instance being
				used to allocate memory for this query.
@returns	@c -1 if no index found, otherwise an integral offset greater
		than or equal to @c 0.
*/
db_index_offset_t db_index_getoffset(scan_t *sp,
				db_uint8 indexon,
				db_eet_t *searchfor,
				db_tuple_t *comparator_tp,
				relation_header_t *comparator_hp,
				db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
