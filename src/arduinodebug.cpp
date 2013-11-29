/******************************************************************************/
/**
@file		arduinodebug.cpp
@author		Graeme Douglas
@brief		C++ code for debug messages on Arduino platform.
@see		For more information, please refer to @ref arduinodebug.h
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

#include "arduinodebug.h"

void ad_printstr(char *str)
{
	Serial.println(str);
	Serial.flush();
}

void ad_printint(int i)
{
	Serial.println(i);
	Serial.flush();
}

void ad_printint_hex(int i)
{
	Serial.println(i, HEX);
	Serial.flush();
}

void ad_printint_bin(int i)
{
	Serial.println(i, BIN);
	Serial.flush();
}

void ad_printchar(char c)
{
	Serial.println(c);
	Serial.flush();
}
