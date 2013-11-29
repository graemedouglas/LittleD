//*****************************************************************************
/**
@file		cfs_init_test_relations.h
@author		Graeme Douglas
@brief		A quick and dirty program for creating test relations for
		benchmarking the database.
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
//*****************************************************************************

#ifndef CFS_INIT_TEST_RELATIONS_H
#define CFS_INIT_TEST_RELATIONS_H
#include "ref.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "dbstorage.h"
#include "eet.h"
#include "dbindex.h"
#include "db_ctconf.h"

#ifdef DB_CTCONF_SETTING_TARGET
#if DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_STD
#include <stdlib.h>

/* Initialize a test relation, USING THE C FILE HANDLING.  THESE RELATIONS
   PERSIST! */
/**
@brief		Build a test relation using C file handling.
@details	This is more of a "proof of concept" method.  The purpose is to
		debug the code problems here before moving to CFS.
*/
db_int init_fs_test_relation(char *relationname, db_int numattr,
				db_int numtuples, db_int bound, db_int seed);
#endif
#endif

#ifdef DB_CTCONF_SETTING_TARGET
#if DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_CONTIKI
#include "contiki.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
#include "lib/random.h"

/* Initialize a test relation using Contiki Filesystem interface (CFS). */
/**
@brief		Build a test relation using Contiki Filesystem interface.
@details	This builds a randomized relation relation to use during
		testing on devices using CFS.  IF USED IN A SIMULATOR, ANY
		RELATION CREATED WITH THIS METHOD WILL VANISH!
*/
db_int init_cfs_test_relation(char *relationname, db_int numattr,
				db_int numtuples, db_int bound, db_int seed);
#endif
#endif

#endif
