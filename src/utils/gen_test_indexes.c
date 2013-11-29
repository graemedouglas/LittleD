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
#include "../dblogic/eet.h"
#include "../dbstorage/dbstorage.h"
#include "../dbindex/dbindex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

db_int main(void)
{
	/* General variable declaration. */
	char charWrite;
	long longWrite;
	db_fileref_t file;
	db_eet_t eet;
	db_eetnode_attr_t attr;
	
	/**** Create DB_IDXM_test_rel3. ****/ 
	db_fileremove("../tests/DB_IDXM_test_rel3");
	file = db_openwritefile("../tests/DB_IDXM_test_rel3");
	
	if (file == NULL)
	{
		perror("Failed to open file \"DB_IDXM_test_rel3\".");
		return -1;
	}
	
	/* Write out the number of indexes for this relation.  */
	charWrite = 1;
	db_filewrite(file, &charWrite, sizeof(char));
	
	/* Write out the length (including \0) of index name. */
	charWrite = 7;
	db_filewrite(file, &charWrite, sizeof(char));
	
	/* Index name.*/
	db_filewrite(file, "tr3idx", charWrite);
	
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
	
	fclose(file);
	
	/**** Create DB_IDX_tr3idx ****/
	db_fileremove("../tests/DB_IDX_tr3idx");
	file = db_openwritefile("../tests/DB_IDX_tr3idx");
	
	if (file == NULL)
	{
		perror("Failed to open file \"DB_IDXM_tr3idx\".");
		return -1;
	}
	
	/* Write out index type. */
	charWrite = DB_INDEX_TYPE_INLINE;
	db_filewrite(file, (unsigned char*)&charWrite, sizeof(char));
	
	/* Write out number of records. */
	longWrite = 2;
	db_filewrite(file, (unsigned char*)&longWrite, sizeof(long));
	
	fclose(file);
	
	/**** Create DB_IDXM_fruit_stock_1. ****/ 
	db_fileremove("../tests/DB_IDXM_fruit_stock_1");
	file = db_openwritefile("../tests/DB_IDXM_fruit_stock_1");
	
	if (file == NULL)
	{
		perror("Failed to open file \"DB_IDXM_fruit_stock_1\".");
		return -1;
	}
	
	/* Write out the number of indexes for this relation.  */
	charWrite = 1;
	db_filewrite(file, &charWrite, sizeof(char));
	
	/* Write out the length (including \0) of index name. */
	charWrite = 7;
	db_filewrite(file, &charWrite, sizeof(char));
	
	/* Index name.*/
	db_filewrite(file, "fs1idx", charWrite);
	
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
	
	fclose(file);
	
	/**** Create DB_IDX_fs1idx ****/
	db_fileremove("../tests/DB_IDX_fs1idx");
	file = db_openwritefile("../tests/DB_IDX_fs1idx");
	
	if (file == NULL)
	{
		perror("Failed to open file \"DB_IDX_fs1idx\".");
		return -1;
	}
	
	/* Write out index type. */
	charWrite = DB_INDEX_TYPE_INLINE;
	db_filewrite(file, (unsigned char*)&charWrite, sizeof(char));
	
	/* Write out number of records. */
	longWrite = 5;
	db_filewrite(file, (unsigned char*)&longWrite, sizeof(long));
	
	fclose(file);
	
	return 0;
}
