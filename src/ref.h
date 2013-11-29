/******************************************************************************/
/**
@file		ref.h
@author		Graeme Douglas
@brief		Header file containing definitions used throughout the database.
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

#ifndef REF_H
#define REF_H

#include "db_types.h"
#include "dbmacros.h"

/* This is used to determine the attribute types. */
/**
@enum		attr_type
@brief		Enumerated type for database types.
*/
typedef enum 
{
	DB_INT = 0,		/**< The database's integer type. */
	DB_STRING = 1,		/**< The database's ASCII string / char type. */
	DB_NULL,		/**< The database's NULL type. */
	DB_TYPES_COUNT		/**< The number of database types. */
} attr_type;

/* Type size references */
/**
@brief		The size of a single byte.  @todo Maybe get rid of this?
*/
static const db_int SIZE_BYTE = sizeof(char);

#endif
