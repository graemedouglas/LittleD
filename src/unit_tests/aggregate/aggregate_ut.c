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
#include "../../dbops/db_ops_types.h"
#include "../../dbobjects/relation.h"
#include "../../dbobjects/tuple.h"
#include "../../dblogic/compare_tuple.h"
#include "../../dblogic/eet.h"
#include "../../dbops/db_ops.h"
#include "../../dbops/scan.h"
#include "../../dbops/project.h"
#include "../../dbops/select.h"
#include "../../dbops/sort.h"
#include "../../dbops/aggregate.h"
#include "../../db_ctconf.h"

#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1

void test_aggregate_1(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 1: Sum all numbers in first attribute of fruit relation.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (1*sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	printf("aggr.base.header->tuple_size: %d\n", aggr.base.header->tuple_size);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_2(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 2: Sum all numbers in third attribute of fruit relation.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (1*sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 78 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_3(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 3: Sum all numbers in third attribute of fruit relation, with\nnull GROUP BY clause.");
	fflush(stdout);
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// GROUP BY expr will be NULL.
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, NULL, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 78 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_4(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 4: Sum all numbers in third attribute of fruit relation, grouping\non the first attribute.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 29 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 12 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_5(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 5: Sum all numbers in first attribute of fruit relation, grouping\non the third attribute.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 6 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_6(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 6: Sum all numbers in first attribute of fruit relation, grouping\non the 4th attribute.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_7(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 7: Sum all numbers in first attribute of fruit relation, grouping\non the second attribute.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbstring_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_8(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 8: Sum all numbers in third attribute of fruit relation, grouping\non the second attribute, having sum > 10.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbstring_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_aggr_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 0;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 12 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 29 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_9(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 9: Sum all numbers in third attribute of fruit relation, grouping\non the fourth attribute, having sum > 15.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_aggr_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 0;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 15;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 39 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_10(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 10: Sum all numbers in fourth attribute of fruit relation, grouping\non the third attribute, having sum > 1000.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_aggr_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 0;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 1000;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_11(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	char* strResult;
	
	puts("**********************************************************************");
	puts("Test 11: Sum all numbers in third attribute of fruit relation and also\nselect second attribute, grouping on the second attribute,\nhaving sum > 11.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbstring_t) + sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbstring_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 0;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 11;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	CuAssertTrue(tc, 1 == init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, oneGroupExpr, 1, &havingExpr, &mm));
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 12 == intResult);
	strResult = getstringbypos(&t, 1, aggr.base.header);
	printf("The string stored at position %d in the tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, strcmp(strResult, "Lemon") == 0);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 29 == intResult);
	strResult = getstringbypos(&t, 1, aggr.base.header);
	printf("The string stored at position %d in the tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, strcmp(strResult, "Orange") == 0);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	strResult = getstringbypos(&t, 1, aggr.base.header);
	printf("The string stored at position %d in the tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, strcmp(strResult, "Pear") == 0);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_12(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_dbstring_t dbstringNode;
	dbstringNode.base.type = DB_EETNODE_CONST_DBSTRING;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	char* strResult;
	
	puts("**********************************************************************");
	puts("Test 12: Sum all numbers in third attribute of fruit relation and also\nselect second attribute, grouping on the second attribute,\nhaving sum > 11 and name != 'Orange'.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbstring_t) + sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbstring_t) + sizeof(db_eetnode_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (4*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + 2*sizeof(db_eetnode_dbint_t) +
				sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbstring_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_AND;
	*arr_p = opNode;
	arr_p++;
	
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 0;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 11;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_NEQ;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbstringNode.string = "Orange";
	*((db_eetnode_dbstring_t*)arr_p) = dbstringNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 12 == intResult);
	strResult = getstringbypos(&t, 1, aggr.base.header);
	printf("The string stored at position %d in the tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, strcmp(strResult, "Lemon") == 0);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	strResult = getstringbypos(&t, 1, aggr.base.header);
	printf("The string stored at position %d in the tuple is: %s\n", 1, strResult);
	CuAssertTrue(tc, strcmp(strResult, "Pear") == 0);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_13(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 13: Select third attribute and sum of first attribute, grouping\non the third and fourth attributes,\nhaving sum > 2.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 2;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 10 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 12 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_14(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 14: Select 7 + third attribute and sum(first attribute) * 5,\ngrouping on the third+10 and fourth attributes,\nhaving sum > 7.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_attr_t));
	twoExpr[0].nodes = malloc((size_t)(twoExpr[0].size));
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_15(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 15: Select 7 + third attribute and 5*sum(first attribute),\ngrouping on the third+10 and fourth attributes,\nhaving sum > 7.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_16(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 16: Select 7 (constant) and 5*sum(first attribute),\ngrouping on the third+10 and fourth attributes,\nhaving sum > 7, also testing rewind.");
	// Build projecting expression.
	twoExpr[0].size = (sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = rewind_dbop((db_op_base_t*)&aggr, &mm);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 7 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_17(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 17: Select 7 + third attribute and 5*count(first attribute),\ngrouping on the third+10 and third attributes,\nhaving count > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_COUNTROWS;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_18(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 18: Select 7 + third attribute and 5*min(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_MIN;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_19(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 19: Select 7 + third attribute and 5*max(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_MAX;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_20(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 20: Select 7 + third attribute and 5*first(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_FIRST;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_21(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 21: Select 7 + third attribute and 5*last(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_LAST;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_22(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 22: Select 7 + third attribute and 5*AND(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_BAND;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_23(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 23: Select 7 + third attribute and 5*OR(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_BOR;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 25 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_24(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 24: Select 7 + third attribute and 5*XOR(first attribute),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_BXOR;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_1", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 20 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_26(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 26: Sum all numbers in third attribute of fruit relation and also\nselect fourth attribute, grouping on the fourth attribute.");
	// Build projecting expression.
	
	twoExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 29 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 12 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 6 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1374 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 9 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_27(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 27: Sum all numbers in third attribute of fruit relation and also\nselect first attribute, grouping on the first attribute.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 1) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1386 == intResult);
	//intResult = getintbypos(&t, 1, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	//CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 29 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 8 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_28(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 28: Count all rows in fruit_stock_2.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_COUNTROWS;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 8 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_29(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t oneExpr[1];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 29: Count rows of tenattrtable, grouping\non the third attribute.");
	// Build projecting expression.
	oneExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	oneExpr[0].nodes = malloc((size_t)oneExpr[0].size);
	oneExpr[0].stack_size = oneExpr[0].size;

	arr_p = oneExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_COUNTROWS;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 3;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), oneExpr, 1, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 4 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(oneExpr[0].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_30(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t oneGroupExpr[1];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 30: Count rows, grouping on the 10th attribute.");
	// Build projecting expression.
	twoExpr[0].size = (2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t) + sizeof(db_eetnode_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	aggrTNode.aggr_type = DB_AGGR_COUNTROWS;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	attrNode.pos = 9;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	oneGroupExpr[0].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	oneGroupExpr[0].nodes = malloc((size_t)oneGroupExpr[0].size);
	oneGroupExpr[0].stack_size = oneGroupExpr[0].size;
	
	arr_p = oneGroupExpr[0].nodes;
	attrNode.pos = 9;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (1*sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "tenattrtable", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, oneGroupExpr, 1, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 1) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(oneGroupExpr[0].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_31(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 31: Select 7 + third attribute and SUM(5 * (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 65 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 15 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 10 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_32(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 32: Select 7 + third attribute and SUM(5 + (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 23 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 8 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 7 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_33(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 33: Select 7 + third attribute and SUM(5 - (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_SUB;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_34(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 34: Select 7 + third attribute and SUM(5 / (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_DIV;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_35(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 35: Select 7 + third attribute and SUM(5 % (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_MOD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_36(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 36: Select 7 + third attribute and SUM(5 AND (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_AND;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_37(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_aggr_attr_t aggrANode;
	aggrANode.base.type = DB_EETNODE_AGGR_ATTR;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 37: Select 7 + third attribute and SUM(5 = (attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_EQ;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	opNode.type = DB_EETNODE_OP_GT;
	*arr_p = opNode;
	arr_p++;
	
	aggrANode.pos = 1;
	aggrANode.tuple_pos = 0;
	*((db_eetnode_aggr_attr_t*)arr_p) = aggrANode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_attr_t*)arr_p)+1));
	
	dbintNode.integer = 0;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_38(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 38: Select 7 + third attribute and SUM(!(attr(0)),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_NOT;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 5;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1381 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_39(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 39: Select 7 + third attribute and SUM((attr(0) ISNULL),\ngrouping on the third+10 and third attributes,\nhaving aggregate > 0.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_OP_ISNULL;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1381 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_40(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t twoExpr[2];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 40: Select 7 + third attribute and SUM(length(attr(1)),\ngrouping on the third+10 and third attributes.");
	// Build projecting expression.
	twoExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	twoExpr[0].nodes = malloc((size_t)twoExpr[0].size);
	twoExpr[0].stack_size = twoExpr[0].size;
	
	twoExpr[1].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoExpr[1].nodes = malloc((size_t)twoExpr[1].size);
	twoExpr[1].stack_size = twoExpr[1].size;
	
	arr_p = twoExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = twoExpr[1].nodes;
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_FUNC_LENGTH_DBSTRING;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), twoExpr, 2, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 9 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 1) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	//intResult = getintbypos(&t, 1, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	//CuAssertTrue(tc, 0 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 6 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 1) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1381 == intResult);
	//intResult = getintbypos(&t, 1, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	//CuAssertTrue(tc, 1 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(twoExpr[0].nodes);
	free(twoExpr[1].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

void test_aggregate_41(CuTest *tc)
{
	/* General variable declaration. */
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t scan;
	aggregate_t aggr;
	tuple_t t;
	db_eet_t threeExpr[3];
	db_eet_t twoGroupExpr[2];
	db_eet_t havingExpr;
	db_eetnode_attr_t attrNode;
	attrNode.base.type = DB_EETNODE_ATTR;
	db_eetnode_dbint_t dbintNode;
	dbintNode.base.type = DB_EETNODE_CONST_DBINT;
	db_eetnode_t opNode;
	db_eetnode_aggr_temp_t aggrTNode;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.value_p = NULL;
	db_eetnode_t *arr_p;
	db_int intResult;
	
	puts("**********************************************************************");
	puts("Test 41: Select 7 + third attribute and SUM(length(attr(1))*count(*))\nand max(attr(0)), grouping on the third+10 and third attributes.");
	// Build projecting expression.
	threeExpr[0].size = (3*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	threeExpr[0].nodes = malloc((size_t)threeExpr[0].size);
	threeExpr[0].stack_size = threeExpr[0].size;
	
	threeExpr[1].size = (4*sizeof(db_eetnode_t) + 3*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + 3*sizeof(db_eetnode_dbint_t));
	threeExpr[1].nodes = malloc((size_t)threeExpr[1].size);
	threeExpr[1].stack_size = threeExpr[1].size;
	
	threeExpr[2].size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_temp_t) + sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	threeExpr[2].nodes = malloc((size_t)threeExpr[2].size);
	threeExpr[2].stack_size = threeExpr[2].size;
	
	arr_p = threeExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	dbintNode.integer = 7;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = threeExpr[1].nodes;
	opNode.type = DB_EETNODE_OP_MULT;
	*arr_p = opNode;
	arr_p++;
	
	aggrTNode.aggr_type = DB_AGGR_SUM;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	opNode.type = DB_EETNODE_FUNC_LENGTH_DBSTRING;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 1;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	aggrTNode.aggr_type = DB_AGGR_COUNTROWS;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	arr_p = threeExpr[2].nodes;
	aggrTNode.base.type = DB_EETNODE_AGGR_TEMP;
	aggrTNode.aggr_type = DB_AGGR_MAX;
	*((db_eetnode_aggr_temp_t*)arr_p) = aggrTNode;
	arr_p->type = DB_EETNODE_AGGR_TEMP;
	arr_p = ((db_eetnode_t*)(((db_eetnode_aggr_temp_t*)arr_p)+1));
	
	attrNode.pos = 0;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	arr_p->type = DB_EETNODE_COUNT;
	arr_p++;
	
	// Build GROUP BY expression.
	twoGroupExpr[0].size = (2*sizeof(db_eetnode_t) + sizeof(db_eetnode_attr_t) + 2*sizeof(db_eetnode_dbint_t));
	twoGroupExpr[0].nodes = malloc((size_t)twoGroupExpr[0].size);
	twoGroupExpr[0].stack_size = twoGroupExpr[0].size;
	
	twoGroupExpr[1].size = (sizeof(db_eetnode_attr_t) + sizeof(db_eetnode_dbint_t));
	twoGroupExpr[1].nodes = malloc((size_t)twoGroupExpr[1].size);
	twoGroupExpr[1].stack_size = twoGroupExpr[1].size;
	
	arr_p = twoGroupExpr[0].nodes;
	opNode.type = DB_EETNODE_OP_ADD;
	*arr_p = opNode;
	arr_p++;
	
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	dbintNode.integer = 10;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	arr_p = twoGroupExpr[1].nodes;
	attrNode.pos = 2;
	attrNode.tuple_pos = 0;
	*((db_eetnode_attr_t*)arr_p) = attrNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_attr_t*)arr_p)+1));
	
	// Build the HAVING expression.
	havingExpr.size = (2*sizeof(db_eetnode_t) + 2*sizeof(db_eetnode_aggr_attr_t) + sizeof(db_eetnode_dbint_t));
	havingExpr.nodes = malloc((size_t)havingExpr.size);
	havingExpr.stack_size = havingExpr.size;
	
	arr_p = havingExpr.nodes;
	dbintNode.integer = 1;
	*((db_eetnode_dbint_t*)arr_p) = dbintNode;
	arr_p = ((db_eetnode_t*)(((db_eetnode_dbint_t*)arr_p)+1));
	
	// Initialize operators.
	init_scan(&scan, "fruit_stock_2", &mm);
	init_aggregate(&aggr, ((db_op_base_t*)&scan), threeExpr, 3, twoGroupExpr, 2, &havingExpr, &mm);
	init_tuple(&t, aggr.base.header->tuple_size, aggr.base.header->num_attr, &mm);
	
	// Run the test.
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 0) == t.isnull[0]);
	//intResult = getintbypos(&t, 0, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	//CuAssertTrue(tc, 17 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 2, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 8 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 2) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 19 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 5 == intResult);
	//intResult = getintbypos(&t, 2, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 2, intResult);
	//CuAssertTrue(tc, 8 == intResult);
	
	/* TODO: Figure out why the second attribute isn't NULL. */
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 1) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 24 == intResult);
	//intResult = getintbypos(&t, 1, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	//CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 2, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 3 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, 0 == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 36 == intResult);
	intResult = getintbypos(&t, 1, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	CuAssertTrue(tc, 6 == intResult);
	intResult = getintbypos(&t, 2, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 2, intResult);
	CuAssertTrue(tc, 2 == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	printf("The result of the call to next(...) is: %d\n", intResult);
	CuAssertTrue(tc, 1 == intResult);
	printf("t.isnull[0]: %d\n", (db_int)(t.isnull[0]));
	CuAssertTrue(tc, (1 << 1) + (1 << 2) == t.isnull[0]);
	intResult = getintbypos(&t, 0, aggr.base.header);
	printf("The db_int stored at position %d in the tuple is: %d\n", 0, intResult);
	CuAssertTrue(tc, 1381 == intResult);
	//intResult = getintbypos(&t, 1, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 1, intResult);
	//CuAssertTrue(tc, 36 == intResult);
	//intResult = getintbypos(&t, 2, aggr.base.header);
	//printf("The db_int stored at position %d in the tuple is: %d\n", 2, intResult);
	//CuAssertTrue(tc,  == intResult);
	
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);
	intResult = next((db_op_base_t*)&aggr, &t, &mm);
	CuAssertTrue(tc, 0 == intResult);

	// Free all memory used.
	close_scan(&scan, &mm);
	close((db_op_base_t*)&aggr, &mm);
	close_tuple(&t, &mm);
	free(threeExpr[0].nodes);
	free(threeExpr[1].nodes);
	free(threeExpr[2].nodes);
	free(twoGroupExpr[0].nodes);
	free(twoGroupExpr[1].nodes);
	free(havingExpr.nodes);
	puts("**********************************************************************");
}

#endif
#endif

CuSuite *DBAggregateGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1

	SUITE_ADD_TEST(suite, test_aggregate_1);
	SUITE_ADD_TEST(suite, test_aggregate_2);
	SUITE_ADD_TEST(suite, test_aggregate_3);
	SUITE_ADD_TEST(suite, test_aggregate_4);
	SUITE_ADD_TEST(suite, test_aggregate_5);
	SUITE_ADD_TEST(suite, test_aggregate_6);
	SUITE_ADD_TEST(suite, test_aggregate_7);
	SUITE_ADD_TEST(suite, test_aggregate_8);
	SUITE_ADD_TEST(suite, test_aggregate_9);
	SUITE_ADD_TEST(suite, test_aggregate_10);
	SUITE_ADD_TEST(suite, test_aggregate_11);
	SUITE_ADD_TEST(suite, test_aggregate_12);
	SUITE_ADD_TEST(suite, test_aggregate_13);
	SUITE_ADD_TEST(suite, test_aggregate_14);
	SUITE_ADD_TEST(suite, test_aggregate_15);
	SUITE_ADD_TEST(suite, test_aggregate_16);
	SUITE_ADD_TEST(suite, test_aggregate_17);
	SUITE_ADD_TEST(suite, test_aggregate_18);
	SUITE_ADD_TEST(suite, test_aggregate_19);
	SUITE_ADD_TEST(suite, test_aggregate_20);
	SUITE_ADD_TEST(suite, test_aggregate_21);
	SUITE_ADD_TEST(suite, test_aggregate_22);
	SUITE_ADD_TEST(suite, test_aggregate_23);
	SUITE_ADD_TEST(suite, test_aggregate_24);
	SUITE_ADD_TEST(suite, test_aggregate_26);
	SUITE_ADD_TEST(suite, test_aggregate_27);
	SUITE_ADD_TEST(suite, test_aggregate_28);
	SUITE_ADD_TEST(suite, test_aggregate_29);
	SUITE_ADD_TEST(suite, test_aggregate_30);
	SUITE_ADD_TEST(suite, test_aggregate_31);
	SUITE_ADD_TEST(suite, test_aggregate_32);
	SUITE_ADD_TEST(suite, test_aggregate_33);
	SUITE_ADD_TEST(suite, test_aggregate_34);
	SUITE_ADD_TEST(suite, test_aggregate_35);
	SUITE_ADD_TEST(suite, test_aggregate_36);
	SUITE_ADD_TEST(suite, test_aggregate_37);
	SUITE_ADD_TEST(suite, test_aggregate_38);
	SUITE_ADD_TEST(suite, test_aggregate_39);
	SUITE_ADD_TEST(suite, test_aggregate_40);
	SUITE_ADD_TEST(suite, test_aggregate_41);

#endif
#endif
	
	return suite;
}

void runAllTests_aggregate()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBAggregateGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
