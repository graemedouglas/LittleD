/******************************************************************************/
/**
@file		eet.c
@author		Graeme Douglas
@brief		The implementation of expression evaluation.
@see		For more information, refer to @ref eet.h.
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

#include "eet.h"

/* Construct a new relation header from array of expressions.*/
db_int createnewheader(relation_header_t *new_hp, relation_header_t *old_hp,
		db_eet_t *exprs, db_uint8 num_exprs, db_query_mm_t *mmp)
{
	/* Must come up with actual number of attributes. */
	db_int i;
	db_int true_num_exprs = 0;
	for (i = 0; i < (db_int)num_exprs; ++i)
	{
		if (NULL == exprs[i].nodes)
		{
			if (-1 == exprs[i].size)
			{
				exprs[i].size = old_hp->num_attr;
			}
			true_num_exprs += exprs[i].size;
		}
		else
		{
			true_num_exprs++;
		}
	}
	
	/* Allocate all the memory necessary, except for exprs. */
	new_hp->size_name = DB_QMM_BALLOC(mmp, sizeof(db_uint8) * true_num_exprs);
	new_hp->names = DB_QMM_BALLOC(mmp, sizeof(char*) * true_num_exprs);
	new_hp->types = DB_QMM_BALLOC(mmp, sizeof(db_uint8) * true_num_exprs);
	new_hp->offsets = DB_QMM_BALLOC(mmp, sizeof(db_uint8) * true_num_exprs);
	new_hp->sizes = DB_QMM_BALLOC(mmp, sizeof(db_uint8) * true_num_exprs);
	new_hp->num_attr = true_num_exprs;
	new_hp->tuple_size = 0;
	
	/* Which attribute we are now on. */
	i = 0;
		
	/* Which expression we are dealing with. */
	db_int k;
	
	/* For each expression in the array, insert into new header. */
	for (k = 0; k < (db_int)num_exprs; ++k, ++i)
	{
		/* For convenience and clarity, create a temporary node
		   pointer. */
		db_eetnode_t *np = exprs[k].nodes;
		if (NULL == np)
		{
			/* Copy out header information for all necessary attributes. */
			db_int j;
			/* For each expression ... */
			for (j = exprs[k].stack_size; j < exprs[k].stack_size + exprs[k].size; ++j, ++i)
			{
				new_hp->size_name[i] = old_hp->size_name[j];
				new_hp->names[i] = old_hp->names[j];
				new_hp->types[i] = old_hp->types[j];
				new_hp->offsets[i] = new_hp->tuple_size;
				new_hp->sizes[i] = old_hp->sizes[j];
				new_hp->tuple_size += new_hp->sizes[i];
			}
			
			/* Since we will increment i at top of loop, must decrement once. */
			--i;
		}
		else if (np->type == DB_EETNODE_ATTR && sizeof(db_eetnode_attr_t) == exprs[k].size)
		{
			/* Translate information from old header to new header.
			*/
			new_hp->size_name[i] = old_hp->
				size_name[(db_int)(((db_eetnode_attr_t*)np)->pos)];
			new_hp->names[i] = old_hp->
				names[(db_int)(((db_eetnode_attr_t*)np)->pos)];
			new_hp->types[i] = old_hp->
				types[(db_int)(((db_eetnode_attr_t*)np)->pos)];
			/* Current tuple size is next offset */
			new_hp->offsets[i] = new_hp->tuple_size;
			new_hp->sizes[i] = old_hp->
				sizes[(db_int)(((db_eetnode_attr_t*)np)->pos)];
			new_hp->tuple_size += new_hp->sizes[i];
		}
		else 
		{
			db_int type = evaluate_eet(&exprs[k], NULL, NULL, &old_hp, 0, mmp);
			if (DB_EETNODE_CONST_NULL == type)
			{
				/* Translate information from old header to new header.
				*/
				new_hp->size_name[i] = 0;
				new_hp->names[i] = NULL;
				new_hp->types[i] = DB_NULL;
				/* Current tuple size is next offset */
				new_hp->offsets[i] = new_hp->tuple_size;
				new_hp->sizes[i] = 0;
				new_hp->tuple_size += new_hp->sizes[i];
			}
			else if (DB_EETNODE_CONST_DBINT == type)
			{
				/* Translate information from old header to new header.
				*/
				new_hp->size_name[i] = 0;
				new_hp->names[i] = NULL;
				new_hp->types[i] = DB_INT;
				/* Current tuple size is next offset */
				new_hp->offsets[i] = new_hp->tuple_size;
				new_hp->sizes[i] = sizeof(db_int);
				new_hp->tuple_size += new_hp->sizes[i];
			}
			else if (DB_EETNODE_CONST_DBSTRING == type)
			{
				/* Translate information from old header to new header.
				*/
				/* FIXME: Right now, if we perform any operation
				          on a strings or set of strings that modifies
				          the length of the result (think like concatenation),
				          this will be wrong.  We need to think seriously about
				          how to resolve this issue.  One idea would be to
				          put size variables in string nodes, or to have a pointer
				          to the set of strings that were created along the way and
				          free them at the end.
				*/
				new_hp->size_name[i] = 0;
				new_hp->names[i] = NULL;
				new_hp->types[i] = DB_STRING;
				/* Current tuple size is next offset */
				new_hp->offsets[i] = new_hp->tuple_size;
				new_hp->sizes[i] =
					strlen(((db_eetnode_dbstring_t*)np)->
						string)+1;
				new_hp->tuple_size += new_hp->sizes[i];
			}
			else
			{
				// TODO: Free stuff.
				return -1;
			}
			/* TODO: Implement any future types here. */
		}
	}
	return 1;
}

/* Advance a pointer num_times times.  Notice, this does no checking, so
  you better know what YOU ARE DOING!!!! */
db_int advanceeetnodepointer(db_eetnode_t **npp, db_int num_times)
{
	db_int i;
	for (i = 0; i < num_times; ++i)
	{
		// TODO: Once certian macro expansion is working, delete dead code.
		if ((*npp)->type == DB_EETNODE_ATTR)
			MOVEPOINTERNUNITS(*npp, *npp, 1, db_eetnode_t*,
					db_eetnode_attr_t*);
		else if ((*npp)->type == DB_EETNODE_AGGR_ATTR)
			MOVEPOINTERNUNITS(*npp, *npp, 1, db_eetnode_t*,
					db_eetnode_aggr_attr_t*);
		else if ((*npp)->type == DB_EETNODE_AGGR_TEMP)
			MOVEPOINTERNUNITS(*npp, *npp, 1, db_eetnode_t*,
					db_eetnode_aggr_temp_t*);
		else if ((*npp)->type == DB_EETNODE_ALIAS)
			MOVEPOINTERNUNITS(*npp, *npp, 1, db_eetnode_t*,
					db_eetnode_alias_t*);
		else if ((*npp)->type == DB_EETNODE_CONST_DBINT)
			MOVEPOINTERNUNITS(*npp, *npp, 1, db_eetnode_t*,
					db_eetnode_dbint_t*);
		else if ((*npp)->type == DB_EETNODE_CONST_DBSTRING)
			MOVEPOINTERNUNITS(*npp, *npp, 1, db_eetnode_t*,
					db_eetnode_dbstring_t*);
		else
			(*npp)++;
		/* TODO: Implement other types here. */
	}
	return 1;
}

/* This function will return the type of the node at position pos in the
   expression. */
db_uint8 geteetnodetypebypos(db_eet_t *eet, db_int pos)
{
	db_eetnode_t *cursor = eet->nodes;
	advanceeetnodepointer(&cursor, pos);
	return (db_uint8)(cursor->type);
}

/* This function will return a pointer to the node at position pos in the
   expression.  */
db_eetnode_t* geteetnodebypos(db_eet_t *eet, db_int pos)
{
	db_eetnode_t *cursor = eet->nodes;
	advanceeetnodepointer(&cursor, pos);
	return cursor;
}

/* Determines the return type of a function or operator given its parameter
   types. */
db_uint8 funcopreturntype(db_uint8 *array)
{
	if (*array >= (db_uint8)DB_EETNODE_OP_NOT && *array <= (db_uint8)DB_EETNODE_OP_BCOMP)
	{
		array++;
		if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_DBINT;
		}
		else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_NULL;
		}
		else
		{
			return (db_uint8)DB_EETNODE_COUNT;
		}
	}
	else if (DB_EETNODE_OP_UNARYNEG == *array)
	{
		array++;
		if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_DBINT;
		}
		else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_NULL;
		}
		else
		{
			return (db_uint8)DB_EETNODE_COUNT;
		}
	}
	else if (*array >= (db_uint8)DB_EETNODE_OP_BAND && *array <= (db_uint8)DB_EETNODE_OP_MOD)
	{
		array++;
		if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
		{
			array++;
			if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_DBINT;
			}
			else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_NULL;
			}
			else
			{
				return (db_uint8)DB_EETNODE_COUNT;
			}
		}
		else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_NULL;
		}
		else
		{
			return (db_uint8)DB_EETNODE_COUNT;
		}
	}
	else if (*array >= (db_uint8)DB_EETNODE_OP_ADD && *array <= (db_uint8)DB_EETNODE_OP_DIV)
	{
		array++;
		if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
		{
			array++;
			if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_DBINT;
			}
			else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_NULL;
			}
			else
			{
				return (db_uint8)DB_EETNODE_COUNT;
			}
		}
		else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_NULL;
		}
		else
		{
			return (db_uint8)DB_EETNODE_COUNT;
		}
	}
	else if (*array >= (db_uint8)DB_EETNODE_OP_LT && *array <= (db_uint8)DB_EETNODE_OP_EQ)
	{
		array++;
		if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
		{
			array++;
			if ((db_uint8)DB_EETNODE_CONST_DBINT == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_DBINT;
			}
			else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_NULL;
			}
			else
			{
				return (db_uint8)DB_EETNODE_COUNT;
			}
		}
		else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == *array)
		{
			array++;
			if ((db_uint8)DB_EETNODE_CONST_DBSTRING == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_DBSTRING;
			}
			else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
			{
				return (db_uint8)DB_EETNODE_CONST_NULL;
			}
			else
			{
				return (db_uint8)DB_EETNODE_COUNT;
			}
		}
		else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_NULL;
		}
		else
		{
			return (db_uint8)DB_EETNODE_COUNT;
		}
	}
	else if (*array == (db_uint8)DB_EETNODE_OP_ISNULL)
	{
		return (db_uint8)DB_EETNODE_CONST_DBINT;
	}
	else if (*array == (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING)
	{
		array++;
		if ((db_uint8)DB_EETNODE_CONST_DBSTRING == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_DBINT;
		}
		else if ((db_uint8)DB_EETNODE_CONST_NULL == *array)
		{
			return (db_uint8)DB_EETNODE_CONST_NULL;
		}
		else
		{
			return (db_uint8)DB_EETNODE_COUNT;
		}
	}
	else
	{
		return (db_uint8)DB_EETNODE_COUNT;
	}
	/* TODO: Implement all future types throughout here. */
	/* TODO: Implement all future functions/operators throughout here. */
}

/* Retrieves the type and length of an expression rooted at node pointed to by
   np. */
db_int subexpr_type_addr(db_eetnode_t *np, db_uint8 *type,
			db_eetnode_t **next_npp, db_uint8 max_stack_size,
			relation_header_t **hpa, db_query_mm_t *mmp)
{
	/*** Variable declaration. ***/
	db_uint8 *stack, *stack_top;
	
	/* Initially, set the value of type to a value greater than any other type's values. */
	*type = DB_EETNODE_CONST_NULL;
	
	stack = DB_QMM_BALLOC(mmp, (db_int)max_stack_size);
	if (stack == NULL)
	{
		return -1;
	}
	else
	{
		/* Set top of stack to bottom of stack. */
		stack_top = stack;
	}
	
	do
	{
		/* Determine the type of the current node. */
		switch (np->type)
		{
			case (db_uint8)DB_EETNODE_ATTR:
				switch(gettypebypos(hpa[(db_int)(((db_eetnode_attr_t*)np)->tuple_pos)],
							((db_eetnode_attr_t*)np)->pos))
				{
					case (db_uint8)DB_INT:
						*stack_top = (db_uint8)DB_EETNODE_CONST_DBINT;
						break;
					case (db_uint8)DB_STRING:
						*stack_top = (db_uint8)DB_EETNODE_CONST_DBSTRING;
						break;
					default:
						DB_QMM_BFREE(mmp, stack);
						return -1;
					/* TODO: Future types here! */
				}
				np = ((db_eetnode_t*)(((db_eetnode_attr_t*)np)+1));
				break;
			case (db_uint8)DB_EETNODE_ALIAS:
				// TODO: THIS MIGHT NOT EVEN BE NECESSARY!
				/* This is a little ugly, but it should work.
				   Call this method on this nodes
				   subexpression. */
				subexpr_type_addr(((db_eetnode_alias_t*)np)->expr->nodes, stack_top,
						NULL, (((db_eetnode_alias_t*)np)->expr->stack_size) / sizeof(db_eetnode_t),
						hpa, mmp);
				np = ((db_eetnode_t*)(((db_eetnode_alias_t*)np)+1));
				break;
			case (db_uint8)DB_EETNODE_CONST_DBINT:
				*stack_top = np->type;
				np = ((db_eetnode_t*)(((db_eetnode_dbint_t*)np)+1));
				break;
			case (db_uint8)DB_EETNODE_CONST_DBSTRING:
				*stack_top = np->type;
				np = ((db_eetnode_t*)(((db_eetnode_dbstring_t*)np)+1));
				break;
			case (db_uint8)DB_EETNODE_CONST_NULL:
			case (db_uint8)DB_EETNODE_OP_NOT:
			case (db_uint8)DB_EETNODE_OP_UNARYNEG:
			case (db_uint8)DB_EETNODE_OP_BCOMP:
			case (db_uint8)DB_EETNODE_OP_BAND:
			case (db_uint8)DB_EETNODE_OP_BOR:
			case (db_uint8)DB_EETNODE_OP_BXOR:
			case (db_uint8)DB_EETNODE_OP_AND:
			case (db_uint8)DB_EETNODE_OP_OR:
			case (db_uint8)DB_EETNODE_OP_XOR:
			case (db_uint8)DB_EETNODE_OP_MOD:
			case (db_uint8)DB_EETNODE_OP_ADD:
			case (db_uint8)DB_EETNODE_OP_SUB:
			case (db_uint8)DB_EETNODE_OP_MULT:
			case (db_uint8)DB_EETNODE_OP_DIV:
			case (db_uint8)DB_EETNODE_OP_LT:
			case (db_uint8)DB_EETNODE_OP_GT:
			case (db_uint8)DB_EETNODE_OP_LTE:
			case (db_uint8)DB_EETNODE_OP_GTE:
			case (db_uint8)DB_EETNODE_OP_NEQ:
			case (db_uint8)DB_EETNODE_OP_EQ:
			case (db_uint8)DB_EETNODE_OP_ISNULL:
			case (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING:
				*stack_top = np->type;
				np++;
				break;
			default:
				DB_QMM_BFREE(mmp, stack);
				return -1;
		}
		stack_top++;
		/* TODO: Add cases for future types/funcs/ops here. */
		
		/* While we can reduce stack more. */
		while (stack_top > stack)
		{
			db_uint8 *stack_lookup = stack_top-1, count = 0;
			
			/* Search for the last operator on stack. */
			while (stack_lookup > stack)
			{
				if ((db_uint8)DB_EETNODE_CONST_DBINT == *stack_lookup ||
				    (db_uint8)DB_EETNODE_CONST_DBSTRING == *stack_lookup ||
				    (db_uint8)DB_EETNODE_CONST_NULL == *stack_lookup)
				{
					count++;
					stack_lookup--;	
				}
				else
				{
					/* Found the last operator. */
					break;
				}
			}
			/* If we have found an operator. */
			if (*stack_lookup >= (db_uint8)DB_EETNODE_OP_NOT && *stack_lookup < (db_uint8)DB_EETNODE_COUNT)
			{
				if (*stack_lookup >= (db_uint8)DB_EETNODE_OP_UNARYNEG && *stack_lookup <= (db_uint8)DB_EETNODE_OP_BCOMP)
				{
					if (count < 1)
					{
						break;
					}
				}
				else if (*stack_lookup >= (db_uint8)DB_EETNODE_OP_BAND && *stack_lookup <= (db_uint8)DB_EETNODE_OP_MOD)
				{
					if (count < 2)
					{
						break;
					}
				}
				else if (*stack_lookup >= (db_uint8)DB_EETNODE_OP_ADD && *stack_lookup <= (db_uint8)DB_EETNODE_OP_DIV)
				{
					if (count < 2)
					{
						break;
					}
				}
				else if (*stack_lookup >= (db_uint8)DB_EETNODE_OP_LT && *stack_lookup <= (db_uint8)DB_EETNODE_OP_EQ)
				{
					if (count < 2)
					{
						break;
					}
				}
				else if (*stack_lookup >= (db_uint8)DB_EETNODE_OP_ISNULL && *stack_lookup <= (db_uint8)DB_EETNODE_OP_ISNULL)
				{
					if (count < 1)
					{
						break;
					}
				}
				else if (*stack_lookup >= (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING && *stack_lookup <= (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING)
				{
					if (count < 1)
					{
						break;
					}
				}
				/** Common code area. **/
				*stack_lookup = funcopreturntype(stack_lookup);
				/* Error check. */
				if ((db_uint8)DB_EETNODE_COUNT == *stack_lookup)
				{
					DB_QMM_BFREE(mmp, stack);
					return -1;
				}
				stack_top = stack_lookup;
			}
			else
			{
				/* Force a return. */
				stack_top = stack;
			}
		}
	} while (stack_top > stack);
	if (next_npp != NULL)
	{
		*next_npp = np;
	}
	*type = *stack;
	DB_QMM_BFREE(mmp, stack);
	if ((db_uint8)DB_EETNODE_CONST_DBINT != *type && 
	    (db_uint8)DB_EETNODE_CONST_DBSTRING != *type && 
	    (db_uint8)DB_EETNODE_CONST_NULL != *type)
	{
		return -1;
	}
	return 1;
}

/*** Evaluate the tree and put result in rp. ***/
/* Notes:
-This function returns 2 when it really means to return NULL.  Operators
need to be aware of this.
*/
// TODO: Implement future types for function.  Might need to be careful w.r.t. scalable sized integers.
/* We perform promotion on the operators arguments.  That is, if the operator
   takes in different types, arguments must be cast up to the lowest commonly
   operable type.  Say we wish to add two integers, one is 8-bits long and the
   other is 16-bits long.  We must cast the 8-bit integer up to a 16-bit
   integer.  It is this process that is called promotion.  */
db_eetnode_t *eet_promote(db_eetnode_t *values, db_uint8 numvalues, db_uint8 *promotedto_p, db_query_mm_t *mmp)
{
	db_eetnode_t	*value = values;
	size_t		 typesize;
	db_uint8	 processed = 0;
	*promotedto_p = 0;
	
	while (processed < numvalues)
	{
		if (value->type > *promotedto_p)
			*promotedto_p = value->type;
		advanceeetnodepointer(&value, 1);
		processed++;
	}
	
	if ((db_uint8)DB_EETNODE_CONST_DBINT == *promotedto_p)
	{
		typesize = sizeof(db_eetnode_dbint_t);
	}
	else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == *promotedto_p)
	{
		typesize = sizeof(db_eetnode_dbstring_t);
	}
	else
	{
		typesize = sizeof(db_eetnode_t);
	}
	
	// TODO: At some point, we will have to deal with allocating space for strings.
	db_eetnode_t *newvals = db_qmm_balloc(mmp, numvalues*typesize);
	if (NULL == newvals)
		return NULL;
	db_eetnode_t *newval;
	MOVEPOINTERNBYTES(newval, newvals, typesize*(numvalues-1), db_eetnode_t*);
	value = values;
	processed = 0;
	
	while (processed < numvalues)
	{
		if ((db_uint8)DB_EETNODE_CONST_DBINT == *promotedto_p)
		{
			newval->type = DB_EETNODE_CONST_DBINT;
			if ((db_uint8)DB_EETNODE_CONST_DBINT == value->type)
			{
				*((db_eetnode_dbint_t*)newval) = *((db_eetnode_dbint_t*)value);
			}
			else if ((db_uint8)DB_EETNODE_CONST_NULL == value->type)
			{
				newval->type = DB_EETNODE_CONST_NULL;
			}
		}
		else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == *promotedto_p)
		{
			if ((db_uint8)DB_EETNODE_CONST_DBINT == value->type)
			{
				return NULL;
			}
			else if ((db_uint8)DB_EETNODE_CONST_NULL == value->type)
			{
				newval->type = DB_EETNODE_CONST_NULL;
			}
			else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == value->type)
			{
				*((db_eetnode_dbstring_t*)newval) = *((db_eetnode_dbstring_t*)value);
			}
		}
		
		MOVEPOINTERNBYTES(newval, newval, -1*typesize, db_eetnode_t*);
		advanceeetnodepointer(&value, 1);
		processed++;
	}
	
	return newvals;
}

db_int eet_numrequiredvals(db_uint8 type)
{
	if ((db_uint8)DB_EETNODE_OP_BAND <= type && (db_uint8)DB_EETNODE_OP_EQ >= type)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

db_int eet_evaloporfunc(db_eetnode_t **valuesp, db_uint8 type, db_uint8 numvalues, db_uint8 promotedto, db_query_mm_t *mmp)
{
	size_t typesize;
	switch (promotedto) {
	case (db_uint8)DB_EETNODE_CONST_DBINT:
		typesize = sizeof(db_eetnode_dbint_t);
		break;
	case (db_uint8)DB_EETNODE_CONST_DBSTRING:
		typesize = sizeof(db_eetnode_dbstring_t);
		break;
	default:
		typesize = sizeof(db_eetnode_t);
		break;
	}
	
	if ((db_uint8)DB_EETNODE_CONST_NULL == promotedto)
	{
		if ((db_uint8)DB_EETNODE_OP_ISNULL == type)
		{
			*valuesp = db_qmm_bextend(mmp, sizeof(db_eetnode_dbint_t)-sizeof(db_eetnode_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = 1;
		}
		else
		{
			(*valuesp)->type = DB_EETNODE_CONST_NULL;
		}
	}
	// TODO: Move AND, OR to be away from others, to make this nicer.
	else if ((db_uint8)DB_EETNODE_OP_AND != type &&
	    (db_uint8)DB_EETNODE_OP_OR != type &&
	    (db_uint8)DB_EETNODE_OP_ISNULL != type &&
	    ((db_uint8)DB_EETNODE_CONST_NULL == (*valuesp)->type || (eet_numrequiredvals(type) > 1 && (db_uint8)DB_EETNODE_CONST_NULL == POINTERATNBYTES(*valuesp, typesize, db_eetnode_t*)->type)))
	{
		(*valuesp)->type = DB_EETNODE_CONST_NULL;
	}
	else if ((db_uint8)DB_EETNODE_CONST_DBINT == promotedto)
	{
		db_eetnode_dbint_t *arr = (db_eetnode_dbint_t*)*valuesp;
		switch (type) {
		case (db_uint8)DB_EETNODE_OP_UNARYNEG:
			arr[0].integer = -1*(arr[0].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_NOT:
			arr[0].integer = !(arr[0].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_BAND:
			arr[0].integer = (arr[0].integer) & (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_BOR:
			arr[0].integer = (arr[0].integer) | (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_BXOR:
			arr[0].integer = (arr[0].integer) ^ (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_AND:
			if (((db_uint8)DB_EETNODE_CONST_NULL == arr[0].base.type && 0 == arr[1].integer) || ((db_uint8)DB_EETNODE_CONST_NULL == arr[1].base.type && 0 == arr[0].integer))
			{
				arr[0].base.type = DB_EETNODE_CONST_DBINT;
				arr[0].integer = 0;
			}
			else if (((db_uint8)DB_EETNODE_CONST_NULL == arr[0].base.type) || ((db_uint8)DB_EETNODE_CONST_NULL == arr[1].base.type))
				arr[0].base.type = DB_EETNODE_CONST_NULL;
			else
				arr[0].integer = (arr[0].integer) && (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_OR:
			if (((db_uint8)DB_EETNODE_CONST_NULL == arr[0].base.type && 1 == arr[1].integer) || ((db_uint8)DB_EETNODE_CONST_NULL == arr[1].base.type && 1 == arr[0].integer))
			{
				arr[0].base.type = DB_EETNODE_CONST_DBINT;
				arr[0].integer = 1;
			}
			else if (((db_uint8)DB_EETNODE_CONST_NULL == arr[0].base.type) || ((db_uint8)DB_EETNODE_CONST_NULL == arr[1].base.type))
				arr[0].base.type = DB_EETNODE_CONST_NULL;
			else
				arr[0].integer = (arr[0].integer) || (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_XOR:
			arr[0].integer = ((arr[0].integer) || (arr[1].integer)) && ((arr[0].integer) != (arr[1].integer));
			break;
		case (db_uint8)DB_EETNODE_OP_MOD:
			if (0 == arr[1].integer)
				arr[0].base.type = DB_EETNODE_CONST_NULL;
			else
				arr[0].integer = (arr[0].integer) % (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_ADD:
			arr[0].integer = (arr[0].integer) + (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_SUB:
			arr[0].integer = (arr[0].integer) - (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_MULT:
			arr[0].integer = (arr[0].integer) * (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_DIV:
			if (0 == arr[1].integer)
				arr[0].base.type = DB_EETNODE_CONST_NULL;
			else
				arr[0].integer = (arr[0].integer) / (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_LT:
			arr[0].integer = (arr[0].integer) < (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_GT:
			arr[0].integer = (arr[0].integer) > (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_LTE:
			arr[0].integer = (arr[0].integer) <= (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_GTE:
			arr[0].integer = (arr[0].integer) >= (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_NEQ:
			arr[0].integer = (arr[0].integer) != (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_EQ:
			arr[0].integer = (arr[0].integer) == (arr[1].integer);
			break;
		case (db_uint8)DB_EETNODE_OP_ISNULL:
			arr[0].integer = 0;
			break;
		default:
			// Invalid operator for type.
			return -1;
		}
	}
	else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == promotedto)
	{
		db_int result;
		db_eetnode_dbstring_t *arr = (db_eetnode_dbstring_t*)*valuesp;
		// TODO: Pull out common code for ROM savings
		switch(type) {
		case (db_uint8)DB_EETNODE_FUNC_LENGTH_DBSTRING:
			result = strlen(((db_eetnode_dbstring_t*)(*valuesp))->string);
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		case (db_uint8)DB_EETNODE_OP_ISNULL:
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			((db_eetnode_dbint_t*)(*valuesp))->integer = 0;
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			break;
		case (db_uint8)DB_EETNODE_OP_LT:
			result = (0 > strcmp(arr[0].string, arr[1].string));
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		case (db_uint8)DB_EETNODE_OP_GT:
			result = (0 < strcmp(arr[0].string, arr[1].string));
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		case (db_uint8)DB_EETNODE_OP_LTE:
			result = (0 >= strcmp(arr[0].string, arr[1].string));
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		case (db_uint8)DB_EETNODE_OP_GTE:
			result = (0 <= strcmp(arr[0].string, arr[1].string));
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		case (db_uint8)DB_EETNODE_OP_EQ:
			result = (0 == strcmp(arr[0].string, arr[1].string));
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		case (db_uint8)DB_EETNODE_OP_NEQ:
			result = (0 != strcmp(arr[0].string, arr[1].string));
			db_qmm_bfree(mmp, *valuesp);
			*valuesp = db_qmm_balloc(mmp, sizeof(db_eetnode_dbint_t));
			(*valuesp)->type = DB_EETNODE_CONST_DBINT;
			((db_eetnode_dbint_t*)(*valuesp))->integer = result;
			break;
		default:
			// Invalid operator for type.
			return -1;
		}
	}
	
	return 1;
}

// TODO: Change tp, hp to single pointers if I can.
db_int evaluate_eet(db_eet_t *exprp, void *rp, tuple_t **tp, relation_header_t **hp, db_uint8 start_from, db_query_mm_t *mmp)
{
	db_eetnode_t	*stack = db_qmm_balloc(mmp, 0);
	db_eetnode_t	*stack_top = stack;
	db_uint8	 numvals = 0;
	db_uint8	 numreqvals;
	
	db_eetnode_t *cursor = exprp->nodes;
	
	advanceeetnodepointer(&cursor, start_from);
	
	while(POINTERBYTEDIST(cursor, exprp->nodes) < exprp->size)
	{
		if ((db_uint8)DB_EETNODE_CONST_NULL == cursor->type)
		{
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_t));
			*stack_top = *cursor;
			numvals++;
		}
		else if ((db_uint8)DB_EETNODE_CONST_DBINT == cursor->type)
		{
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_dbint_t));
			*((db_eetnode_dbint_t*)stack_top) = *((db_eetnode_dbint_t*)cursor);
			numvals++;
		}
		else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == cursor->type)
		{
			stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_dbstring_t));
			*((db_eetnode_dbstring_t*)stack_top) = *((db_eetnode_dbstring_t*)cursor);
			numvals++;
		}
		else if ((db_uint8)DB_EETNODE_ATTR == cursor->type)
		{
			// TODO: Fix the ifs.  Please.
			//if ((db_uint8)DB_EETNODE_ATTR == value->type)
			db_int pos = (db_int)(((db_eetnode_attr_t*)cursor)->pos);
			db_uint8 attr_type = gettypebypos(hp[(db_int)(((db_eetnode_attr_t*)cursor)->tuple_pos)], pos);
			db_uint8 isnull = 0;
			if (NULL != rp)
			{
				isnull = tp[(db_int)(((db_eetnode_attr_t*)cursor)->tuple_pos)]->
						isnull[pos/8];
				isnull &= (1 << (pos % 8)); /* Extract this attributes null bit. */
				isnull = (isnull >> (pos % 8)); /* Extract this attributes null bit. */
			}
			
			/* If NULL == rp, we are only finding assumed type of expression, adjust accordingly. */
			if (1 == isnull)
			{
				stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_t));
				stack_top->type = DB_EETNODE_CONST_NULL;
				numvals++;
			}
			else if ((db_uint8)DB_INT == attr_type)
			{
				stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_dbint_t));
				stack_top->type = DB_EETNODE_CONST_DBINT;
				if (NULL == rp)
					((db_eetnode_dbint_t*)stack_top)->integer = 1;
				else
					((db_eetnode_dbint_t*)stack_top)->integer = 
						getintbypos(tp[((db_eetnode_attr_t*)cursor)->tuple_pos],
						((db_eetnode_attr_t*)cursor)->pos, hp[((db_eetnode_attr_t*)cursor)->tuple_pos]);
				numvals++;
			}
			else if ((db_uint8)DB_STRING == attr_type)
			{
				stack_top = db_qmm_bextend(mmp, sizeof(db_eetnode_dbstring_t));
				stack_top->type = DB_EETNODE_CONST_DBSTRING;
				if (NULL == rp)
					((db_eetnode_dbstring_t*)stack_top)->string = "\0";
				else
					((db_eetnode_dbstring_t*)stack_top)->string = 
						getstringbypos(tp[((db_eetnode_attr_t*)cursor)->tuple_pos],
						((db_eetnode_attr_t*)cursor)->pos, hp[((db_eetnode_attr_t*)cursor)->tuple_pos]);
				numvals++;
			}
		}
		else	/* It is an operator. */
		{
			numreqvals = eet_numrequiredvals(cursor->type);
			if (numreqvals > numvals)
			{
				db_qmm_bfree(mmp, stack_top);
				return -1;
			}
			
			db_uint8 promotedto;
			db_eetnode_t *newvalues = eet_promote(stack_top, numreqvals, &promotedto, mmp);
			if (NULL == newvalues)
			{
				db_qmm_bfree(mmp, stack_top);
				return -1;
			}
			
			
			switch (eet_evaloporfunc(&newvalues, cursor->type, numreqvals, promotedto, mmp)) {
			case 1:
				break;
			default:
				db_qmm_bfree(mmp, newvalues);
				db_qmm_bfree(mmp, stack_top);
				return -1;
			}
			
			db_eetnode_t *freeto = stack_top;
			advanceeetnodepointer(&freeto, numreqvals);
			
			if ((db_uint8)DB_EETNODE_CONST_NULL == newvalues->type)
			{
				db_eetnode_t temp;
				temp = *newvalues;
				db_qmm_bfree(mmp, newvalues);
				stack_top = db_qmm_bextend(mmp, POINTERBYTEDIST(stack_top, freeto)+sizeof(db_eetnode_t));
				*stack_top = temp;
			}
			else if ((db_uint8)DB_EETNODE_CONST_DBINT == newvalues->type)
			{
				db_eetnode_dbint_t temp;
				temp = *((db_eetnode_dbint_t*)newvalues);
				db_qmm_bfree(mmp, newvalues);
				stack_top = db_qmm_bextend(mmp, POINTERBYTEDIST(stack_top, freeto)+sizeof(db_eetnode_dbint_t));
				*((db_eetnode_dbint_t*)stack_top) = temp;
			}
			else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == newvalues->type)
			{
				db_eetnode_dbstring_t temp;
				temp = *((db_eetnode_dbstring_t*)newvalues);
				db_qmm_bfree(mmp, newvalues);
				stack_top = db_qmm_bextend(mmp, POINTERBYTEDIST(stack_top, freeto)+sizeof(db_eetnode_dbstring_t));
				*((db_eetnode_dbstring_t*)stack_top) = temp;
			}
			
			numvals = numvals - numreqvals + 1;
		}
		
		advanceeetnodepointer(&cursor, 1);
	}
	
	
	// TODO: Handle aggregates.
	if (numvals > 1)
	{
		db_qmm_bfree(mmp, stack_top);
		return -1;
	}
	else if (NULL == rp)
	{
		db_int toret = stack_top->type;
		db_qmm_bfree(mmp, stack_top);
		return toret;
	}
	else if ((db_uint8)DB_EETNODE_CONST_NULL == stack_top->type)
	{
		db_qmm_bfree(mmp, stack_top);
		return 2;
	}
	else if ((db_uint8)DB_EETNODE_CONST_DBINT == stack_top->type)
	{
		*((db_int*)rp) = ((db_eetnode_dbint_t*)stack_top)->integer;
	}
	else if ((db_uint8)DB_EETNODE_CONST_DBSTRING == stack_top->type)
	{
		*((char**)rp) = ((db_eetnode_dbstring_t*)stack_top)->string;
	}
	
	db_qmm_bfree(mmp, stack_top);
	return 1;
}
