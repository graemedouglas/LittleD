//*****************************************************************************
/**
@file		cfs_init_test_relations.c
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
//*****************************************************************************

#include "cfs_init_test_relations.h"

db_int integerlength(db_int value)
{
	db_int length = 0;
	if (0 == value)
		length = 1;
	while (value)
	{
		length++;
		value/=10;
	}
	return length;
}

#ifdef DB_CTCONF_SETTING_TARGET
#if DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_STD
/* Initialize a test relation, USING THE C FILE HANDLING.  THESE RELATIONS
   PERSIST! */
db_int init_fs_test_relations(char *relationname, db_int numattr,
				db_int numtuples, db_int bound, db_int seed)
{
	/* General variable declarations. */
	char charWrite;
	db_int intWrite;
	char offset = 0;
	char size = 0;
	FILE *rel1;
	db_int i, j;
	// Allocate enough room for any integer.
	char attrname[5 + integerlength((db_int)DB_INT_MAX)];
	
	/* Seed the random number generator to always generate the same
	   relation. */
	srand(seed);
	/**** Create the new relation. ****/ 
	size = 0;
	offset = 0;
	remove(relationname);
	rel1 = fopen(relationname, "wb");
	
	if (rel1 == NULL)
	{
		perror("Failed to open file.");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = numattr;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/** Header information for all attributes. **/
	for (i = 0; i < numattr; ++i)
	{
		/* Size of name. */
		charWrite = 5 + integerlength(i);
		fwrite(&charWrite, sizeof(char), 1, rel1);
		
		sprintf(attrname, "attr%d", i);
		/* Attribute name. */
		fwrite(attrname, sizeof(char), strlen(attrname), rel1);
		
		/* Attribute type. */
		charWrite = 0;
		fwrite(&charWrite, sizeof(char), 1, rel1);
		
		/* Attribute offset. */
		offset += size;
		fwrite(&offset, sizeof(char), 1, rel1);
		
		/* Attribute size. */
		size = (char)sizeof(db_int);
		fwrite(&size, sizeof(char), 1, rel1);
	}
	
	/*** Write out tuples. ***/
	for (i = 0; i < numtuples; ++i)
	{
		/* Determine size of isnull array. */
		db_int isnull_size = numattr / 8;
		if (0 < numattr % 8)
			isnull_size++;
		/* Write out isnull bit array. */
		for (j = 0; j < isnull_size; ++j)
		{
			charWrite = 0xFF;
			fwrite(&charWrite, sizeof(char), 1, rel1);
		}
		
		/* Write out attributes. */
		for (j = 0; j < numattr; ++j)
		{
			intWrite = rand() % bound;
			fwrite(&intWrite, sizeof(db_int), 1, rel1);
		}
	}
	fclose(rel1);
	return 0;
}
#endif
#endif

#ifdef DB_CTCONF_SETTING_TARGET
#if DB_CTCONF_SETTING_TARGET == DB_CTCONF_OPTION_TARGET_CONTIKI

/* Initialize a test relation using Contiki Filesystem interface. */
db_int init_cfs_test_relations(char *relationname, db_int numattr,
				db_int numtuples, db_int bound, db_int seed, db_int type)
{
	/* General variable declarations. */
	char charWrite;
	db_int intWrite;
	char offset = 0;
	char size = 0;
	int cfs_fd;
	cfs_remove(relationname);
	db_int i, j;
	// Allocate enough room for any integer.
	char attrname[5 + integerlength((db_int)DB_INT_MAX)];
	
	/* Seed the random number generator to always generate the same
	   relation. */
	random_init(seed);
	/**** Create the new relation. ****/ 
	size = 0;
	offset = 0;
	//cfs_coffee_reserve(relationname, 100);
	cfs_fd = cfs_open(relationname, CFS_WRITE);
	if (0 > cfs_fd)
	{
		printf("Could not open file for writing.\n");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = numattr;
	cfs_write(cfs_fd, &charWrite, 1*sizeof(char));
	
	/** Header information for all attributes. **/
	for (i = 0; i < numattr; ++i)
	{
		/* Size of name. */
		charWrite = 5 + integerlength(i);
		cfs_write(cfs_fd, &charWrite, 1*sizeof(char));
		
		sprintf(attrname, "attr%d", i);
		/* Attribute name. */
		cfs_write(cfs_fd, attrname, (strlen(attrname)+1)*sizeof(char));
		
		/* Attribute type. */
		charWrite = 0;
		cfs_write(cfs_fd, &charWrite, 1*sizeof(char));
		
		/* Attribute offset. */
		offset += size;
		cfs_write(cfs_fd, &offset, 1*sizeof(char));
		
		/* Attribute size. */
		size = (char)sizeof(db_int);
		cfs_write(cfs_fd, &size, 1*sizeof(char));
	}
	
	db_int autoid = 0;
	/*** Write out tuples. ***/
	for (i = 0; i < numtuples; ++i)
	{
		/* Determine size of isnull array. */
		db_int isnull_size = numattr / 8;
		if (0 < numattr % 8)
			isnull_size++;
		/* Write out isnull bit array. */
		for (j = 0; j < isnull_size; ++j)
		{
			charWrite = 0x0;
			cfs_write(cfs_fd, &charWrite, 1*sizeof(char));
		}
		
		/* Write out attributes. */
		for (j = 0; j < numattr; ++j)
		{
			if (1 == type && 0==j)
			{
				intWrite = autoid;
				autoid++;
			}
			else
			{
				intWrite = (db_int)((random_rand()) % bound);
			}
			cfs_write(cfs_fd, &intWrite, 1*sizeof(db_int));
		}
	}
	
	cfs_close(cfs_fd);
	/* Write out index data, if necessary. */
	if (1==type)
	{
		/* General variable declaration. */
		long longWrite;
		db_fileref_t file;
		db_eet_t eet;
		db_eetnode_attr_t attr;
		char *tempname[100];
		
		sprintf(tempname, "DB_IDXM_%s", relationname);
		file = db_openwritefile(tempname);
		
		if (file == DB_STORAGE_NOFILE)
		{
			puts("CAN'T OPEN IDXM FILE!");
			return -1;
		}
		
		/* Write out the number of indexes for this relation.  */
		charWrite = 1;
		db_filewrite(file, &charWrite, sizeof(char));
		
		/* Write out the length (including \0) of index name. */
		charWrite = 2+strlen(relationname);
		db_filewrite(file, &charWrite, sizeof(char));
		
		sprintf(tempname, "i%s", relationname);
		
		/* Index name.*/
		db_filewrite(file, tempname, charWrite);
		
		/* Write out the number of expressions for this index.  */
		charWrite = 1;
		db_filewrite(file, &charWrite, sizeof(char));
		
		/* Write out the expression. */
		eet.size = sizeof(db_eetnode_attr_t);
		eet.stack_size = sizeof(db_eetnode_dbint_t);
		db_filewrite(file, (unsigned char*)(&eet), sizeof(db_eet_t));
		attr.base.type = DB_EETNODE_ATTR;
		attr.pos = 0;
		attr.tuple_pos = 0;
		db_filewrite(file, (unsigned char*)(&attr), sizeof(db_eetnode_attr_t));
		
		db_fileclose(file);
		
		sprintf(tempname, "DB_IDX_i%s", relationname);
		file = db_openwritefile(tempname);
		
		if (file == DB_STORAGE_NOFILE)
		{
			puts("CAN'T OPEN IDX FILE!");
			return -1;
		}
		
		/* Write out index type. */
		charWrite = DB_INDEX_TYPE_INLINE;
		db_filewrite(file, (unsigned char*)&charWrite, sizeof(char));
		
		/* Write out number of records. */
		longWrite = numtuples;
		db_filewrite(file, (unsigned char*)&longWrite, sizeof(long));
		
		db_fileclose(file);
	}
	return 0;
}
#endif
#endif
