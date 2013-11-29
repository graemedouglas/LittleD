/******************************************************************************/
/**
@file		SD_c_iface.h
@author		Graeme Douglas
@brief		A wrapper around Arduino SD file handling so C code can call
		it.
@details	Since Arduino code is all C++ and this database is written in
		C, we must use a wrapper to use SD files on Arduinos.
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
#ifndef SD_C_IFACE_H
#define SD_C_IFACE_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief		Wrapper around Arduino File type.
*/
typedef struct _SD_File SD_File;

/**
@brief		Arduino SD File write modes.
*/
typedef enum
{
	SD_FILE_MODE_READ  = 0,	/**< Open a file for reading (from start). */
	SD_FILE_MODE_WRITE = 1,	/**< Open a file for reading and writing
				     (from end). */
	SD_FILE_MODE_COUNT	/**< Number of elements in enum. */
} SD_File_Mode_t;

/**
@brief		Check to see if an Arduino SD File exists.
@param		filepath	String containing path to file (basic filename).
@returns	@c 1 if the file exists, @c 0 otherwise.
*/
int SD_File_Exists(char *filepath);

/**
@brief		Wrapper around Arduino SD file write method.
@param		file		Pointer to a pointer of the C file structure
				type associated with the C++ SD file object.
				The pointer pointed to by this pointer
				will be set to the newly allocated object.
@param		filepath	String containing path to file (basic filename).
@param		mode		What mode to open the file under.
@returns	A file for reading, or @c NULL if an error occurred.
*/
int SD_File_Open(SD_File **file, char *filepath, unsigned char mode);

/**
@brief		Wrapper around Arduino SD file write method.
@param		file	Pointer to C file struct type associated with an SD
			file object.
@param		buf	Pointer to the data that is to be written.
@param		size	The number of bytes to be written.
@returns	The number of bytes written.
*/
size_t SD_File_Write(SD_File *file, const uint8_t *buf, size_t size);

/**
@brief		Wrapper around Arduino SD file read method.
@param		file	Pointer to C file struct type associated with an SD
			file object.
@param		buf	Pointer to the memory segment to be read into.
@param		nbytes	The number of bytes to be read.
@returns	A read status.
		(?)
*/
int SD_File_Read(SD_File *file, void *buf, uint16_t nbytes);

/**
@brief		Wrapper around Arduino SD file read method.
@param		file	Pointer to C file struct type associated with an SD
			file object.
@param		pos	The position in the file to seek to.
			(from the beginning?)
@returns	@c 1 for success, @c 0 for failure.
*/
int SD_File_Seek(SD_File *file, unsigned long pos);

/**
@brief		Wrapper around Arduino SD file position method.
@param		file	Pointer to C file struct type associated with an SD
			file object.
@returns	The position of the cursor within the file.
*/
unsigned long SD_File_Position(SD_File *file);

/**
@brief		Wrapper around Arduino SD file close method.
@param		file	Pointer to C file struct type associated with an SD
			file object.
*/
void SD_File_Close(SD_File *file);

/**
@brief		Wrapper around Arduino SD file remove method.
@param		filepath	The string containing the path to the file.
@returns	@c 1 if the file was removed successfully, @c 0 otherwise.
*/
int SD_File_Remove(char *filepath);

/**
@brief		Wrapper around Arduino SD file begin method.
@param		csPin	The pin connected to the chip select line of the SD
			card.
@todo		Is there a safe number to use as a signal to use default
		pin?
*/
int SD_File_Begin(uint8_t csPin);

#ifdef __cplusplus
}
#endif

#endif
