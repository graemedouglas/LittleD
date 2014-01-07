/*****************************************************************************/
/**
@file		eet.h
@author		Graeme Douglas
@brief		Structure definitions and method prototypes for handling
		expressions.
@details	Expressions are stored as "Evaluable Expression Trees", which
		are arrays where each term in the expression is stored as a
		node in the array, in post-order fashion (reverse polish
		notation).  A shunting-yard style algorithm is used for
		efficient evaluation.
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
@todo		To be more clear, maybe rename anything EET with pfe or rpne.
*/
/*****************************************************************************/


#ifndef EET_H
#define EET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include "../ref.h"
#include "../dbmm/db_query_mm.h"
#include "../dbobjects/relation.h"
#include "../dbobjects/tuple.h"
#include "db_aggr_func_codes.h"

/* Enumerated type for node types. This classifies attributes, constants, and 
 * functions/predicates/operations (all called operations for simplicities
 * sake). */
/**
@enum		db_eetnode_type
@brief		EET node types referred to during expression evaluation.
@details	The types have been grouped together by their behaviour.
		For functions and operators, this is the number of arguments
		each take and what is expected to be outputed, roughly
		speaking.  If argument type matters, then this is taken into
		consideration.  The point is to make it simpler to write the
		evaluation code.
*/
typedef enum
{
	/* Attributes and constant types. A couple of notes:
	 * *** The types have been grouped into categories that rougly behave
	 *     the same way.  If you want to implement some other operator,
	 *     please do so, but group it with the operators that behave
	 *     similarilly (for instance, AND takes two ints as arguments and
	 *     outputs a single db_int, etc). */
	DB_EETNODE_ATTR = 0,
	DB_EETNODE_AGGR_TEMP,		/**< Aggregation node, used in
					     materialization of nodes. */
	DB_EETNODE_AGGR_ATTR,		/**< Aggregation node, used in
					     expressions that represent
					     conditions, like HAVING
					     clauses, where the attribute
					     corresponding to the result
					     of the aggregate function
					     has already been evaluated. */
	DB_EETNODE_ALIAS,		/**< A node referring to another
					     expression, resulting from
					     SELECT ... as myalias ... */
	DB_EETNODE_CONST_NULL,		/**< The constant NULL. It is
					     entirely type agnostic. */
	DB_EETNODE_CONST_DBINT,		/**< An integer constant. */
	DB_EETNODE_CONST_DBSTRING,	/**< A string constant. */
	/* Operation/function types. */
	DB_EETNODE_OP_UNARYNEG,		/**< The negative of a number. */
	DB_EETNODE_OP_NOT,		/**< Negation */
	DB_EETNODE_OP_BCOMP,		/**< Bitwise complement. Flips bits. */
	DB_EETNODE_OP_BAND,		/**< Bitwise @c AND @c (a & b) */
	DB_EETNODE_OP_BOR,		/**< Bitwise @c OR @c (a | b) */
	DB_EETNODE_OP_BXOR,		/**< Bitwise @c XOR @c (a ^ b) */
	DB_EETNODE_OP_AND,		/**< Logical @c AND @c (a && b) */
	DB_EETNODE_OP_OR,		/**< Logical @c OR @c (a || b) */
	DB_EETNODE_OP_XOR,		/**< Logical @c XOR (exclusive OR)*/
	DB_EETNODE_OP_MOD,		/**< Remainder and mod are synonyms */
	DB_EETNODE_OP_ADD,		/**< Addition. */
	DB_EETNODE_OP_SUB,		/**< Subtraction. */
	DB_EETNODE_OP_MULT,		/**< Multiplication. */
	DB_EETNODE_OP_DIV,		/**< Division. */
	DB_EETNODE_OP_LT,		/**< Less than. */
	DB_EETNODE_OP_GT,		/**< Greater than. */
	DB_EETNODE_OP_LTE,		/**< Less than or equal to. */
	DB_EETNODE_OP_GTE,		/**< Greater than or equal to. */
	DB_EETNODE_OP_NEQ,		/**< Not equal to. */
	DB_EETNODE_OP_EQ,		/**< Equal to. */
	DB_EETNODE_OP_ISNULL,		/**< NULL check. */
	DB_EETNODE_FUNC_LENGTH_DBSTRING,/**< String length. */
	/* Brackets and commas are only temporary while parsing. */
	DB_EETNODE_LPAREN,		/**< Left parenthesis. */
	DB_EETNODE_RPAREN,		/**< Right parenthesis. */
	DB_EETNODE_COMMA,		/**< Comma. */
	DB_EETNODE_COUNT		/**< Total number of types. */
} db_eetnode_type;

/*** Node type structures. ***/
/* Basic node structure.  Args extend this structure */
/**
@struct		db_eetnode_t
@brief		The base type for all EET nodes.
@details	This type must be the first member of inherited node type.
*/
typedef struct
{
	/*@{*/
	db_uint8 type;		/**< The type of the node. */
	/*@}*/
} db_eetnode_t;

/*** Evaluable expression tree structure. ***/
/**
@struct		db_eet_t
@brief		Evaluable expression tree control structure.
@details	This structure represents expressions in the database.  The
		root node of the expression tree is always the node in the
		nodes array.  The nodes array _MUST_ be written out in
		pre-order in the array to maximze execution time.
@todo		-Calculate the @stack_size variable, sometime in the future.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t *nodes;	/**< The array of nodes. Nodes _MUST_ be
				     pre-order sorted. */
	db_int size;		/**< Total number of bytes consumed by tree.
				     _NOT_ total number of nodes, since node
				     size can vary. */
	db_int stack_size;	/**< The maximum number of bytes needed to
				     evaluate the tree.  This can be calculated
				     by determining the number of bytes needed
				     to traverse from the root to each leaf and
				     storing the maximum.*/
	/*@}*/
} db_eet_t;

/* Extended attribute node type. */
/**
@struct		db_eetnode_attr_t
@brief		EET node representing an attribute in the database.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t base;	/**< The base node supertype. */
	db_uint8 pos;		/**< The attributes position in the tuple of
				     interest. */
	db_uint8 tuple_pos;	/**< If there are multiple tuples to reference
				     in a given context, this is which tuple
				     and schema (relation header) to reference.
				*/
	db_int tokenstart;	/**< The start of the expression token. */
	/*@}*/
} db_eetnode_attr_t;

/* Extended aggregate function node type, used during materialization of
   tuples. */
/**
@struct		db_eetnode_aggr_temp_t
@brief		EET node type representing aggregate functions.
@details	This type is meant to represent the actual aggregate function.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t base;	/**< The supertype of this node type. */
	db_uint8 aggr_type;	/**< Aggregate function code. */
	db_uint8 aggr_isnull;	/**< Flag to determine if this aggregate value
				     is NULL or not. (1 -> yes, 0 -> no). */
	void *value_p;		/**< A pointer to the aggregate value associated
				     to this node.  Note that this must be set
				     _BEFORE_ evaluating the entire expression.
				*/
	db_eetnode_t *jump_p;	/**< We use this to store where to "jump" to
				     after we are done evaluating the sub
				     expression of the aggregate. */
	db_uint8 subexpr_type;	/**< This is the resulting type of the evaluated
				     subexpression. */
	/*@}*/
} db_eetnode_aggr_temp_t;

/* Extended aggregate function node type, used after materialization of tuples.
   Basically, the idea here is that an aggregate function must always be
   evaluated to some value in some tuple.  Essentially, this type is the same
   as an attribute, but we flag it differently, and we also give it an
   aggregate function code.  This way we can process it appropriately in the
   aggregate operator. */
/**
@brief		This is used specially once an aggregate value has been
		evaluated.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t base;	/* Inherit from base type. */
	db_uint8 aggr_type;	/* Aggregate function code. */
	db_uint8 pos;		/* The attributes position. */
	db_uint8 tuple_pos;	/* Which tuple/header pointer to use, in the 
				   array. */
	/*@}*/
} db_eetnode_aggr_attr_t;

/**
@brief		The idea here is to allow entire expression to be referenced
		more than once in a query without specifying the entire thing.
@details	Right now, this is not used.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t base;	/**< Supertype of this struct. */
	db_eet_t *expr;		/**< Pointer to an EET.  This EET is the
				     expression we wish to alias.
				*/
	void *value;		/**< Value of the alias' sub-expression, once
				     it has been evaluated.
				@todo I am not convinced this will work.  Worst
				      case scenario is that we evaluate the expression
				      every time, although it would be nice to avoid
				      this.  We need to make sure that we know when to
				      free this value, and how to tell what type it is.
				*/
	/*@}*/
} db_eetnode_alias_t;

/* Extended constant integer node type. */
/**
@struct		db_eetnode_dbint_t
@brief		The constant integer EET node type.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t base;	/**< The supertype of this node. */
	db_int integer;		/**< The value of the constant. */
	/*@}*/
} db_eetnode_dbint_t;

/* Extended constant string node type. */
/**
@struct		db_eetnode_dbstring_t
@brief		The constant string EET node type.
*/
typedef struct
{
	/*@{*/
	db_eetnode_t base;	/**< Supertype of this node. */
	char* string;		/**< The string pointer. */
	/*@}*/
} db_eetnode_dbstring_t;

/* Construct a new relation header an array of attribute positions. */
/**
@brief		Creates a new relation header from an array of projecting
		expressions.
@param		hp		A pointer to the new header.
@param		old_hp		A pointer to the old header.
@param		exprs		The array of expressions to be projected.
@param		num_exprs	The number of expressions in the above array.
@param		mmp		A pointer to the memory manager that will
				allocate space for the current query.
@returns	@c -1 if any error occurs, @c 1 otherwise.
*/
db_int createnewheader(relation_header_t *hp,
		relation_header_t *old_hp,
		db_eet_t *exprs,
		db_uint8 num_exprs,
		db_query_mm_t *mmp);

/* Advance a pointer num_times times.  Notice, this does no checking, so
  you better no what YOU ARE DOING!!!! */
/**
@brief		Advances the EET node pointer pointed to by @p npp.
@param		npp		Pointer to the pointer to be moved.
@param		num_times	The number of times to increment the pointer.
@returns	@c 1.	@todo Figure out if there is any errors that we could detect here.
*/
db_int advanceeetnodepointer(db_eetnode_t **npp, db_int num_times);

/* This function will return the type of the node at position pos in the
   expression. */
/**
@brief		Return the eetnode type at a given position in an expression.
@param		eet		Pointer to the EET control structure
				representing the expression of interest.
@param		pos		The position of the node we wish to get the type of.
@returns	@c 1.	@todo Figure out if there is any errors that we could detect here.
@todo		Use this method in all other "advance pointers n times" type of methods, where possible.
*/
db_uint8 geteetnodetypebypos(db_eet_t *eet,	/* Pointer to the expresion
						   tree. */
			db_int pos);		/* Position to check. */

/* This function will return a pointer to the node at position pos in the
   expression.  */
/**
@brief		Get address of EET node at a given position in an expression.
@param		eet		Pointer to the EET control structure
				representing the expression of interest.
@param		pos		The position of the node we wish to get the
				address of.
@returns	@c 1.	@todo Figure out if there is any errors that we could detect here.
*/
db_eetnode_t* geteetnodebypos(db_eet_t *eet, db_int pos);

/* Retrieves the type and next node address of a sub-expression rooted at node
   pointed to by np. */
/**
@brief		Get the resulting type and length of a subexpression.
@param		np		Pointer to the node that is the root of the
				subexpression of interest.
@param		type		Pointer to the variable which we wish to put
				the resulting type in.
@param		npp		Pointer to the variable where it is desired to
				put the pointer to the node immediately
				following the sub-expression of interest.
				Ex. Consider the expression: = + 1 2 3
				    If we were to call this method on the
				    sub-expression rooted at '+', npp
				    would end up pointing at '3'.
@param		max_stack_size	The maximum amount of memory needed allowed to
				execute this method.  In general, a safe
				assumption is that the size of the expression
				divided by @c sizeof(db_eetnode_t) will be
				enough memory.
@param		hpa		An array of relation header pointers (schemas).
				If any node in the sub-expression has the type
				db_eetnode_attr_t, then its tuple_pos field
				should be the index of the schema in @p hpa
				that describes to the same relation that the
				attribute belongs to.
@param		mmp		A pointer to the memory manager being used
				to allocate memory for query this expression
				is owned by.
@returns	@c -1 if there is an error, @c 1 otherwise.
@todo		Either modify for postfix expressions, or delete if not needed.
*/
db_int subexpr_type_addr(db_eetnode_t *np,
		db_uint8 *type,
		db_eetnode_t **next_npp,
		db_uint8 max_stack_size,
		relation_header_t **hpa,
		db_query_mm_t *mmp);

/* Evaluate the tree and put result in rp. */
/**
@brief		Evaluate an expression and get its final value.
@param		eet		Pointer to the EET control structure
				representing the expression of interest.
@param		rp		A pointer to a memory location where we wish
				to return the type of the expression.  If this
				value is @c NULL, then a special mode is engaged
				that causes the engine to determine the
				type of the expression instead of the result.
@param		tp		An array of tuple pointers.  It should contain
				pointers for all tuples that will be referenced
				in the expression.  Any @c db_eetnode_attr nodes
				in the expression must have their @p tuple_pos
				structure members be the index of the tuple it
				belongs to in this array.
@param		hp		An array of relation header pointers.  It should
				contain pointers for all headers that will be
				referenced in the expression.  Any
				@c db_eetnode_attr nodes in the expression must
				have their @p tuple_pos structure members be the
				index of the header for the relation that the
				attribute belongs, as it is placed in @p hp.
@param		start_from	The position of the node to start evaluating
				from.  This parameter allows an expression
				containing aggregates to be evaluated properly.
@param		mmp		A pointer to the memory manager being used
				to allocate memory for query this expression
				is owned by.
@returns	@c -1 if there is an error, @c 2 if the expression returns
		@c NULL, @c 1 otherwise.  If the type determining mode was
		enabled, it will return the type of the resulting eetnode,
		assuming no errors are present.
*/
db_int evaluate_eet(db_eet_t *eet,
		void* rp,
		db_tuple_t **tp,
		relation_header_t **hp,
		db_uint8 start_from,
		db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
