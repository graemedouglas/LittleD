/******************************************************************************/
/**
@author		Graeme Douglas
@file		db_ops.h
@brief		Interface for generic database operators.
@details	This is a file that contains database operator related methods.
		The main goal of this file is to help achieve an object-oriented
		style to all operators by simulating inheritence/polymorphism
		with structs.

		Notes:
			-Assumption is made that the first parameter passed
			to all functions below is a pointer to the _GENERIC_
			operator.  Beyond that, no assumptions are made.
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

#ifndef DB_OP_H
#define DB_OP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "db_ops_types.h"
#include "../dbmm/db_query_mm.h"
#include "../dbobjects/tuple.h"
#include "scan.h"
#include "project.h"
#include "select.h"
#include "ntjoin.h"
#include "osijoin.h"
#include "sort.h"
#include "aggregate.h"

/**
@brief		Find the index that uses this attribute.
@param		sp		The scan operator that is indexed.
@param		attrp		The attribute that needs to be indexed.
@returns	Which index to use, or else @c -1.
*/
db_int8 findindexon(scan_t *sp, db_eetnode_attr_t *attrp);

/*
A generic next method that can be called on any operator.
	-It is assumed that next always returns 1 on success,
	0 on no more tuples, and any other number on failure.
*/
/**
@brief		Get the next tuple from the operator.
@param		op		Pointer to the operator.
@param		next_tp		Pointer to the initialized tuple
				to place ther result in.
@param		mmp		A pointer to the memory manager pointer
				that will allocate memory for the operator.
@returns	@c 0 if no more tuples to return, @c -1 if an error occurs,
		@c 1 otherwise.
*/
db_int next(db_op_base_t *op, db_tuple_t *next_tp, db_query_mm_t *mmp);

/* A generic rewind method. */
/**
@brief		Allow the operator to be re-used.  Rewind is recursive.
@param		op		Pointer to the operator to be re-initialized.
@param		mmp		A pointer to the memory manager pointer
				that will allocate memory for the operator.
@returns	@c 1 if the operator (and its children) were rewound, @c -1
		if an error occurs, @c 0 otherwise.
*/
db_int rewind_dbop(db_op_base_t *op, db_query_mm_t *mmp);

/* A generic close method. */
/**
@brief		Cleanly, non-recursively destroy an operator.
@param		op		Pointer to the operator to be closed.
@param		mmp		A pointer to the memory manager pointer
				that will allocate memory for the operator.
@returns	@c 1 if the operator (and its children) were closed, @c -1
		if an error occurs, @c 0 otherwise.
*/
void close(db_op_base_t *op, db_query_mm_t *mmp);

/* Get the number of childrem an operator has. */
/**
@brief		Get the number of children an operator has.
@param		op		Pointer to the operator.
@returns	The number of children the operator has.
*/
db_int numopchildren(db_op_base_t *op);

/* Close an entire execution tree recursively. */
/**
@brief		Close the entire execution tree.
@param		op		Pointer to the root operator in the execution
				tree.
@param		mmp		A pointer to the memory manager pointer
				that will allocate memory for the operator.
@returns	@c 1 if the execution tree was cleanly dstroyed, @c -1
		otherwise.
*/
db_int closeexecutiontree(db_op_base_t *op, db_query_mm_t *mmp);

#ifdef __cplusplus
}
#endif

#endif
