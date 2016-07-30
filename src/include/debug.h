//*****************************************************************************
/**
@file		debug.h
@author		Andrii Kuplevakhskyi
@brief		Debug utlities (printf() etc.). The utilities can be
                enabled or disabled

@details
@copyright	Copyright 2016 Andrii Kuplevakhskyi
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

#include <stdio.h>

#ifndef DEBUG_H
#define DEBUG_H

#ifdef ENABLE_DEBUG
 #define DEBUG_ENABLED (1)
#else // ENABLE_DEBUG
 #define DEBUG_ENABLED (0)
#endif //ENABLE_DEBUG


#define PRINTF(...) \
    do \
    { \
        if (0 != DEBUG_ENABLED) \
        { \
            printf(__VA_ARGS__); \
        } \
    } while (1 == 0)


#endif // DEBUG_H

