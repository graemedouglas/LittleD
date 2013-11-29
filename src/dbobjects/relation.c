/******************************************************************************/
/**
@file		relation.c
@author		Graeme Douglas
@brief		Implementation of in-memory relation handling.
@see		For more information, refer to relation.h
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

#include "relation.h"

/*** Methods for retrieving positions and offsets of a relations attributes */
/* Returns the position of a relation's attribute by searching using name. */
db_int getposbyname(relation_header_t *hp, char* attr_name)
{
	db_int toRet = -1; /* By default, return an error code. */
	
	db_int i = 0;
	/* Find the position by linear search. */
	for (i = 0; i < (db_int)hp->num_attr; ++i)
	{
		if (strcmp(hp->names[i], attr_name)==0)
		{
			toRet = i;
			break;
		}
	}
	return toRet;
}

/* Returns offset of an attribute based on its position in the record. */
db_uint8 getoffsetbypos(relation_header_t *hp, db_int pos)
{
	if (pos < 0 || pos >= (db_int)hp->num_attr)
		return -1;      /* Return error condition */
	else
		return hp->offsets[pos];
}

/* Returns the offset of a relation's attribute by searching using name. */
db_uint8 getoffsetbyname(relation_header_t *hp, char* attr_name)
{
	db_int pos = getposbyname(hp, attr_name);
	return getoffsetbypos(hp, pos);
}

/* Return the type of an attribute given its position in tuple. */
db_uint8 gettypebypos(relation_header_t *hp, db_int pos)
{
	return hp->types[pos];
}

db_int getrelationheader(relation_header_t **hpp, char *relationname, db_query_mm_t *mmp)
{
	db_fileref_t relation = db_openreadfile(relationname);
	
	db_filerewind(relation);

	*hpp = DB_QMM_BALLOC(mmp, sizeof(relation_header_t));

	/** Read away header info and store appropriately. ***/
	db_fileread(relation, &((*hpp)->num_attr), 1);
	
	/* Allocate all the appropriate memory space */
	(*hpp)->size_name = DB_QMM_BALLOC(mmp, (*hpp)->num_attr * sizeof(db_uint8));
	(*hpp)->names = DB_QMM_BALLOC(mmp, (*hpp)->num_attr * sizeof(char*));
	(*hpp)->types = DB_QMM_BALLOC(mmp, (*hpp)->num_attr * sizeof(db_uint8));
	(*hpp)->offsets = DB_QMM_BALLOC(mmp, (*hpp)->num_attr * sizeof(db_uint8));
	(*hpp)->sizes = DB_QMM_BALLOC(mmp, (*hpp)->num_attr * sizeof(db_uint8));
	
	db_int i;		/* Looping variable */
	(*hpp)->tuple_size = 0;
	for (i = 0; i < (db_int)((*hpp)->num_attr); i++) 
	{
		/* Read in the size of the ith name. */
		db_fileread(relation, &((*hpp)->size_name[i]), sizeof(db_uint8));
		
		/* Read in the attribute name for the ith attribute */
		(*hpp)->names[i] = DB_QMM_BALLOC(mmp, (*hpp)->size_name[i]*sizeof(char));
		db_fileread(relation, (unsigned char*)(*hpp)->names[i], (db_int)((*hpp)->size_name[i]));
		
		/* Read in the attribute type for the ith attribute */
		db_fileread(relation, &((*hpp)->types[i]), sizeof(db_uint8));
		
		/* Read in the attribute offset for the ith attribute */
		db_fileread(relation, &((*hpp)->offsets[i]), sizeof(db_uint8));
		
		/* Read in the attribute size for the ith attribute */
		db_fileread(relation, &((*hpp)->sizes[i]), sizeof(db_uint8));
		
		(*hpp)->tuple_size += (*hpp)->sizes[i];
	}
	
	db_fileclose(relation);
	
	return 1;
}

db_int freerelationheader(relation_header_t *hp, db_query_mm_t *mmp)
{
	/** Free all the previously allocated memory. */
	/* Free size_name array */
	DB_QMM_BFREE(mmp, hp->size_name);
	
	/* Free all the individual name arrays */
	int i;
	for (i = 0; i < (db_int)(hp->num_attr); i++)
	{
		DB_QMM_BFREE(mmp, hp->names[i]);
	}
	
	/* Free the name array itself */
	DB_QMM_BFREE(mmp, hp->names);
	
	DB_QMM_BFREE(mmp, hp->types);
	
	DB_QMM_BFREE(mmp, hp->offsets);
	
	DB_QMM_BFREE(mmp, hp->sizes);
	
	DB_QMM_BFREE(mmp, hp);
	
	return 1;
}
