/******************************************************************************/
/**
@file		dbcreate.h
@author		Graeme Douglas
@brief		The databases create functions.
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

#ifndef DBCREATE_H
#define DBCREATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../ref.h"
#include "../db_ctconf.h"
#include "dblexer.h"

/**
@brief		Process the different CREATE statements for the database.
@param		lexerp		A pointer to the lexer instance variable
				that is generating the token stream.
@param		end		The first index of a character in the
				command being lexed not in this statement.
@param		mmp		The per-query memory manager being used
				to allocate memory for this statement.
@returns	@c 1 if the statement was successful, @c -1 if an error occured,
		@c 0 otherwise.
*/
db_int processCreate(db_lexer_t *lexerp, db_int end, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
