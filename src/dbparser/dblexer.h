/******************************************************************************/
/**
@file		dblexer.h
@author		Graeme Douglas
@brief		Database lexer interface.
@details	A lexer breaks apart some well-formed string of characters
		into smaller chunks of logically related characters.  In this
		case, tokens are defined to be atomic groups of characters
		representing ideas such as clauses, quotes (", ', etc.),
		operators, values (either numeric or strings), function names,
		and other such things.  A secondary but equally important task
		the lexer completes is removal of useless information, namely
		comments.
		
		A couple of important notes:
		-What words the lexer recognizes as independent tokens, be it
		clauses, functions, or operators, are defined in dblexer.c.
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

#ifndef DBLEXER_H
#define DBLEXER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../ref.h"
#include "../dblogic/eet.h"
#include "../dblogic/compare_tuple.h"
#include "../dberror.h"

/**
@struct		db_lexer_token_t
@brief		Struct representing a lexer token.
@details	A token is an "atomic" part of the SQL command.  Examples
		include clauses, operators, and values.
*/
typedef struct db_lexer_token
{
	db_uint8		type;	/**< The token type of the token. */
	db_int			info;	/**< Other information about the token.
					*/
	db_int			bcode;	/**< Bytecode information for this
					     token. */
	db_int			start;	/**< The offset of the first character
					     of the SQL string that is be to be
					     included in the token. */
	db_int			end;	/**< The offset of the first character
					     immediately following the last
					     of the SQL string to be included
					     in the token.
					     Ex:
						SELECT * FROM ...
					              ^
					              ----- This is @c end.
					*/
} db_lexer_token_t;

/*** Control structure for database's lexer. ***/
/**
@struct		db_lexer_t
@brief		Control structure for the database's lexer.
@details	This structure contains all the state information needed to
		lex supported SQL statements.  The idea is lex using as little
		memory as possible.
*/
typedef struct db_lexer_t
{
	db_lexer_token_t	token;	/**< The token that was most recently
					     discovered by the lexer. */
	char*			command;/**< The SQL string that is to be lexed
					     and parsed. */
	db_int			length;	/**< The number of characters in
					     @c command. */
	db_int			offset;	/**< The offset of the next character to
					     lex in @c command. */
} db_lexer_t;

/*** Enum for token types ***/
/**
@enum		db_lexer_tt_t
@brief		Enumeration giving that identifies any type of token
		the lexer might return.
*/
typedef enum db_lexer_token_type
{
	DB_LEXER_TT_IDENT = 0,	/**< Database identifier. */
	DB_LEXER_TT_INT,	/**< Integer value. */
	DB_LEXER_TT_DECIMAL,	/**< Decimal value. */
	DB_LEXER_TT_STRING,	/**< String value. */
	DB_LEXER_TT_OP,		/**< Operator. */
	DB_LEXER_TT_FUNC,	/**< Function name. */
	DB_LEXER_TT_AGGRFUNC,	/**< Aggregate function name. */
	DB_LEXER_TT_LPAREN,	/**< Left parenthesis, (. */
	DB_LEXER_TT_RPAREN,	/**< Right parenthesis, ).*/
	DB_LEXER_TT_IDENTCONJ,	/**< Identifier conjugation, namely '.' in A.b
				*/
	DB_LEXER_TT_COMMA,	/**< Comma. */
	DB_LEXER_TT_RESERVED,	/**< Reserved word. */
	DB_LEXER_TT_TERMINATOR,	/**< Statement termination symbol. */
	DB_LEXER_TT_COUNT	/**< Number of meaningful values. */
} db_lexer_tt_t;

/**
@brief		Enumerated type that provides some secondary information
		about a token.
*/
typedef enum db_lexer_token_info
{
	DB_LEXER_TOKENINFO_UNIMPLEMENTED = -1,	/**< Unimplemented flag. */
	DB_LEXER_TOKENINFO_UNIMPORTANT = 0,	/**< Information unimportant. */
	DB_LEXER_TOKENINFO_COMMANDCLAUSE = 1,	/**< Command clause. These
						     include @c SELECT,
						     @c CREATE, etc. */
	DB_LEXER_TOKENINFO_SUBCLAUSE,		/**< Clauses which depend
						     on existance of command
						     clauses, like
						     @c WHERE. */
	DB_LEXER_TOKENINFO_JOINTYPE_SPECIAL,	/**< Special @c JOIN type,
						     like @c OUTER or
						     @c NATURAL. */
	DB_LEXER_TOKENINFO_JOINTYPE_NORMAL,	/**< All non-special joins. */
	DB_LEXER_TOKENINFO_JOINON,		/**< @c ON keyword. */
	DB_LEXER_TOKENINFO_ALIAS_INDICATOR,	/**< @c AS keyword. */
	DB_LEXER_TOKENINFO_LITERAL_TABLE,	/**< @c TABLE keyword. */
	DB_LEXER_TOKENINFO_LITERAL_VALUES,	/**< @c VALUES keyword. */
	DB_LEXER_TOKENINFO_TYPE_DBINT,		/**< @c INT keyword. */
	DB_LEXER_TOKENINFO_TYPE_DBSTRING,	/**< @c STRING keyword. */
	DB_LEXER_TOKENINFO_COUNT		/**< Number of elements in this
						     enumerated type. */
} db_lexer_tokeninfo_t;

/**
@brief		Enumerated type that provides bytecode relevant information
		for a token.
*/
typedef enum db_lexer_token_bcode
{
	DB_LEXER_TOKENBCODE_UNIMPLEMENTED = -1,		/**< Unimplemented. */
	DB_LEXER_TOKENBCODE_UNIMPORTANT = 0,		/**< Not needed. */
	DB_LEXER_TOKENBCODE_CLAUSE_CREATE = 1,		/**< @c CREATE
							     command. */
	DB_LEXER_TOKENBCODE_CLAUSE_INSERT,		/**< @c INSERT command
							*/
	DB_LEXER_TOKENBCODE_CLAUSE_FROM,		/**< @c FROM clause. */
	DB_LEXER_TOKENBCODE_CLAUSE_WHERE,		/**< @c WHERE clause. */
	DB_LEXER_TOKENBCODE_CLAUSE_GROUPBY,		/**< @c GROUP @c BY
							     clause. */
	DB_LEXER_TOKENBCODE_CLAUSE_HAVING,		/**< @c HAVING clause.*/
	DB_LEXER_TOKENBCODE_CLAUSE_ORDERBY,		/**< @c ORDER @c BY
							     clause. */
	DB_LEXER_TOKENBCODE_CLAUSE_SELECT,		/**< @c SELECT command.
							*/
	DB_LEXER_TOKENBCODE_JOIN_ABSOLUTE,		/**< @c JOIN keyword. */
	DB_LEXER_TOKENBCODE_JOINDECORATOR_LEFT,		/**< @c LEFT keyword. */
	DB_LEXER_TOKENBCODE_JOINDECORATOR_RIGHT,	/**< @c RIGHT keyword.*/
	DB_LEXER_TOKENBCODE_JOINDECORATOR_FULL,		/**< @c FULL keyword. */
	DB_LEXER_TOKENBCODE_JOINDECORATOR_OUTER,	/**< @c OUTER keyword.*/
	DB_LEXER_TOKENBCODE_JOINDECORATOR_NATURAL,	/**< @c NATURAL
							     keyword. */
	DB_LEXER_TOKENBCODE_JOINDECORATOR_INNER,	/**< @c INNER keyword.*/
	DB_LEXER_TOKENBCODE_JOINDECORATOR_CROSS,	/**< @c CROSS keyword.*/
	DB_LEXER_TOKENBCODE_COUNT			/**< Number of values
							     in enumeration. */
} db_lexer_tokenbcode_t;

/*** Some predefined macros for certain lexer special character values. ***/
/**
@brief		Identifier quotes; allows special characters to be used
		in an identifier.
*/
#ifndef DB_LEXER_QT_IDENT
#	define DB_LEXER_QT_IDENT '"'
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER QUOTE TYPE MACROS!"
#endif

/**
@brief		Sting quotes; specifies the beginning and ending of a string.
*/
#ifndef DB_LEXER_QT_STRING
#	define DB_LEXER_QT_STRING '\''
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER QUOTE TYPE MACROS!"
#endif

/**
@brief		The marker that ends a single command.
*/
#ifndef DB_LEXER_TERMINATOR
#	define DB_LEXER_TERMINATOR ';'
#else
#	error "FATAL ERROR: NAMING CLASH ON STATEMENT TERMINATOR MACROS!"
#endif

/*** Function prototypes ***/
/* Initialize the lexer. */
/**
@brief		Initialize the database lexer.
@param		lexerp		Pointer to the lexer control structure instance
				to initialize.  Any state information currently
				held here will be lost.
@param		command		Pointer to the string command to be lexed.
*/
void lexer_init(db_lexer_t *lexerp, char* command);

/* Lexing function. */
/**
@brief		Get the next lexed token, if there is one.
@details	If a new token is discovered, the token is placed in
		@p lexerp->token.
@param		lexerp		Pointer to the lexer control structure instance
				to get the next token from.  The offset will
				be moved on each call.
@returns	@c 1 if a new token was found, @c 0 if no new token is found,
		@c -1 if an error has occured.
*/
db_int lexer_next(db_lexer_t *lexerp);

/**
@brief		Get the token at a given offset, without changing lexer's state.
@param		tokenp		Pointer to a token variable to put the token
				data in.
@param		lexer		Copy of the lexer that holds the command
				that is being lexed.  _The contents of the
				original lexer state structure will not be
				touched._
@param		offset		The starting offset of the token to be parsed.
@param		skipnum		The number of tokens to skip.  Allows you
				to pull a token you know exists later on.
@returns	@c if token data set, @c -1 an error occured, @c 0 otherwise.
*/
db_int gettokenat(db_lexer_token_t *tokenp, db_lexer_t lexer, db_int offset, db_uint8 skipnum);

/* Get the length of a token. */
/**
@brief		Get the length of a token (number characters it contains).
@param		tokenp		Pointer to the token whose length is desired.
@returns	@p tokenp->end - @p tokenp->start
*/
db_int gettokenlength(db_lexer_token_t *tokenp);

/* Get the string value stored in a token. */
/**
@brief		Get the string value stored in a token.
@param		tokenp		Pointer to the token structure from which to
				extract the string.
@param		str		Pointer to the character buffer to place the
				contents in.  This must be managed by the caller
				(make sure to declare a buffer of the right
				length first).
@param		lexerp		Pointer to the lexer instance to which the token
				belongs to.
@returns	@c 1 if a string has been put into @p str, otherwise 0.
*/
db_int gettokenstring(db_lexer_token_t *tokenp, char* str, db_lexer_t *lexerp);

/**
@brief		Get the integer value of a integral token.
@param		tokenp		The token to convert.
@param		lexerp		The lexer that generated the token.
@returns	The integer value of the token.
*/
db_int getintegerfromtoken(db_lexer_token_t *tokenp, db_lexer_t *lexerp);

/* Check to see if a token and a string are equal in contents,
   case insensitively. */
/**
@brief		Compare a token to a string.
@param		tokenp		Pointer to the token to compare.
@param		string		Pointer to the string to compare.
@param		stringlength	Length of the string to compare not including
				the null-byte.
@param		lexerp		Pointer to lexer instance @p tokenp is from.
@param		casesensitive	Set to 1 to compare case sensitively, 0
				otherwise.
@returns	@c 1 if the strings are equal, @c 0 otherwise.
*/
db_int token_stringequal(db_lexer_token_t *tokenp, char *string,
	db_int stringlength, db_lexer_t *lexerp, db_uint8 casesensitive);

/* Checks which clause index this is in the clauses array. */
/**
@brief		Determine the index of the clauses array of this token.
@param		tokenp		Pointer to the token to check.
@param		lexerp		Pointer to the lexer instance @p tokenp belongs
				to.
@returns	@c -1 if the token is not a clause, a non-negative integer otherwise.
*/
db_int whichclause(db_lexer_token_t *tokenp, db_lexer_t *lexerp);

#ifdef __cplusplus
}
#endif

#endif
