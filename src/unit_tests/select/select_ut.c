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

/* The unit tests for the selection operator. */
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

void test_select_0(CuTest *tc)
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
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_int intResult;
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 1: Relation 3, select where first field is 1.");
	
	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;
	
	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "test_rel3", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, select.base.header);
	printf("The first db_int value in the returned tuple is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_select_1(CuTest *tc)
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
	db_eetnode_dbstring_t dbstringnode;
	dbstringnode.base.type = DB_EETNODE_CONST_DBSTRING;
	db_int intResult;
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 2: Relation 3, select where second field is \"linux\".");

	/* Variable delcaration */
	eet.size = (0*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbstringnode.string = "linux";
	*((db_eetnode_dbstring_t*)arr_p) = dbstringnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	attrnode.pos = 1;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "test_rel3", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, select.base.header);
	printf("The first db_int value in the returned tuple is: %d\n", intResult);
	CuAssertTrue(tc, 73 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_select_2(CuTest *tc)
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
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_int intResult;
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 3: Relation 3, select where first field is -14. Should be no such tuples.");

	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "test_rel3", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_select_3(CuTest *tc)
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
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_dbstring_t dbstringnode;
	dbstringnode.base.type = DB_EETNODE_CONST_DBSTRING;
	db_int intResult;
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 4: Relation 3, select where first field is 1 or second is \"linux\".");

	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)(eet.size));
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	attrnode.pos = 1;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbstringnode.string = "linux";
	*((db_eetnode_dbstring_t*)arr_p) = dbstringnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	opnode.type = DB_EETNODE_OP_OR;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "test_rel3", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	fflush(stdout);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, select.base.header);
	printf("The first db_int value in the returned tuple is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	fflush(stdout);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, select.base.header);
	printf("The first db_int value in the returned tuple is: %d\n", intResult);
	CuAssertTrue(tc, 73 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_select_4(CuTest *tc)
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
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_dbstring_t dbstringnode;
	dbstringnode.base.type = DB_EETNODE_CONST_DBSTRING;
	db_int intResult;
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 5: fruit_stock_2, select where first field is 1 or second is \"Lime\".");
	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)(eet.size));
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	attrnode.pos = 1;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbstringnode.string = "Lime";
	*((db_eetnode_dbstring_t*)arr_p) = dbstringnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	opnode.type = DB_EETNODE_OP_OR;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "fruit_stock_2", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, select.base.header);
	printf("The int value in the returned tuple is: %d\n", getintbypos(&t, 3, select.base.header));
	printf("The first db_int value in the returned tuple is: %d\n", intResult);
	CuAssertTrue(tc, 5 == intResult);
	fflush(stdout);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_select_5(CuTest *tc)
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
	db_eetnode_dbint_t dbintnode;
	dbintnode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_dbstring_t dbstringnode;
	dbstringnode.base.type = DB_EETNODE_CONST_DBSTRING;
	db_int intResult;
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 6: fruit_stock_2, select where first field is 1 or second is \"Canucks\".");
	/* Variable delcaration */
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)(eet.size));
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	dbintnode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	attrnode.pos = 1;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbstringnode.string = "Canucks";
	*((db_eetnode_dbstring_t*)arr_p) = dbstringnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_EQ;
	*(arr_p) = opnode;
	arr_p++;
	
	opnode.type = DB_EETNODE_OP_OR;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "fruit_stock_2", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_select_6(CuTest *tc)
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
	scan_t scan;
	select_t select;
	tuple_t t;
	
	puts("********************************************************************************");
	puts("Test 7: tenattrtable, select where first attr(0) is null and attr(9) is null.");
	/* Variable delcaration */
	eet.size = (0*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)(eet.size));
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	attrnode.pos = 0;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_ISNULL;
	*(arr_p) = opnode;
	arr_p++;
	
	attrnode.pos = 9;
	attrnode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrnode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opnode.type = DB_EETNODE_OP_ISNULL;
	*(arr_p) = opnode;
	arr_p++;
	
	opnode.type = DB_EETNODE_OP_AND;
	*(arr_p) = opnode;
	arr_p++;
	
	/* Build up query tree. */
	init_scan(&scan, "tenattrtable", &mm);
	init_select(&select, &eet, (db_op_base_t*)&scan, &mm);
	init_tuple(&t, select.base.header->tuple_size, select.base.header->tuple_size, &mm);

	/* Carry out test. */
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, 1 == t.isnull[0]);
	CuAssertTrue(tc, 2 == t.isnull[1]);
	CuAssertTrue(tc, 1 == getintbypos(&t, 1, scan.base.header));

	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next_select(&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&select, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close_scan(&scan, &mm);
	close_select(&select, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

CuSuite *DBSelectGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_select_0);
	SUITE_ADD_TEST(suite, test_select_1);
	SUITE_ADD_TEST(suite, test_select_2);
	SUITE_ADD_TEST(suite, test_select_3);
	SUITE_ADD_TEST(suite, test_select_4);
	SUITE_ADD_TEST(suite, test_select_5);
	SUITE_ADD_TEST(suite, test_select_6);
	
	return suite;
}

void runAllTests_select()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBSelectGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
