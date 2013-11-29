/******************************************************************************/
/**
@file		db_types.h
@author		Graeme Douglas
@brief		Header file containing type information to be used throughout
		the database.
@details	This file relies on pstdint.h for definitions of 8, 16, and
		32 bit integers, except for certain target platforms.
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

#ifndef DB_TYPES_H
#define DB_TYPES_H

#include "db_ctconf.h"
#ifdef DB_CTCONF_SETTING_TARGET
	#if DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_CONTIKI
		#include <stdint.h>
	#elif DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_ARDUINO
		#include <stdint.h>
	#elif defined GNUC || defined __llvm__ || defined __clang__
		#include <stdint.h>
	#else
		#include "pstdint.h"
	#endif
#else
	#error "CONFIGURATION FILE MUST HAVE DB_CTCONF_SETTING_TARGET DEFINED!"
#endif

/*** Fixed sized integers. ***/
/* 1 byte signed integers. */
/**
@brief		One byte signed integer.
*/
typedef int8_t db_int8;

/**
@brief		Maxmimum value for a one-byte signed integer.
*/
#ifndef DB_INT8_MAX
#define DB_INT8_MAX 0x7F
#endif

/**
@brief		Minimum value for a one-byte signed integer.
*/
#ifndef DB_INT8_MIN
#define DB_INT8_MIN 0x80
#endif


/* 2 byte signed integers. */
/**
@brief		Two byte signed integer.
*/
typedef int16_t db_int16;

/**
@brief		Maxmimum value for a two-byte signed integer.
*/
#ifndef DB_INT16_MAX
#define DB_INT16_MAX 0x7FFF
#endif

/**
@brief		Minimum value for a two-byte signed integer.
*/
#ifndef DB_INT16_MIN
#define DB_INT16_MIN 0x8000
#endif


/* 4 byte signed integers. */
/**
@brief		Four byte signed integer.
*/
typedef int32_t db_int32;

/**
@brief		Maxmimum value for a four-byte signed integer.
*/
#ifndef DB_INT32_MAX
#define DB_INT32_MAX 0x7FFFFFFF
#endif

/**
@brief		Minimum value for a four-byte signed integer.
*/
#ifndef DB_INT32_MIN
#define DB_INT32_MIN 0x80000000
#endif


/* 1 byte unsgined integers. */
/**
@brief		One byte unsigned integer.
*/
typedef uint8_t db_uint8;

/**
@brief		Maxmimum value for a one-byte unsigned integer.
*/
#ifndef DB_UINT8_MAX
#define DB_UINT8_MAX 0xFF
#endif

/**
@brief		Minimum value for a one-byte unsigned integer.
*/
#ifndef DB_UINT8_MIN
#define DB_UINT8_MIN 0x00
#endif


/* 2 byte unsgined integers. */
/**
@brief		Two byte unsigned integer.
*/
typedef uint16_t db_uint16;

/**
@brief		Maxmimum value for a two-byte unsigned integer.
*/
#ifndef DB_UINT16_MAX
#define DB_UINT16_MAX 0xFFFF
#endif

/**
@brief		Minimum value for a two-byte unsigned integer.
*/
#ifndef DB_UINT16_MIN
#define DB_UINT16_MIN 0x0000
#endif


/* 4 byte unsgined integers. */
/**
@brief		Four byte unsigned integer.
*/
typedef uint32_t db_uint32;

/**
@brief		Maxmimum value for a four-byte unsigned integer.
*/
#ifndef DB_UINT32_MAX
#define DB_UINT32_MAX 0xFFFFFFFF
#endif

/**
@brief		Minimum value for a two-byte unsigned integer.
*/
#ifndef DB_UINT32_MIN
#define DB_UINT32_MIN 0x00000000
#endif


/*** Variable sized integers. ***/
/* The general db_int type.  The size of this type is allowed to change with
 * the size of integers on the target system. */
/**
@brief		Variably sized signed integer.
*/
typedef int db_int;

/**
@brief		Maximum value for platform specific integer.
*/
#ifndef DB_INT_MAX
#if INT_MAX == 0x7F
#	define DB_INT_MAX 0x7F
#elif INT_MAX == 0x7FFF
#	define DB_INT_MAX 0x7FFF
#elif INT_MAX == 0x7FFFFFFF
#	define DB_INT_MAX 0x7FFFFFFF
#else
#ifdef _PSTDINT_H_INCLUDED
#	error "Integer size unsupported"
#else
#	define DB_INT_MAX INT_MAX
#endif
#endif
#endif

/**
@brief		Minimum value for platform-specific integer.
*/
#ifndef DB_INT_MIN
#if INT_MAX == 0x7F
#	define DB_INT_MIN 0x80
#elif INT_MAX == 0x7FFF
#	define DB_INT_MIN 0x8000
#elif INT_MAX == 0x7FFFFFFF
#	define DB_INT_MIN 0x80000000
#else
#ifdef _PSTDINT_H_INCLUDED
#	error "Integer size unsupported"
#else
#	define DB_INT_MIN INT_MIN
#endif
#endif
#endif
/* A note about database strings
 ** I find it totally unecessary to define database strings on several grounds.
 ** char's are entirely universal.  Secondly, strings in C are really character
 ** arrays / pointers, so typedef'ing any string will inevitably lead to
 ** array size confusion.
*/

#endif
