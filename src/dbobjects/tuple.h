//*****************************************************************************
/**
@file		tuple.h
@author		Graeme Douglas
@brief		Method prototype and structure definitions for managing
		tuples.
@details	Notes:
		
		-See relation.h and relation.c

		-A tuple is an array of consecutive bytes.
		
		-To interpret the array of bytes, we must have the header
		information.
		
		-Assumption is made that no two attributes in a relation have
		the same name.
		
		-All positions start from 0, NOT 1.
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
//*****************************************************************************

#ifndef TUPLE_H
#define TUPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include "../dbmm/db_query_mm.h"
#include "../ref.h"
#include "relation.h"

/* Tuple structure. */
/**
@struct db_tuple_t
@brief 		In-memory representation of a tuple.
@details	A tuple is just a sequence of bytes that stores information,
		in addition to a bit-array signalling which attributes are
		actually NULL.  This bit-array is actually a byte array as
		well, rounded up to accomodate for the number of bits required.
*/
typedef struct
{
	/*@{*/
	char *isnull;			/**< Bit array (rounded up to nearest
					     byte) where bit @c i indicates
					     whether or not the @c i'th
					     attribute is NULL or not (@c 1
					     indicates a NULL value, @c 0
					     otherwise). */
	char *bytes;			/**< Array of bytes that represents all
					     the values for the tuple.
					*/
	/*@}*/
} db_tuple_t;

/*** Methods for retrieving information from a tuple. */
/* Retrieve a db_int from a tuple given its attribute name. */
/** 
@brief 		Retrieves an integer stored in a tuple from a relation
		given the attribute's name.
@param		tp		Pointer to the tuple from which to retrieve
				integer from.
@param		attr_name	String name of the attribute.
@param		hp		Pointer to schema information for the relation.
@return		The value of the attribute whose name is @c attr_name.
*/
db_int getintbyname(db_tuple_t *tp,	/* Pointer to tuple structure. */
		char *attr_name,	/* Name of attribute to return value
					   of. */
		relation_header_t *hp);	/* Pointer to relation header
					   structure. */

/* Retrieve an db_int from a tuple given its attribute position. */
/** 
@brief 		Retrieves an integer stored in a tuple given its ordered
		position in the tuple.
@param		tp		Pointer to the tuple from which to retrieve
				integer from.
@param		hp		Pointer to schema information for the relation.
@param		pos		Integer position of the attribute.
@return		The integer value of the attribute whose ordered position is 
		@c pos.
*/
db_int getintbypos(db_tuple_t *tp,		/* Pointer to tuple structure. */
		db_int pos,		/* The position of the attribute
					   to return the value of. */
		relation_header_t *hp);	/* Pointer to relation header
					   structure. */

/* Retrieve a string from a tuple given its attribute name. */
/** 
@brief 		Retrieves a string stored in a tuple given the string name
		of the attribute which contains the value.
@param		tp		Pointer to the tuple from which to retrieve
				string from.
@param		attr_name	String name of the attribute.
@param		hp		Pointer to schema information for the relation.
@return		The string value of the attribute whose name is @c attr_name.
*/
char* getstringbyname(db_tuple_t *tp,	/* Pointer to tuple structure. */
		char *attr_name,	/* Name of attribute to return value
					   of. */
		relation_header_t *hp);	/* Pointer of relation header
					   structure. */

/* Retrieve a string from a tuple given its attribute position */
/** 
@brief 		Retrieves a string stored in an attribute from a relation
		given its ordered position within the tuple.
@param		tp		Pointer to the tuple from which to retrieve
				string from.
@param		pos		Integer position of the attribute.
@param		hp		Pointer to schema information for the relation.
@return		The string value of the attribute whose name is @c pos.
*/
char* getstringbypos(db_tuple_t *tp,	/* Pointer to tuple structure. */
		db_int pos,		/* position of the attribute to return
					   the value of. */
		relation_header_t *hp);	/* Pointer to relation header
					   structure. */

/* Retrieve a void pointer from attribute at position pos. */
/** 
@brief 		Retrieves the type of an attribute in a tuple given its ordered
		position within the tuple.
@param		tp		Pointer to the tuple from which to retrieve
				string from.
@param		pos		Integer position of the attribute.
@param		hp		Pointer to schema information for the relation.
@return		The string value of the attribute whose name is @c pos.
*/
void* getvoidpbypos(db_tuple_t *tp,	/* Pointer to tuple structure. */
		db_int pos,		/* Position of the attribute to
					   get the pointer of. */
		relation_header_t *hp);	/* Pointer of relation header
					   structure. */

/**
@brief		Copy bytes from one tuple to another.
@param		to	Pointer to the tuple to copy the bytes to.
@param		from	Pointer to the tuple to copy the bytes from.
@param		tstart	The first byte to overwrite.
@param		fstart	The first byte to read.
@param		howmany	How many bytes to copy.
*/
void copytuplebytes(db_tuple_t *to,
		db_tuple_t *from,
		int tstart,
		int fstart,
		int howmany);

/**
@brief		Copy isnull bytes from one tuple to another.
@param		to	Pointer to the tuple to copy the bytes to.
@param		from	Pointer to the tuple to copy the bytes from.
@param		tstart	The first byte to overwrite.
@param		fstart	The first byte to read.
@param		howmany	How many bytes to copy.
*/
void copytupleisnull(db_tuple_t *to,
		db_tuple_t *from,
		int tstart,
		int fstart,
		int howmany);

/* Create a tuple */
/** 
@brief 		Initializes a tuple.
@details	Initilization of a tuple involves allocating dynamic memory
		for the byte array and the bit-array.  Since memory must be
		allocated for a tuple, a tuple cannot be used before it is
		initialized, and must be closed after it is initialized.
@param		tp		Pointer to the tuple to initialize
@param		tuple_size	The size, in bytes, of the tuple.  This size
				does NOT included the size of the bit array
				for NULL signalling.
@param		num_attr	The number of attributes in the relation.
@param		mmp		A pointer to the memory manager that will be
				used to allocate memory. Pass NULL to use
				@c malloc(..).
@return		1 on success, -1 otherwise.
*/
db_int init_tuple(db_tuple_t *tp,		/* Pointer to tuple structure. */
		db_uint8 tuple_size,	/* The number of bytes to make the
					   tuple. */
		db_uint8 num_attr,	/* The number of attributes in the
					   relation. */
		db_query_mm_t *mmp);

/* Destroy a tuple, specifically its bytes */
/** 
@brief 		Closes a tuple.
@details	De-allocates memory previously allocated for a tuple.
@param		tp		Pointer to the tuple to initialize
@param		mmp		A pointer to the memory manager that will be
				used to de-allocate memory.  Pass NULL
				to use @c free(..).
@return		1 on success, -1 otherwise.
*/
db_int close_tuple(db_tuple_t *tp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
