/******************************************************************************/
/**
@author		Graeme Douglas
@file		dbparseexpr.c
@brief		Code which converts an SQL expression to an expression that can
		be evaluated by the backend.
@see		For more information, refer to @ref dbparseexpr.h.
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

#include "dbparseexpr.h"

/*** Operator precedence table.  Returns 0 if operator is not implemented. ***/
/**
@brief		Get the operator precedence of an operator.
@details	Operators of higher precedence return lower values.
@param		opcode		The expression node type of the operator.
@returns	The precedence of the operator.
*/
db_uint8 opprecedence(db_uint8 opcode)
{
	switch (opcode)
	{
		case (db_uint8)DB_EETNODE_OP_BCOMP:
			return 1;
		case (db_uint8)DB_EETNODE_OP_MULT:
		case (db_uint8)DB_EETNODE_OP_DIV:
		case (db_uint8)DB_EETNODE_OP_MOD:
			return 2;
		case (db_uint8)DB_EETNODE_OP_ADD:
		case (db_uint8)DB_EETNODE_OP_SUB:
		case (db_uint8)DB_EETNODE_OP_UNARYNEG:
		case (db_uint8)DB_EETNODE_OP_BAND:
		case (db_uint8)DB_EETNODE_OP_BOR:
		case (db_uint8)DB_EETNODE_OP_BXOR:
			return 3;
		case (db_uint8)DB_EETNODE_OP_EQ:
		case (db_uint8)DB_EETNODE_OP_LT:
		case (db_uint8)DB_EETNODE_OP_LTE:
		case (db_uint8)DB_EETNODE_OP_GT:
		case (db_uint8)DB_EETNODE_OP_GTE:
			return 4;
		case (db_uint8)DB_EETNODE_OP_NOT:
			return 5;
		case (db_uint8)DB_EETNODE_OP_AND:
			return 6;
		case (db_uint8)DB_EETNODE_OP_OR:
			return 7;
		default:
			return 0;
	}
}

/*** Compare two operator's precedences.  If either operator is not
     implemented, we return -2.  ***/
/**
@brief		Compare the operator precedence of two operators.
@param		a	The expression node type of the first operator
			to compare.
@param		b	The expression node type of the second operator
			to compare.
@returns	@c -2 if either @p a or @p b are unimplemented operators,
		@c -1 if @p a is of higher precedence than @p b, @c 1 if
		@p b is of higher precendence than @p a, @c 0 otherwise.
*/
db_int compareopprecedence(db_uint8 a, db_uint8 b)
{
	a = opprecedence(a);
	b = opprecedence(b);
	if (0 == a || 0 == b)
		return -2;
	else if (a > b)
		return 1;
	else if (a < b)
		return -1;
	else
		return 0;
}

/* Check if an operator is unary. */
/**
@brief		Determine if an operator is unary.
@param		type	The expression node type of the operator.
@returns	@c 1 if it is a unary operator, @c 0 otherwise.
*/
db_int isunary(db_uint8 type)
{
	switch (type) {
	case (db_uint8)DB_EETNODE_OP_BCOMP:
	case (db_uint8)DB_EETNODE_OP_NOT:
	case (db_uint8)DB_EETNODE_OP_UNARYNEG:
	case (db_uint8)DB_EETNODE_OP_ISNULL:
		return 1;
	default:
		return 0;
	}
}

/* Pop items off the operator stack. */
/**
@brief		Pop operators off the operator stack.
@details	Pop oeprators off the operator stack so long as the limit
		is not encountered and the next operator to be popped is not
		unary or of lower precedence than the type about to be pused,
		if one is to be pushed onto the stack afterwards.
@param		type		The expression node type that will be pushed
				onto the stack once the popping is complete.
				Set to @c -1 if no type is to be pushed.
@param		exprp		A pointer to the raw bytecode expression
				array that is being constructed.
@param		stack		A pointer to the bottom of the operator
				stack.
@param		stack_topp	A pointer to the beginning of the top-most
				operator on the operator stack.
@param		sizep		A pointer to a variable storing the size
				of the expression that is being built.
@param		limit		The expression node type of an operator
				to stop popping at.  Can also be the type
				of a bracket or comma.
@param		mmp		A pointer to the per-query memory manager
				allocating space for the query.
@returns	@c -1 of am error occurred, @c 1 otherwise.
*/
db_int parseexpr_pop(db_int type, db_eetnode_t **exprp, db_eetnode_t *stack, db_eetnode_t **stack_topp, db_int *sizep, db_uint8 limit, db_query_mm_t *mmp)
{
	while (*stack_topp != stack && limit != (*stack_topp)->type && (type == -1 || compareopprecedence(type, (*stack_topp)->type) >= 0))
	{
		if (isunary(type) || (isunary((*stack_topp)->type) && type != -1 && compareopprecedence(type, (*stack_topp)->type) >= 0))
		{
			return 1;
		}
		else if (1!=db_qmm_fextend(mmp, sizeof(db_eetnode_t)))
		{
			return -1;
		}
		db_eetnode_t *newnodep = POINTERATNBYTES(*exprp, *sizep, db_eetnode_t*);
		*sizep += sizeof(db_eetnode_t);
		*newnodep = *(*stack_topp);
		*stack_topp = db_qmm_bextend(mmp, (db_int)(-sizeof(db_eetnode_t)));
	}
	return 1;
}
/**
@brief		Check if a type if a function.
@details	A boolean clause for checking the type of a function.
@param		t	The type of the expression node to check.
*/
#ifndef ISFUNC
#define ISFUNC(t) ((db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING >= (t) && (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING <= (t))
#else
#error "MACRO NAME CLASH ON ISFUNC!"
#endif

// TODO: Error messages.
db_int parseexpression(db_eetnode_t **exprp, db_lexer_t *lexerp, db_int start,
			db_int end, db_query_mm_t *mmp, db_uint8 jointolast)
{
	db_int lasttype = DB_EETNODE_COUNT;
	db_int size = 0;
	
	/* Re-initialize the lexer. */
	lexer_init(lexerp, lexerp->command);
	lexerp->offset = start;
	
	if (1!=jointolast)
		*exprp = db_qmm_falloc(mmp, 0);
	else
	{
		size = DB_QMM_SIZEOF_FTOP(mmp);
	}
	
	db_eetnode_t *stack = db_qmm_balloc(mmp, 0);
	
	if ((void*)0 == *exprp || (void*)0 == stack)
	{
		return -1;
	}
	db_eetnode_t *stack_top = stack;
	
	while (lexerp->offset < end && 1==lexer_next(lexerp) && lexerp->token.start < end)
	{
		if ((db_uint8)DB_LEXER_TT_INT == lexerp->token.type)
		{
			// TODO: Check the size of the integer and build appropriately in future.
			/* Allocate space for the node. */
			// TODO: Abstract this part?
			if (1 != db_qmm_fextend(mmp, sizeof(db_eetnode_dbint_t)))
			{
				return -1;
			}
			db_eetnode_dbint_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_dbint_t*);
			size+=sizeof(db_eetnode_dbint_t);
			
			/* Build the node! */
			newnodep->base.type = DB_EETNODE_CONST_DBINT;
			char temp[gettokenlength(&(lexerp->token))+1];
			gettokenstring(&(lexerp->token), temp, lexerp);
			newnodep->integer = atoi(temp);	// TODO: Better way to do this?  Maybe without using library?
			lasttype = DB_EETNODE_CONST_DBINT;
		}
		else if ((db_uint8)DB_LEXER_TT_STRING == lexerp->token.type)
		{
			/* Allocate space for the node. */
			if (1 != db_qmm_fextend(mmp, sizeof(db_eetnode_dbstring_t)))
			{
				return -1;
			}
			db_eetnode_dbstring_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_dbstring_t*);
			size+=sizeof(db_eetnode_dbstring_t);
			
			/* For now, simply store beginning offset of string in
			   pointer. Its a hack, but it works. */
			newnodep->base.type = DB_EETNODE_CONST_DBSTRING;
			newnodep->string = (char*)(lexerp->token.start-1);
			lasttype = DB_EETNODE_CONST_DBSTRING;
		}
		else if ((db_uint8)DB_LEXER_TT_PLACEHOLDER == lexerp->token.type)
		{
			if (1 != db_qmm_fextend(mmp, sizeof(db_eetnode_placeholder_t)))
			{
				return -1;
			}
			db_eetnode_placeholder_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_placeholder_t*);
			size+=sizeof(db_eetnode_placeholder_t);
			
			/* Build the node! */
			newnodep->base.type = DB_EETNODE_PLACEHOLDER;
			newnodep->placeholdertype	= lexerp->token.info;
			newnodep->offset		= lexerp->token.end;
			lasttype = DB_EETNODE_PLACEHOLDER;
		}
		else if ((db_uint8)DB_LEXER_TT_IDENT == lexerp->token.type)
		{
			/* Create start and end offsets for attributes,
			   relations, and databases. */
			db_int attr_s=lexerp->token.start, attr_e=lexerp->token.end, rel_s=-1, rel_e=-1, db_s=-1, db_e=-1;
			db_int offset = lexerp->offset;
			db_uint8 counter = 0;	/* Count number of parts to identifier. */
			while (counter < 2 && lexerp->offset < end && 1==lexer_next(lexerp))
			{
				if (lexerp->token.type == (db_uint8)DB_LEXER_TT_IDENTCONJ)
				{
					if (lexerp->offset < end && 1==lexer_next(lexerp) &&
					    lexerp->token.type == (db_uint8)DB_LEXER_TT_IDENT)
					{
						/* Shift the identifier start/end over. */
						db_s = rel_s;
						db_e = rel_e;
						rel_s = attr_s;
						rel_e = attr_e;
						attr_s = lexerp->token.start;
						attr_e = lexerp->token.end;
						offset = lexerp->offset;
					}
					else
					{
						DB_ERROR_MESSAGE("expected identifier after '.'", lexerp->token.start, lexerp->command);
						return -1;
					}
				}
				else
				{
					lexerp->offset = offset;
					break;
				}
			}
			/* Allocate space for the node. */
			if (1 != db_qmm_fextend(mmp, sizeof(db_eetnode_attr_t)))
			{
				return -1;
			}
			db_eetnode_attr_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_attr_t*);
			size+=sizeof(db_eetnode_attr_t);
			
			/* Build the node! */
			newnodep->base.type = DB_EETNODE_ATTR;
			/* Note, we will borrow pos field so that we can do
			   less work on setup. */
			if (db_s != -1)
			{
				newnodep->tokenstart = db_s;
				newnodep->pos = 3;
			}
			else if (rel_s != -1)
			{
				newnodep->tokenstart = rel_s;
				newnodep->pos = 2;
			}
			else if (attr_s != -1)
			{
				newnodep->tokenstart = attr_s;
				newnodep->pos = 1;
			}
		}
		else if ((db_uint8)DB_LEXER_TT_OP == lexerp->token.type)
		{
			db_int type = lexerp->token.bcode;
			
			/* Four cases in which a subtraction should become a unary
			   negative:
				-Its the first token in the expression.
				-Its the first subtraction after a '('.
				-its the first subtraction after a comma in a function.
				-Its the first subtraction after another operator.
			*/
			if ((db_uint8)DB_EETNODE_OP_SUB == type &&
			     (DB_EETNODE_COUNT == lasttype || DB_EETNODE_LPAREN == lasttype ||
                              DB_EETNODE_COMMA == lasttype ||
			       (DB_EETNODE_OP_UNARYNEG <= lasttype && DB_EETNODE_OP_ISNULL >= lasttype)))
			{
				type = DB_EETNODE_OP_UNARYNEG;
			}
			
			/* If pushorplace == 0, place new operator in the output
			   string.  Otherwise, put it on the stack. */
			
			/* While there are operators of higher or equal
			   on top of stack, pop them off. */
			switch (parseexpr_pop(type, exprp, stack, &stack_top, &size, (db_uint8)DB_EETNODE_COUNT, mmp)) {
			case 1:
				break;
			default:
				return -1;
			}
			
			/* Push operator onto the stack. */
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_t));
			if (NULL == stack_top)
			{
				return -1;
			}
			stack_top->type = type;
			
			lasttype = type;
		}
		else if ((db_uint8)DB_LEXER_TT_COMMA == lexerp->token.type)
		{
			switch (parseexpr_pop(-1, exprp, stack, &stack_top, &size, (db_uint8)DB_EETNODE_LPAREN, mmp)) {
			case 1:
				break;
			default:
				return -1;
			}
			lasttype = DB_EETNODE_COMMA;
		}
		else if ((db_uint8)DB_LEXER_TT_FUNC == lexerp->token.type)
		{
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_t));
			if (NULL == stack_top)
			{
				return -1;
			}
			stack_top->type = lexerp->token.bcode;
			
			if (lexerp->offset < end && 1==lexer_next(lexerp) && lexerp->token.start < end && (db_uint8)DB_EETNODE_LPAREN == lexerp->token.bcode)
			{
				lexerp->offset = lexerp->token.start;
			}
			else
			{
				return -1;
			}
			//lasttype = stack_top->type;
		}
		else if ((db_uint8)DB_LEXER_TT_AGGRFUNC == lexerp->token.type)
		{
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_aggr_temp_t));
			if (NULL == stack_top)
			{
				return -1;
			}
			stack_top->type = DB_EETNODE_AGGR_TEMP;
			((db_eetnode_aggr_temp_t*)stack_top)->aggr_type = lexerp->token.bcode;
			
			if (lexerp->offset < end && 1==lexer_next(lexerp) && lexerp->token.start < end && (db_uint8)DB_EETNODE_LPAREN == lexerp->token.bcode)
			{
				lexerp->offset = lexerp->token.start;
			}
			else
			{
				return -1;
			}
			//lasttype = DB_EETNODE_AGGR_TEMP;
		}
		else if ((db_uint8)DB_LEXER_TT_LPAREN == lexerp->token.type)
		{
			/* Push bracket onto the stack. */
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_t));
			if (NULL == stack_top)
			{
				return -1;
			}
			stack_top->type = DB_EETNODE_LPAREN;
			lasttype = DB_EETNODE_LPAREN;
		}
		else if ((db_uint8)DB_LEXER_TT_RPAREN == lexerp->token.type)
		{
			switch (parseexpr_pop(-1, exprp, stack, &stack_top, &size, (db_uint8)DB_EETNODE_LPAREN, mmp)) {
			case 1:
				break;
			default:
				return -1;
			}
			
			if (stack_top == stack)
			{
				return -1;
			}
			else if ((db_uint8)DB_EETNODE_LPAREN == stack_top->type)
			{
				if (NULL!=(stack_top=db_qmm_bextend(mmp, (db_int)(-sizeof(db_eetnode_t)))) && stack_top != stack)
				{
					if (ISFUNC(stack_top->type))
					{
						if (1 != db_qmm_fextend(mmp, sizeof(db_eetnode_t)))
						{
							return -1;
						}
						db_eetnode_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_t*);
						size+=sizeof(db_eetnode_t);
						*newnodep = *stack_top;
						stack_top = db_qmm_bextend(mmp, (db_int)(-sizeof(db_eetnode_t)));
					}
					else if ((db_uint8)DB_EETNODE_AGGR_TEMP == stack_top->type)
					{
						if (1 != db_qmm_fextend(mmp, sizeof(db_eetnode_aggr_temp_t)))
						{
							return -1;
						}
						db_eetnode_aggr_temp_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_aggr_temp_t*);
						size+=sizeof(db_eetnode_aggr_temp_t);
						*newnodep = *((db_eetnode_aggr_temp_t*)stack_top);
						stack_top = db_qmm_bextend(mmp, (db_int)(-sizeof(db_eetnode_aggr_temp_t)));
					}
				}
				else if (NULL == stack_top)
				{
					return -1;
				}
			}
			lasttype = DB_EETNODE_RPAREN;
		}
	}
	switch (parseexpr_pop(-1, exprp, stack, &stack_top, &size, (db_uint8)DB_EETNODE_COUNT, mmp)) {
	case 1:
		break;
	default:
		return -1;
	}
	
	db_qmm_bfree(mmp, stack_top);
	
	if (jointolast)
	{
		/* Push AND onto the stack. */
		if (1!=db_qmm_fextend(mmp, sizeof(db_eetnode_t)))
		{
			return -1;
		}
		db_eetnode_t *newnodep = POINTERATNBYTES(*exprp, size, db_eetnode_t*);
		size+=sizeof(db_eetnode_t);
		newnodep->type = DB_EETNODE_OP_AND;
	}
	
	/* We will now use stack as a cursor to create all strings we need. */
	stack = *exprp;
	while (POINTERBYTEDIST(stack, *exprp) < size)
	{
		if ((db_uint8)DB_EETNODE_CONST_DBSTRING == stack->type)
		{
			db_eetnode_dbstring_t *newnodep = (db_eetnode_dbstring_t*)stack;
			lexerp->offset = (db_int)(newnodep->string);
			lexer_next(lexerp);
			
			/* Get length of the token string. */
			db_int tokenlength = gettokenlength(&(lexerp->token));
			/* Allocate memory on front for new string. */
			// TODO: BALLOC if possible?
			char *string = db_qmm_balloc(mmp, tokenlength + 1);
			/* Copy the string data over. */
			int i;
			for (i = 0; i < tokenlength; ++i)
			{
				string[i] = lexerp->command[lexerp->token.start + i];
			}
			/* Make sure to null-terminate the string. */
			string[i] = '\0';
			/* Build the node! */
			newnodep->base.type = DB_EETNODE_CONST_DBSTRING;
			newnodep->string = string;
		}
		advanceeetnodepointer(&stack, 1);
	}
	
	return 1;
}


/* Setup and verify attributes are good.

TODO: We have implicitly assumed that no projection involving complex expressions will occur
      below the joins.  this may not be a good assumption, I don't know.
	-I mean things like "SELECT 2*T.x".  "2*T.x" is complex.
*/
/**
@brief		Setup an attribute's expression node given the operator
		it will be evaluated at.
@param		attrnodep	A pointer to the attribute expression node
				to setup.
@param		lexerp		A pointer to the lexer instance variable
				used to generate these tokens.
@param		evalpoint	The operator which the expression belongs to.
@param		scans		The array of scans that will be generating
				tuples from the relations involved
				in the query.
@param		numscans	The number of scan operators in the @p scans
				array.
@param		depth		The depth of the evaluation point in the
				execution tree.
*/
db_int setupattribute(db_eetnode_attr_t *attrnodep, db_lexer_t *lexerp, db_op_base_t *evalpoint, scan_t *scans, db_uint8 numscans, int depth)
{
	/* Walk the query tree, from given point. */
	if (DB_NTJOIN == evalpoint->type || DB_OSIJOIN == evalpoint->type)
	{
		db_int retval;
		
		/* Try left child. */
		if (1==(retval=setupattribute(attrnodep, lexerp, ((ntjoin_t*)evalpoint)->lchild, scans, numscans, depth+1))){}
		else if (-1 == retval)
		{
			return retval;
		}
		/* Try right child. */
		else if (1==(retval=setupattribute(attrnodep, lexerp, ((ntjoin_t*)evalpoint)->rchild, scans, numscans, depth+1)))
		{
			if (depth > 0)
			{
				attrnodep->pos += ((ntjoin_t*)evalpoint)->lchild->header->num_attr;
				attrnodep->tuple_pos = 0;
			}
			else
			{
				attrnodep->tuple_pos = 1;
			}
		}
		else if (-1 == retval)
		{
			return retval;
		}
		else
		{
			return 0;
		}
	}
	else if ((DB_PROJECT == evalpoint->type || DB_AGGREGATE == evalpoint->type) && depth > 0)
	{
		// TODO What to do here?  Simply error?  Not clear.
	}
	else if (1 == numopchildren(evalpoint))
	{
		db_int retval = setupattribute(attrnodep, lexerp, ((db_op_onechild_t*)evalpoint)->child, scans, numscans, depth+1);
		if (1 == retval)
		{
			attrnodep->tuple_pos = 0;
		}
		return retval;
	}
	else
	{
		db_lexer_token_t temptoken;
		
		/* Recall, we have set attrnodep->pos to be the number of tokens
		   specified in the query for this attribute. */
		/* Handle parts of the identifier. */
		
		/* No table identifier, must check for ambiguity! */
		// TODO: Pull this out so ambiguity/existence check done only once per attribute reference.
		if (1 == attrnodep->pos)
		{
			gettokenat(&temptoken, *lexerp, attrnodep->tokenstart, 0);
			char attrname[gettokenlength(&temptoken)+1];
			gettokenstring(&temptoken, attrname, lexerp);
			
			db_int count = 0;
			db_int i;
			for (i = 0; i < (db_int)numscans; ++i)
			{
				if (-1 != getposbyname(scans[i].base.header, attrname))
					count++;
			}
			if (0 == count)
			{
				DB_ERROR_MESSAGE("attribute does not exist", attrnodep->tokenstart, lexerp->command);
				return -1;
			}
			else if (count > 1)
			{
				DB_ERROR_MESSAGE("column name ambiguous", attrnodep->tokenstart, lexerp->command);
				return -1;
			}
			/* Otherwise, good to go! */
		}
		/* Check if this is the correct table. */
		else if (2 == attrnodep->pos)
		{
			gettokenat(&temptoken, *lexerp, attrnodep->tokenstart, 0);
			char tablename[gettokenlength(&temptoken)+1];
			gettokenstring(&temptoken, tablename, lexerp);
			gettokenat(&temptoken, *lexerp, ((scan_t*)evalpoint)->start, 0);
			
			if(!token_stringequal(&temptoken, tablename, sizeof(tablename)-1, lexerp, 0))
			{
				return 0;
			}
			gettokenat(&temptoken, *lexerp, attrnodep->tokenstart, 2);
			attrnodep->tokenstart = temptoken.start;
		}
		else
		{
			DB_ERROR_MESSAGE("unsupported - database identifier", attrnodep->tokenstart, lexerp->command);
			return -1;
		}
		
		/* IMPORTANT: Once here, code expects atrnodep->tokenstart to
		   be actual start of attr name token! */
		
		gettokenat(&temptoken, *lexerp, attrnodep->tokenstart, 0);
		char attrname[gettokenlength(&temptoken)+1];
		gettokenstring(&temptoken, attrname, lexerp);
		
		db_int retval = getposbyname(evalpoint->header, attrname);
		switch(retval)
		{
			case -1:
				DB_ERROR_MESSAGE("attribute does not exist", attrnodep->tokenstart, lexerp->command);
				return -1;
			default:
				attrnodep->pos = (db_uint8)retval;
				break;
		}
		attrnodep->tuple_pos = 0;
	}
	// TODO: Handle other operators?
	return 1;
}

db_int verifysetupattributes(db_eet_t *eetp, db_lexer_t *lexerp, db_op_base_t *evalpoint, scan_t *scans, db_uint8 numscans, int startdepth)
{
	db_eetnode_t *cursor = eetp->nodes;
	db_int retval;
	while (POINTERBYTEDIST(cursor, eetp->nodes) < eetp->size)
	{
		if (DB_EETNODE_ATTR == cursor->type)
		{
			retval = setupattribute((db_eetnode_attr_t*)cursor, lexerp, evalpoint, scans, numscans, startdepth);
			if (1 != retval)
			{
				return retval;
			}
		}
		advanceeetnodepointer(&cursor, 1);
	}
	
	return 1;
}
