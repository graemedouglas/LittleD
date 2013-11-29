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
#include "../../dbparser/dblexer.h"

// Utility functions.
void clearstring(char* str, int length)
{
	int i;
	for (i = 0; i < length; ++i)
		str[i] = '\0';
}

/* Test that creating a lexer instance works. */
void TestInitLexer(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1;";
	lexer_init(&lexer, command);
	CuAssertTrue(tc, 1);
}

/* Test lexing a simple statement. */
void TestLexer_1(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement, no spaces. */
void TestLexer_2(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement, with a larger integer. */
void TestLexer_3(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 7643;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("7643", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a string. */
void TestLexer_4(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 'hElLo';";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("hElLo", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a string and whitespace. */
void TestLexer_5(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT      \n\n \t \v \t \f \r \t    'Goodbye';";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 7);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("Goodbye", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a string and no whitespace. */
void TestLexer_6(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT'Goodbye';";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 7);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("Goodbye", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a string and a comment. */
void TestLexer_7(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT --THIS IS A COMMENT\n'Goodbye';";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 7);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("Goodbye", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a string and two comments. */
void TestLexer_8(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT --THIS IS A COMMENT\n'Good--bye'--;\n;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 9);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("Good--bye", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with an int, a string and two comments. */
void TestLexer_9(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT  63, --THIS IS A COMMENT\n'Good--bye'--;\n;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("63", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 9);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("Good--bye", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with an decimal, a string and two comments. */
void TestLexer_10(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT  63.741, --THIS IS A COMMENT\n'Good--bye'--;\n;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_DECIMAL);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("63.741", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 9);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("Good--bye", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with an string, an int, and a decimal. */
void TestLexer_11(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 'string', 865, .0021;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("string", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("865", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_DECIMAL);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".0021", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_12(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 + 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("+", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_13(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 - 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("-", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_14(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 * 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("*", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_15(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 / 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("/", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_16(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 % 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("%", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_17(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 = 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_18(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 > 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(">", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_19(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 < 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("<", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_20(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 >= 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(">=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_21(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1 <= 3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("<=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_22(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1<3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("<", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 integers and an operator. */
void TestLexer_23(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1<=3;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("<=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 decimals and an operator. */
void TestLexer_24(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT 1.2<=.30;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_DECIMAL);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("1.2", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("<=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_DECIMAL);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".30", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with 2 strings and an operator. */
void TestLexer_25(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT'hello'<='.30';";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("hello", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("<=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".30", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a function whose parameter is a string. */
void TestLexer_26(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT LENGTH('apple');";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_FUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("LENGTH", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("apple", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing a simple statement with a function whose parameter is a string. */
void TestLexer_27(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT LENGTH ( ' apple ' ) ;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_FUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("LENGTH", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 7);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(" apple ", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing statement with a function whose parameter is an identifier from a table. */
void TestLexer_28(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT LENGTH(\"apple\") FROM \"t\";";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_FUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("LENGTH", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("apple", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("FROM", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing statement with a function whose parameter is an identifier from a table. */
void TestLexer_29(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT LENGTH(\"t\".\"apple\") FROM \"t\";";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_FUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("LENGTH", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("apple", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("FROM", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing statement with a function whose parameter is an identifier from a table. */
void TestLexer_30(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECT LENGTH(\"t\".apple) FROM t;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_FUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("LENGTH", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("apple", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("FROM", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing statement with a function whose parameter is an identifier from a table. */
void TestLexer_31(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "select sum(\"t\".apple), coUnt(\"FROM\") FRoM t;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("select", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_AGGRFUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("sum", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("apple", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_AGGRFUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("coUnt", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("FROM", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("FRoM", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
}

/* Test lexing complex statement. */
void TestLexer_32(CuTest *tc)
{
	db_lexer_t lexer;
	char command[] = "SELECt sum(3*(19 + (t.x % 7))), t.x*s.y\nFRoM t CROSS JOIN r LEFT JOIN r aS s ON s.b = r.b\nWHERE u = 'apples'\n GROUP BY t.x, t.y, u, s.b, r.b\nHAVING sum(t.x) > 10\nORDER BY t.x ASC, t.y DESC;";
	char buffer[40];
	clearstring(buffer, 40);
	lexer_init(&lexer, command);
	
	// Complete the tests.
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("SELECt", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_AGGRFUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("sum", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("3", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("*", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("19", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("+", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("x", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("%", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("7", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("x", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("*", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("s", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("y", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("FRoM", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("CROSS", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("JOIN", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("r", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("LEFT", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("JOIN", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("r", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("aS", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("s", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("ON", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("s", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("b", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("r", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("b", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("WHERE", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("u", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("=", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_STRING);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("apples", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("GROUP", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("BY", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("x", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("y", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("u", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("s", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("b", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("r", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("b", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 6);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("HAVING", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_AGGRFUNC);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("sum", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_LPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("(", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("x", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RPAREN);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(")", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_OP);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(">", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_INT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("10", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 5);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("ORDER", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 2);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("BY", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("x", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 3);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("ASC", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_COMMA);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(",", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("t", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp(".", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_IDENT);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("y", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 4);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_RESERVED);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, 0==strcmp("DESC", buffer));
	clearstring(buffer, 40);
	
	CuAssertTrue(tc, 1==lexer_next(&lexer));
	CuAssertTrue(tc, gettokenlength(&(lexer.token)) == 1);
	CuAssertTrue(tc, lexer.token.type == (db_uint8)DB_LEXER_TT_TERMINATOR);
	gettokenstring(&(lexer.token), buffer, &lexer);
	CuAssertTrue(tc, DB_LEXER_TERMINATOR==*buffer);
	clearstring(buffer, 40);

	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
	CuAssertTrue(tc, 0==lexer_next(&lexer));
/*
*/
}

/* Useful debugging code
printf("token start: %d\n", lexer.token.start);
printf("token end: %d\n", lexer.token.end);
fflush(stdout);
*/

/* Create the suite of tests. */
CuSuite* LexerGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, TestInitLexer);
	SUITE_ADD_TEST(suite, TestLexer_1);
	SUITE_ADD_TEST(suite, TestLexer_2);
	SUITE_ADD_TEST(suite, TestLexer_3);
	SUITE_ADD_TEST(suite, TestLexer_4);
	SUITE_ADD_TEST(suite, TestLexer_5);
	SUITE_ADD_TEST(suite, TestLexer_6);
	SUITE_ADD_TEST(suite, TestLexer_7);
	SUITE_ADD_TEST(suite, TestLexer_8);
	SUITE_ADD_TEST(suite, TestLexer_9);
	SUITE_ADD_TEST(suite, TestLexer_10);
	SUITE_ADD_TEST(suite, TestLexer_11);
	SUITE_ADD_TEST(suite, TestLexer_12);
	SUITE_ADD_TEST(suite, TestLexer_13);
	SUITE_ADD_TEST(suite, TestLexer_14);
	SUITE_ADD_TEST(suite, TestLexer_15);
	SUITE_ADD_TEST(suite, TestLexer_16);
	SUITE_ADD_TEST(suite, TestLexer_17);
	SUITE_ADD_TEST(suite, TestLexer_18);
	SUITE_ADD_TEST(suite, TestLexer_19);
	SUITE_ADD_TEST(suite, TestLexer_20);
	SUITE_ADD_TEST(suite, TestLexer_21);
	SUITE_ADD_TEST(suite, TestLexer_22);
	SUITE_ADD_TEST(suite, TestLexer_23);
	SUITE_ADD_TEST(suite, TestLexer_24);
	SUITE_ADD_TEST(suite, TestLexer_25);
	SUITE_ADD_TEST(suite, TestLexer_26);
	SUITE_ADD_TEST(suite, TestLexer_27);
	SUITE_ADD_TEST(suite, TestLexer_28);
	SUITE_ADD_TEST(suite, TestLexer_29);
	SUITE_ADD_TEST(suite, TestLexer_30);
	SUITE_ADD_TEST(suite, TestLexer_31);
	SUITE_ADD_TEST(suite, TestLexer_32);
	
	return suite;
}

void runAllTests_dblexer(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = LexerGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
