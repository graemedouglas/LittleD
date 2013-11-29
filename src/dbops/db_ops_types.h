//*****************************************************************************
/**
@file		db_ops_types.h
@author		Graeme Douglas
@brief		Type definitions for relational operators.
@details	This is a utility header file that contains database operator
		related information.  The main goal of this file is to achieve
		an object-oriented style to all operators by simulating
		inheritence and polymorphism with well constructed structs.

		Notes:
		*** _NO_ method prototypes should appear in this file. ***
		-The idea is that the header files for all of the operators
		should be able to reference this file safely, db_ops.h should
		be able to reference this file and operator header files safely
		and all operator implementation files should be able to
		reference db_ops.h safely without creating a circular
		dependency.
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
//*****************************************************************************


#ifndef DB_OPS_TYPES_H
#define DB_OPS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../dbobjects/relation.h"
#include "../dblogic/eet.h"
#include "../dbindex/dbindex_types.h"
#include "../ref.h"
#include "../db_ctconf.h"
#include "../dbstorage/dbstorage.h"
#include <stdio.h>

/* The following is used to define database operator types.*/
/**
@brief		Database relational operator enumerated types.
@details	These are the types that indicate what type an operator is
		when the generic operator type is handled.
*/
typedef enum
{
	DB_SCAN = 0,	/**< File based scan operator. @todo Write table-based equivalent. */
	DB_PROJECT,	/**< Relational projection operator. */
	DB_SELECT,	/**< Relational selection operator. */
	DB_NTJOIN,	/**< Relational inner join operator. */
	DB_OSIJOIN,	/**< Relational inner join operator. */
	DB_SORT,	/**< Relaitonal sort operator. */
	DB_AGGREGATE,	/**< Relational aggregate operator. */
	DB_OP_COUNT	/**< Number of enumerated values/types. */
} db_op_type;

/* The "base class" of all operator types. */
/**
@struct		db_op_base_t
@brief		The generic type of all operators.
@details	The following struct is used as the "base class" for all
		operators, and the first member of any database operator's
		struct _MUST_ be an instance of this struct.
*/
typedef struct
{
	/*@{*/
	db_op_type type;		/**< The type of the operator.  @todo WRITE A REFERENCE! */
	relation_header_t *header;	/**< Pointer to the schema for the
					     operator. */
	/*@}*/
} db_op_base_t;

/**
@struct		db_op_onechild_t
@brief		A generic structure that can be used as a cast for
		operators with only once child.
@details	For some algorithms, it is cleaner to treat several operators
		as essentially the same, provided they have one child.
*/
typedef struct
{
	db_op_base_t base;		/**< The supertype of this struct. */
	db_op_base_t *child;		/**< Pointer to the this operator's
					     child in the query execution tree.
					*/
} db_op_onechild_t;

/*** This section defines all structs needed for the operators. ***/
/* Scan struct. */
/**
@struct		scan_t
@brief		The scan operator type.
*/
typedef struct {
	/*@{*/
	db_op_base_t	base;		/**< The supertype of this struct. */
	db_int		start;		/**< First character of tables name. */
	db_int		end;		/**< First character following tables
					     name. */
	long		tuple_start;	/**< Offset of first tuple record. */
	db_index_meta_t	idx_meta_data;	/**< File containing metadata for
					     index. */
	db_fileref_t	relation;	/**< File pointer to relation file. */
	db_int8		indexon;	/**< Which index attribute index scan
					     from. */
	db_int		stopat;		/**< Value to stop scanning at. */
	/*@}*/
} scan_t;

/* Project struct */
/**
@struct		project_t
@brief		The relational projection operator.
@details	The projection operator chooses columns from a table to return.
		It is also possible to "add" columns to the new relation
		defined by this operator.  From an SQL perspective, this
		operator is the "SELECT <column-list> ..." portion of the
		query.
*/
typedef struct
{
	/*@{*/
	db_op_base_t base;		/**< The supertype of this struct. */
	db_op_base_t *child;		/**< Pointer to the this operator's
					     child in the query execution tree.
					*/
	db_eet_t *exprs;		/**< Array the evaluable
					     expression trees (EETs) that
					     describe the columns to project.
					
					     Note the caller must manage this
					     members memory.
					*/
	db_uint8 num_exprs;
	/*@}*/
} project_t;

/* Select struct. */
/**
@struct		select_t
@brief		The relational selection operator.
@details	The selection operator chooses tuples that match a given
		predicate, and discards the rest.  From an SQL perspective,
		this operator is the WHERE clause of a query.
*/
typedef struct
{
	/*@{*/
	db_op_base_t base;		/**< The supertype of this struct. */
	db_op_base_t *child;		/**< This operator's child in the query
					     execution plan. */
	db_eet_t *tree;			/**< Pointer to the evaluable
					     expression tree that describes
					     the predicate to match tuples on.
					*/
	/*@}*/
} select_t;

/* Join struct. */
/**
@struct		ntjoin_t
@brief		The relational (nested-tuple) join operator.
@details	A join takes two tables and, for each row from the left
		relation, adds to it all the attributes of the right relation.
		The operator also has the ability to selectively eliminate
		tuples that don't match some condition, in the same way
		relational selection does.
@todo		Maybe change lt to be a pointer such that it could be interchanged
		with indexon? would have to deal with number issue, unless pointer
		was to a struct with pointer and size... maybe.  Same goes for lvalid/bitinfo.
@todo		Merge them this and @ref osijoin_t into one.
*/
typedef struct
{
	/*@{*/
	db_op_base_t base;		/**< The supertype of this struct. */
	db_eet_t *tree;			/**< The condition that must be met
					     in order for the tuples to join.
					*/
	db_eet_t *uexpr;		/**< The expression to match against. */
	db_uint8 indexon;		/**< Which index to use. */
	db_op_base_t *lchild;		/**< This operator's left child in the
					     query execution tree. */
	db_op_base_t *rchild;		/**< This operator's right child in the
					     query execution tree. */
	tuple_t lt;			/**< The currently considered tuple of
					     of the left child. */
	db_uint8 lvalid;		/**< A flag that signals if there are
					     any tuples remaining in the left
					     relation or not.
					@todo Rename l_valid?
					*/
	/*@}*/
} ntjoin_t;

/**
@brief		A one-sided index join.
*/
typedef struct
{
	/*@{*/
	db_op_base_t base;		/**< The supertype of this struct. */
	db_eet_t *tree;			/**< The condition that must be met
					     in order for the tuples to join.
					*/
	db_eet_t *uexpr;		/**< The expression to match against. */
	db_uint8 indexon;		/**< Which index to use. */
	db_op_base_t *lchild;		/**< This operator's left child in the
					     query execution tree. */
	db_op_base_t *rchild;		/**< This operator's right child in the
					     query execution tree. */
	tuple_t ut;			/**< The currently considered tuple of
					     of the unindexed child. */
	db_uint8 bitinfo;		/**< Bit information pertinent to
					     the join.
					*/
	/*@}*/
} osijoin_t;

/* Sort struct. */
/**
@struct		sort_t
@brief		The relational sort operator.
@details	Sorting takes a relation and some set of keys in the form of
		expressions and orders the relation by keys.  In SQL terms,
		this operator correlates to the "ORDER BY <ordering-list>"
		clause, but it could be used in other places to speed up
		other operators, such as joins and aggregates, in the future.
@todo		This operator currently utilizes a selection sorting algorithm,
		which is O(n^2).  Work should be done to try and improve this to
		some O(nlogn) algorithm, although this could be challenging
		given out limited RAM and write-time constraints.
*/
typedef struct
{
	/*@{*/
	db_op_base_t base;		/**< The supertype of this struct. */
	db_op_base_t *child;		/**< Pointer to this operator's child
					     in the query exectuion tree. */
	tuple_t *previous_tp;		/**< Pointer to a copy of the previous
					     tuple returned by this operator.
					*/
	db_int next_count;		/**< The number of duplicate tuples
					     to return.  While this is greater
					     than 0, the same tuple is returned.
					*/
	db_eet_t *sort_exprs;		/**< Array of expressions to sort the
					     relation on.
					*/
	db_uint8 num_expr;		/**< The number of elements of
					     sort_exprs. */
	db_uint8* order;		/**< An array of small ints the same
					     size as sort_exprs (num_exprs big).
					     If the ith element in the array is
					     DB_TUPLE_ORDER_ASC, the ith
					     expression in sort_exprs will be
					     sorted in ascending order.  Same
					     goes for DB_TUPLE_ORDER_DESC,
					     conversely.
					*/
	/*@}*/
} sort_t;

#ifdef DB_CTCONF_SETTING_FEATURE_AGGREGATION
#if DB_CTCONF_SETTING_FEATURE_AGGREGATION == 1
/* Aggregation struct. */
/**
@struct		aggregate_t
@brief		The aggregate operator.
@details	Aggregation applies some function over a set of values and
		returns the result.  This correlates to the
		"GROUP BY <group-list>" in an SQL query.
*/
typedef struct
{
	/*@{*/
	db_op_base_t base;		/**< The base type of this struct. */
	db_op_base_t *child;		/**< This operator's child in the
					     query execution tree. */
	tuple_t *previous_tp;		/**< Pointer to a copy of the previous
					     tuple aggregated on.  FOR INTERNAL
					     USE ONLY.
					*/
	db_int next_count;		/**< The number of tuples to be
					     aggregated on currently.  FOR
					     INTERNAL USE ONLY. */
	db_eet_t *exprs;		/**< Array of evaluable expression
					     trees that describe the columns
					     to project out. */
	db_int8 **aggr_locs;		/**< 2d-array of aggregate function
					     node locations.
					     @c aggr_locs[i] @c == @c NULL
					     indicates that this expression
					     has no aggregate.
					     @c aggr_locs[i][j] == -1
					     indicates that there are no more
					     aggregate values in @c exprs[i].
					*/
	db_uint8 num_expr;		/**< The number of elements of
					     exprs and aggr_locs. */
	db_eet_t *groupby_exprs;	/**< Array of evaluable expression
					     trees that describe what to sort
					     the aggregates on. */
	db_uint8 num_groupby_expr;	/**< The number of elements of
					     groupby_exprs. */
	db_eet_t *having_expr;		/**< The having clause's evaluable
					     expression tree. */
	db_int tuples_seen;		/**< The total number of tuples
					     processed. This is needed for
					     computing AVG, etc. */
	/*@}*/
} aggregate_t;
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
