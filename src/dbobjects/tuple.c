/******************************************************************************/
/**
@file		tuple.c
@author		Graeme Douglas
@brief		Implementation of in memory tuple handling.
@see		For more information, refer to @ref tuple.h.
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

#include "tuple.h"

/*** Methods for retrieving information from a tuple. */
/* Retrieve an db_int from a tuple given its attribute name. */
db_int getintbyname(db_tuple_t *tp, char *attr_name, relation_header_t *hp)
{
	db_uint8 offset = getoffsetbyname(hp, attr_name);
	/* Convert char pointer to db_int pointer so we can return db_int
	   value. */
	return *((db_int*)(&(tp->bytes[offset])));
}

/* Retrieve an db_int from a tuple given its attribute position. */
db_int getintbypos(db_tuple_t *tp, db_int pos, relation_header_t *hp)
{
	db_uint8 offset = getoffsetbypos(hp, pos);

	/* Convert char pointer to db_int pointer so we can return
	   db_int value. */
	return *((db_int*)(&(tp->bytes[(db_int)offset])));
}

/* Retrieve a string from a tuple given its attribute name. */
char* getstringbyname(db_tuple_t *tp, char *attr_name, relation_header_t *hp)
{
	db_uint8 offset = getoffsetbyname(hp, attr_name);

	/* Convert and return correct pointer types. */
	return (char*)(&(tp->bytes[(db_int)offset]));
}

/* Retrieve a string from a tuple given its attribute position */
char* getstringbypos(db_tuple_t *tp, db_int pos, relation_header_t *hp)
{
	db_uint8 offset = getoffsetbypos(hp, pos);
	/* Convert char pointer to db_int pointer so we can return db_int
	   value. */
	return (char*)(&(tp->bytes[(db_int)offset]));
}

/* Retrieve a void pointer from attribute at position pos. */
void* getvoidpbypos(db_tuple_t *tp, db_int pos, relation_header_t *hp)
{
	db_uint8 offset = getoffsetbypos(hp, pos);

	/* Convert and return void pointer. */
	return (void*)(&(tp->bytes[(db_int)offset]));
}

void copytuplebytes(db_tuple_t *to, db_tuple_t *from, int tstart, int fstart,
		int howmany)
{
	howmany += tstart;
	for (; tstart < howmany; ++tstart, ++fstart)
	{
		to->bytes[tstart] = from->bytes[fstart];
	}
}
void copytupleisnull(db_tuple_t *to, db_tuple_t *from, int tstart, int fstart,
		int howmany)
{
	howmany += tstart;
	for (; tstart < howmany; ++tstart, ++fstart)
	{
		to->isnull[tstart] = from->isnull[fstart];
	}
}

/* Create a tuple */
db_int init_tuple(db_tuple_t *tp, db_uint8 tuple_size, db_uint8 num_attr,
		db_query_mm_t *mmp)
{
	/* Determine minimum number of bytes needed to create a bit vector
	   num_attr bits. */
	db_int toalloc = ((db_int)num_attr) / 8;
	if (((db_int)num_attr) % 8 > 0)
		toalloc++;
	
	tp->bytes = DB_QMM_BALLOC(mmp, SIZE_BYTE*((size_t)tuple_size));
	tp->isnull = DB_QMM_BALLOC(mmp, SIZE_BYTE*((size_t)toalloc));
	
	/* Write 0's in bit array to avoid confusion with garbage. */
	db_int i = 0;
	for (; i < toalloc; i++)
	{
		tp->isnull[i] = 0;
	}
	
	//printf("tp->bytes (after malloc): %p\n", tp->bytes);
	return 0;
}

/* Destroy a tuple, specifically its bytes */
db_int close_tuple(db_tuple_t *tp, db_query_mm_t *mmp)
{
	DB_QMM_BFREE(mmp, tp->bytes);
	DB_QMM_BFREE(mmp, tp->isnull);
	
	tp->bytes = NULL;
	tp->isnull = NULL;
	
	return 0;
}
