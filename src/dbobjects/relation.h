//*****************************************************************************
/**
@file		relation.h
@author		Graeme Douglas
@brief		Method prototype and structure definitions for managing
		relations.
@details	Conceptually, relations can be files or tables on stable
		stable storage, or they could also be the result of some
		operator, despite the fact all operators work in a "tuple
		at a time" fashion.  Some operators, such as projections,
		aggregates, and joins, will even create there own new schemas
		(headers).
		
		Some strong assumptions are made.  Memory footprint of any
		tuple must not exceed 255 bytes.  There cannot be more than 255
		attributes.  The size of an attribute cannot exceed 255 bytes.
		An attributes name cannot be longer than 255 bytes (including
		terminator).
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

#ifndef RELATION_H
#define RELATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "../ref.h"
#include "../dbstorage/dbstorage.h"
#include "../dbmm/db_query_mm.h"

/* Relation header (AKA schema information). */
/**
@struct relation_header_t
@brief Referable representation of a relation's schema.
*/
typedef struct
{
	/*@{*/
        db_uint8 num_attr;	/**< The number of attributes in relation. */
	db_uint8 *size_name;	/**< Size of the attribute's name in bytes,
				     including the terminator. */
        char **names;		/**< Array of attribute names. */
        db_uint8 *types;	/**< Array of attribute types. */
        db_uint8 *offsets;	/**< Array of attribute offsets, in bytes. @todo remove this, do calculation instead. */
        db_uint8 *sizes;	/**< Array of attribute sizes, in bytes. */
	db_uint8 tuple_size;	/**< Size of a tuple, in bytes. */
	/*@}*/
} relation_header_t;

/*** Methods for retrieving positions/offsets of a relations attributes ***/
/* Returns the position of a relation's attribute given its ordered name. */
/** 
@brief 		Returns the position of a relation's attribute given the
		attributes name.
@details 	This function will return a position to an attribute given
		the attribute name.  This is not preferable, since it is
		possible for an attribute to have no name.
@param		hp		Pointer to schema information for the relation
				of the attribute to find the position of.
@param		attr_name	String name of the attribute whose position is
				desired.
@return		The position of the attribute whose name is @c attr_name.
*/
db_int getposbyname(relation_header_t *hp,	/* Pointer to relation header
						   structure. */
		char* attr_name);		/* Name of attribute to return
						   position of. */

/* Returns the offset of a relation's attribute given its ordered position. */
/**
@brief		Returns offset of an attribute based on its ordered position in
		the relations schema.
@param		hp		Pointer to the schema of the relation.
@param		pos		Position of the attribute whose offset we
				desire.  Should not be less than @c 0 or
				greater than @c hp->num_attr.		
@return		Byte-based offset of the attribute at ordered position @c pos.
@todo		Change this to use sums instead of storing offsets.
*/
db_uint8 getoffsetbypos(relation_header_t *hp,	/* Pointer to relation header
						   structure. */
		db_int pos);			/* Position of attribute to get
						   byte offset of. */

/* Returns the offset of a relation's attribute given its name. */
/**
@brief		Retrieves offset of an attribute based on its name in.
@param		hp		Pointer to the schema of the relation.
@param		attr_name	String name of the attribute whose offset is
				desired.
@return		Byte-based offset of the attribute at ordered position @c pos.
@todo		Change this to use sums instead of storing offsets.
*/
db_uint8 getoffsetbyname(relation_header_t *hp,/* Pointer to relation header
						   structure. */
		char* attr_name);		/* Name of attribute to return
	       					   position of. */

/* Return the type of an attribute given its position in tuple. */
/**
@brief		Retrieves type of an attribute based on its name in.
@param		hp		Pointer to the schema of the relation.
@param		pos		Position of the attribute whose offset is
				desired.  Position should be no less than @c 0
				and no more than @c hp->num_attr.
@return		Type of attribute at ordered position @c pos in relation.
*/
db_uint8 gettypebypos(relation_header_t *hp,	/* Pointer to relaiton header
						   structure. */
		db_int pos);			/* Position of attribute to
						   return the type of. */

/* Get relation header information for a relation. */
/**
@brief		Create in memory copy of relation meta data information.
@param		hpp		Pointer to a relation schema pointer.
				The pointer being pointed to is that which
				should be set to point to the newly created
				schema information.
@param		relationname	The name of the relation to generate
				schema information for.
@param		mmp		A pointer to the per-query memory manager
				instance that will be used to allocate
				memory from.
@returns	@c 1 if the schema information was generated correctly,
		@c 0 otherwise.
*/
db_int getrelationheader(relation_header_t **hpp,
		char *relationame,
		db_query_mm_t *mmp);

/* Remove relation header information from memory. */
/**
@brief		Remove the relation header information from memory.
@param		hp		The pointer to the schema information
				to be freed.
@param		mmp		The per-query memory manager used to allocated
				memory for the schema data.
@returns	@c 1 if the schema information was removed correctly,
		@c 0 otherwise.
*/
db_int freerelationheader(relation_header_t *hp,
		db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
