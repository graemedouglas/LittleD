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

#include "../ref.h"
#include "../dbstorage/dbstorage.h"
#include "../dbmm/db_query_mm.h"
#include "../dbparser/dbparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

db_int main(void)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	int memsegsize = 700; /* In bytes. */
	char memseg[memsegsize];
	init_query_mm(&mm, memseg, memsegsize);
	
	int i;
	char size = 0;
	
	/**** Create test_rel1. ****/ 
	db_fileremove("../tests/test_rel1");
	db_fileremove("./test_rel1");
	char *test_rel1_cquery = "CREATE TABLE test_rel1 (Apple INT)";
	char *test_rel1_iqueries[] = {
		"INSERT INTO test_rel1 VALUES (1)",
		"INSERT INTO test_rel1 VALUES (2)",
		"INSERT INTO test_rel1 VALUES (3)",
		"INSERT INTO test_rel1 VALUES (9)",
	};
	init_query_mm(&mm, memseg, memsegsize);
	parse(test_rel1_cquery, &mm);
	size = sizeof(test_rel1_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(test_rel1_iqueries[i], &mm);
	}

	/**** Create test_rel2. ****/ 
	db_fileremove("test_rel2");
	db_fileremove("../tests/test_rel2");
	char *test_rel2_cquery = "CREATE TABLE test_rel2 (A1 INT, a2 STRING(10), \"3\" INT)";
	char *test_rel2_iqueries[] = {
	}; /* Empty relation. */
	init_query_mm(&mm, memseg, memsegsize);
	parse(test_rel2_cquery, &mm);
	size = sizeof(test_rel2_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(test_rel2_iqueries[i], &mm);
	}
	
	/**** Create test_rel3. ****/ 
	db_fileremove("../tests/test_rel3");
	db_fileremove("test_rel3");
	char *test_rel3_cquery = "CREATE TABLE test_rel3 (A1 INT, a2 STRING(10), \"3\" INT)";
	char *test_rel3_iqueries[] = {
		"INSERT INTO test_rel3 VALUES (1, 'Hi', 1)",
		"INSERT INTO test_rel3 VALUES (73, 'linux', 4023)",
	};
	init_query_mm(&mm, memseg, memsegsize);
	parse(test_rel3_cquery, &mm);
	size = sizeof(test_rel3_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(test_rel3_iqueries[i], &mm);
	}
	
	/**** Create fruit_stock_1. ****/ 
	db_fileremove("fruit_stock_1");
	db_fileremove("../tests/fruit_stock_1");
	char *fruit_stock_1_cquery = "CREATE TABLE fruit_stock_1 (id INT, name STRING(10), qty INT, price INT)";
	char *fruit_stock_1_iqueries[] = {
		"INSERT INTO fruit_stock_1 VALUES (1, 'Apple', 10, 2)",
		"INSERT INTO fruit_stock_1 VALUES (2, 'Orange', 29, 3)",
		"INSERT INTO fruit_stock_1 VALUES (3, 'Pear', 17, 1)",
		"INSERT INTO fruit_stock_1 VALUES (4, 'Lemon', 12, 4)",
		"INSERT INTO fruit_stock_1 VALUES (5, 'Lime', 10, 3)",
	};
	init_query_mm(&mm, memseg, memsegsize);
	parse(fruit_stock_1_cquery, &mm);
	size = sizeof(fruit_stock_1_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(fruit_stock_1_iqueries[i], &mm);
	}
	
	/**** Create fruit_stock_2. ****/ 
	db_fileremove("fruit_stock_2");
	db_fileremove("../tests/fruit_stock_2");
	char *fruit_stock_2_cquery = "CREATE TABLE fruit_stock_2 (id INT, name STRING(10), qty INT, price INT)";
	char *fruit_stock_2_iqueries[] = {
		"INSERT INTO fruit_stock_2 (price) VALUES (2)",
		"INSERT INTO fruit_stock_2 VALUES (2, 'Orange', 29, 3)",
		"INSERT INTO fruit_stock_2 (id, qty, price) VALUES (3, 17, 1)",
		"INSERT INTO fruit_stock_2 (name, qty, price) VALUES ('Lemon', 12, 4)",
		"INSERT INTO fruit_stock_2 (id, name, price) VALUES (5, 'Lime', 3)",
		"INSERT INTO fruit_stock_2 (qty, price) VALUES (1374, 9)",
		"INSERT INTO fruit_stock_2 (name, price) VALUES ('Peach', 6)",
		"INSERT INTO fruit_stock_2 (id, price) VALUES (8, 2)",
	};
	init_query_mm(&mm, memseg, memsegsize);
	parse(fruit_stock_2_cquery, &mm);
	size = sizeof(fruit_stock_2_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(fruit_stock_2_iqueries[i], &mm);
	}
	
	/**** Create tenattrtable. ****/ 
	db_fileremove("tenattrtable");
	db_fileremove("../tests/tenattrtable");
	char *tenattrtable_cquery = "CREATE TABLE tenattrtable (a0 INT, a1 INT, a2 INT, a3 INT, a4 INT, a5 INT, a6 INT, a7 INT, a8 INT, a9 INT)";
	char *tenattrtable_iqueries[] = {
		"INSERT INTO tenattrtable VALUES (0, 0, 0, 0, 0, 0, 0, 0, 0, 0)",
		"INSERT INTO tenattrtable (a1, a2, a3, a4, a5, a6, a7, a8) VALUES (1, 1, 1, 1, 1, 1, 1, 1)",
	};
	init_query_mm(&mm, memseg, memsegsize);
	parse(tenattrtable_cquery, &mm);
	size = sizeof(tenattrtable_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(tenattrtable_iqueries[i], &mm);
	}
	
	/**** Create db_dummy (To be used for queries that do not involve a scan). ****/ 
	db_fileremove("db_dummy");
	db_fileremove("../tests/db_dummy");
	char *db_dummy_cquery = "CREATE TABLE db_dummy (a0 INT)";
	char *db_dummy_iqueries[] = {
		"INSERT INTO db_dummy VALUES (0)"
	};
	init_query_mm(&mm, memseg, memsegsize);
	parse(db_dummy_cquery, &mm);
	size = sizeof(db_dummy_iqueries)/sizeof(char*);
	for (i = 0; i < size; i++)
	{
		init_query_mm(&mm, memseg, memsegsize);
		parse(db_dummy_iqueries[i], &mm);
	}
	
	return 0;
}
