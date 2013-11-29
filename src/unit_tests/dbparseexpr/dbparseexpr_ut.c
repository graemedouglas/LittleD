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
#include "../../dbmacros.h"
#include "../CuTest.h"
#include "../../dbparser/dbparseexpr.h"

void TestParseExpr_1(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "1";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 1 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
}

void TestParseExpr_2(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "((((1))))";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 1 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
}

void TestParseExpr_3(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "((((1)+2)))";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 1 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_ADD == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_4(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "2+4*8-90";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 4 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 8 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_ADD == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 90 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_SUB == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_5(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "(2+4)*(8-90)";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 4 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_ADD == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 8 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 90 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_SUB == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_6(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "1/((2+4)*(8-90)%273)";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 1 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 4 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_ADD == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 8 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 90 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_SUB == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 273 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MOD == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_DIV == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_7(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "- - -(-(-(-6)));";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 6 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}


void TestParseExpr_8(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "!-110;";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 110 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_NOT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_9(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "-!110;";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 110 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_NOT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_10(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "~-!110;";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 110 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_NOT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_BCOMP == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_11(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "a*5;";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_ATTR == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_attr_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 5 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_12(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "328*t.x;";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 328 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_ATTR == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_attr_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_13(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "328*d.t.a;";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 328 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_ATTR == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_attr_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_14(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "MAX(2*t.a);";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_ATTR == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_attr_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_AGGR_TEMP == np->type);
	CuAssertTrue(tc, DB_AGGR_MAX == ((db_eetnode_aggr_temp_t*)np)->aggr_type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_aggr_temp_t), db_eetnode_t*);
}

void TestParseExpr_15(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "'apple'";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBSTRING == np->type);
	CuAssertTrue(tc, 0==strcmp(((db_eetnode_dbstring_t*)np)->string, "apple"));
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbstring_t), db_eetnode_t*);
}

void TestParseExpr_16(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "MAX(LENGTH('apple'))";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	db_eetnode_t *np = expr;
	
printf("np->type: %d\n", np->type);
fflush(stdout);
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBSTRING == np->type);
	CuAssertTrue(tc, 0==strcmp(((db_eetnode_dbstring_t*)np)->string, "apple"));
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbstring_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_AGGR_TEMP == np->type);
	CuAssertTrue(tc, DB_AGGR_MAX == ((db_eetnode_aggr_temp_t*)np)->aggr_type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_aggr_temp_t), db_eetnode_t*);
}


#if 0
#endif
/* Test second and third phases.  This code expression does not make sense but allows for important testing. */
void TestParseExpr_17(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "LENGTH(LENGTH('string1'),T.a*2+1/3)";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0);
	
	/* Confirm that expression converted properly. */
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBSTRING == np->type);
	CuAssertTrue(tc, 0==strcmp(((db_eetnode_dbstring_t*)np)->string, "string1"));
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbstring_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_ATTR == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_attr_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_MULT == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 1 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 3 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_DIV == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_ADD == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

void TestParseExpr_18(CuTest *tc)
{
	/* Lexer data. */
	char sqlexpr[] = "-(1 - -2);";
	db_eetnode_t *expr;
	db_lexer_t lexer;
	lexer_init(&lexer, sqlexpr);
	
	/* Memory management data. */
	db_int segment_size = 1000;
	unsigned char segment[segment_size];
	db_query_mm_t mm;
	init_query_mm(&mm, segment, sizeof(segment));
	
	/* Do the whole process. */
	CuAssertTrue(tc, 1 == parseexpression(&expr, &lexer, 0, strlen(sqlexpr), &mm, 0));
	
	db_eetnode_t *np = expr;
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 1 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_CONST_DBINT == ((db_eetnode_dbint_t*)np)->base.type);
	CuAssertTrue(tc, 2 == ((db_eetnode_dbint_t*)np)->integer);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_dbint_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_SUB == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
	
	CuAssertTrue(tc, (db_uint8)DB_EETNODE_OP_UNARYNEG == np->type);
	MOVEPOINTERNBYTES(np, np, sizeof(db_eetnode_t), db_eetnode_t*);
}

//printf("np->type: %d\n", np->type);
//fflush(stdout);

/* Create the suite of tests. */
CuSuite* DBParseExprGetSuite()
{
	CuSuite* suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, TestParseExpr_1);
	SUITE_ADD_TEST(suite, TestParseExpr_2);
	SUITE_ADD_TEST(suite, TestParseExpr_3);
	SUITE_ADD_TEST(suite, TestParseExpr_4);
	SUITE_ADD_TEST(suite, TestParseExpr_5);
	SUITE_ADD_TEST(suite, TestParseExpr_6);
	SUITE_ADD_TEST(suite, TestParseExpr_7);
	SUITE_ADD_TEST(suite, TestParseExpr_8);
	SUITE_ADD_TEST(suite, TestParseExpr_9);
	SUITE_ADD_TEST(suite, TestParseExpr_10);
	SUITE_ADD_TEST(suite, TestParseExpr_11);
	SUITE_ADD_TEST(suite, TestParseExpr_12);
	SUITE_ADD_TEST(suite, TestParseExpr_13);
	SUITE_ADD_TEST(suite, TestParseExpr_14);
	SUITE_ADD_TEST(suite, TestParseExpr_15);
	SUITE_ADD_TEST(suite, TestParseExpr_16);
	SUITE_ADD_TEST(suite, TestParseExpr_17);
	SUITE_ADD_TEST(suite, TestParseExpr_18);
	
	return suite;
}

void runAllTests_dbparseexpr(void)
{
	CuString *output = CuStringNew();
	CuSuite *suite = DBParseExprGetSuite();
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
