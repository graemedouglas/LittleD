/******************************************************************************/
/**
@file		dblexer.c
@author		Graeme Douglas
@brief		Lexer implementation for the database.
@see		For more information, refer to @ref dblexer.h.
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

#include "dblexer.h"

/*** Macros for Lexer modes ***/
/**
@brief		Value used to determine values of lexing modes.
*/
#ifndef DB_LEXER_MODE_BASE
#	define DB_LEXER_MODE_BASE 0
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Mode indicating no mode has been chosen.
*/
#ifndef DB_LEXER_MODE_NONE
#	define DB_LEXER_MODE_NONE DB_LEXER_MODE_BASE + 1
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		General lexing mode.
@details	Mode chosen when it is unclear what mode to choose based on
		input so far.
*/
#ifndef DB_LEXER_MODE_GENERAL
#	define DB_LEXER_MODE_GENERAL DB_LEXER_MODE_BASE + 2
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Lexing mode for lexing out integers.
*/
#ifndef DB_LEXER_MODE_INTEGER
#	define DB_LEXER_MODE_INTEGER DB_LEXER_MODE_BASE + 3
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Lexing mode for lexing out numbers with decimal places.
*/
#ifndef DB_LEXER_MODE_DECIMAL
#	define DB_LEXER_MODE_DECIMAL DB_LEXER_MODE_BASE + 4
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Lexing mode for lexing out string values.
*/
#ifndef DB_LEXER_MODE_STRING
#	define DB_LEXER_MODE_STRING DB_LEXER_MODE_BASE + 5
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Lexing mode for lexing out identifiers.
*/
#ifndef DB_LEXER_MODE_IDENT
#	define DB_LEXER_MODE_IDENT DB_LEXER_MODE_BASE + 6
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Lexing mode for lexing out an operator.
*/
#ifndef DB_LEXER_MODE_OP
#	define DB_LEXER_MODE_OP DB_LEXER_MODE_BASE + 7
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Lexing mode for lexing out a comment.
*/
#ifndef DB_LEXER_MODE_COMMENT
#	define DB_LEXER_MODE_COMMENT DB_LEXER_MODE_BASE + 8
#else
#	error "FATAL ERROR: NAMING CLASH ON LEXER MODE MACROS!"
#endif

/**
@brief		Struct for storing information about reserved words.
*/
struct keyword
{
	const char* 	word;	/**< The keyword. **/
	db_int 		info;	/**< Integer information tied to keyword. */
	db_int 		bcode;	/**< Bytecode information tied to keyword. */
};

/*** Keyword dictionaries ***/
/* Note this one is not ordered to be efficiently searchable.  It is ordered
   so that clauses can be processed in the correct order. */
/**
@brief		A keyword lookup for clauses.
*/
static struct keyword clauses[] = {
	{"CREATE",	DB_LEXER_TOKENINFO_COMMANDCLAUSE,	DB_LEXER_TOKENBCODE_CLAUSE_CREATE},
	{"INSERT",	DB_LEXER_TOKENINFO_COMMANDCLAUSE,	DB_LEXER_TOKENBCODE_CLAUSE_INSERT},
	{"DROP",	DB_LEXER_TOKENINFO_COMMANDCLAUSE,	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"DELETE",	DB_LEXER_TOKENINFO_COMMANDCLAUSE,	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"INSERT",	DB_LEXER_TOKENINFO_COMMANDCLAUSE,	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"FROM",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_CLAUSE_FROM},
	{"WHERE",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_CLAUSE_WHERE},
	{"GROUP",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_CLAUSE_GROUPBY},
	{"HAVING",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_CLAUSE_HAVING},
	{"ORDER",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_CLAUSE_ORDERBY},
	{"SELECT",	DB_LEXER_TOKENINFO_COMMANDCLAUSE,	DB_LEXER_TOKENBCODE_CLAUSE_SELECT},
	{"EXPLAIN",	DB_LEXER_TOKENINFO_COMMANDCLAUSE, 	DB_LEXER_TOKENBCODE_UNIMPLEMENTED}
};

/**
@brief		A keyword lookup for other reserved words.
@details	Does not include clauses, operators, or functions or any type.
*/
static struct keyword reservedwords[] = {
	{"BY",		DB_LEXER_TOKENINFO_UNIMPLEMENTED, 	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"AS",		DB_LEXER_TOKENINFO_ALIAS_INDICATOR,	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"IS",		DB_LEXER_TOKENINFO_UNIMPLEMENTED, 	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"ON",		DB_LEXER_TOKENINFO_JOINON,		DB_LEXER_TOKENBCODE_UNIMPORTANT},
	{"IN",		DB_LEXER_TOKENINFO_UNIMPLEMENTED,	DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"NOT",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"INT",		DB_LEXER_TOKENINFO_TYPE_DBINT,	 	DB_LEXER_TOKENBCODE_UNIMPORTANT},
	{"STRING",	DB_LEXER_TOKENINFO_TYPE_DBSTRING, 	DB_LEXER_TOKENBCODE_UNIMPORTANT},
	{"INTO",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_LEXER_TOKENBCODE_UNIMPLEMENTED},
	{"LEFT",	DB_LEXER_TOKENINFO_JOINTYPE_SPECIAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_LEFT},
	{"RIGHT",	DB_LEXER_TOKENINFO_JOINTYPE_SPECIAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_RIGHT},
	{"FULL",	DB_LEXER_TOKENINFO_JOINTYPE_SPECIAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_FULL},
	{"OUTER",	DB_LEXER_TOKENINFO_JOINTYPE_SPECIAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_OUTER},
	{"NATURAL",	DB_LEXER_TOKENINFO_JOINTYPE_SPECIAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_NATURAL},
	{"INNER",	DB_LEXER_TOKENINFO_JOINTYPE_NORMAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_INNER},
	{"CROSS",	DB_LEXER_TOKENINFO_JOINTYPE_NORMAL,	DB_LEXER_TOKENBCODE_JOINDECORATOR_CROSS},
	{"JOIN",	DB_LEXER_TOKENINFO_JOINTYPE_NORMAL,	DB_LEXER_TOKENBCODE_JOIN_ABSOLUTE},
	{"ASC",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_TUPLE_ORDER_ASC},
	{"DESC",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_TUPLE_ORDER_DESC},
	{"TABLE",	DB_LEXER_TOKENINFO_LITERAL_TABLE,	DB_LEXER_TOKENBCODE_UNIMPORTANT},
	{"VALUES",	DB_LEXER_TOKENINFO_LITERAL_VALUES,	DB_LEXER_TOKENBCODE_UNIMPORTANT}
};

/**
@brief		Keyword lookup for operators.
*/
static struct keyword operators[] = {
	{">=",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_GTE},
	{">",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_GT},
	{"<=",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_LTE},
	{"<",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_LT},
	{"=",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_EQ},
	{"!=",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_NEQ},
	{"+",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_ADD},
	{"-",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_SUB},
	{"~",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_BCOMP},
	{"*",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_MULT},
	{"/",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_DIV},
	{"%",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_MOD},
	{"!",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_NOT},
	{"AND",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_AND},
	{"OR",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_OP_OR}
};

/**
@brief		Keyword lookup for functions.
*/
static struct keyword functions[] = {
	{"LENGTH",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_EETNODE_FUNC_LENGTH_DBSTRING},
	{"CONCAT",	DB_LEXER_TOKENINFO_UNIMPLEMENTED,	DB_LEXER_TOKENBCODE_UNIMPLEMENTED}
};

/**
@brief		Keyword lookup for aggregate functions.
*/
static struct keyword aggrfunctions[] = {
	{"SUM",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_AGGR_SUM},
	{"MAX",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_AGGR_MAX},
	{"MIN",		DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_AGGR_MIN},
	{"LAST",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_AGGR_LAST},
	{"COUNT",	DB_LEXER_TOKENINFO_UNIMPORTANT,		DB_AGGR_COUNTROWS}
};

/**
@brief		Get token info for a token in one of the keyword arrays.
@param		array		The array of keywords from which the token
				belongs to.
@param		idx		The index of the keyword whose token
				information is desired.
@returns	The token information for the keyword.
*/
db_int getkeywordinfo(struct keyword *array, db_int idx)
{
	return array[idx].info;
}

/**
@brief		Get bytecode data for a token in one of the keyword arrays.
@param		array		The array of keywords which the token
				belongs to.
@param		idx		The index of the keyword whose bytecode
				data is desired.
@returns	The token information for the keyword.
*/
db_int getkeywordbcode(struct keyword *array, db_int idx)
{
	return array[idx].bcode;
}

//TODO
/**************** USeful debugging function that should go if not needed. *****/
/**
@brief		Print the contents of a token.
@param		tokenp		A pointer to the token variable to print.
@param		lexerp		A pointer to the lexer variable that
				generated the token.
*/
void printtoken(db_lexer_token_t *tokenp, db_lexer_t *lexerp)
{
	db_int i = tokenp->start;
	while (i < tokenp->end && lexerp->command[i] != '\0')
	{
		printf("%c", lexerp->command[i]);
		fflush(stdout);
		++i;
	}
	puts("");
	fflush(stdout);
}
/**************** USeful debugging function that should go if not needed. *****/

/**
@brief		A string length function.
@details	The goal here is to have the option of not using the standard
		string library to reduce code footprint and support devices
		that may not have a standard string library.
@param		str		The string whose length we wish to find.
@returns	The length @p str _not_ including the null-byte.
@todo		Decide if this is necessary or desired.  Might always be better
		off with standard library functions.
*/
db_int strlength(char *str)
{
	db_int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}

/** Some internal helper functions. **/
/* Returns c's upper case equivalent, if applicable. */
/**
@brief		Capitalize a character, if its a letter.
@param		c		The character to be capitalized.
@return		If @p c is a lower case letter, then its capitalized form,
		otherwise @p c.
*/
char tocapital(char c)
{
	if (c >= 0x61 && c <= 0x7A)
	{
		return (c - 32);
	}
	else
	{
		return c;
	}
}

/* Returns c's lower case equivalent, if applicable. */
/**
@brief		Get the lower-case form of a character.
@param		c		The character to be put into lower-case.
@returns	If @p c is an upper-case letter, then its lower-case
		equivalent, otherwise @p c.
@todo		Do we really need this function?
*/
char tounicase(char c)
{
	if (c >= 0x41 && c <= 0x5A)
	{
		return (c + 32);
	}
	else
	{
		return c;
	}
}

/* Check if there are more characters to consume. */
/**
@brief		Check if a lexer instance has any more characters to consume.
@param		lexerp		Pointer to the lexer instance that is to be checked.
@returns	@c 1 if the lexer has more lexible characters, @c 0 otherwise.
*/
db_int lexible(db_lexer_t *lexerp)
{
	return (db_int)(lexerp->offset < lexerp->length && lexerp->offset >= 0);
}

/* Get the next character to analyze without updating offset. */
/**
@brief		Look at the next character without advancing the lexer's
		offset.
@param		lexerp		A pointer to the lexer instance variable
				whose next character is to be looked at.
@param		nchar		A pointer to a @c char to place the next
				character in.
@returns	@c 1 if a character could be looked at, @c 0 otherwise.
*/
db_int peekchar(db_lexer_t *lexerp, char *nchar)
{
	if (lexible(lexerp))
	{
		*nchar = lexerp->command[lexerp->offset];
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Get the next character to analyze and update the offset. */
/**
@brief		Get the next character and advance the offset of the lexer.
@param		lexerp		A pointer to the lexer instance variable
				whose next character is to be looked at.
@param		nchar		A pointer to a @c char to place the next
				character in.
*/
db_int nextchar(db_lexer_t *lexerp, char* nchar)
{
	if (peekchar(lexerp, nchar))
	{
		lexerp->offset += 1;
		return 1;
	}
	else
		return 0;
}

/* Add moveby to lexerp->offset. */
/**
@brief		Move the offset of the lexer.
@param		lexerp		A pointer to the lexer instance variable
				whose offset we wish to move.
@param		moveby		The amount to move the offset by.  May
				be positive or negative.
*/
void moveoffset(db_lexer_t *lexerp, db_int moveby)
{
	lexerp->offset += moveby;
}

/* Checks if c is whitespace. */
/**
@brief		Check if a character is whitespace.
@param		c		The character to check.
@returns	@c 1 if @p c is whitespace, @c 0 otherwise.
*/
db_int iswhitespace(char c)
{
	if(c == ' '  ||
	   c == '\f' ||
	   c == '\n' ||
	   c == '\r' ||
	   c == '\t' ||
	   c == '\v' )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Check if current character is whitespace. */
/**
@brief		Check if the next lexerp character is whitespace.
@param		lexerp		A pointer to the lexer instance variable
				whose next character is to be checked.
@returns	@c 1 if the lexer's next character is whitespace, @c 0 otherwise.
*/
db_int offset_iswhitespace(db_lexer_t *lexerp)
{
	char nchar;
	if(peekchar(lexerp, &nchar) && iswhitespace(nchar))
	{
		return 1;
	}
	else
		return 0;
}

/* Checks if c is a letter. */
/**
@brief		Check if a character is a letter, either upper- or lower-case.
@param		c		The character to check.
@returns	@c 1 if @p c is a letter, @c 0 otherwise.
*/
db_int isletter(char c)
{
	if ((c >= 0x41 && c <= 0x5A) ||
	    (c >= 0x61 && c <= 0x7A) ||
	    ('_' == c))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Checks if current character is a letter. */
/**
@brief		Check if the next lexerp character is a letter.
@param		lexerp		A pointer to the lexer instance variable
				whose next character is to be checked.
@returns	@c 1 if the lexer's next character is a letter, @c 0 otherwise.
*/
db_int offset_isletter(db_lexer_t *lexerp)
{
	char nchar;
	if (peekchar(lexerp, &nchar) && isletter(nchar))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Checks if c is a digit. */
/**
@brief		Check if a character is a digit.
@param		c		The character to check.
@returns	@c 1 if @p c is a digit, @c 0 otherwise.
*/
db_int isdigit_(char c)
{
	if (c >= 0x30 && c <= 0x39)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Checks if current character is a digit. */
/**
@brief		Check if the next lexerp character is a digit.
@param		lexerp		A pointer to the lexer instance variable
				whose next character is to be checked.
@returns	@c 1 if the lexer's next character is a digit, @c 0 otherwise.
*/
db_int offset_isdigit(db_lexer_t *lexerp)
{
	char nchar;
	if (peekchar(lexerp, &nchar) && isdigit_(nchar))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

db_int token_stringequal(db_lexer_token_t *tokenp, char *string, db_int stringlength, db_lexer_t *lexerp, db_uint8 casesensitive)
{
	if (stringlength != tokenp->end - tokenp->start)
	{
		return 0;
	}
	int i = tokenp->start, j = 0;
	while (i < tokenp->end && string[j] != '\0')
	{
		if (!casesensitive && (tocapital(lexerp->command[i]) != tocapital(string[j])))
		{
			return 0;
		}
		else if (casesensitive && lexerp->command[i] != string[j])
		{
			return 0;
		}
		else
		{
			++i;
			++j;
		}
	}
	if (j == stringlength)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* Determines the position of the token in an array of strings. */
/**
@brief		Determine the index of the token in an array of keywords.
@param		tokenp		A pointer to the token to lookup.
@param		lexerp		A pointer to the lexer instance variable that
				the token was generated by.
@param		array		The keyword array to find the index in.
@param		araysize	The number of elements in the array.
@returns	If the token does not exist in the keyword array, @c -1,
		otherwise the index of the token in the keyword array.
@todo		Make the lookup faster?
*/
db_int token_whichelement(db_lexer_token_t *tokenp, db_lexer_t *lexerp, struct keyword *array, int arraysize)
{
	/* For each reserved word, see if the token is the same as the reserved word. */
	db_int i = 0, j;
	for (; i < arraysize; ++i)
	{
		/* This probably seems rediculous, but compiler warnings are
		   grosser than this solution, so just keep your socks on
		   straight. */
		db_int max = 0;
		while (array[i].word[max] != '\0')
			max++;
		char temp[max];
		for (j = 0; j < max; ++j)
		{
			temp[j] = array[i].word[j];
		}
		if (token_stringequal(tokenp, temp, max, lexerp, 0))
		{
			return i;
		}
	}
	return -1;
}

/* Checks if the token is belongs to the array passed in. */
/**
@brief		Determine if a token is in an array of keywords.
@param		tokenp		A pointer to the token to lookup.
@param		lexerp		A pointer to the lexer instance variable that
				the token was generated by.
@param		array		The keyword array to find the index in.
@param		araysize	The number of elements in the array.
@param		which		A pointer to a @c db_int that is set
				with the index of the token in the array.
				If the this is @c NULL, we check without
				setting.
@returns	@c 1 if the token is in the keyword array, @c 0 otherwise.
*/
db_int token_iselement(db_lexer_token_t *tokenp, db_lexer_t *lexerp, struct keyword *array, int arraysize, db_int *which)
{
	if (NULL == which)
	{
		return (token_whichelement(tokenp,lexerp,array,arraysize)>-1);
	}
	
	*which = token_whichelement(tokenp, lexerp, array, arraysize);
	return (*which > -1);
}

/* Checks if token is a reserved word. */
/**
@brief		Determine if a token is in fact a clause keyword.
@param		tokenp		A pointer to the token to check.
@param		lexerp		A pointer to the lexer instance variable
				that generated the token.
@param		which		A pointer to a @p db_int that will be set
				with the index of the token in the
				@c clauses array.
@returns	@c 0 if the token is a clause keyword, @c 1 otherwise.
*/
db_int isclause(db_lexer_token_t *tokenp, db_lexer_t *lexerp, db_int *which)
{
	return token_iselement(tokenp, lexerp, clauses, sizeof(clauses)/sizeof(struct keyword), which);
}

/* Checks if token is a reserved word. */
/**
@brief		Determine if a token is a reserved word.  This includes clauses.
@param		tokenp		A pointer to the token to check.
@param		lexerp		A pointer to the lexer instance variable
				that generated the token.
@param		which		A pointer to a @p db_int that will be set
				with the index of the token in the
				@c reserved array or the @c clauses array.
@returns	@c 0 if the token is a reserved word, @c 1 otherwise.
*/
db_int isreserved(db_lexer_token_t *tokenp, db_lexer_t *lexerp, db_int *which)
{
	return token_iselement(tokenp, lexerp, reservedwords,
		sizeof(reservedwords)/sizeof(struct keyword), which)||isclause(tokenp, lexerp, which);
}

/* Checks if token is an operator. */
/**
@brief		Determine if a token is an operator.
@param		tokenp		A pointer to the token to check.
@param		lexerp		A pointer to the lexer instance variable
				that generated the token.
@param		which		A pointer to a @p db_int that will be set
				with the index of the token in the
				@c reserved array or the @c operators array.
@returns	@c 0 if the token is an operator, @c 1 otherwise.
*/
db_int isoperator(db_lexer_token_t *tokenp, db_lexer_t *lexerp, db_int *which)
{
	return token_iselement(tokenp, lexerp, operators, sizeof(operators)/sizeof(struct keyword),
				which);
}

/* Checks if a token is the start of an operator. */
/**
@brief		Determine if a character is the start of an operator.
@param		c		The character to check.
@returns	@c 0 if the character is the start of an operator.
*/
db_int charisstartofoperator(char c)
{
	/* For each operator... */
	int i = 0;
	for (; i < sizeof(operators) / sizeof(struct keyword); ++i)
	{
		if (tocapital(c) == tocapital(operators[i].word[0]))
			return 1;
	}
	return 0;
}

/* Checks if a token is the start of an operator. */
/**
@brief		Determine if a token is the start of an operator.
@param		tokenp		A pointer to the token to check.
@param		lexerp		A pointer to the lexer instance variable
				that generated the token.
@returns	@c 1 if the token is the start of an operator, @c 0 otherwise.
*/
db_int isstartofoperator(db_lexer_token_t *tokenp, db_lexer_t *lexerp)
{
	/* For each operator... */
	int i = 0, j = 0;
	for (; i < sizeof(operators) / sizeof(struct keyword); ++i)
	{
		/* For every character in shorter of token/operator. */
		for (; j < tokenp->end - tokenp->start && j < sizeof(operators[i]); ++j)
		{
			if (tocapital(lexerp->command[tokenp->start + j]) == tocapital(operators[i].word[j]))
				return 1;
		}
	}
	return 0;
}

/* Checks if token is a reserved word. */
/**
@brief		Determine if a token is a function
@param		tokenp		A pointer to the token to check.
@param		lexerp		A pointer to the lexer instance variable
				that generated the token.
@param		which		A pointer to a @p db_int that will be set
				with the index of the token in the
				@c reserved array or the @c functions array.
@returns	@c 0 if the token is a function, @c 1 otherwise.
*/
db_int isfunction(db_lexer_token_t *tokenp, db_lexer_t *lexerp, db_int *which)
{
	return token_iselement(tokenp, lexerp, functions, sizeof(functions)/sizeof(struct keyword), which);
}

/* Checks if token is a reserved word. */
/**
@brief		Determine if a token is an aggregate function.
@param		tokenp		A pointer to the token to check.
@param		lexerp		A pointer to the lexer instance variable
				that generated the token.
@param		which		A pointer to a @p db_int that will be set
				with the index of the token in the
				@c reserved array or the @c aggrfuncs array.
@returns	@c 0 if the token is an operator, @c 1 otherwise.
*/
db_int isaggrfunction(db_lexer_token_t *tokenp, db_lexer_t *lexerp, db_int *which)
{
	return token_iselement(tokenp, lexerp, aggrfunctions, sizeof(aggrfunctions)/sizeof(struct keyword), which);
}

/* Set token information.  Pass in -1 for offset value if that offset shouldn't
   be updated. */
/**
@brief		Setup a token with necessary information.
@param		tokenp		A pointer to the token to be set.
@param		type		The type of the token to set.
@param		which		The index of the token in its appropriate
				keyword array.
@param		start		The starting offset of the token.  Pass a negative
				value to not set this value.
@param		end		The first offset within character stream _NOT_
				a part of this token.  Pass a negative value to not
				set.
*/
void settoken(db_lexer_token_t *tokenp, db_uint8 type, db_int which, db_int start, db_int end)
{
	tokenp->info = 0;
	tokenp->bcode = 0;
	tokenp->type = type;
	
	// TODO: Fix this condition for things that don't need a which.
	if (which > -1)
	{
		if ((db_uint8)DB_LEXER_TT_OP == type)
		{
			tokenp->info = getkeywordinfo(operators, which);
			tokenp->bcode = getkeywordbcode(operators, which);
		}
		else if ((db_uint8)DB_LEXER_TT_FUNC == type)
		{
			tokenp->info = getkeywordinfo(functions, which);
			tokenp->bcode = getkeywordbcode(functions, which);
		}
		else if ((db_uint8)DB_LEXER_TT_AGGRFUNC == type)
		{
			tokenp->info = getkeywordinfo(aggrfunctions, which);
			tokenp->bcode = getkeywordbcode(aggrfunctions, which);
		}
		else if ((db_uint8)DB_LEXER_TT_RESERVED == type)
		{
			tokenp->info = getkeywordinfo(reservedwords, which);
			tokenp->bcode = getkeywordbcode(reservedwords, which);
			/* Handle clauses. */
		}
		else if ((db_uint8)DB_LEXER_TT_LPAREN == type)
		{
			tokenp->info = DB_LEXER_TOKENINFO_UNIMPORTANT;
			tokenp->bcode = (db_uint8)DB_EETNODE_LPAREN;
		}
		else if ((db_uint8)DB_LEXER_TT_RPAREN == type)
		{
			tokenp->info = DB_LEXER_TOKENINFO_UNIMPORTANT;
			tokenp->bcode = (db_uint8)DB_EETNODE_RPAREN;
		}
		else if ((db_uint8)DB_LEXER_TT_COMMA == type)
		{
			tokenp->info = DB_LEXER_TOKENINFO_UNIMPORTANT;
			tokenp->bcode = (db_uint8)DB_EETNODE_COMMA;
		}
		else
		{
			tokenp->info = -2;
			tokenp->bcode = -2;
		}
	}
	if (start > -1)
		tokenp->start = start;
	if (end > -1)
		tokenp->end = end;
}

/*** External functions ***/
/* Initialize the lexer */
void lexer_init(db_lexer_t *lexerp, char* command)
{
	lexerp->command = command;
	lexerp->length = strlength(command);
	/* Set initial offset to 0, the beginning of the command string. */
	lexerp->offset = 0;
}

/* Lex function */
db_int lexer_next(db_lexer_t *lexerp)
{
	/** Initial variable declaration. **/
	db_int mode = DB_LEXER_MODE_NONE;
	
	/* The idea here is to only set the lexers token if when it is clear
	   a new token has been found.  If newtoken.start == newtoken.end,
	   then there is no token to return. */
	db_lexer_token_t newtoken;
	settoken(&newtoken, DB_LEXER_TT_COUNT, -1, lexerp->offset, lexerp->offset);
	
	/* Temporary character variable. */
	char tchar;
	
	/* Ignore all whitespace. */
	while (offset_iswhitespace(lexerp))
	{
		moveoffset(lexerp, 1);
		newtoken.start++;
		newtoken.end++;
	}
	
	if (!lexible(lexerp))
	{
		/* Do a quick error check. */
		if (lexerp->offset < 0)
			return -1;
		else
			return 0;
	}
	
	/* Lex until there is a new token. */
	db_uint8 done = 0;
	while (lexible(lexerp) && !done)
	{
		switch(mode)
		{
			case DB_LEXER_MODE_NONE:
				/* Note that there must be more characters,
				   in this case. */
				nextchar(lexerp, &tchar);
				newtoken.end++;
				if (tchar == '(')
				{
					settoken(&newtoken, DB_LEXER_TT_LPAREN,
						0, -1, lexerp->offset);
					done = 1;
				}
				else if (tchar == ')')
				{
					settoken(&newtoken, DB_LEXER_TT_RPAREN,
						0, -1, lexerp->offset);
					done = 1;
				}
				else if (tchar == ',')
				{
					settoken(&newtoken, DB_LEXER_TT_COMMA,
						0, -1, lexerp->offset);
					done = 1;
				}
				else if (isdigit_(tchar))
				{
					mode = DB_LEXER_MODE_INTEGER;
				}
				else if (tchar == '.')
				{
					/* Need to be careful here. If digit
					   follows '.', change mode to DECIMAL,
					   otherwise return an identifier 
					   conjunction. */
					if (!peekchar(lexerp, &tchar) ||
					    (!isdigit_(tchar) &&
					     !isletter(tchar) &&
					     '*' != tchar &&
					     tchar != DB_LEXER_QT_IDENT))
					{
						DB_ERROR_MESSAGE("incorrect input after \".\"", lexerp->offset, lexerp->command);
						return -1;	// TODO: This is a hard problem.  Do we signal errors or not?  I'm not sure.
					}
					else if (isdigit_(tchar))
					{
						mode = DB_LEXER_MODE_DECIMAL;
					}
					else
					{
						settoken(&newtoken,
							DB_LEXER_TT_IDENTCONJ,
							-1,
							lexerp->offset-1,
							lexerp->offset);
						done = 1;
					}
				}
				else if (tchar == DB_LEXER_QT_STRING)
				{
					newtoken.start = lexerp->offset;
					mode = DB_LEXER_MODE_STRING;
				}
				else if (tchar == DB_LEXER_QT_IDENT)
				{
					newtoken.start = lexerp->offset;
					mode = DB_LEXER_MODE_IDENT;
				}
				else if (isletter(tchar))
				{
					mode = DB_LEXER_MODE_GENERAL;
				}
				else if (charisstartofoperator(tchar))
				{
					mode = DB_LEXER_MODE_OP;
				}
				else if (tchar == DB_LEXER_TERMINATOR)
				{
					settoken(&newtoken,
						DB_LEXER_TT_TERMINATOR,
						-1,
						lexerp->offset-1,
						lexerp->offset);
					done = 1;
				}
				else
				{
					//TODO: This still isn't right, reports offset of token after invalid chracter.
					DB_ERROR_MESSAGE("invalid character", lexerp->offset, lexerp->command);
					return -1;
				}
				break;
			case DB_LEXER_MODE_GENERAL:
				if (offset_isletter(lexerp) || offset_isdigit(lexerp))
				{
					newtoken.end++;
					moveoffset(lexerp, 1);
				}
				else
				{
					db_int which;
					if (isclause(&newtoken, lexerp, &which))
					{
						settoken(&newtoken, DB_LEXER_TT_RESERVED, which, -1, -1);
						newtoken.info = getkeywordinfo(clauses, which);
						newtoken.bcode = getkeywordbcode(clauses, which);
						done = 1;
					}
					else if (isreserved(&newtoken, lexerp, &which))
					{
						settoken(&newtoken, DB_LEXER_TT_RESERVED, which, -1, -1);
						done = 1;
					}
					else if (isoperator(&newtoken, lexerp, &which))
					{
						settoken(&newtoken, DB_LEXER_TT_OP, which, -1, -1);
						done = 1;
					}
					else if (isfunction(&newtoken, lexerp, &which))
					{
						settoken(&newtoken, DB_LEXER_TT_FUNC, which, -1, -1);
						done = 1;
					}
					else if (isaggrfunction(&newtoken, lexerp, &which))
					{
						settoken(&newtoken, DB_LEXER_TT_AGGRFUNC, which, -1, -1);
						done = 1;
					}
					else
					{
						settoken(&newtoken, DB_LEXER_TT_IDENT, -1, -1, -1);
						done = 1;
					}
				}
				break;
			case DB_LEXER_MODE_INTEGER:
				/* If no more chars to lex, return integer. */
				if (0 == nextchar(lexerp, &tchar))
				{
					settoken(&newtoken, DB_LEXER_TT_INT, 0, -1, -1);
					done = 1;
				}
				else if (tchar == '.')
				{
					newtoken.end++;
					mode = DB_LEXER_MODE_DECIMAL;
				}
				else if (isdigit_(tchar))
				{
					newtoken.end++;
				}
				else
				{
					moveoffset(lexerp, -1);
					settoken(&newtoken, DB_LEXER_TT_INT, 0, -1, -1);
					done = 1;
				}
				break;
			case DB_LEXER_MODE_DECIMAL:
				if(nextchar(lexerp, &tchar) && isdigit_(tchar))
				{
					newtoken.end++;
				}
				else
				{
					/* Need to move back offset. */
					moveoffset(lexerp, -1);
					settoken(&newtoken, DB_LEXER_TT_DECIMAL, -1, -1, -1);
					done = 1;
				}
				break;
			case DB_LEXER_MODE_STRING:
				if(!nextchar(lexerp, &tchar))
				{
					DB_ERROR_MESSAGE("no matching end quote", lexerp->offset, lexerp->command);
					return -1;
				}
				else if (tchar == DB_LEXER_QT_STRING)
				{
					settoken(&newtoken, DB_LEXER_TT_STRING, -1, -1, -1);
					done = 1;
				}
				else
				{
					newtoken.end++;
				}
				break;
			case DB_LEXER_MODE_IDENT:
				if(!nextchar(lexerp, &tchar))
				{
					DB_ERROR_MESSAGE("no matching end quote", lexerp->offset, lexerp->command);
					return -1;
				}
				else if (tchar == DB_LEXER_QT_IDENT)
				{
					settoken(&newtoken, DB_LEXER_TT_IDENT, -1, -1, -1);
					done = 1;
				}
				else
				{
					newtoken.end++;
				}
				break;
			case DB_LEXER_MODE_OP:
				if(!nextchar(lexerp, &tchar))
				{
					DB_ERROR_MESSAGE("incomplete token", lexerp->offset, lexerp->command);
					return -1;
				}
				db_int which;
				/* Temporarily extend operator to see if it is
				   still an oeprator. */
				newtoken.end++;
				if (!isoperator(&newtoken, lexerp, &which))
				{
					/* Check for comments. */
					if (token_stringequal(&newtoken, "--", 2, lexerp, 0))
					{
						mode = DB_LEXER_MODE_COMMENT;
					}
					else
					{
						newtoken.end--;
						if (isoperator(&newtoken, lexerp, &which))
						{
							moveoffset(lexerp, -1);
							settoken(&newtoken, DB_LEXER_TT_OP, which, -1, -1);
							done = 1;
						}
						else
						{
							DB_ERROR_MESSAGE("invalid operator", lexerp->offset, lexerp->command);
							return -1;
						}
					}
				}
				else {}
				break;
			case DB_LEXER_MODE_COMMENT:
				if(!nextchar(lexerp, &tchar))
				{
					return 0;
				}
				while (lexible(lexerp) && tchar != '\n' &&
				       tchar != '\r')	// TODO: More characters to watch for?
				{
					nextchar(lexerp, &tchar);
				}
				if (tchar == '\n' || tchar == '\r')
				{
					/* Restart the lexing process. */
					newtoken.start = lexerp->offset;
					newtoken.end = lexerp->offset;
					mode = DB_LEXER_MODE_NONE;
				}
				break;
			default:
				return -1;
		}
	} /* End of while loop here! */
	
	if (newtoken.end != newtoken.start)
	{
		/* If the type isn't set, set it! */
		if ((db_uint8)DB_LEXER_TT_COUNT == newtoken.type)
		{
			if (DB_LEXER_MODE_INTEGER == mode)
			{
				settoken(&newtoken, DB_LEXER_TT_INT, -1,-1,-1);
			}
			else if (DB_LEXER_MODE_DECIMAL == mode)
			{
				settoken(&newtoken, DB_LEXER_TT_DECIMAL, -1,-1,-1);
			}
			else if (DB_LEXER_MODE_GENERAL == mode)
			{
				int which;
				if (isclause(&newtoken, lexerp, &which))
				{
					settoken(&newtoken, DB_LEXER_TT_RESERVED, which, -1, -1);
					newtoken.info = getkeywordinfo(clauses, which);
					newtoken.bcode = getkeywordbcode(clauses, which);
				}
				else if (isreserved(&newtoken, lexerp, &which))
				{
					settoken(&newtoken, DB_LEXER_TT_RESERVED, which,-1,-1);
				}
				else
				{
					settoken(&newtoken, DB_LEXER_TT_IDENT, -1,-1,-1);
				}
			}
			else
			{
				// TODO: Some error.
			}
		}
		lexerp->token = newtoken;
		return 1;
	}
	else
	{
		return 0;
	}
}

db_int gettokenat(db_lexer_token_t *tokenp, db_lexer_t lexer, db_int offset, db_uint8 skipnum)
{
	lexer.offset = offset;
	
	db_uint8 i = 0;
	while(1==lexer_next(&lexer) && i < skipnum)
		++i;
	if (i != skipnum) { return 0; }
	
	*tokenp = lexer.token;
	
	return 0;
}

db_int gettokenlength(db_lexer_token_t *tokenp)
{
	return (tokenp->end - tokenp->start);
}

db_int gettokenstring(db_lexer_token_t *tokenp, char* str, db_lexer_t *lexerp)
{
	db_int i;
	for (i = 0; i < gettokenlength(tokenp); ++i)
		str[i] = lexerp->command[tokenp->start + i];
	if (i != 0)
	{
		str[i] = '\0';
		return 1;
	}
	else
		return 0;
}

db_int getintegerfromtoken(db_lexer_token_t *tokenp, db_lexer_t *lexerp)
{
	char intstr[gettokenlength(tokenp)+1];
	gettokenstring(tokenp, intstr, lexerp);
	return atoi(intstr);
}

/* Determine which index the clause is. */
db_int whichclause(db_lexer_token_t *tokenp, db_lexer_t *lexerp)
{
	return token_whichelement(tokenp, lexerp, clauses, sizeof(clauses)/sizeof(struct keyword));
}
