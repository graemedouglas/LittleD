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

/* The unit tests for scan operator. */
#include <stdio.h>
#include <string.h>
#include "../CuTest.h"
#include "../../dbmacros.h"
#include "../../dbops/scan.h"

void test_scan_0(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "test_rel1", &mm));
	tuple_t next;
	init_tuple(&next, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	db_int result;
	db_int attr;
	
	puts("*************************************************************");
	puts("Testing scanning of test relation 1.\n");
	/** Verify that the header information is as we expect. */
	printf("The number of attributes for this relation is: %d\n", s.base.header->num_attr);
	CuAssertTrue(tc, s.base.header->num_attr == 1);
	printf("Attribute %d's name is: %s\n", 0, s.base.header->names[0]);
	CuAssertTrue(tc, 0 == (strcmp(s.base.header->names[0], "Apple")));
	printf("Attribute %d's type is: %d\n", 0, s.base.header->types[0]);
	CuAssertTrue(tc, s.base.header->types[0] == 0);
	printf("Attribute %d's offset is: %d\n", 0, s.base.header->offsets[0]);
	CuAssertTrue(tc, s.base.header->offsets[0] == 0);
	printf("Attribute %d's size is: %d\n", 0, (db_int)s.base.header->sizes[0]);
	CuAssertTrue(tc, (size_t)s.base.header->sizes[0] == sizeof(db_int));
	
	/** Verify that the records are as we expect. */
	
	result = next_scan(&s, &next, &mm);
	printf("Tuple's isnull array: %d\n", (db_int)(next.isnull[0]));
	CuAssertTrue(tc, next.isnull[0] == 0);
	
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 1 == attr);
	attr = getintbyname(&next, "Apple", s.base.header);
	printf("The db_int in the tuple, retrieved by name \"%s\", is: %d\n", "Apple", attr);
	CuAssertTrue(tc, 1 == attr);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 2 == attr);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 3 == attr);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbyname(&next, "Apple", s.base.header);
	printf("The db_int in the tuple, retrieved by name \"%s\", is: %d\n", "Apple", attr);
	CuAssertTrue(tc, 9 == attr);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 0 == result);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 0 == result);
	rewind_scan(&s, &mm);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 1 == attr);
	rewind_scan(&s, &mm);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 1 == attr);
	
	close_tuple(&next, &mm);
	close_scan(&s, &mm);
	puts("*************************************************************");
}

void test_scan_1(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t s;
	init_scan(&s, "test_rel2", &mm);
	tuple_t next;
	init_tuple(&next, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	db_int result;
	
	puts("*************************************************************");
	puts("Testing scanning of test relation 2.\n");
	/** Verify that the header information is as we expect. */
	printf("The number of attributes for this relation is: %d\n", s.base.header->num_attr);
	CuAssertTrue(tc, s.base.header->num_attr == 3);
	printf("Attribute %d's name is: %s\n", 0, s.base.header->names[0]);
	CuAssertTrue(tc, 0 == (strcmp(s.base.header->names[0], "A1")));
	printf("Attribute %d's type is: %d\n", 0, s.base.header->types[0]);
	CuAssertTrue(tc, s.base.header->types[0] == 0);
	printf("Attribute %d's offset is: %d\n", 0, s.base.header->offsets[0]);
	CuAssertTrue(tc, s.base.header->offsets[0] == 0);
	printf("Attribute %d's size is: %d\n", 0, (db_int)s.base.header->sizes[0]);
	CuAssertTrue(tc, s.base.header->sizes[0] == sizeof(db_int));
	printf("Attribute %d's name is: %s\n", 1, s.base.header->names[1]);
	CuAssertTrue(tc, 0 == (strcmp(s.base.header->names[1], "a2")));
	printf("Attribute %d's type is: %d\n", 1, s.base.header->types[1]);
	CuAssertTrue(tc, s.base.header->types[1] == 1);
	printf("Attribute %d's offset is: %d\n", 1, s.base.header->offsets[1]);
	CuAssertTrue(tc, s.base.header->offsets[1] == 4);
	printf("Attribute %d's size is: %d\n", 1, (db_int)s.base.header->sizes[1]);
	CuAssertTrue(tc, s.base.header->sizes[1] == 10);
	
	result = next_scan(&s, &next, &mm);
	
	CuAssertTrue(tc, 0 == result);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 0 == result);
	close_tuple(&next, &mm);
	close_scan(&s, &mm);
	puts("*************************************************************");
}

void test_scan_2(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t s;
	tuple_t next;
	db_int result;
	db_int attr;
	char* str_attr;
	puts("*************************************************************");
	puts("Testing scanning of test relation 3.\n");
	init_scan(&s, "test_rel3", &mm);
	init_tuple(&next, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	/** Verify that the header information is as we expect. */
	printf("The number of attributes for this relation is: %d\n", s.base.header->num_attr);
	CuAssertTrue(tc, s.base.header->num_attr == 3);
	printf("Attribute %d's name is: %s\n", 0, s.base.header->names[0]);
	CuAssertTrue(tc, 0 == (strcmp(s.base.header->names[0], "A1")));
	printf("Attribute %d's type is: %d\n", 0, s.base.header->types[0]);
	CuAssertTrue(tc, s.base.header->types[0] == 0);
	printf("Attribute %d's offset is: %d\n", 0, s.base.header->offsets[0]);
	CuAssertTrue(tc, s.base.header->offsets[0] == 0);
	printf("Attribute %d's size is: %d\n", 0, (db_int)s.base.header->sizes[0]);
	CuAssertTrue(tc, s.base.header->sizes[0] == sizeof(db_int));
	printf("Attribute %d's name is: %s\n", 1, s.base.header->names[1]);
	CuAssertTrue(tc, 0 == (strcmp(s.base.header->names[1], "a2")));
	printf("Attribute %d's type is: %d\n", 1, s.base.header->types[1]);
	CuAssertTrue(tc, s.base.header->types[1] == 1);
	printf("Attribute %d's offset is: %d\n", 1, s.base.header->offsets[1]);
	CuAssertTrue(tc, s.base.header->offsets[1] == 4);
	printf("Attribute %d's size is: %d\n", 1, (db_int)s.base.header->sizes[1]);
	CuAssertTrue(tc, s.base.header->sizes[1] == 10);
	printf("Attribute %d's name is: %s\n", 2, s.base.header->names[2]);
	CuAssertTrue(tc, 0 == (strcmp(s.base.header->names[2], "3")));
	printf("Attribute %d's type is: %d\n", 2, s.base.header->types[2]);
	CuAssertTrue(tc, s.base.header->types[2] == 0);
	printf("Attribute %d's offset is: %d\n", 2, s.base.header->offsets[2]);
	CuAssertTrue(tc, s.base.header->offsets[2] == 14);
	printf("Attribute %d's size is: %d\n", 2, (db_int)s.base.header->sizes[2]);
	CuAssertTrue(tc, s.base.header->sizes[2] == 4);

	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 1 == attr);
	str_attr = getstringbypos(&next, 1, s.base.header);
	printf("The string in the tuple, retrieved at position %d, is: %s\n", 1, str_attr);
	CuAssertTrue(tc, 0 == strcmp("Hi", str_attr));
	str_attr = getstringbyname(&next, "a2", s.base.header);
	printf("The string in the tuple, retrieved by name \"a2\", is: %s\n", str_attr);
	CuAssertTrue(tc, 0 == strcmp("Hi", str_attr));
	attr = getintbypos(&next, 2, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 2, attr);
	CuAssertTrue(tc, 1 == attr);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 1 == result);
	attr = getintbypos(&next, 0, s.base.header);
	printf("The db_int in the tuple, retrieved at position %d, is: %d\n", 0, attr);
	CuAssertTrue(tc, 73 == attr);
	str_attr = getstringbyname(&next, "a2", s.base.header);
	printf("The string in the tuple, retrieved by name \"a2\", is: %s\n", str_attr);
	CuAssertTrue(tc, 0 == strcmp("linux", str_attr));
	attr = getintbyname(&next, "3", s.base.header);
	printf("The db_int in the tuple, retrieved by name \"%s\", is: %d\n", "3", attr);
	CuAssertTrue(tc, 4023 == attr);
	result = next_scan(&s, &next, &mm);
	CuAssertTrue(tc, 0 == result);
	close_tuple(&next, &mm);
	close_scan(&s, &mm);
	puts("*************************************************************");
}

void test_scan_3(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t s;
	tuple_t next;
	
	puts("*************************************************************");
	puts("Testing scanning of fruit_stock_2 (NULL scanning).\n");
	init_scan(&s, "fruit_stock_2", &mm);
	init_tuple(&next, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 7 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 0 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 2 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 1 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 4 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 3 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 5 == next.isnull[0]);
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 6 == next.isnull[0]);
	
	CuAssertTrue(tc, 0 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 0 == next_scan(&s, &next, &mm));
	
	close_tuple(&next, &mm);
	close_scan(&s, &mm);
	puts("*************************************************************");
}

void test_scan_4(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	scan_t s;
	tuple_t next;
	
	puts("*************************************************************");
	puts("Testing scanning of tenattrtable (NULL scanning).\n");
	init_scan(&s, "tenattrtable", &mm);
	init_tuple(&next, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 0 == next.isnull[0]);
	CuAssertTrue(tc, 0 == next.isnull[1]);
	
	CuAssertTrue(tc, 1 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 1 == next.isnull[0]);
	CuAssertTrue(tc, 2 == next.isnull[1]);
	
	CuAssertTrue(tc, 0 == next_scan(&s, &next, &mm));
	CuAssertTrue(tc, 0 == next_scan(&s, &next, &mm));
	
	close_tuple(&next, &mm);
	close_scan(&s, &mm);
	puts("*************************************************************");
}

CuSuite *DBScanGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_scan_0);
	SUITE_ADD_TEST(suite, test_scan_1);
	SUITE_ADD_TEST(suite, test_scan_2);
	SUITE_ADD_TEST(suite, test_scan_3);
	SUITE_ADD_TEST(suite, test_scan_4);
	
	return suite;
}

void runAllTests_scan()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBScanGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
