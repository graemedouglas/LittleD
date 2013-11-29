/******************************************************************************/
/**
@file		db_ctconf.h
@author		Graeme Douglas
@brief		Compile time options for the database.
@details	Certain features for the database need to be "mixed in" at
		compile time.  For instance, it would be beneficial to only
		use features that are needed for a given platform or
		application, to reduce binary size, and make includes a whole
		bunch cleaner.  This is the file that controls all of that, and
		must be configured _BEFORE_ compiling the database.
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

#ifndef DB_CTCONF_H
#define DB_CTCONF_H

/**** Configuration Options. ****/
/******************************************************************************/
/*** Options for target platform. ***/
/* Option to compile for standard platform (average PC). */
/**
@def		DB_CTCONF_OPTION_TARGET_STD
@brief		Standard build target platform.
@details	The standard target platform. Should be used for compiling on 
		the average workstation/PC.
*/
#ifndef DB_CTCONF_OPTION_TARGET_STD
#define DB_CTCONF_OPTION_TARGET_STD 0
#endif

/* Option to compile for CONTIKI OS. */
/**
@def		DB_CTCONF_OPTION_TARGET_CONTIKI
@brief		Build target is Contiki OS.
@details	Build the database to run on Contiki OS.  This mostly means
		using CFS-based scan operator instead of binary-file scan.
*/
#ifndef DB_CTCONF_OPTION_TARGET_CONTIKI
#define DB_CTCONF_OPTION_TARGET_CONTIKI 1
#endif

/* Option to compile for ARDUINO. */
/**
@def		DB_CTCONF_OPTION_TARGET_ARDUINO
@brief		Build target is Arduino.
@details	Build the database to run on Arduino.  This means linking
		into the SD.h library to manipulate files.
*/
#ifndef DB_CTCONF_OPTION_TARGET_ARDUINO
#define DB_CTCONF_OPTION_TARGET_ARDUINO 2
#endif
/******************************************************************************/

/**** Configuration Settings. ****/
/******************************************************************************/
/*** Target Setting. ***/
/**
@def		DB_CTCONF_SETTING_TARGET
@brief		Setting determining which platform to compile for.
@details	This setting must be set to one of the options under the
		namespace DB_CTCONF_OPTION_TARGET_.
*/
#ifndef DB_CTCONF_SETTING_TARGET
#define DB_CTCONF_SETTING_TARGET DB_CTCONF_OPTION_TARGET_STD
#endif

/*** Feature settings. ***/
/* Option to enable aggregates. */
/**
@def		DB_CTCONF_SETTING_FEATURE_AGGREGATION
@brief		If @c 1, include aggregates in the database.  Otherwise, don't.
@details	Use this flag to remove the aggregation feature from the
		database.
*/
#ifndef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#define DB_CTCONF_SETTING_FEATURE_AGGREGATION 0
#endif

/* Option to enable aggregates. */
/**
@def		DB_CTCONF_SETTING_FEATURE_SORT
@brief		If @c 1, include aggregates in the database.  Otherwise, don't.
@details	Use this flag to remove the aggregation feature from the
		database.
*/
#ifndef DB_CTCONF_SETTING_FEATURE_SORT
#define DB_CTCONF_SETTING_FEATURE_SORT 0
#endif

/**
@brief		If @c 1, support CREATE TABLE statements.
*/
#ifndef DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
#define DB_CTCONF_SETTING_FEATURE_CREATE_TABLE 1
#endif

/**
@brief		If this is equal to @c 1, error messages will be displayed
		appropriately.
*/
#ifndef DB_CTCONF_SETTING_FEATURE_ERROR_MESSAGES
#define DB_CTCONF_SETTING_FEATURE_ERROR_MESSAGES 1
#endif

/**
@brief		If this is equal to @c 1, maximum amount of memory used during
		a query will be profiled.
*/
#ifndef DB_CTCONF_PROFILE_MAXMEM
#define DB_CTCONF_PROFILE_MAXMEM 1
#endif

/**
@brief		The maximum length of a string atttibute (not including the
		null-byte).
*/
#ifndef DB_CTCONF_SETTING_MAXSTRINGLENGTH
#define DB_CTCONF_SETTING_MAXSTRINGLENGTH 200
#endif
/******************************************************************************/

#endif
