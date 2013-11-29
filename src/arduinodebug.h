/******************************************************************************/
/**
@file		arduinodebug.h
@author		Graeme Douglas
@brief		Header containing arduino debug message code.
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
#ifndef ARDUINODEBUG_H
#define ARDUINODEBUG_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief		Print a string.
@param		str	The array of characters to be printed.
*/
void ad_printstr(char *str);

/**
@brief		Print an int in decimal format.
@param		i	The integer to be printed.
*/
void ad_printint(int i);

/**
@brief		Print an int in hexadecimal format.
@param		i	The integer to be printed.
*/
void ad_printint_hex(int i);

/**
@brief		Print an int in binary format.
@param		i	The integer to be printed.
*/
void ad_printint_bin(int i);

/**
@brief		Print a character.
@param		i	The integer to be printed.
*/
void ad_printchar(char c);

#ifdef __cplusplus
}
#endif

#endif
