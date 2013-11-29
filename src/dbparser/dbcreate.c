/******************************************************************************/
/**
@file		dbcreate.c
@author		Graeme Douglas
@brief		Implementation of databases create functions.
@see		For more information, refer to @ref dbcreate.h.
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
/******************************************************************************/

#include "dbcreate.h"
#include "../db_ctconf.h"

#if defined(DB_CTCONF_SETTING_FEATURE_CREATE_TABLE) && 1==DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
/**
@brief		Create a table.
@details	This function creates a table from a simplified CREATE TABLE
		statement.  This function assumes that the 'CREATE' and
		'TABLE' tokens have been thrown away; that is, the next token
		to process is the table's name.
@param		lexerp		A pointer to the lexer instance variable that
				is generating the token stream.
@param		end		The index of the first character in the command
				being lexed that is not part of the CREATE
				statement.
@param		mmp		A pointer to the per-query memory manager being
				used to process this query.
@returns	@c 1 if the table was created successfully, @c -1 if an error
		occurred, @c 0 otherwise.
*/
db_int createTable(db_lexer_t *lexerp, db_int end, db_query_mm_t *mmp)
{
	db_fileref_t newtable;
	char *tablename;
	
	/* Create a file with table name. */
	if (1==lexer_next(lexerp) && (db_uint8)DB_LEXER_TT_IDENT == lexerp->token.type)
	{
		tablename = db_qmm_falloc(mmp, gettokenlength(&(lexerp->token))+1);
		gettokenstring(&(lexerp->token), tablename, lexerp);
		
		if (db_fileexists(tablename))
		{
			DB_ERROR_MESSAGE("duplicate table name", lexerp->token.start, lexerp->command);
			db_qmm_ffree(mmp, tablename);
			return -1;
		}
		
		newtable = db_openwritefile(tablename);
	}
	
	/* Throw away the bracket. */
	if (1!=lexer_next(lexerp) || DB_LEXER_TT_LPAREN != lexerp->token.type)
	{
		DB_ERROR_MESSAGE("missing '('", lexerp->offset, lexerp->command);
		db_fileclose(newtable);
		db_fileremove(tablename);
		db_qmm_ffree(mmp, tablename);
		return -1;
	}
	
	db_uint8 attrcount = 0;
	int i;
	db_lexer_token_t *arr = db_qmm_balloc(mmp, 0);
	
	while (1==lexer_next(lexerp) && DB_LEXER_TT_RPAREN != lexerp->token.type)
	{
		if (attrcount > 0 && DB_LEXER_TT_COMMA != lexerp->token.type)
		{
			DB_ERROR_MESSAGE("duplicate attr name", lexerp->token.start, lexerp->command);
			db_qmm_bfree(mmp, arr);
			db_fileclose(newtable);
			db_fileremove(tablename);
			db_qmm_ffree(mmp, tablename);
			return -1;
		}
		else if (attrcount > 0 && 1!=lexer_next(lexerp))
		{
			DB_ERROR_MESSAGE("dangling comma", lexerp->offset, lexerp->command);
			db_qmm_bfree(mmp, arr);
			db_fileclose(newtable);
			db_fileremove(tablename);
			db_qmm_ffree(mmp, tablename);
			return -1;
		}
		
		if (DB_LEXER_TT_IDENT == lexerp->token.type)
		{
			int length = gettokenlength(&(lexerp->token));
			char attrname[length+1];
			gettokenstring(&(lexerp->token), attrname, lexerp);
			
			/* Validate unique attribute name. */
			for (i = 0; i < (int)attrcount; ++i)
			{
				if (1==token_stringequal(arr+i, attrname, length, lexerp, 0))
				{
					DB_ERROR_MESSAGE("duplicate attr name", lexerp->token.start, lexerp->command);
					db_qmm_bfree(mmp, arr);
					db_fileclose(newtable);
					db_fileremove(tablename);
					db_qmm_ffree(mmp, tablename);
					return -1;
				}
			}
			
			/* Add current token to set to check in future. */
			arr=db_qmm_bextend(mmp, sizeof(db_lexer_token_t));
			arr[0] = lexerp->token;
			
			/* We will perform some hackiness here.  Since we need
			   to know the number of attributes before we write,
			   we will simply store the values of the types, and
			   if necessary, lengths, in the tokens. */
			if (1==lexer_next(lexerp) && DB_LEXER_TOKENINFO_TYPE_DBINT == lexerp->token.info)
			{
				arr[0].type = DB_INT;
				arr[0].bcode = sizeof(db_int);
			}
			/* Extract size of the string. */
			else if (DB_LEXER_TOKENINFO_TYPE_DBSTRING == lexerp->token.info &&
			         1==lexer_next(lexerp) && DB_LEXER_TT_LPAREN == lexerp->token.type &&
			         1==lexer_next(lexerp) && DB_LEXER_TT_INT == lexerp->token.type &&
			         (length=getintegerfromtoken(&(lexerp->token), lexerp)) > 0 &&
			         length <= DB_CTCONF_SETTING_MAXSTRINGLENGTH &&
			         1==lexer_next(lexerp) && DB_LEXER_TT_RPAREN == lexerp->token.type)
			{
				arr[0].type = DB_STRING;
				arr[0].bcode = length;
			}
			/* TODO: Implement future types here. */
			else
			{
				DB_ERROR_MESSAGE("bad attribute type", lexerp->token.start, lexerp->command);
				db_qmm_bfree(mmp, arr);
				db_fileclose(newtable);
				db_fileremove(tablename);
				db_qmm_ffree(mmp, tablename);
				return -1;
			}
		}
		else
		{
			DB_ERROR_MESSAGE("bad attribute declaration", lexerp->token.start, lexerp->command);
			db_qmm_bfree(mmp, arr);
			db_fileclose(newtable);
			db_fileremove(tablename);
			db_qmm_ffree(mmp, tablename);
			return -1;
		}
		
		attrcount++;
	}
	
	/* Build out the new relation file. */
	db_filewrite(newtable, &attrcount, sizeof(db_uint8));
	db_uint8 offset = 0;
	db_uint8 temp;
	
	while (attrcount > 0)
	{
		/* Write out the length of the attribute name. */
		db_uint8 length = (db_uint8)gettokenlength(&(arr[((int)attrcount)-1]))+1;
		char attrname[(int)length];
		gettokenstring(&(arr[((int)attrcount)-1]), attrname, lexerp);
		
		/* Write out the attribute name. */
		db_filewrite(newtable, &length, sizeof(db_uint8));
		db_filewrite(newtable, attrname, (size_t)length);
		
		/* Write out the attribute type. */
		temp = (db_uint8)(arr[((int)attrcount)-1].type);
		db_filewrite(newtable, &temp, sizeof(db_uint8));
		
		/* Write out the offset and size. */
		temp = (db_uint8)arr[((int)attrcount)-1].bcode;
		db_filewrite(newtable, &offset, sizeof(db_uint8));
		db_filewrite(newtable, &temp, sizeof(db_uint8));
		offset += temp;
		
		attrcount--;
	}
	
	db_qmm_bfree(mmp, arr);
	db_fileclose(newtable);
	
	/* Throw away the bracket. */
	if (DB_LEXER_TT_RPAREN != lexerp->token.type)
	{
		DB_ERROR_MESSAGE("missing ')'", lexerp->offset, lexerp->command);
		db_fileremove(tablename);
		db_qmm_ffree(mmp, tablename);
		return -1;
	}
	
	if (end < lexerp->offset)
	{
		DB_ERROR_MESSAGE("malformed statement", 0, lexerp->command);
		db_fileremove(tablename);
		db_qmm_ffree(mmp, tablename);
		return -1;
	}
	
	db_qmm_ffree(mmp, tablename);
	return 1;
}
#endif

db_int processCreate(db_lexer_t *lexerp, db_int end, db_query_mm_t *mmp)
{
	lexer_next(lexerp);
	switch (lexerp->token.info) {
	case DB_LEXER_TOKENINFO_LITERAL_TABLE:
#if defined(DB_CTCONF_SETTING_FEATURE_CREATE_TABLE) && 1==DB_CTCONF_SETTING_FEATURE_CREATE_TABLE
		return createTable(lexerp, end, mmp);
#else
		return -1;
#endif
	default:
		return -1;
	}
}
