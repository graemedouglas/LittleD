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

/* The unit tests for projection operator. */
#include <stdio.h>
#include <string.h>
#include "../CuTest.h"
#include "../../dbobjects/relation.h"
#include "../../dbobjects/tuple.h"
#include "../../dbops/db_ops.h"
#include "../../dbops/project.h"
#include "../../dbops/scan.h"

void test_project_0(CuTest *tc)
{
	/* Variable declarations */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	project_t p;
	scan_t s;
	db_op_base_t *o;
	db_eet_t oneExpr[1];
	db_eetnode_t *arr_p;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	tuple_t next_t;
	db_int result;
	db_int i_attr;

	/* Test on relation 1, project out only field in tuple. */
	puts("*************************************************************");
	puts("Testing relation 1, projecting out only field.");
	init_scan(&s, "test_rel1", &mm);
	o = (db_op_base_t*)(&s);
	
	oneExpr[0].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	oneExpr[0].nodes = malloc(oneExpr[0].size);
	oneExpr[0].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbint_t));
	
	arr_p = oneExpr[0].nodes;
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	init_project(&p, o, oneExpr, 1, &mm);
	init_tuple(&next_t, s.base.header->tuple_size, s.base.header->tuple_size, &mm);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The db_int value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 1 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The db_int value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 2 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The db_int value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 3 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The db_int value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 9 == i_attr);

	close_scan(&s, &mm);
	close_project(&p, &mm);
	close_tuple(&next_t, &mm);
	free(oneExpr[0].nodes);
	puts("*************************************************************");
}

void test_project_1(CuTest *tc)
{
	/* Variable declarations */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	project_t p;
	scan_t s;
	db_op_base_t *o;
	db_eet_t oneExpr[1];
	db_eetnode_t *arr_p;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	tuple_t next_t;
	db_int result;
	char* s_attr;
	
	puts("*************************************************************");
	puts("Testing relation 3, projecting out only second attribute.");
	init_scan(&s, "test_rel3", &mm);
	o = (db_op_base_t*)(&s);
	
	oneExpr[0].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	oneExpr[0].nodes = malloc(oneExpr[0].size);
	oneExpr[0].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbstring_t));
	
	arr_p = oneExpr[0].nodes;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	init_project(&p, o, oneExpr, 1, &mm);
	init_tuple(&next_t, s.base.header->tuple_size, s.base.header->tuple_size, &mm);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	s_attr = getstringbypos(&next_t, 0, p.base.header);
	printf("The value in the tuple is: %s\n", s_attr);
	CuAssertTrue(tc, 0 == strcmp(s_attr, "Hi"));

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	s_attr = getstringbyname(&next_t, "a2", p.base.header);
	printf("The value in the tuple is: %s\n", s_attr);
	CuAssertTrue(tc, 0 == strcmp(s_attr, "linux"));

	close_scan(&s, &mm);
	close_project(&p, &mm);
	close_tuple(&next_t, &mm);
	free(oneExpr[0].nodes);
	puts("*************************************************************");
}

void test_project_2(CuTest *tc)
{
	/* Variable declarations */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	project_t p;
	scan_t s;
	db_op_base_t *o;
	db_eet_t twoExpr[2];
	db_eetnode_t *arr_p;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	tuple_t next_t;
	db_int result;
	db_int i_attr;
	
	puts("*************************************************************");
	puts("Testing relation 3, projecting out first and third attributes");
	init_scan(&s, "test_rel3", &mm);
	o = (db_op_base_t*)(&s);
	
	twoExpr[0].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	twoExpr[0].nodes = malloc(twoExpr[0].size);
	twoExpr[0].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbint_t));
	
	twoExpr[1].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	twoExpr[1].nodes = malloc(twoExpr[1].size);
	twoExpr[1].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbint_t));
	
	arr_p = twoExpr[0].nodes;	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p = twoExpr[1].nodes;	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	init_project(&p, o, twoExpr, 2, &mm);
	init_tuple(&next_t, s.base.header->tuple_size, s.base.header->tuple_size, &mm);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The first value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 1 == i_attr);
	i_attr = 789;
	i_attr = getintbypos(&next_t, 1, p.base.header);
	printf("The second value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 1 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The first value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 73 == i_attr);
	i_attr = getintbypos(&next_t, 1, p.base.header);
	printf("The second value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 4023 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_project(&p, &mm);
	close_tuple(&next_t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	puts("*************************************************************");
}

void test_project_3(CuTest *tc)
{
	/* Variable declarations */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	project_t p;
	project_t p2;
	scan_t s;
	db_op_base_t *o;
	db_eet_t oneExpr[1];
	db_eet_t twoExpr[2];
	db_eetnode_t *arr_p;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	tuple_t next_t;
	db_int result;
	db_int i_attr;
	
	puts("*************************************************************");
	puts("Testing relation 3, projecting out first and third attributes,\nfollowed by projecting out only third attribute.");
	init_scan(&s, "test_rel3", &mm);
	o = (db_op_base_t*)(&s);
	
	twoExpr[0].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	twoExpr[0].nodes = malloc(twoExpr[0].size);
	twoExpr[0].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbint_t));
	
	twoExpr[1].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	twoExpr[1].nodes = malloc(twoExpr[1].size);
	twoExpr[1].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbint_t));
	
	arr_p = twoExpr[0].nodes;	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p = twoExpr[1].nodes;	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	oneExpr[0].size = (db_uint8)(1*sizeof(db_eetnode_attr_t));
	oneExpr[0].nodes = malloc(oneExpr[0].size);
	oneExpr[0].stack_size = (db_uint8)(1*sizeof(db_eetnode_dbstring_t));
	
	arr_p = oneExpr[0].nodes;	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	init_project(&p, o, twoExpr, 2, &mm);
	init_project(&p2, (db_op_base_t*)&p, oneExpr, 1, &mm);
	init_tuple(&next_t, s.base.header->tuple_size, s.base.header->tuple_size, &mm);

	result = next((db_op_base_t*)(&p2), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p2.base.header);
	printf("The first value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 1 == i_attr);

	result = next((db_op_base_t*)(&p2), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p2.base.header);
	printf("The first value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 4023 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_project(&p, &mm);
	close_project(&p2, &mm);
	close_tuple(&next_t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(oneExpr[0].nodes);
	puts("*************************************************************");
}

void test_project_4(CuTest *tc)
{
	/* Variable declarations */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	project_t p;
	scan_t s;
	db_op_base_t *o;
	db_eet_t twoExpr[2];
	db_eetnode_t *arr_p;
	db_eetnode_t opNode;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	tuple_t next_t;
	db_int result;
	db_int i_attr;
	
	puts("*************************************************************");
	puts("Testing relation 3, projecting length(attr(1)) and attr(0) + attr(2).");
	init_scan(&s, "test_rel3", &mm);
	o = (db_op_base_t*)(&s);
	
	twoExpr[0].size = (db_uint8)(1*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbstring_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)(twoExpr[0].size));
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (db_uint8)(2*sizeof(db_eetnode_attr_t) + 0*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_FUNC_LENGTH_DBSTRING;
	*(arr_p) = opNode;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_ADD;
	*(arr_p) = opNode;
	arr_p++;
	
	init_project(&p, o, twoExpr, 2, &mm);
	init_tuple(&next_t, s.base.header->tuple_size, s.base.header->tuple_size, &mm);
	
	result = next((db_op_base_t*)(&p), &next_t, &mm);
	printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The first value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 2 == i_attr);
	i_attr = 789;
	i_attr = getintbypos(&next_t, 1, p.base.header);
	printf("The second value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 2 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 1 == result);
	i_attr = getintbypos(&next_t, 0, p.base.header);
	printf("The first value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 5 == i_attr);
	i_attr = getintbypos(&next_t, 1, p.base.header);
	printf("The second value in the tuple is: %d\n", i_attr);
	CuAssertTrue(tc, 4096 == i_attr);

	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 0 == result);
	result = next((db_op_base_t*)(&p), &next_t, &mm);
	//printf("The value of result is: %d\n", result);
	CuAssertTrue(tc, 0 == result);

	close_scan(&s, &mm);
	close_project(&p, &mm);
	close_tuple(&next_t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	puts("*************************************************************");
}

CuSuite *DBProjectGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_project_0);
	SUITE_ADD_TEST(suite, test_project_1);
	SUITE_ADD_TEST(suite, test_project_2);
	SUITE_ADD_TEST(suite, test_project_3);
	SUITE_ADD_TEST(suite, test_project_4);
	
	return suite;
}

void runAllTests_project()
{
	CuString *output = CuStringNew();
	CuSuite *suite = DBProjectGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
