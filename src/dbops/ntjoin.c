/******************************************************************************/
/**
@file		ntjoin.c
@author		Graeme Douglas
@brief		Implementation of the ensted-tuple join oeprator.
@see		For more information, reference @ref ntjoin.h.
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
#include "ntjoin.h"
#include "db_ops.h"
#include "../dblogic/eet.h"

/* Initialize the selection operator. */
db_int init_ntjoin(ntjoin_t *jp, db_eet_t *ep, db_op_base_t *lchild, db_op_base_t *rchild, db_query_mm_t *mmp)
{
	/*** Construct new header. ***/
	jp->base.header = DB_QMM_BALLOC(mmp, sizeof(relation_header_t));
	
	/* Number of attributes / tuple size for joined tuple is sum of
	 * children's number of attributes / tuple size. */
	jp->base.header->num_attr = lchild->header->num_attr + rchild->header->num_attr;
	jp->base.header->tuple_size = lchild->header->tuple_size + rchild->header->tuple_size;
	
	/* Allocate all necessary memory for the header. */
	jp->base.header->size_name = DB_QMM_BALLOC(mmp, sizeof(db_uint8)*(jp->base.header->num_attr));
	jp->base.header->names = DB_QMM_BALLOC(mmp, sizeof(char*)*(jp->base.header->num_attr));
	jp->base.header->types = DB_QMM_BALLOC(mmp, sizeof(db_uint8)*(jp->base.header->num_attr));
	jp->base.header->offsets = DB_QMM_BALLOC(mmp, sizeof(db_uint8)*(jp->base.header->num_attr));
	jp->base.header->sizes = DB_QMM_BALLOC(mmp, sizeof(db_uint8)*(jp->base.header->num_attr));
	
	/* Write out left child's header properties into new header. */
	db_int i;
	for (i = 0; i < (db_int)(lchild->header->num_attr); i++)
	{
		*((jp->base.header->size_name)+i) = *((lchild->header->size_name)+i);
		*((jp->base.header->names)+i) = *((lchild->header->names)+i);
		*((jp->base.header->types)+i) = *((lchild->header->types)+i);
		*((jp->base.header->offsets)+i) = *((lchild->header->offsets)+i);
		*((jp->base.header->sizes)+i) = *((lchild->header->sizes)+i);
	}
	db_uint8 first_roffset = lchild->header->offsets[i-1] + lchild->header->sizes[i-1];
	
	/* Write out right child's header properties new header.*/
	for (i = 0; i < (db_int)(rchild->header->num_attr); i++)
	{
		*(((jp->base.header->size_name)+i)+lchild->header->num_attr) = *((rchild->header->size_name)+i);
		*(((jp->base.header->names)+i)+lchild->header->num_attr) = *((rchild->header->names)+i);
		*(((jp->base.header->types)+i)+lchild->header->num_attr) = *((rchild->header->types)+i);
		*(((jp->base.header->offsets)+i)+lchild->header->num_attr) = first_roffset + *((rchild->header->offsets)+i);
		*(((jp->base.header->sizes)+i)+lchild->header->num_attr) = *((rchild->header->sizes)+i);
	}
	
	jp->base.type = DB_NTJOIN;
	jp->tree = ep;
	jp->lchild = lchild;
	jp->rchild = rchild;
	init_tuple(&(jp->lt), jp->lchild->header->tuple_size, jp->lchild->header->num_attr, mmp);
	
	/* Get the first tuple from left relation, store the result. */	
	jp->lvalid = (db_uint8)next(jp->lchild, &(jp->lt), mmp);
	return 1;
}

/* Re-start the operator from the beginning. This assumes the operator has
 * been initialized. */
db_int rewind_ntjoin(ntjoin_t *jp, db_query_mm_t *mmp)
{
	rewind_dbop(jp->lchild, mmp);
	rewind_dbop(jp->rchild, mmp);
	jp->lvalid = (db_uint8)next(jp->lchild, &(jp->lt), mmp);
	return 1;
}

/* Find next tuple that passes the join condition.
   -This assumes that the evaluation tree will always reduce to either 0 or 1
    for all tuples. 
   -The algorithm used is the following:
		while lt is a valid tuple from lchild
			for each tuple rt remaining in rchild
				if rt joins lt
					return joined tuple
			lt <- next tuple from lchild
			rewind rchild
*/
db_int next_ntjoin(ntjoin_t *jp, tuple_t *next_tp, db_query_mm_t *mmp)
{
	/* Create necessary result variable. */
	db_int result = 0, retval;
	tuple_t rt;
	init_tuple(&rt, jp->rchild->header->tuple_size, jp->rchild->header->num_attr, mmp);
	
	/* Build arrays to be passed to eet evaluation engine. */
	relation_header_t *hpa[2];
	hpa[0] = jp->lchild->header;
	hpa[1] = jp->rchild->header;
	tuple_t *tpa[2];
	tpa[0] = &(jp->lt);
	tpa[1] = &rt;
	db_int looper;
	
	while (1 == jp->lvalid)
	{
		looper = next(jp->rchild, &rt, mmp);
		while (looper)
		{
			/* Evaluate join condition eet on this tuple, if it
			   exists.  If its NULL, all tuples pass (cross prod).
			*/
			if (jp->tree != NULL && jp->tree->nodes != NULL)
			{
				retval = evaluate_eet(jp->tree, &result, tpa, hpa, 0, mmp);
			}
			else
			{
				/* If there is no join condition, allow all
				   tuples to pass. */
				retval = 1;
				result = 1;
			}
			
			if (1 == result && 1 == retval)
			{
				/*** Do the join by combining the tuples
				 *   together. ***/
				/* Write out left tuples bytes into new tuple.
				 * */
				/* Write out left tuples isnull into new tuple.
				 * */
				db_int l_isnullsize = (db_int)(jp->lchild->header->num_attr) % 8 > 0
					? ((db_int)(jp->lchild->header->num_attr) / 8) + 1
					: ((db_int)(jp->lchild->header->num_attr) / 8);
				
				copytuplebytes(next_tp, &(jp->lt), 0, 0, jp->lchild->header->tuple_size);
				
				copytupleisnull(next_tp, &(jp->lt), 0, 0, l_isnullsize);
				
				/* Write out right tuples bytes directly after
				 * left tuples bytes. */
				copytuplebytes(next_tp, &rt, jp->lchild->header->tuple_size, 0, jp->rchild->header->tuple_size);
				
				/* Write out right tuples isnull into new tuple.
				 * */
				db_int i;
				db_int j = (db_int)(jp->lchild->header->num_attr);
				for (i = 0; i < (db_int)(jp->rchild->header->num_attr); ++i)
				{
					/* If this bit is 0 ... */
					if (0 == (rt.isnull[i/8] & (1 << (i % 8))))
					{
						next_tp->isnull[j/8] &= ~(1 << (j % 8));
					}
					else
					{
						next_tp->isnull[j/8] |= (1 << (j % 8));
					}
					j++;
				}
				close_tuple(&rt, mmp);
				return 1;
			}
			looper = next(jp->rchild, &rt, mmp);
		}
		if (0 != looper)	/* Cannot be 1 at this point. */
		{
			close_tuple(&rt, mmp);
			return next(jp->rchild, &rt, mmp);	/* Return error */
		}

		/* Get next tuple from operator below. */
		jp->lvalid = next(jp->lchild, &(jp->lt), mmp);

		if (jp->lvalid != 1 && jp->lvalid != 0)
		{
			close_tuple(&rt, mmp);
			return jp->lvalid;
		}
		else
		{
			rewind_dbop(jp->rchild, mmp);
		}
	}
	close_tuple(&rt, mmp);
	return jp->lvalid;
}

/* Close the selection operator. */
db_int close_ntjoin(ntjoin_t *jp, db_query_mm_t *mmp)
{
	close_tuple(&(jp->lt), mmp);
	/* Free all header properties that were allocated */
	DB_QMM_BFREE(mmp, jp->base.header->size_name);
	DB_QMM_BFREE(mmp, jp->base.header->names);
	DB_QMM_BFREE(mmp, jp->base.header->types);
	DB_QMM_BFREE(mmp, jp->base.header->offsets);
	DB_QMM_BFREE(mmp, jp->base.header->sizes);
	
	/* Free the header itself. */
	DB_QMM_BFREE(mmp, jp->base.header);
	
	/* For the time being, do nothing! */
	return 1;
}
