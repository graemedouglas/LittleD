/**
@author		Graeme Douglas
@brief		A quick and dirty program for creating a set of test
		relations on any platform.
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

//#include "../ref.h"
//#include "../relation.h"
//#include "../tuple.h"
#include "../ref.h"
#include "../dbstorage/dbstorage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

db_int main(void)
{
	/* General variable declaration. */
	char charWrite;
	db_int intWrite;
	char offset = 0;
	char size = 0;
	FILE *rel1;
	
	/**** Create test_rel1. ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/test_rel1");
	rel1 = db_openwritefile("../tests/test_rel1");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"test_rel1\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 1;
	db_filewrite(rel1, &charWrite, sizeof(char));
	
	/** Header information for first attribute **/
	/* Size of name.*/
	charWrite = 6;
	db_filewrite(rel1, &charWrite, sizeof(char));
	
	/* Attribute name.*/
	db_filewrite(rel1, "Apple", 6*sizeof(char));
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/*** Write out tuples. ***/
	/** Write out tuple 1. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 2. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 2;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 3. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 4. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 9;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	fclose(rel1);
	
	/**** Create test_rel2. ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/test_rel2");
	rel1 = db_openwritefile("../tests/test_rel2");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"test_rel2\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/** Header information for first attribute **/
	/* Size of name.*/
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("A1", sizeof(char), 3, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for second attribute. **/
	/* Size of name.*/
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("a2", sizeof(char), 3, rel1);
	
	/* Attribute type.*/
	charWrite = 1;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = 10;
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for third attribute **/
	/* Size of name.*/
	charWrite = 2;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("3", sizeof(char), 2, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	fclose(rel1);
	
	/**** Create test_rel3. ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/test_rel3");
	rel1 = db_openwritefile("../tests/test_rel3");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"test_rel3\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/** Header information for first attribute **/
	/* Size of name.*/
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("A1", sizeof(char), 3, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for second attribute. **/
	/* Size of name.*/
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("a2", sizeof(char), 3, rel1);
	
	/* Attribute type.*/
	charWrite = 1;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = 10;
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for third attribute **/
	/* Size of name.*/
	charWrite = 2;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("3", sizeof(char), 2, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/*** Write out tuples. ***/
	/** Write out tuple 1. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2.*/
	fwrite("Hi\0\0\0\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3.*/
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 2. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 73;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("linux\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 4023;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	fclose(rel1);
	
	/**** Create fruit_stock_1. ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/fruit_stock_1");
	rel1 = db_openwritefile("../tests/fruit_stock_1");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"fruit_stock_1\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 4;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	//fprintf(1, "%d", toWrite);
	
	/** Header information for first attribute **/
	/* Size of name.*/
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("id", sizeof(char), 3, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for second attribute **/
	/* Size of name.*/
	charWrite = 5;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("name", sizeof(char), 5, rel1);
	
	/* Attribute type.*/
	charWrite = 1;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = 10;
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for third attribute **/
	/* Size of name.*/
	charWrite = 4;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("qty", sizeof(char), 4, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for fourth attribute **/
	/* Size of name.*/
	charWrite = 6;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("price", sizeof(char), 6, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	
	/*** Write out tuples. ***/
	/** Write out tuple 1. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2.*/
	fwrite("Apple\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3.*/
	intWrite = 10;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4.*/
	intWrite = 2;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 2. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 2;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2.*/
	fwrite("Orange\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3.*/
	intWrite = 29;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4.*/
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 3. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2.*/
	fwrite("Pear\0\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3.*/
	intWrite = 17;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4.*/
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 4. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 4;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2.*/
	fwrite("Lemon\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3.*/
	intWrite = 12;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4.*/
	intWrite = 4;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 5. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1.*/
	intWrite = 5;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2.*/
	fwrite("Lime\0\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3.*/
	intWrite = 10;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4.*/
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	fclose(rel1);
	
	/**** Create fruit_stock_2. ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/fruit_stock_2");
	rel1 = db_openwritefile("../tests/fruit_stock_2");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"fruit_stock_2\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 4;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	//fprintf(1, "%d", toWrite);
	
	/** Header information for first attribute **/
	/* Size of name.*/
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("id", sizeof(char), 3, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for second attribute **/
	/* Size of name.*/
	charWrite = 5;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("name", sizeof(char), 5, rel1);
	
	/* Attribute type.*/
	charWrite = 1;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = 10;
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for third attribute **/
	/* Size of name.*/
	charWrite = 4;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("qty", sizeof(char), 4, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for fourth attribute **/
	/* Size of name.*/
	charWrite = 6;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name.*/
	fwrite("price", sizeof(char), 6, rel1);
	
	/* Attribute type.*/
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset.*/
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size.*/
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	
	/*** Write out tuples. ***/
	/** Write out tuple 1. **/
	/* Write out isnull bit array */
	charWrite = 0x07;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Apple\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 10;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 2;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 2. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 2;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Orange\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 29;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 3. **/
	/* Write out isnull bit array */
	charWrite = 0x02;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Pear\0\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 17;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 4. **/
	/* Write out isnull bit array */
	charWrite = 0x01;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 4;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Lemon\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 12;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 4;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 5. **/
	/* Write out isnull bit array */
	charWrite = 0x04;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 5;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Lime\0\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 10;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 3;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 6. **/
	/* Write out isnull bit array */
	charWrite = 0x03;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 6;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Cherry\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 1374;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 9;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 7. **/
	/* Write out isnull bit array */
	charWrite = 0x05;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 7;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Peach\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 6;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 8. **/
	/* Write out isnull bit array */
	charWrite = 0x06;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 8;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	fwrite("Prune\0\0\0\0", sizeof(char), 10, rel1);
	
	/* Attribute 3. */
	intWrite = 11;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 2;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	fclose(rel1);
	
	/**** Create tenattrtable. ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/tenattrtable");
	rel1 = db_openwritefile("../tests/tenattrtable");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"tenattrtable\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 10;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	//fprintf(1, "%d", toWrite);
	
	/** Header information for first attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a0", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for second attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a1", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 3rd attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a2", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 4th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a3", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 5th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a4", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 6th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a5", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 7th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a6", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 8th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a7", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 9th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a8", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Header information for 10th attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a9", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Write out tuple 1. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 3. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 5. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 6. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 7. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 8. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 9. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 10. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/** Write out tuple 2. **/
	/* Write out isnull bit array */
	charWrite = 0x01;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	charWrite = 0x02;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 2. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 3. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 4. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 5. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 6. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 7. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 8. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 9. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	/* Attribute 10. */
	intWrite = 1;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	fclose(rel1);
	
	/**** Create db_dummy (To be used for queries that do not involve a scan). ****/ 
	size = 0;
	offset = 0;
	db_fileremove("../tests/db_dummy");
	rel1 = db_openwritefile("../tests/db_dummy");
	
	if (rel1 == DB_STORAGE_NOFILE)
	{
		perror("Failed to open file \"db_dummy\".");
		return -1;
	}
	
	/* Write out the number of attributes into the relation. */
	charWrite = 1;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/** Header information for first attribute **/
	/* Size of name. */
	charWrite = 3;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute name. */
	fwrite("a0", sizeof(char), 3, rel1);
	
	/* Attribute type. */
	charWrite = 0;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute offset. */
	offset += size;
	fwrite(&offset, sizeof(char), 1, rel1);
	
	/* Attribute size. */
	size = (char)sizeof(db_int);
	fwrite(&size, sizeof(char), 1, rel1);
	
	/** Write out tuple 1. **/
	/* Write out isnull bit array */
	charWrite = 0x00;
	fwrite(&charWrite, sizeof(char), 1, rel1);
	
	/* Attribute 1. */
	intWrite = 0;
	fwrite(&intWrite, sizeof(db_int), 1, rel1);
	
	fclose(rel1);
	
	return 0;
}
