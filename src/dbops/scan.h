/******************************************************************************/
/**
@file		scan.h
@author		Graeme Douglas
@brief		The scan operator.
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

#ifndef SCAN_H
#define SCAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../db_ctconf.h"
#include "../ref.h"
#include "db_ops_types.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include "../dbindex/dbindex_types.h"
#include "../db_ctconf.h"
#if defined(DB_CTCONF_SETTING_TARGET) && DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_ARDUINO
#include "../arduinodebug.h"
#endif
#include <stdio.h>
#include <stdlib.h>

/* Initialize the scan operator. */
/**
@brief		Initialize a scan operator.
@param		sp		A pointer to the scan operator to be intialized.
@param		relationName	The name of the relation to be scanned.
@param		mmp		A pointer to the per-query memory manager that
				will be allocate memory for this query.
@returns	@c 1 if the operator was successfully initialized, @c -1 if an
		error occurred.
*/
db_int init_scan(scan_t *sp,
		char* relationName,
		db_query_mm_t *mmp);

/* Rewind a scan operator. */
/**
@brief		Rewind the scan operator.
@see		Reference @ref rewind_dbop for more information.
*/
db_int rewind_scan(scan_t *sp, db_query_mm_t *mmp);

/* Retrieve the next tuple from the relation. */
/**
@brief		Retrieve the next tuple from storage mechanism.
@see		Reference @ref next for more information.
*/
db_int next_scan(scan_t *sp, db_tuple_t *next_tp, db_query_mm_t *mmp);

/* Close scan. */
/**
@brief		Safely deconstruct the scan operator.
@see		Reference @ref close for more inforamtion.
*/
void close_scan(scan_t *sp, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
