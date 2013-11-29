/******************************************************************************/
/**
@file		db_query_mm.c
@author		Graeme Douglas
@brief		A stack-based memory manager to be used on a per-query basis.
@see		For more information, refer to @ref db_query_mm.h.
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

#include "db_query_mm.h"
#include "../dbmacros.h"
#include "../db_ctconf.h"

/* Initialize the query memory manager instance. */
db_int init_query_mm(db_query_mm_t *mmp, void *segment, db_int size)
{
	mmp->segment = segment;
	mmp->size = size;
	mmp->next_front = segment;
	mmp->last_back = ((void*)((char*)segment)+(size));
	mmp->errcode = 0;	/* The stable state. */
#if defined(DB_CTCONF_PROFILE_MAXMEM) && DB_CTCONF_PROFILE_MAXMEM == 1
	mmp->maxused = 0;
#endif
	return 1;
}

/*** Memory allocation methods. ***/
/* Allocating on the front stack. */
void* db_qmm_falloc(db_query_mm_t *mmp, db_int size)
{
	/* Check that size requested is valid. */
	if (size > mmp->size)
	{
		mmp->errcode = -2;
		return NULL;
	}
	/* Check that size requested does not cause collision with back
	   stack. */
	else if (POINTERATNBYTES(mmp->next_front, size+sizeof(db_int)+sizeof(void*), void*) > ((void*)mmp->last_back))
	{
		mmp->errcode = -1;
		return NULL;
	}
	/* The request can be satisfied.  We can do our necessary size setting
	   and pointer voodo. */
	*((db_int*)(mmp->next_front)) = size;	/* Set the size. */
	
	/* Set the ptr to return. */
	void* ptr = POINTERATNBYTES(mmp->next_front, sizeof(db_int), void*);
	
	/* Here comes the pointer trickery.  First, move next_front to point
	   at location just after allocated space. */
	mmp->next_front = POINTERATNBYTES(ptr, size, void*);
	
	/* Next, we point this location to the beginning of size value for
	   this allocated space. */
	*((void**)(mmp->next_front)) = POINTERATNBYTES(mmp->next_front, -1*(size + sizeof(db_int)), void*);
	
	/* Finally, increment next_front by one.  Ready to allocate again! */
	mmp->next_front = POINTERATNBYTES(mmp->next_front, sizeof(void*), void*);
	
#if defined(DB_CTCONF_PROFILE_MAXMEM) && DB_CTCONF_PROFILE_MAXMEM == 1
	db_int dist = mmp->size -
			POINTERBYTEDIST(mmp->last_back, mmp->next_front);
	
	if (dist > mmp->maxused)
		mmp->maxused = dist;
#endif
	
	return ptr;
}

/* Allocating on the back stack. */
void* db_qmm_balloc(db_query_mm_t *mmp, db_int size)
{
	/* Check that size requested is valid. */
	if (size > mmp->size)
	{
		mmp->errcode = -2;
		return NULL;
	}
	/* Check that size requested does not cause collision with back
	   stack. */
	else if (POINTERATNBYTES(mmp->next_front, size + sizeof(db_int), void*) >
				((void*)(mmp->last_back)))
	{
		mmp->errcode = -1;
		return NULL;
	}
	/* The request can be satisfied.  We can do our necessary size setting
	   and pointer voodo. */
	
	/* Move the last_back to it's proper loation. */
	mmp->last_back = POINTERATNBYTES(mmp->last_back, -1*(size+sizeof(db_int)), void*);
	
	/* Set the size. */
	*((db_int*)(mmp->last_back)) = size;
	
#if defined(DB_CTCONF_PROFILE_MAXMEM) && DB_CTCONF_PROFILE_MAXMEM == 1
	db_int dist = mmp->size -
			POINTERBYTEDIST(mmp->last_back, mmp->next_front);
	
	if (dist > mmp->maxused)
		mmp->maxused = dist;
#endif
	
	return POINTERATNBYTES(mmp->last_back, sizeof(db_int), void*);
}

/*** Deallocation methods. ***/
/* Free memory off the front stack. */
db_int db_qmm_ffree(db_query_mm_t *mmp, void *ptr)
{
	/* Retrieve the size of the memory chunk we are to free. If it is
	   flagged, remove the flag. */
	db_int size = *POINTERATNBYTES(ptr, -1*sizeof(db_int), db_int*);
	if ((size & (1 << ((8*sizeof(db_int))-1))) ==
		(1 << ((8*sizeof(db_int))-1)))
	{
		size &= ~(1 << ((8*sizeof(db_int))-1));
	}
	
	/* If the memory to free is not in range... */
	if (ptr == NULL
		 || POINTERATNBYTES(ptr, -1*sizeof(db_int), void*) < mmp->segment
		  || POINTERATNBYTES(ptr, size+sizeof(void*), void*)
			> mmp->next_front)
	{
		mmp->errcode = -6;
		return -1;
	}
	
	/* If this is the last chunk of memory on the front stack, free it,
	   and free any chained fragmented sections. */
	else if (((void*)(((char*)ptr)+size+sizeof(void*))) == mmp->next_front)
	{
		mmp->next_front = *((void**)(((char*)ptr)+size));
		/* While the preceding section is also fragmented, continue to
		   free the memory. */
		while ((*((db_int*)(mmp->next_front))
			& (1 << ((8*sizeof(db_int))-1)))
			 == (1 << ((8*sizeof(db_int))-1))
			&& mmp->next_front > mmp->segment)
		{
			mmp->next_front = *(((void**)(mmp->next_front))-1);
		}
		return 1;
	}
	else
	{
		/* Otherwise, set the next nodes previous pointer to the beginning of
	   	   this node. */
		/* Get address of next node. */
		void *temp = POINTERATNBYTES(ptr, size+sizeof(void*), void*);
		
		/* Get the size of the allocated space in this node. */
		size = *((db_int*)temp);
		if ((size & (1 << ((8*sizeof(db_int))-1))) ==
			(1 << ((8*sizeof(db_int))-1)))
		{
			mmp->errcode = -5;
			return -1;
		}
		
		/* Set the next nodes flag in it's size variable. */
		*((db_int*)temp) |= (1 << ((8*sizeof(db_int))-1));
		
		/* Move to pointer at end of the node. */
		temp = POINTERATNBYTES(temp, size + sizeof(db_int), void*);
		
		/* Set its pointer to the beginning of the current node. */
		*((void**)temp) = POINTERATNBYTES(ptr, -1*sizeof(db_int), void*);
		
		/* Set the error code. */
		mmp->errcode = -3;
		return 0;
	}
}

/* Free memory off the front stack. */
db_int db_qmm_bfree(db_query_mm_t *mmp, void *ptr)
{
	/* Retrieve the size of the memory chunk we are to free. If it is
	   flagged, remove the flag. */
	db_int size = *POINTERATNBYTES(ptr, -1*sizeof(db_int), db_int*);
	if ((size & (1 << ((8*sizeof(db_int))-1))) ==
		(1 << ((8*sizeof(db_int))-1)))
	{
		size &= ~(1 << ((8*sizeof(db_int))-1));
	}
	
	/* If the memory to free is not in range... */
	if (ptr == NULL
		 || ptr >= POINTERATNBYTES(mmp->segment, mmp->size, void*)
		  || POINTERATNBYTES(ptr, -1*sizeof(db_int), void*) < mmp->last_back)
	{
		mmp->errcode = -6;
		return -1;
	}
	
	/* If this is the top chunk of memory on the front stack, free it
	   and all lower nodes flagged as fragmented. */
	else if (POINTERATNBYTES(ptr, -1*sizeof(db_int), void*) == mmp->last_back)
	{
		/* While there are sections flagged as fragmented and we aren't
		   at the edge of the memory segment, move stack pointer. */
		do
		{
			mmp->last_back = POINTERATNBYTES(ptr, size, void*);
			
			/* Retrieve the size of the next chunk we are to
			   consider. If it is flagged, remove the flag. */
			size = *((db_int*)(((char*)(mmp->last_back))));
			if ((size & (1 << ((8*sizeof(db_int))-1))) ==
				(1 << ((8*sizeof(db_int))-1)))
			{
				size &= ~(1 << ((8*sizeof(db_int))-1));
			}
			
			/* Advance past size variable, if we aren't already at
			   the end. */
			if (mmp->last_back <
				POINTERATNBYTES(mmp->segment, mmp->size, void*))
			{
				ptr = POINTERATNBYTES(mmp->last_back, sizeof(db_int), void*);
			}
		}
		while((mmp->last_back <
				POINTERATNBYTES(mmp->segment, mmp->size, void*))
			&& (*((db_int*)(mmp->last_back))
				& (1 << ((8*sizeof(db_int))-1))) ==
				(1 << ((8*sizeof(db_int))-1)));
		return 1;
	}
	
	/* Otherwise, set this nodes fragmentation bit to 1. */
	else
	{
		*POINTERATNBYTES(ptr, -1*sizeof(db_int), db_int*) |= 
			(1 << ((8*sizeof(db_int))-1));
		
		/* Set the appropriate error code. */
		mmp->errcode = -4;
		
		/* Return false. */
		return 0;
	}
}

/* Extend last allocation to an additional size, if possible. */
db_int db_qmm_fextend(db_query_mm_t *mmp, db_int size)
{
	/* Check that size requested is valid. */
	if (size > mmp->size)
	{
		mmp->errcode = -2;
		return -1;
	}
	
	/* Check that something has actually been allocated. */
	else if (mmp->next_front == mmp->segment)
	{
		mmp->errcode = -7;
		return -1;
	}
	
	/* Check that size requested does not cause collision with back
	   stack. */
	else if (POINTERATNBYTES(mmp->next_front, size, void*) >
				((void*)(mmp->last_back)))
	{
		mmp->errcode = -4;
		return -1;
	}
	
	/** We can satisfy this request. **/
	/* Change size of this segment to new size, respecting flag bit. */
	db_int tempsize = *((db_int*)(*((void**)((char*)(mmp->next_front)
				-sizeof(void*)))));
	if ((tempsize & (1 << ((8*sizeof(db_int))-1))) ==
		(1 << ((8*sizeof(db_int))-1)))
	{
		/* Temporarilly remove the flag. */
		tempsize &= ~(1 << ((8*sizeof(db_int))-1));
		
		/* Add to get new size. */
		tempsize += size;
		
		/* Retoggle flag-bit. */
		tempsize |= (1 << ((8*sizeof(db_int))-1));
	}
	else
	{
		tempsize += size;
	}
	
	/* Write out the new size. */
	*((db_int*)(*((void**)((char*)(mmp->next_front)-sizeof(void*)))))
		= tempsize;
	
	/* Untoggle size flag again. */
	tempsize &= ~(1 << ((8*sizeof(db_int))-1));
	
	/* Set mmp->next_front. */
	mmp->next_front = POINTERATNBYTES(mmp->next_front, size, void*);
	//mmp->next_front = ((void*)(((char*)mmp->next_front)+size));
	
	/* Setup new free pointer. */
	*POINTERATNBYTES(mmp->next_front, -1*sizeof(void*), void**) =
		POINTERATNBYTES(mmp->next_front, -1*(tempsize+sizeof(db_int)+sizeof(void*)), void*);
	
#if defined(DB_CTCONF_PROFILE_MAXMEM) && DB_CTCONF_PROFILE_MAXMEM == 1
	db_int dist = mmp->size -
			POINTERBYTEDIST(mmp->last_back, mmp->next_front);
	
	if (dist > mmp->maxused)
		mmp->maxused = dist;
#endif
	
	return 1;
}

/* Extend the most recently allocated memory segment on back stack. */
void* db_qmm_bextend(db_query_mm_t *mmp, db_int size)
{
	/* Check that we haven't requested more memory than total size. */
	if (size > mmp->size)
	{
		mmp->errcode = -2;
		return NULL;
	}
	
	/* Check that we have adequate memory. */
	else if (mmp->last_back - mmp->segment < size)
	{
		mmp->errcode = -1;
		return NULL;
	}
	else if (mmp->last_back == POINTERATNBYTES(mmp->segment, mmp->size, void*))
	{
		mmp->errcode = -7;
		return NULL;
	}
	
	/* We can satisfy this request. Get the size of the new segment.
	   Since this is the top most node, cannot be flagged. */
	db_int tempsize = *((db_int*)(mmp->last_back)) + size;
	
	/* Extend segment. */
	mmp->last_back = POINTERATNBYTES(mmp->last_back, -1*size, void*);
	*((db_int*)mmp->last_back) = tempsize;
	
#if defined(DB_CTCONF_PROFILE_MAXMEM) && DB_CTCONF_PROFILE_MAXMEM == 1
	db_int dist = mmp->size -
			POINTERBYTEDIST(mmp->last_back, mmp->next_front);
	
	if (dist > mmp->maxused)
		mmp->maxused = dist;
#endif
	
	return POINTERATNBYTES(mmp->last_back, sizeof(db_int), void*);
}

/* Deallocate all memory on front stack. */
void db_qmm_fclear(db_query_mm_t *mmp)
{
	mmp->next_front = mmp->segment;
}

/* Deallocate all memory on back stack. */
void db_qmm_bclear(db_query_mm_t *mmp)
{
	mmp->last_back = POINTERATNBYTES(mmp->segment, mmp->size, void*);
}
