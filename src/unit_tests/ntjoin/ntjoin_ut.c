/**
@author		Graeme Douglas
@brief
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

#include <string.h>
#include <stdio.h>
#include "../CuTest.h"
#include "../../dbobjects/relation.h"
#include "../../dbobjects/tuple.h"
#include "../../dblogic/eet.h"
#include "../../dbops/db_ops.h"
#include "../../dbops/scan.h"
#include "../../dbops/project.h"
#include "../../dbops/select.h"
#include "../../dbops/ntjoin.h"

void test_ntjoin_1(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t opnode;
	db_eetnode_attr_t attrnode;
	attrnode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	char* strResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 1: Join relaiton 3 with itself, on the first attribute.");

	/* Variable delcaration */
	eet.size = (0*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 1;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_2(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t opnode;
	db_eetnode_attr_t attrnode;
	attrnode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	char* strResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 2: Join relaiton 3 with itself, where left's first attribute = right's third attribute.");

	/* Variable delcaration */
	eet.size = (0*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;
	
	/* Build the expression tree, in evaluable form. */	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	attrnode.pos = 2;
	attrnode.tuple_pos = 1;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_3(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_int intResult;
	char* strResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	puts("********************************************************************************");
	puts("Test 3: Do a cross product of test_rel3 with itself (join condition is true).");

	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 0*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	fflush(stdout);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_4(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_int intResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 4: Join relaiton 3 with itself, where the join condition is false");

	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 0*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_5(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_int intResult;
	char* strResult;
	scan_t lscan, rscan, rrscan;
	ntjoin_t ntjoin;
	ntjoin_t ntjoin2;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 5: Join relaiton 3 with itself twice, both cross products.");

	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 0*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_scan(&rrscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin2, &eet, (db_op_base_t*)&rscan, (db_op_base_t*)&rrscan, &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&ntjoin2, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 6, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 6, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 7, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 7, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 8, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 8, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close_scan(&rrscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close((db_op_base_t*)&ntjoin2, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_6(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t opnode;
	db_eetnode_attr_t attrnode;
	attrnode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 6: Join fruit_stock_1 with fruit_stock_2 on first attribute of each.");

	/* Variable delcaration */
	eet.size = (0*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 1;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&lscan, "fruit_stock_1", &mm);
	init_scan(&rscan, "fruit_stock_2", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 5) == t.isnull[0]);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 6) == t.isnull[0]);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_7(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t opnode;
	db_eetnode_attr_t attrnode;
	attrnode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 7: Join tenattrtable to tenattrtable on second attribute of each.");

	/* Variable delcaration */
	eet.size = (0*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	attrnode.pos = 1;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	attrnode.pos = 1;
	attrnode.tuple_pos = 1;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&lscan, "tenattrtable", &mm);
	init_scan(&rscan, "tenattrtable", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 1, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	printf("t.isnull[1]: %d\n", (db_int)(t.isnull[1]));
	CuAssertTrue(tc, 0 == t.isnull[1]);
	printf("t.isnull[2]: %d\n", (db_int)(t.isnull[2]));
	CuAssertTrue(tc, 0 == t.isnull[2]);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 1, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 1 == t.isnull[0]);
	printf("t.isnull[1]: %d\n", (db_int)(t.isnull[1]));
	CuAssertTrue(tc, (1 << 1) + (1 << 2) == t.isnull[1]);
	printf("t.isnull[2]: %d\n", (db_int)(t.isnull[2]));
	CuAssertTrue(tc, (1 << 3) == t.isnull[2]);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_ntjoin_8(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_int intResult;
	char* strResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 8: Do a cross product of test_rel3 with itself (eet = NULL).");

	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin, NULL, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);
	
	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	fflush(stdout);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	puts("********************************************************************************");
}

void test_ntjoin_9(CuTest *tc)
{
	/* General variable declaration */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	db_eet_t eet;
	db_int intResult;
	char* strResult;
	scan_t lscan, rscan;
	ntjoin_t ntjoin;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 9: Do a cross product of test_rel3 with itself, where there is no join condition (eet.nodes = NULL).");

	/* Variable delcaration */
	eet.nodes = NULL;

	/* Build up query tree. */
	init_scan(&lscan, "test_rel3", &mm);
	init_scan(&rscan, "test_rel3", &mm);
	init_ntjoin(&ntjoin, &eet, (db_op_base_t*)&lscan, (db_op_base_t*)&rscan, &mm);
	init_tuple(&t, ntjoin.base.header->tuple_size, ntjoin.base.header->num_attr, &mm);

	/* Carry out test. */
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	fflush(stdout);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 1 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "Hi"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 1, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 2, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	intResult = getintbypos(&t, 3, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 3, intResult);
	CuAssertTrue(tc, 73 == intResult);
	strResult = getstringbypos(&t, 4, ntjoin.base.header);
	printf("The string value at position %d in the returned tuple is: %s\n", 4, strResult);
	CuAssertTrue(tc, 0 == strcmp(strResult, "linux"));
	intResult = getintbypos(&t, 5, ntjoin.base.header);
	printf("The db_int value at position %d in the returned tuple is: %d\n", 5, intResult);
	CuAssertTrue(tc, 4023 == intResult);
	
	intResult = next_ntjoin(&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	intResult = next((db_op_base_t*)&ntjoin, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	close_scan(&lscan, &mm);
	close_scan(&rscan, &mm);
	close((db_op_base_t*)&ntjoin, &mm);
	close_tuple(&t, &mm);
	puts("********************************************************************************");
}

CuSuite *DBNTJoinGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_ntjoin_1);
	SUITE_ADD_TEST(suite, test_ntjoin_2);
	SUITE_ADD_TEST(suite, test_ntjoin_3);
	SUITE_ADD_TEST(suite, test_ntjoin_4);
	SUITE_ADD_TEST(suite, test_ntjoin_5);
	SUITE_ADD_TEST(suite, test_ntjoin_6);
	SUITE_ADD_TEST(suite, test_ntjoin_7);
	SUITE_ADD_TEST(suite, test_ntjoin_8);
	SUITE_ADD_TEST(suite, test_ntjoin_9);
	
	return suite;
}

void runAllTests_ntjoin()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBNTJoinGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
