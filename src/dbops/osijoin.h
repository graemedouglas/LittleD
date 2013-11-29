/******************************************************************************/
/**
@file		osijoin.h
@author		Graeme Douglas
@brief		One-sided index join.
@details	A join that requires one of the two input relations
		be indexed.
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

#ifndef OSIJOIN_H
#define OSIJOIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../dbindex/dbindex.h"
#include "db_ops_types.h"
#include "osijoin.h"
#include "db_ops.h"
#include "../dblogic/eet.h"

/**
@brief		Check if the right child is indexed.
*/
#ifndef DB_OSIJOIN_RINDEXED
#define DB_OSIJOIN_RINDEXED(bitinfo) \
((bitinfo) & (1))
#else
#error "MACRO NAME CLASH ON DB_OSI_RINDEXED"
#endif

/**
@brief		Update if there are more tuples in the unindexed relation.
*/
#ifndef DB_OSIJOIN_UPDATE_UMORE
#define DB_OSIJOIN_UPDATE_UMORE(bitinfo, updater) \
if (1==(updater)){ \
 ((bitinfo) |= (1 << 1)); \
} else { \
 ((bitinfo) &= ~(1 << 1)); \
}
#else
#error "MACRO NAME CLASH ON DB_OSI_UPDATE_UMORE"
#endif

/**
@brief		Check if the last call to next(..) on the unindexed tuple
		was successful.
*/
#ifndef DB_OSIJOIN_UMORE
#define DB_OSIJOIN_UMORE(bitinfo) \
(((bitinfo) & (1 << 1)) >> 1)
#else
#error "MACRO NAME CLASH ON DB_OSI_UMORE"
#endif

/**
@brief		Update if an indexed value was found.
*/
#ifndef DB_OSIJOIN_UPDATE_IVALID
#define DB_OSIJOIN_UPDATE_IVALID(bitinfo, updater) \
if (-1!=(updater)){ \
 ((bitinfo) |= (1 << 2)); \
} else { \
 ((bitinfo) &= ~(1 << 2)); \
}
#else
#error "MACRO NAME CLASH ON DB_OSI_UPDATE_UMORE"
#endif

/**
@brief		Check if an indexed value was found.
*/
#ifndef DB_OSIJOIN_IVALID
#define DB_OSIJOIN_IVALID(bitinfo) \
(((bitinfo) & (1 << 2)) >> 2)
#else
#error "MACRO NAME CLASH ON DB_OSI_UMORE"
#endif

/* Initialize the operator. */
/**
@brief		Attempt to initialize an osijoin.
@details	Its not really a good idea to use this method as it first
		attempts to initialize an @ref ntjoin_t object, then tries
		to convert it to an @ref osijoin_t object.  If this method
		returns @c 0, either the initialization or the conversion
		could have failed, and you will not not know which.
		The author used it for testing, and for that purpose it
		worked well.  You are better off to call @ref init_ntjoin
		followed by @ref setup_osijoin to handle the errors
		yourself.
@param		jp	A pointer to the join object to initialize.
@param		ep	A pointer to the join expression to initalize on.
@param		lchild	A pointer to the left child of the join operator.
@param		rchild	A pointer to the right child of the join operator.
@param		mmp	A pointer to the memory manager instance variable
			used to allocate memory for this query.
@returns	@c 1 if a one-sided join was properly initialized, @c 0
		otherwise.
@see		For more information on things to be wary of, reference
		@ref setup_osijoin.
*/
db_int init_osijoin(osijoin_t *jp,
		db_eet_t *ep,
		db_op_base_t *lchild,
		db_op_base_t *rchild,
		db_query_mm_t *mmp);

/* Setup a one-sided index-based join from a nested tuple. */
/**
@brief		Convert an nested-tuple join into a one-sided
		index join.
@param		jp	A pointer to an already initialized instance
			of an @ref ntjoin_t object.  If this method
			returns @c 0, it is fine to assume that
			the object will still function as an @ref ntjoin_t
			operator.
@param		mmp	A pointer to the memory manager instance variable
			that will be used for allocating memory in this
			query.
@returns	@c 1 if @c jp was converted, @c 0 otherwise.
*/
db_int setup_osijoin(osijoin_t *jp, db_query_mm_t *mmp);

/* Re-start the operator from the beginning. This assumes the operator has
   been initialized. */
/**
@brief		Rewind an instance of an @ref osijoin_t.
@param		jp	The join operator to be rewound.
@param		mmp	The memory manager being used to allocate memory
			for this query.
@returns	@c 1 if the operator was rewound, @c 0 otherwise.
*/
db_int rewind_osijoin(osijoin_t *jp, db_query_mm_t *mmp);

/**
@brief		Rewind an instance of an @ref osijoin_t.
@param		jp	The join operator to be rewound.
@param		next_tp	A pointer to the initiazlized tuple that will
			store the next result of this operator.
@param		mmp	A pointer to the per-query memory manager being
			used to allocate memory for this query.
@returns	@c 1 if a tuple is computed, @c 0 otherwise.
*/
db_int next_osijoin(osijoin_t *jp, tuple_t *next_tp, db_query_mm_t *mmp);

/* Close the selection operator. */
/**
@brief		Cleanly destroy an instance of an @ref osijoin_t object.
@param		jp	A pointer to the one-sided index join operator
			to destroy.
@param		mmp	A pointer to the per-query memory manager being
			used to allocate memory for this query.
@returns	@c 1 if successful, @c 0 otherwise.
*/
db_int close_osijoin(osijoin_t *jp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
