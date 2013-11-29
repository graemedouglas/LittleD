/******************************************************************************/
/**
@file		select.c
@author		Graeme Douglas
@brief		The implementation of the relation selection operator.
@see		For more information, refer to @ref select.h.
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

#include "db_ops_types.h"
#include "select.h"
#include "db_ops.h"
#include "../dblogic/eet.h"

/* Initialize the selection operator. */
db_int init_select(select_t *sp, db_eet_t *ep, db_op_base_t *child, db_query_mm_t *mmp)
{
	sp->base.type = DB_SELECT;
	sp->base.header = child->header;
	sp->tree = ep;
	sp->child = child;
	return 1;
}

/* This method assumes the scan is already initialized */
db_int rewind_select(select_t *sp, db_query_mm_t *mmp)
{
	rewind_dbop(sp->child, mmp);
	return 1;
}

/* Find next tuple that passes the selection condition.
   -This assumes that the evaluation tree will alway s reduce to either 0 or 1
    all tuples. */
db_int next_select(select_t *sp, tuple_t *next_tp, db_query_mm_t *mmp)
{
	/* Create necessary result variable. */
	db_int result, retval = 0;
	while(1)
	{
		/* Get next tuple from operator below. */
		result = next(sp->child, next_tp, mmp);
		/* Evaluate select eet on this tuple.
		   -Note that we can make the assumption that the header and
		    tuple arrays will be only one attribute big, hence why
		    the following code works. */
		if (1==result)
		{
			retval = evaluate_eet(sp->tree, &result, &next_tp, &(sp->base.header), 0, mmp);
			if (1 == result && 1 == retval)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	return result;
}

/* Close the selection operator. */
db_int close_select(select_t *sp, db_query_mm_t *mmp)
{
	/* For the time being, do nothing! */
	return 1;
}
