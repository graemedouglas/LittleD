/******************************************************************************/
/**
@file		gen_bench_relations.h
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
/******************************************************************************/

#ifndef GEN_BENCH_RELATIONS_H
#define GEN_BENCH_RELATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../ref.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "../dbstorage/dbstorage.h"
#include "../dblogic/eet.h"
#include "../dbindex/dbindex.h"
#include "../db_ctconf.h"
#include <stdlib.h>

/* Initialize a test relation using Contiki Filesystem interface (CFS). */
/**
@brief		Build a test relation for any system.
*/
db_int gen_bench_relation(char *relationname, db_int numattr,
				db_int numtuples, db_int bound, db_int seed, db_int type);

#ifdef __cplusplus
}
#endif

#endif
