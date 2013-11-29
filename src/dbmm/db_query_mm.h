/******************************************************************************/
/**
@file		db_query_mm.h
@author		Graeme Douglas
@brief		The interface for the database's per query memory manager.
@details	This memory manager takes a stack-based allocation approach.
		The idea is to enforce a policy where we allocate memory
		strictly in consecutive bytes, and free memory from the end of
		the stack as often as we can.  In fact, we will use two stacks:
		one whose top is moving towards the end of the memory segment,
		called the "front", and one whose top is moving towards the
		beginning of the memory segment, called the "back".  This gives
		the parser and execution engine sdome flexibility as it
		allocates memory.
		
		Further, the memory manager can tolerate pointers being
		freed in an "incorrect order."  This gives the database
		even more flexibility, as needed.
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

#ifndef DB_QUERY_MM_H
#define DB_QUERY_MM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "../ref.h"
#include "../db_ctconf.h"

/* Query memory manager struct containing all relevent information. */
// TODO: To solve pointer checking issue, store total front and back alloc'd, check this instead.
/**
@struct		db_query_mm_t;
@brief		The control structure for the per-query memory manager.
*/
typedef struct
{
	void 	*segment;	/**< The memory segment that is designated for
				     this query to have control over. */
	db_int	size;		/**< The size of the segment, in bytes. */
	void	*next_front;	/**< The next free memory address of the front
				     stack. */
	void	*last_back;	/**< The last address use by the back stack. */
	db_int8	errcode;	/**< An error code variable that can be checked
				     when we mangle instead of manage. Error
				     codes are:
				      -1: No memory left to allocate. (Front
				          to back collision).
				      -2: Size exceeds size of segment.
				      -3: Attempted to free space not at the top
				          of the front stack.
				      -4: Attempted to free space not at the top
				          of the bottom stack.
				      -5: Attempted to free fragmented space.
				      -6: Attempted to free memory that has not
				          been allocated.
				      -7: Attempted to extend memory that has
				          not been allocated.
				@todo: Write a real enum for these values!
				*/
#if defined(DB_CTCONF_PROFILE_MAXMEM) && DB_CTCONF_PROFILE_MAXMEM == 1
	db_int maxused;		/* Profile the maximum amount of memory used. */
#endif
} db_query_mm_t;

/* Initialize the query memory manager instance. */
/**
@brief		Initialize an instance of the per-query memory manager.
@param		mmp		Pointer to the memory manager control structure
				to inialize.
@param		segment		Pointer to the memory segment that is designated
				for this instace of the per-query memory manager.
@param		size		The size of @p segment, in bytes.
@returns	@c -1 if any error occurs, @c 1 otherwise.
*/
db_int init_query_mm(db_query_mm_t *mmp,
		void *segment,
		db_int size);

/*** Memory allocation methods. ***/
/* Allocating on the front stack. */
/**
@brief		Allocate memory on the front stack.
@param		mmp		Pointer to the memory manager control structure
				for this instace of the per-query memory
				manager.
@param		size		The amount of memory to allocate.  Note that
				more memory than what is requested is actually
				consumed in order to properly manage the memory.
@returns	@c NULL if the request cannot be satsified, otherwise a pointer
		to a region of memory of the size that has been requested.
*/
void* db_qmm_falloc(db_query_mm_t *mmp,
		db_int size);

/* Allocating on the back stack. */
/**
@brief		Allocate memory on the back stack.
@param		mmp		Pointer to the memory manager control structure
				for this instace of the per-query memory
				manager.
@param		size		The amount of memory to allocate.  Note that
				more memory than what is requested is actually
				consumed in order to properly manage the memory.
@returns	@c NULL if the request cannot be satsified, otherwise a pointer
		to a region of memory of the size that has been requested.
*/
void* db_qmm_balloc(db_query_mm_t *mmp,
		db_int size);

/*** Deallocation methods. ***/
/* Free memory off the front stack. */
/**
@brief		Free memory from the front stack.
@details	If the pointer requested to be freed isn't to a section that is
		at the top of the stack but is at the front of a lower allocated
		section, then the section of memory becomes "fragmented", and
		will be freed whenever the section immediately above it is
		successfully freed.
@param		mmp		Pointer to the memory manager control structure
				for this instace of the per-query memory
				manager.
@param		ptr		Pointer to the beginning of the memory region
				that is to be freed.  This must be the same
				pointer that was orignally allocated.
@returns	@c 1 if the memory was successfully freed, @c -1 if there was a
		critical error, and @c 0 if the memory was not freed but instead
		fragmented.
*/
db_int db_qmm_ffree(db_query_mm_t *mmp,
		void *ptr);

/* Free memory off the back stack. */
/**
@brief		Free memory from the back stack.
@details	If the pointer requested to be freed isn't to a section that is
		at the top of the stack but is at the front of a lower allocated
		section, then the section of memory becomes "fragmented", and
		will be freed whenever the section immediately above it is
		successfully freed.
@param		mmp		Pointer to the memory manager control structure
				for this instace of the per-query memory
				manager.
@param		ptr		Pointer to the beginning of the memory region
				that is to be freed.  This must be the same
				pointer that was orignally allocated.
@returns	@c 1 if the memory was successfully freed, @c -1 if there was a
		critical error, and @c 0 if the memory was not freed but instead
		fragmented.
*/
db_int db_qmm_bfree(db_query_mm_t *mmp,
		void *ptr);

/* Extend the memory on the front stack. */
/**
@brief		Extend the top section of memory on the front stack.
@details	There are moments when we will want to have more memory in the
		top segment of memory than we currently allow for.  We handle
		this by simply extending its size.  This requires no additional
		overhead and guarantees that the data currently present in the
		section will be preserved.
@param		mmp		Pointer to the instance of the per-query memory
				manager that the segment desired to be extended
				belongs to.
@param		size		The number of bytes we wish to extend the top
				most section of memory on the front stack by.
@returns	@c 1 if the memory segment was successfully extended, @c -1
		otherwise.
*/
db_int db_qmm_fextend(db_query_mm_t *mmp,
		db_int size);

/* Extend the most recently allocated memory segment on back stack. */
/**
@brief		Extend the top section of memory on the back stack.
@details	This method is meant to extend the top-most section of memory
		on the back stack.  This is useful when we want to use a "stack"
		datastructure that is only meant to be temporary.  This means
		that the stack can dynamically grow without necessarily knowing
		how big it will end up being when we start.  This also allows
		for efficient memory management in such cases.
		
		One should be warned, however, not to abuse this feature.
		Remember that the entire extended section, including the part
		previously allocated, will be deallocated when freed.  Use this
		feature wisely!
@param		mmp		Pointer to the instance of the per-query memory
				manager that the segment desired to be extended
				belongs to.
@param		size		The number of bytes we wish to extend the top
				most section of memory on the back stack by.
@returns	Pointer to extended memory if successful, @c NULL otherwise.
*/
void* db_qmm_bextend(db_query_mm_t *mmp, db_int size);

/* Deallocate all memory on front stack. */
/**
@brief		Deallocate all memory allocated on front stack.
@param		mmp		Pointer to the instance of the per-query memory
				manager whose front stack is to be cleared.
*/
void db_qmm_fclear(db_query_mm_t *mmp);

/* Deallocate all memory on front stack. */
/**
@brief		Deallocate all memory allocated on back stack.
@param		mmp		Pointer to the instance of the per-query memory
				manager whose back stack is to be cleared.
*/
void db_qmm_bclear(db_query_mm_t *mmp);

/* Macro to get size of top memory chunk on back stack. */
/**
@brief		Get the size of the top memory chunk on back stack.
@param		mmp		Pointer to a per-query memory manager.
*/
#ifndef DB_QMM_SIZEOF_BTOP
#define DB_QMM_SIZEOF_BTOP(mmp) *((db_int*)(((unsigned char*)((mmp)->last_back))))
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_SIZEOF_BTOP\""
#endif

/* Macro to get size of a memory chunk on back stack. */
/**
@brief		Get the size of any chunk on back stack.
*/
#ifndef DB_QMM_SIZEOF_BCHUNK
#define DB_QMM_SIZEOF_BCHUNK(p) *((db_int*)(((unsigned char*)(p))-sizeof(db_int)))
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_SIZEOF_BCHUNK\""
#endif

/* Macro to get size of top memory chunk on front stack. */
/**
@brief		Get the size of the top memory chunk on front stack.
@param		mmp		Pointer to a per-query memory manager.
*/
#ifndef DB_QMM_SIZEOF_FTOP
#define DB_QMM_SIZEOF_FTOP(mmp) (*((db_int*)(*(void**)(((unsigned char*)((mmp)->next_front))-sizeof(void*)))))
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_SIZEOF_FTOP\""
#endif

/* Macros to allocate space either using malloc if memory manager pointer is
   NULL or using the memory manager. If using memory manager, goes on front
   stack */
/**
@brief		Allocate space using either the memory manager pointed to by
		@p m, on the front stack, or if @p m is NULL using system
		@c malloc().
@param		m		Pointer to a per-query memory manager.
@param		s		The number of bytes to be allocated.
*/
#ifndef DB_QMM_FALLOC
#ifndef DB_USEMALLOC
#define DB_QMM_FALLOC(m, s) \
(void*)0 != (m) ? db_qmm_falloc((m), (s)) : malloc((s))
#else
#define DB_QMM_FALLOC(m, s) malloc((s))
#endif
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_FALLOC\""
#endif

/* Macros to allocate space either using malloc if memory manager pointer is
   NULL or using the memory manager. If using memory manager, goes on back
   stack */
/**
@brief		Allocate space using either the memory manager pointed to by
		@p m, on the back stack, or if @p m is NULL using system
		@c malloc().
@param		m		Pointer to a per-query memory manager.
@param		s		The number of bytes to be allocated.
*/
#ifndef DB_QMM_BALLOC
#ifndef DB_USEMALLOC
#define DB_QMM_BALLOC(m, s) \
(void*)0 != (m) ? db_qmm_balloc((m), (s)) : malloc((s))
#else
#define DB_QMM_BALLOC(m, s) malloc((s))
#endif
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_BALLOC\""
#endif

/* Macros to de-allocate space either using free if memory manager pointer is
   NULL or using the memory manager. If using memory manager, frees off front
   stack. */
/**
@brief		De-allocate space using either the memory manager pointed to by
		@p m, off the front stack, or if @p m is NULL using system
		@c free().
@param		m		Pointer to a per-query memory manager.
@param		p		Pointer to the memory section to free.
*/
#ifndef DB_QMM_FFREE
#ifndef DB_USEMALLOC
#define DB_QMM_FFREE(m, p) \
(void*)0 != (m) ? db_qmm_ffree((m), (p)) : free((p))
#else
#define DB_QMM_FFREE(m, p) free((p))
#endif
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_FALLOC\""
#endif

/* Macros to de-allocate space either using free if memory manager pointer is
   NULL or using the memory manager. If using memory manager, frees on back
   stack. */
/**
@brief		De-allocate space using either the memory manager pointed to by
		@p m, off the back stack, or if @p m is NULL using system
		@c free().
@param		m		Pointer to a per-query memory manager.
@param		p		Pointer to the memory section to free.
*/
#ifndef DB_QMM_BFREE
#ifndef DB_USEMALLOC
#define DB_QMM_BFREE(m, p) \
(void*)0 != (m) ? db_qmm_bfree((m), (p)) : free((p))
#else
#define DB_QMM_BFREE(m, p) free((p))
#endif
#else
#error "FATAL ERROR: MACRO NAME CLASH ON \"DB_QMM_FALLOC\""
#endif

#ifdef __cplusplus
}
#endif

#endif
