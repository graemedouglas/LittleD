/******************************************************************************/
/**
@file		SD_c_iface.cpp
@author		Graeme Douglas
@brief		Wrapper code for Arduino SD file handling.
@details
@see		Reference @ref SD_c_iface.h for more information.
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
#include "SD_c_iface.h"

#include "SD.h"

/**
@brief		A structure that translates a file object to a C-compatible
		struct.
*/
struct _SD_File
{
	File f;		/**< The Arduino SD File object we to use. */
};

int SD_File_Exists(char *filepath)
{
	return (int)(SD.exists(filepath));
}

int SD_File_Open(SD_File **file, char *filepath, unsigned char mode)
{
	if (!file) return 0;
	if (SD_FILE_MODE_READ == mode)
		mode = FILE_READ;
	else if (SD_FILE_MODE_WRITE == mode)
		mode = FILE_WRITE;
	*file      = new struct _SD_File();
	(*file)->f = SD.open(filepath, mode);
	if (!((*file)->f))
	{
		return 0;
	}
	return 1;
}

size_t SD_File_Write(SD_File *file, const uint8_t *buf, size_t size)
{
	return (file) ? file->f.write(buf, size) : 0;
}

int SD_File_Read(SD_File *file, void *buf, uint16_t nbytes)
{
	return (file) ? file->f.read(buf, nbytes) : -1;
}

int SD_File_Seek(SD_File *file, unsigned long pos)
{
	return (file) ? file->f.seek(pos) : 0;
}

// TODO: How do we signal an error here?
unsigned long SD_File_Position(SD_File *file)
{
	return (file) ? file->f.position() : 0;
}

void SD_File_Close(SD_File *file)
{
	if (file) file->f.close();
	delete file;
}

int SD_File_Remove(char *filepath)
{
	return SD.remove(filepath);
}

int SD_File_Begin(uint8_t csPin)
{
	SD.begin(csPin);
}
