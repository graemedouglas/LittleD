/******************************************************************************/
/**
@author		Graeme Douglas
@file		dbmacros.h
@brief		A place for various macros to reside.
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

#ifndef DBMACROS_H
#define DBMACROS_H

/**
@brief		Macro for moving any pointer some number of bytes cleanly.
@param		d	The destination pointer.  This is the pointer that is
			to be set.
@param		s	The source pointer.  This is the pointer that is to be
			moved.  IT WILL _NOT_ BE SET!
@param		n	The integral number of bytes to move.  May be negative.
@param		t	The pointer type to cast back to.  _Do not include
			include brackets for the type._
@todo		Make it so all calls to this macro actualy use
		@ref POINTERATNBYTES.
*/
#ifndef MOVEPOINTERNBYTES
#define MOVEPOINTERNBYTES(d, s, n, t) (d) = (t)(((unsigned char*)((s)))+(n))
#else
#error "MACRO NAME CLASH ON MOVEPOINTERNBYTES!"
#endif

/**
@brief		Macro for accessing any pointer some number of bytes away
		cleanly.
@param		s	The source pointer.  This is the pointer that is to be
			moved.  IT WILL _NOT_ BE SET!
@param		n	The integral number of bytes to move.  May be negative.
@param		t	The pointer type to cast back to.  _Do not include
			include brackets for the type._
*/
#ifndef POINTERATNBYTES
#define POINTERATNBYTES(s, n, t) ((t)(((unsigned char*)((s)))+(n)))
#else
#error "MACRO NAME CLASH ON POINTERATNBYTES!"
#endif

/**
@brief		Macro for measuring the distance between the two pointers.
@details	Calculates the distance in bytes between p1 and p2.
		If p1 comes after p2, the result shall be positive.
		Keep in mind, this only works within the same array (same
		rules as normal in C).
@param		p1	The pointer to be subtracted from.
@param		p2	The pointer to be subtracted.
*/
#ifndef POINTERBYTEDIST
#define POINTERBYTEDIST(p1, p2) (((unsigned char*)(p1)) - ((unsigned char*)(p2)))
#else
#error "MACRO NAME CLASH ON POINTERBYTEDIST!"
#endif

/**
Macro for moving any pointer @a s by @a n units cleanly by casting it
to type source pointer type @a ts then casting it back to destination
pointer type @td.
@brief		Move a pointer exactly a number of units cleanly.
@param		d	The destination pointer.  This is the pointer that is
			to be set.
@param		s	The source pointer.  This is the pointer that is to be
			moved.  IT WILL _NOT_ BE SET!
@param		n	The integral number of units (_not_ bytes) to move.
			May be negative.
@param		ts	The pointer type to cast the source to before moving.
			_Do not include include brackets for the type._
@param		td	The pointer type to cast the result to before assigning.
			_Do not include include brackets for the type._
*/
#ifndef MOVEPOINTERNUNITS
#define MOVEPOINTERNUNITS(d, s, n, td, ts) (d) = (td)(((ts)((s)))+(n))
#else
#error "MACRO NAME CLASH ON MOVEPOINTERUNITS!"
#endif

#endif
