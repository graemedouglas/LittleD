/******************************************************************************/
/**
@author		Graeme Douglas
@file		dberror.h
@brief		A file containing macro definitions for error reporting.
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

#ifndef DBERROR_H
#define DBERROR_H

#include "db_ctconf.h"

/**
@brief		Print error message.
@param		message		The error to print.
@param		offset		The offset of the token or character that lead
				to the error.
@param		command		The command that caused the error.
*/
#ifndef DB_ERROR_MESSAGE
#if defined(DB_CTCONF_SETTING_FEATURE_ERROR_MESSAGES) && 0!=DB_CTCONF_SETTING_FEATURE_ERROR_MESSAGES
#define DB_ERROR_MESSAGE(message, offset, command) \
do { \
printf("ERROR AT COLUMN %d OF \"%s\":\n\t%s\n", (offset), (command), (message)); \
} while(0)
#else
#define DB_ERROR_MESSAGE(message, offset, command)
#endif
#else
#error "MACRO NAME CLASH ON DB_ERROR_MESSAGE!"
#endif

#endif
