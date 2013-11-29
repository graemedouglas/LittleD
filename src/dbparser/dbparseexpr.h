/******************************************************************************/
/**
@author		Graeme Douglas
@file		dbparseexpr.h
@brief		Portion of parser that converts SQL expressions to executable
		bytecode.
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

#ifndef DBPARSEEXPR_H
#define DBPARSEEXPR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dblexer.h"
#include "../dbobjects/relation.h"
#include "../dbops/db_ops.h"
#include "../dbmm/db_query_mm.h"
#include "../dblogic/eet.h"

/* Parse an expression into evaluable form. */
/**
@brief		Convert an SQL expression to bytecode.
@details	This method completely translates an expression, expressed in
		SQL, to executable bytecode.  This method will fail if
		their is not enough memory to parse the expression out.
@param		exprp		A pointer which, after successful execution
				of the method, will point to the first node
				of the converted expression, on the back stack.
@param		lexerp		A pointer to the lexer which will be used to
				parse the expression.
@param		start		The position of the first character that is
				a part of the expression, in the query string.
@param		end		The position of the first character _AFTER_
				@p start that should _NOT_ included in the
				expression.
@param		mmp		A pointer to the per-query memory manager that
				is being used for this query.
@param		jointolast	A Boolean flag indicating if this expression
				is to be joined to the top most section
				of memory on the back stack.  Set to @c 1 if
				that is desired, @c 0 otherwise.
@returns	@c -1 if any error occured, @c 1 otherwise.
*/
db_int parseexpression(db_eetnode_t **exprp,
			db_lexer_t *lexerp,
			db_int start,
                        db_int end,
			db_query_mm_t *mmp,
			db_uint8 jointolast);


/**
@brief		Verify and properly setup attributes in an expression.
@details	This method will look for attributes in the expression
		and translate their identifiers into an executable form.
@param		eetp		A pointer to the expression to be processed.
@param		lexerp		A pointer to the lexer currently in use.
@param		evalpoint	A pointer to the operator where the expression
				is being evaluated from.
@param		scans		Pointer to the array of table scan operators.
@param		numscans	The number of scans in the array.
@param		startdepth	The intended depth of the evaluation point.
				Set to @c 0 if the expression is actually
				evaluated at this point. Essentially,
				if the root evaluation point is a join and
				this is set to @c 1 or bigger, pretend
				the evaluation point is _NOT_ a join.
@returns	@c 1 if all attributes verified and processed successfully,
		@c -1 if an error occured, @c 0 otherwise.
*/
db_int verifysetupattributes(db_eet_t *eetp,
				db_lexer_t *lexerp,
				db_op_base_t *evalpoint,
				scan_t *scans,
				db_uint8 numscans,
				int startdepth);

#ifdef __cplusplus
}
#endif

#endif
