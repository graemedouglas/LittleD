/******************************************************************************/
/**
@file		db_aggr_func_codes.h
@author		Graeme Douglas
@brief		This file defines aggregate function codes.
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

#ifndef DB_AGGR_FUNC_CODES_H
#define DB_AGGR_FUNC_CODES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Enumerated type for aggregate function codes.  Aggregate functions on the
   same type should be kept together (all aggregate values on ints should be
   kept together, etc.
   -Important note: Make sure you properly differentiate between COUNTROWS
    (the aggregate function COUNT) and COUNT (the number of enumerated types
    in this file).
 */
/**
@enum		db_aggr_func_code_t
@brief		Enumerated type for database aggregate function code.
*/
typedef enum
{
	DB_AGGR_COUNTROWS = 0,	/**< @c COUNT(...) aggregate function code. */
	DB_AGGR_BAND,		/**< Bitwise @c AND aggregate function code. */
	DB_AGGR_BOR,		/**< Bitwise @c OR aggregate function code. */
	DB_AGGR_BXOR,		/**< Bitwise @c XOR aggregate function code. */
	DB_AGGR_SUM,		/**< @c SUM(...) aggregate function code. */
	DB_AGGR_MAX,		/**< @c MAX(...) aggregate function code. */
	DB_AGGR_MIN,		/**< @c MIN(...) aggregate function code. */
	DB_AGGR_FIRST,		/**< @c FIRST(...) aggregate function code. */
	DB_AGGR_LAST,		/**< @c LAST(...) aggregate function code. */
	DB_AGGR_AVG_DBINT,	/**< @c AVG(...) aggregate function code. */
	DB_AGGR_STDDEVPOP,	/**< @c STDDEVPOP(...) (population standard
				     deviation) aggregate function code. */
	DB_AGGR_STDDEVSAM,	/**< @c STDDEVSAM(...) (sample standard
				     deviation) aggregate function code. */
	DB_AGGR_VARPOP,		/**< @c VARPOP(...) (population variance)
				     aggregate function code. */
	DB_AGGR_VARSAM,		/**< @c VARSAM(...) (sample variance) aggregate
				     function code. */
	DB_AGGR_COUNT		/**< @c The number of enumerated values. */
} db_aggr_func_code_t;

#ifdef __cplusplus
}
#endif

#endif
