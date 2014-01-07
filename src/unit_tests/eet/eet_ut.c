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

/* The unit tests for evaluable expression trees. */
#include <string.h>
#include <stdio.h>
#include "../CuTest.h"
#include "../../dbobjects/relation.h"
#include "../../dbobjects/tuple.h"
#include "../../dblogic/eet.h"
#include "../../dbops/db_ops.h"
#include "../../dbops/scan.h"

void test_eet_1(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	/* Variable declaration. */
	db_eet_t eet;
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	
	db_eetnode_t *arr_p = eet.nodes;
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	
	puts("********************************************************************************");
	puts("Tree #1:");
	
	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_LT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, NULL, NULL, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_2(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #2:");
	
	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eetnode_attr_t leftAttr;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	relation_header_t *hap[1];
	
	eet.size = (sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	arr_p = eet.nodes;
	hap[0] = s.base.header;
	
	/* Build the expression tree, in evaluable form. */	
	leftAttr.base.type = DB_EETNODE_ATTR;
	leftAttr.pos = 2;
	leftAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = leftAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_3(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #3:");
	
	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eetnode_attr_t leftAttr;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftAttr.base.type = DB_EETNODE_ATTR;
	leftAttr.pos = 0;
	leftAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = leftAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = -4;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_ADD;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GTE;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	free(eet.nodes);
	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	puts("********************************************************************************");
}

void test_eet_4(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #4:");
	
	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -4;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_SUB;
	*(arr_p) = secondOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, -5 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");	
}

void test_eet_5(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #5:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;
	
	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -4;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_EQ;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_DIV;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	CuAssertTrue(tc, 1==evaluate_eet(&eet, &result, tap, hap, 0, &mm));
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_6(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("Tree #6:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_BOR;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	firstOp.type = DB_EETNODE_OP_BAND;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_7(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #7:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_BXOR;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_BXOR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_8(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #8:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 8;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MOD;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_MOD;
	*(arr_p) = secondOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_9(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #9:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 8;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_MOD;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MOD;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_10(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #10:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_11(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #11:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_XOR;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_XOR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_12(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #12:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	db_eetnode_attr_t rightAttr;
	
	//eet.size = (3*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_XOR;
	*(arr_p) = secondOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_XOR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_13(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #13:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	
	eet.size = (sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_NOT;
	*(arr_p) = secondOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_NOT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_14(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #14:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_t secondOp;
	
	eet.size = (sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (sizeof(db_eetnode_dbint_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	secondOp.type = DB_EETNODE_OP_BCOMP;
	*(arr_p) = secondOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_BCOMP;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_15(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #15:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_dbstring_t leftStr, rightStr;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "abaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_LT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_16(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #16:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	db_eetnode_dbstring_t leftStr, rightStr;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "abaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_17(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #17:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbstring_t leftStr, rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "abaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_LTE;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_18(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #18:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbstring_t leftStr, rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "aaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GTE;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_19(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #19:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbstring_t leftStr, rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "abaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_NEQ;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	
	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_20(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #20:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbstring_t leftStr, rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "abaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_EQ;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_21(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #21:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbstring_t leftStr, rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "aaaaa!";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_LTE;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_22(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #22:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_attr_t rightAttr;
	db_eetnode_dbstring_t leftStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (3*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	eet.size = (2*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = (2*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_t));
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "Hi";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_EQ;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_23(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #23:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_eetnode_attr_t rightAttr;
	db_eetnode_dbstring_t rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	//eet.size = (5*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 8*sizeof(db_eetnode_t));
	eet.size = (5*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 8*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));

	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;

	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	firstOp.type = DB_EETNODE_OP_ADD;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 4;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;

	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;

	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));

	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;

	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));

	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "Te";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));

	firstOp.type = DB_EETNODE_OP_LT;
	*(arr_p) = firstOp;
	arr_p++;

	firstOp.type = DB_EETNODE_OP_NOT;
	*(arr_p) = firstOp;
	arr_p++;

	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_24(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #24:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_eetnode_attr_t rightAttr;
	db_eetnode_dbstring_t rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	eet.size = (5*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 8*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;

	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ADD;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 4;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "Te";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_LT;
	*(arr_p) = firstOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_NOT;
	*(arr_p) = firstOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	
	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	free(eet.nodes);
	puts("********************************************************************************");
}

void test_eet_25(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Tree #25:");

	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "test_rel3", &mm);
	relation_header_t *hap[1];
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, (db_uint8)s.base.header->tuple_size, s.base.header->num_attr, &mm);
	tap[0] = &t;
	hap[0] = s.base.header;
	next_scan(&s, &t, &mm);
	db_eetnode_t firstOp;
	db_eetnode_dbint_t leftVal;
	db_eetnode_dbint_t rightVal;
	db_eetnode_attr_t rightAttr;
	db_eetnode_dbstring_t rightStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	eet.size = (5*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 8*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	arr_p = eet.nodes;
	
	/* Build the expression tree, in evaluable form. */	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ADD;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 3;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 4;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MULT;
	*(arr_p) = firstOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightVal.base.type = DB_EETNODE_CONST_DBINT;
	rightVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = rightVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	rightStr.base.type = DB_EETNODE_CONST_DBSTRING;
	rightStr.string = "Apple";
	*((db_eetnode_dbstring_t*)arr_p) = rightStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));

	firstOp.type = DB_EETNODE_OP_LT;
	*(arr_p) = firstOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_NOT;
	*(arr_p) = firstOp;
	arr_p++;
	
	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("Result: %d\n", result);
	CuAssertTrue(tc, 1 == result);

	free(eet.nodes);
	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	puts("********************************************************************************");
}

void test_eet_26(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Comprehensive Test 1: Test Operators on various NULL elements.");
	
	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "fruit_stock_2", &mm);
	db_tuple_t *tap[1];
	db_tuple_t t;
	relation_header_t *hap[1];
	hap[0] = s.base.header;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	db_eetnode_t firstOp;
	db_int retval;
	db_eetnode_dbint_t leftVal;
	db_eetnode_attr_t rightAttr;
	db_eetnode_dbstring_t leftStr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	// Throw away first 5 tuples.
	next_scan(&s, &t, &mm);
	next_scan(&s, &t, &mm);
	next_scan(&s, &t, &mm);
	next_scan(&s, &t, &mm);
	next_scan(&s, &t, &mm);
	next_scan(&s, &t, &mm);
	
	eet.size = (5*sizeof(db_eetnode_dbint_t) + 1*sizeof(db_eetnode_dbstring_t) + 2*sizeof(db_eetnode_attr_t) + 8*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	tap[0] = &t;
	hap[0] = s.base.header;
	
	/// + -14 attr(0)
	// Build up expression to evaluate.	
	eet.size = (1*sizeof(db_eetnode_dbint_t) + 0*sizeof(db_eetnode_dbstring_t) + 1*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ADD;
	*(arr_p) = firstOp;
	arr_p++;

	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);

	/// / attr(0) -14
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));

	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_DIV;
	*(arr_p) = firstOp;
	arr_p++;

	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// % -14 attr(0)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MOD;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// % attr(0) -14
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = -14;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_MOD;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// OR 1 attr(0)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	
	/// OR attr(0) 1
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	
	/// OR 0 attr(0)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// OR attr(0) 0
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_OR;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// AND 0 attr(0)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	
	/// AND attr(0) 0
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	
	/// AND 1 attr(0)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// AND attr(0) 1
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_AND;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// > attr(0) 1
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// > 1 attr(0)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftVal.base.type = DB_EETNODE_CONST_DBINT;
	leftVal.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = leftVal;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 0;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// > attr(1) "apple"
	// Build up expression to evaluate.
	eet.size = sizeof(db_eetnode_dbstring_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t);
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "apple";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// > "apple" attr(1)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	leftStr.base.type = DB_EETNODE_CONST_DBSTRING;
	leftStr.string = "apple";
	*((db_eetnode_dbstring_t*)arr_p) = leftStr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_GT;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// length attr(1)
	// Build up expression to evaluate.
	eet.size = sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t);
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_FUNC_LENGTH_DBSTRING;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 2 == retval);
	
	/// isnull attr(1)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 1;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ISNULL;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	
	/// isnull attr(3)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 3;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ISNULL;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	
	free(eet.nodes);
	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	puts("********************************************************************************");
}

void test_eet_27(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	puts("********************************************************************************");
	puts("Comprehensive Test 2: Test IS NULL on multi-byte isnull array NULL.");
	
	/* Variable delcaration */
	scan_t s;
	s.base.type = DB_SCAN;
	init_scan(&s, "tenattrtable", &mm);
	db_tuple_t *tap[1];
	db_tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	relation_header_t *hap[1];
	hap[0] = s.base.header;
	db_eetnode_t firstOp;
	db_int retval;
	db_eetnode_attr_t rightAttr;
	db_int result;
	db_eet_t eet;
	db_eetnode_t *arr_p;
	
	
	// Throw away first tuple.
	next_scan(&s, &t, &mm);
	next_scan(&s, &t, &mm);
	
	eet.size = (1*sizeof(db_eetnode_attr_t) + 1*sizeof(db_eetnode_t));
	eet.nodes = malloc((size_t)eet.size);
	eet.stack_size = eet.size;
	tap[0] = &t;
	hap[0] = s.base.header;
	
	/// isnull attr(8)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 8;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ISNULL;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	
	/// isnull attr(9)
	// Build up expression to evaluate.
	arr_p = eet.nodes;
	
	rightAttr.base.type = DB_EETNODE_ATTR;
	rightAttr.pos = 9;
	rightAttr.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = rightAttr;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	firstOp.type = DB_EETNODE_OP_ISNULL;
	*(arr_p) = firstOp;
	arr_p++;
	
	/* Evaluate the tree. */
	retval = evaluate_eet(&eet, &result, tap, hap, 0, &mm);
	printf("retval: %d\n", retval);
	CuAssertTrue(tc, 1 == retval);
	printf("result: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	
	free(eet.nodes);
	close_scan(&s, &mm);
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 2000 == POINTERBYTEDIST(mm.last_back, mm.next_front));
	puts("********************************************************************************");
}


CuSuite *DBEETGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_eet_1);
	SUITE_ADD_TEST(suite, test_eet_2);
	SUITE_ADD_TEST(suite, test_eet_3);
	SUITE_ADD_TEST(suite, test_eet_4);
	SUITE_ADD_TEST(suite, test_eet_5);
	SUITE_ADD_TEST(suite, test_eet_6);
	SUITE_ADD_TEST(suite, test_eet_7);
	SUITE_ADD_TEST(suite, test_eet_8);
	SUITE_ADD_TEST(suite, test_eet_9);
	SUITE_ADD_TEST(suite, test_eet_10);
	SUITE_ADD_TEST(suite, test_eet_11);
	SUITE_ADD_TEST(suite, test_eet_12);
	SUITE_ADD_TEST(suite, test_eet_13);
	SUITE_ADD_TEST(suite, test_eet_14);
	SUITE_ADD_TEST(suite, test_eet_15);
	SUITE_ADD_TEST(suite, test_eet_16);
	SUITE_ADD_TEST(suite, test_eet_17);
	SUITE_ADD_TEST(suite, test_eet_18);
	SUITE_ADD_TEST(suite, test_eet_19);
	SUITE_ADD_TEST(suite, test_eet_20);
	SUITE_ADD_TEST(suite, test_eet_21);
	SUITE_ADD_TEST(suite, test_eet_22);
	SUITE_ADD_TEST(suite, test_eet_23);
	SUITE_ADD_TEST(suite, test_eet_24);
	SUITE_ADD_TEST(suite, test_eet_25);
	SUITE_ADD_TEST(suite, test_eet_26);
	SUITE_ADD_TEST(suite, test_eet_27);
#if 0
#endif
	
	return suite;
}

void runAllTests_eet()
{
	CuString *output = CuStringNew();
	CuSuite *suite = DBEETGetSuite();
	
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
