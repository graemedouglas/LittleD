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
#include "../../dbmacros.h"
#include "../../dbmm/db_query_mm.h"
#include "../../dbparser/dblexer.h"
#include "../../dbparser/dbcreate.h"
#include "../../dbstorage/dbstorage.h"

#if defined(DB_CTCONF_SETTING_FEATURE_CREATE_TABLE) && 1==DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
void test_dbcreate_1(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE mytable (attr0 INT);";
	char tablename[] = "mytable";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, 1==processCreate(&lexer, strlen(command), &mm));
	
	db_fileref_t newtable = db_openreadfile(tablename);
	db_uint8 temp8;
	char tempstring[100];
	
	/* Number of attributes. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 1==temp8);
	
	/* Header information for attr0. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 6==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 6==db_fileread(newtable, (unsigned char*)tempstring, 6));
	CuAssertTrue(tc, 0==strcmp("attr0", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_INT==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 0==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, ((db_uint8)(sizeof(db_int)))==temp8);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbcreate_2(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE cooltable (myattr STRING(11));";
	char tablename[] = "cooltable";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, 1==processCreate(&lexer, strlen(command), &mm));
	
	db_fileref_t newtable = db_openreadfile(tablename);
	db_uint8 temp8;
	char tempstring[100];
	
	/* Number of attributes. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 1==temp8);
	
	/* Header information for first attribute. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 7==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 7==db_fileread(newtable, (unsigned char*)tempstring, 7));
	CuAssertTrue(tc, 0==strcmp("myattr", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_STRING==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 0==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 11==temp8);
	
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbcreate_3(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] =
"CREATE TABLE mytable (\n\
\tid INT,\n\
\tstr STRING(7),\n\
\tstr1 STRING(9),\n\
\tint1 INT, \n\
\tint2 INT\n\
);";
	char tablename[] = "mytable";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, 1==processCreate(&lexer, strlen(command), &mm));
	
	db_fileref_t newtable = db_openreadfile(tablename);
	db_uint8 temp8;
	db_uint8 offset = 0;
	char tempstring[100];
	
	/* Number of attributes. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 5==temp8);
	
	/* Header information for first attribute. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 3==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 3==db_fileread(newtable, (unsigned char*)tempstring, 3));
	CuAssertTrue(tc, 0==strcmp("id", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_INT==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, offset==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, ((db_uint8)sizeof(db_int))==temp8);
	offset+=temp8;
	
	/* Header information for second attribute. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 4==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 4==db_fileread(newtable, (unsigned char*)tempstring, 4));
	CuAssertTrue(tc, 0==strcmp("str", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_STRING==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, offset==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 7==temp8);
	offset+=temp8;
	
	/* Header information for third attribute. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 5==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 5==db_fileread(newtable, (unsigned char*)tempstring, 5));
	CuAssertTrue(tc, 0==strcmp("str1", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_STRING==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, offset==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 9==temp8);
	offset+=temp8;
	
	/* Header information for fourth attribute. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 5==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 5==db_fileread(newtable, (unsigned char*)tempstring, 5));
	CuAssertTrue(tc, 0==strcmp("int1", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_INT==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, offset==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, ((db_uint8)sizeof(db_int))==temp8);
	offset+=temp8;
	
	/* Header information for fifth attribute. */
	/* Size of name. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, 5==temp8);
	/* Contents of name. */
	CuAssertTrue(tc, 5==db_fileread(newtable, (unsigned char*)tempstring, 5));
	CuAssertTrue(tc, 0==strcmp("int2", tempstring));
	/* Attribute type. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, DB_INT==temp8);
	/* Attribute offset. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, offset==temp8);
	/* Attribute size. */
	CuAssertTrue(tc, 1==db_fileread(newtable, &temp8, 1));
	CuAssertTrue(tc, ((db_uint8)sizeof(db_int))==temp8);
	offset+=temp8;
	
	/* Header information for second attribute. */
	CuAssertTrue(tc, 1==db_fileremove(tablename));
}

void test_dbcreate_fail_1(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE mytable (attr0 INT, attr0 STRING(2));";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, -1==processCreate(&lexer, strlen(command), &mm));
}

void test_dbcreate_fail_2(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE mytable (attr0 INT, attr1 INT, attr2 STRING(3), attr0 STRING(2));";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, -1==processCreate(&lexer, strlen(command), &mm));
}

void test_dbcreate_fail_3(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE mytable (attr0 INT, attr1 FAKETYPE, attr2 STRING(3), attr0 STRING(2));";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, -1==processCreate(&lexer, strlen(command), &mm));
}

void test_dbcreate_fail_4(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE mytable (attr0 INT, attr1 STRING, attr2 STRING(3), attr0 STRING(2));";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, -1==processCreate(&lexer, strlen(command), &mm));
}

void test_dbcreate_fail_5(CuTest *tc)
{
	db_query_mm_t mm;
	char segment[2000];
	init_query_mm(&mm, segment, 2000);
	
	char command[] = "CREATE TABLE mytable (attr0 INT, attr1 STRING(3),);";
	db_lexer_t lexer;
	lexer_init(&lexer, command);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, DB_LEXER_TT_RESERVED == lexer.token.type);
	CuAssertTrue(tc, DB_LEXER_TOKENINFO_COMMANDCLAUSE == lexer.token.info);
	CuAssertTrue(tc, DB_LEXER_TOKENBCODE_CLAUSE_CREATE == lexer.token.bcode);
	CuAssertTrue(tc, -1==processCreate(&lexer, strlen(command), &mm));
}
#endif

CuSuite *DBCreateGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
#if defined(DB_CTCONF_SETTING_FEATURE_CREATE_TABLE) && 1==DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
	SUITE_ADD_TEST(suite, test_dbcreate_1);
	SUITE_ADD_TEST(suite, test_dbcreate_2);
	SUITE_ADD_TEST(suite, test_dbcreate_3);
	SUITE_ADD_TEST(suite, test_dbcreate_fail_1);
	SUITE_ADD_TEST(suite, test_dbcreate_fail_2);
	SUITE_ADD_TEST(suite, test_dbcreate_fail_3);
	SUITE_ADD_TEST(suite, test_dbcreate_fail_4);
	SUITE_ADD_TEST(suite, test_dbcreate_fail_5);
#endif
	
	return suite;
}

void runAllTests_dbcreate()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBCreateGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
