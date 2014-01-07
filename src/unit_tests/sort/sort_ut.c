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
#include "../../dblogic/compare_tuple.h"
#include "../../dblogic/eet.h"
#include "../../dbops/db_ops.h"
#include "../../dbops/scan.h"
#include "../../dbops/project.h"
#include "../../dbops/select.h"
#include "../../dbops/sort.h"

void test_sort_1(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 1: Sort test_rel3 using first attribute in ascending order.");
	fflush(stdout);
	
	oneExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	oneOrder[0] = (db_uint8)DB_TUPLE_ORDER_ASC;
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(oneExpr[0].nodes))) = attrNode;
	
	init_scan(&scan, "test_rel3", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	//printf("Result of next call: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	free(oneExpr[0].nodes);
	close_tuple(&t, &mm);
	puts("**********************************************************************");
}

void test_sort_2(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 2: Sort test_rel3 using first attribute, in descending order.");
	fflush(stdout);
	oneExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	oneOrder[0] = DB_TUPLE_ORDER_DESC;
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(oneExpr[0].nodes))) = attrNode;
	
	init_scan(&scan, "test_rel3", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 73 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
}

void test_sort_3(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 3: Sort fruit_stock_1 using third attribute, in descending order.");
	fflush(stdout);
	oneExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	
	oneOrder[0] = DB_TUPLE_ORDER_DESC;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(oneExpr[0].nodes))) = attrNode;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
}

void test_sort_4(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 4: Sort fruit_stock_1 using third attribute, in ascending order.");
	fflush(stdout);
	oneExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	
	oneOrder[0] = DB_TUPLE_ORDER_ASC;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(oneExpr[0].nodes))) = attrNode;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
}

void test_sort_5(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 5: Sort fruit_stock_1 using second attribute, in ascending order.");
	fflush(stdout);
	
	oneExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbstring_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	
	oneOrder[0] = DB_TUPLE_ORDER_ASC;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(oneExpr[0].nodes))) = attrNode;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
}

void test_sort_6(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t twoExpr[2];
	db_uint8 twoOrder[2];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 6: Sort fruit_stock_1 using 4th, 3rd attributes, in ascending order.");
	fflush(stdout);
	
	twoExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	twoOrder[0] = DB_TUPLE_ORDER_ASC;
	twoOrder[1] = DB_TUPLE_ORDER_ASC;
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(twoExpr[0].nodes))) = attrNode;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(twoExpr[1].nodes))) = attrNode;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), twoExpr, 2, twoOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	puts("**********************************************************************");
}

void test_sort_7(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t twoExpr[2];
	db_uint8 twoOrder[2];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 7: Sort fruit_stock_1 using 4th and 3rd attributes, in asc/desc order (respectively).");
	fflush(stdout);
	
	twoExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	twoOrder[0] = DB_TUPLE_ORDER_ASC;
	twoOrder[1] = DB_TUPLE_ORDER_DESC;
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(twoExpr[0].nodes))) = attrNode;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(twoExpr[1].nodes))) = attrNode;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), twoExpr, 2, twoOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	puts("**********************************************************************");
}

void test_sort_8(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_t opNode;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 8: Sort fruit_stock_1 using 3rd/4th, in ascending order.");
	fflush(stdout);
	
	oneExpr[0].size = (2*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t)+ 1*sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	oneOrder[0] = DB_TUPLE_ORDER_ASC;
	
	arr_p = oneExpr[0].nodes;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	opNode.type = DB_EETNODE_OP_DIV;
	*arr_p = opNode;
	arr_p++;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	fflush(stdout);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	fflush(stdout);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	fflush(stdout);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	fflush(stdout);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	fflush(stdout);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
}

void test_sort_9(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_t opNode;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 9: Sort fruit_stock_1 using (3rd > 4th), in descending order.");
	fflush(stdout);
	oneExpr[0].size = (2*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t)+ 2*sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	oneOrder[0] = DB_TUPLE_ORDER_DESC;
	
	arr_p = oneExpr[0].nodes;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	opNode.type = DB_EETNODE_OP_NOT;
	*arr_p = opNode;
	arr_p++;
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
}

void test_sort_10(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t twoExpr[2];
	db_uint8 twoOrder[2];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_t opNode;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 10: Sort fruit_stock_1 using two complex expressions.");
	fflush(stdout);
	
	twoExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbstring_t)+ 1*sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t)+ 1*sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	twoOrder[0] = DB_TUPLE_ORDER_ASC;
	twoOrder[1] = DB_TUPLE_ORDER_ASC;
	
	arr_p = twoExpr[0].nodes;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	opNode.type = DB_EETNODE_FUNC_LENGTH_DBSTRING;
	*arr_p = opNode;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	opNode.type = DB_EETNODE_OP_MOD;
	*arr_p = opNode;
	arr_p++;
	
	init_scan(&scan, "fruit_stock_1", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), twoExpr, 2, twoOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	puts("**********************************************************************");
}

void test_sort_11(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t twoExpr[2];
	db_uint8 twoOrder[2];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_t opNode;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 11: Sort fruit_stock_1 using two complex expressions.");
	fflush(stdout);
	twoExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbstring_t)+ 1*sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	twoExpr[1].size = (2*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t)+ 1*sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	twoOrder[0] = DB_TUPLE_ORDER_ASC;
	twoOrder[1] = DB_TUPLE_ORDER_ASC;
	
	arr_p = twoExpr[0].nodes;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	opNode.type = DB_EETNODE_FUNC_LENGTH_DBSTRING;
	*arr_p = opNode;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(arr_p))) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)(arr_p))+1));
	
	opNode.type = DB_EETNODE_OP_MOD;
	*arr_p = opNode;
	arr_p++;
	
	init_scan(&scan, "fruit_stock_2", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), twoExpr, 2, twoOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	// TODO: I really should change getintbypos and such to return to an address.
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 0) + (1 << 1) + (1 << 2) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, sort.base.header);
	//printf("Int value at position %d the tuple: %d\n", 0, intResult);
	//CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 1) + (1 << 2) == t.isnull[0]);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 8 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 1) == t.isnull[0]);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 0) + (1 << 1) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, sort.base.header);
	//printf("Int value at position %d the tuple: %d\n", 0, intResult);
	//CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 2) == t.isnull[0]);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 0) + (1 << 2) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, sort.base.header);
	//printf("Int value at position %d the tuple: %d\n", 0, intResult);
	//CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, sort.base.header);
	//printf("Int value at position %d the tuple: %d\n", 0, intResult);
	//CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	

	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	puts("**********************************************************************");
}

void test_sort_12(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	sort_t sort;
	db_tuple_t t;
	db_eet_t oneExpr[1];
	db_uint8 oneOrder[1];
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 12: Sort tenattrtable using 10th attribute, in descending order.");
	fflush(stdout);
	
	oneExpr[0].size = (1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;
	
	oneOrder[0] = DB_TUPLE_ORDER_DESC;
	
	attrNode.pos = 9;
	attrNode.tuple_pos = 0;
	(*((db_eetnode_attr_t*)(oneExpr[0].nodes))) = attrNode;
	
	init_scan(&scan, "tenattrtable", &mm);
	init_sort(&sort, (db_op_base_t*)(&scan), oneExpr, 1, oneOrder, &mm);
	init_tuple(&t, sort.base.header->tuple_size, sort.base.header->num_attr, &mm);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, 0 == t.isnull[0]);
	CuAssertTrue(tc, 0 == t.isnull[1]);
	intResult = getintbypos(&t, 1, sort.base.header);
	intResult = getintbypos(&t, 1, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	CuAssertTrue(tc, (1 << 1) == t.isnull[1]);
	intResult = getintbypos(&t, 1, sort.base.header);
	intResult = getintbypos(&t, 1, sort.base.header);
	printf("Int value at position %d the tuple: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&sort, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	close((db_op_base_t*)&sort, &mm);
	close((db_op_base_t*)&scan, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	puts("**********************************************************************");
	/*
	*/
}

CuSuite *DBSortGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_sort_1);
	SUITE_ADD_TEST(suite, test_sort_2);
	SUITE_ADD_TEST(suite, test_sort_3);
	SUITE_ADD_TEST(suite, test_sort_4);
	SUITE_ADD_TEST(suite, test_sort_5);
	SUITE_ADD_TEST(suite, test_sort_6);
	SUITE_ADD_TEST(suite, test_sort_7);
	SUITE_ADD_TEST(suite, test_sort_8);
	SUITE_ADD_TEST(suite, test_sort_9);
	SUITE_ADD_TEST(suite, test_sort_10);
	SUITE_ADD_TEST(suite, test_sort_11);
	SUITE_ADD_TEST(suite, test_sort_12);
	
	return suite;
}

void runAllTests_sort()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBSortGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
