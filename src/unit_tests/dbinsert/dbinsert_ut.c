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

#include <stdio.h>
#include <string.h>
#include "../CuTest.h"
#include "../../ref.h"
#include "../../dbobjects/relation.h"
#include "../../dbmacros.h"
#include "../../dbmm/db_query_mm.h"
#include "../../dbparser/dblexer.h"
#include "../../dbparser/dbparser.h"
#include "../../dbparser/dbinsert.h"
#include "../../dbstorage/dbstorage.h"

#if defined(DB_CTCONF_SETTING_FEATURE_CREATE_TABLE) && 1==DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
void test_dbinsert_1(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (attr0 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1)";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 1==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_2(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (attr0 STRING(10));";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES ('abcdefghi')";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	tempstrp = getstringbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcdefghi", tempstrp));
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_3(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (attr0 STRING(10));";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES ('abcd')";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	tempstrp = getstringbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcd", tempstrp));
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_4(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (attr0 STRING(10), attr1 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES ('abcd', 45)";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	tempstrp = getstringbypos(&t, 0, s.base.header);
	temp = getintbypos(&t, 1, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcd", tempstrp));
	CuAssertTrue(tc, 45==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_5(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (attr0 STRING(10), attr1 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES ('abcdefghi', 45)";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	tempstrp = getstringbypos(&t, 0, s.base.header);
	temp = getintbypos(&t, 1, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcdefghi", tempstrp));
	CuAssertTrue(tc, 45==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_6(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (-3, 'abcdefghi', 45);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, -3==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcdefghi", tempstrp));
	CuAssertTrue(tc, 45==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_7(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a0, a1, a2) VALUES (-3, 'abcdefghi', 45);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, -3==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcdefghi", tempstrp));
	CuAssertTrue(tc, 45==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_8(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a2, a1, a0) VALUES (-3, 'abcdefghi', 45);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, -3==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcdefghi", tempstrp));
	CuAssertTrue(tc, 45==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_9(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a1, a2, a0) VALUES ('abcdefghi', -3, 45);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, -3==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 0==strcmp("abcdefghi", tempstrp));
	CuAssertTrue(tc, 45==temp);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_10(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a2, a1) VALUES (5, 'aabbc');";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, 5==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 0==strcmp("aabbc", tempstrp));
	CuAssertTrue(tc, 0==temp);
	CuAssertTrue(tc, 1==t.isnull[0]);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_11(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a1, a0) VALUES ('aabbc', 5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 5==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, 0==strcmp("aabbc", tempstrp));
	CuAssertTrue(tc, 0==temp);
	CuAssertTrue(tc, 4==t.isnull[0]);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_12(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a2) VALUES (5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 1==insert_command(&lexer, strlen(command), &mm));
	
	db_uint8 temp8;
	db_int temp;
	char tempstring[100];
	char *tempstrp;
	
	scan_t s;
	CuAssertTrue(tc, 1==init_scan(&s, "mytable", &mm));
	
	tuple_t t;
	init_tuple(&t, s.base.header->tuple_size, s.base.header->num_attr, &mm);
	
	temp = next_scan(&s, &t, &mm);
	CuAssertTrue(tc, 1==temp);
	temp = getintbypos(&t, 0, s.base.header);
	CuAssertTrue(tc, 0==temp);
	tempstrp = getstringbypos(&t, 1, s.base.header);
	temp = getintbypos(&t, 2, s.base.header);
	CuAssertTrue(tc, 0==strcmp("", tempstrp));
	CuAssertTrue(tc, 5==temp);
	CuAssertTrue(tc, 3==t.isnull[0]);
	
	close_tuple(&t, &mm);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_1(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a3) VALUES (5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_2(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable1 (a0) VALUES (5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_3(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT VALUES (5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_4(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a0 a1 a2) VALUES (5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_5(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a0";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_6(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (a0, a1, a2) (5, '1234', 4);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_7(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable (5);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_8(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES 1, '2', 3);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_9(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, '2', 3, 4);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_10(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, '2' 3);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_11(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, '2',);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_12(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, '2'";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_13(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, '2',";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_14(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, -'2', 4)";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbinsert_error_15(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char *createcommand = "CREATE TABLE mytable (a0 INT, a1 STRING(13), a2 INT);";
	char *tablename = "mytable";
	CuAssertTrue(tc, DB_PARSER_OP_NONE==parse(createcommand, &mm));
	char *command = "INSERT mytable VALUES (1, '2', 4";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	lexer_next(&lexer);
	CuAssertTrue(tc, 0==insert_command(&lexer, strlen(command), &mm));
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}
#endif

CuSuite *DBInsertGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
#if defined(DB_CTCONF_SETTING_FEATURE_CREATE_TABLE) && 1==DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
	SUITE_ADD_TEST(suite, test_dbinsert_1);
	SUITE_ADD_TEST(suite, test_dbinsert_2);
	SUITE_ADD_TEST(suite, test_dbinsert_3);
	SUITE_ADD_TEST(suite, test_dbinsert_4);
	SUITE_ADD_TEST(suite, test_dbinsert_5);
	SUITE_ADD_TEST(suite, test_dbinsert_6);
	SUITE_ADD_TEST(suite, test_dbinsert_7);
	SUITE_ADD_TEST(suite, test_dbinsert_8);
	SUITE_ADD_TEST(suite, test_dbinsert_9);
	SUITE_ADD_TEST(suite, test_dbinsert_10);
	SUITE_ADD_TEST(suite, test_dbinsert_11);
	SUITE_ADD_TEST(suite, test_dbinsert_12);
	SUITE_ADD_TEST(suite, test_dbinsert_error_1);
	SUITE_ADD_TEST(suite, test_dbinsert_error_2);
	SUITE_ADD_TEST(suite, test_dbinsert_error_3);
	SUITE_ADD_TEST(suite, test_dbinsert_error_4);
	SUITE_ADD_TEST(suite, test_dbinsert_error_5);
	SUITE_ADD_TEST(suite, test_dbinsert_error_6);
	SUITE_ADD_TEST(suite, test_dbinsert_error_7);
	SUITE_ADD_TEST(suite, test_dbinsert_error_8);
	SUITE_ADD_TEST(suite, test_dbinsert_error_9);
	SUITE_ADD_TEST(suite, test_dbinsert_error_10);
	SUITE_ADD_TEST(suite, test_dbinsert_error_11);
	SUITE_ADD_TEST(suite, test_dbinsert_error_12);
	SUITE_ADD_TEST(suite, test_dbinsert_error_13);
	SUITE_ADD_TEST(suite, test_dbinsert_error_14);
	SUITE_ADD_TEST(suite, test_dbinsert_error_15);
#endif
	
	return suite;
}

void runAllTests_dbinsert()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBInsertGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
