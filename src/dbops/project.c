/******************************************************************************/
/**
@file		project.c
@author		Graeme Douglas
@brief		Implementation of the projection operator.
@see		For external method documentation, @ref project.h.
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
#include "project.h"
#include "db_ops.h"

/* Initialize the projection operator. */
db_int init_project(project_t *pp, db_op_base_t *childop_p, db_eet_t* exprs,
		db_uint8 num_exprs, db_query_mm_t *mmp)
{
	pp->base.type = DB_PROJECT;
	pp->exprs = exprs;
	pp->num_exprs = num_exprs;
	
	pp->base.header = DB_QMM_BALLOC(mmp, sizeof(relation_header_t));
	db_int result = createnewheader(pp->base.header,
		childop_p->header, exprs, num_exprs, mmp);
	pp->child = childop_p;
	return result;
}

/* This method assumes the scan is already initialized */
db_int rewind_project(project_t *pp, db_query_mm_t *mmp)
{
	rewind_dbop(pp->child, mmp);
	return 1;
}

/* Retrieve next tuple. 
   This assumes a properly initialized tuple with apropriate number of bytes
   is passed in.
*/
db_int next_project(project_t *pp, tuple_t *next_tp, db_query_mm_t *mmp)
{
	tuple_t temp_t;
	init_tuple(&temp_t, pp->child->header->tuple_size, pp->child->header->num_attr, mmp);
	
	/* Get the next tuple from the child */
	db_int result = next(pp->child, &temp_t, mmp);
	if (1 != result)
	{
		close_tuple(&temp_t, mmp);
		return result;
	}
	else
	{
		/* For each byte in the old tuple, if it belongs to an
		 * attribute in the new tuple, copy it.
		 */
		db_int i = 0, k = 0;	/* k for attributes, i for expressions. */
		for (; i < (db_int)(pp->num_exprs); ++k, ++i)
		{
			if (NULL == pp->exprs[i].nodes)
			{
				db_int j_a, j_bytes;
				for (j_a = pp->exprs[i].stack_size; j_a < pp->exprs[i].stack_size + pp->exprs[i].size; ++j_a)
				{
					for (j_bytes = 0; j_bytes < (db_int)(pp->base.header->sizes[k]); ++j_bytes)
					{
						next_tp->bytes[pp->base.header->offsets[k] + j_bytes] =
							temp_t.bytes[(db_int)(pp->child->header->offsets[j_a]) + j_bytes];
					}
					
					if (1==(((temp_t.isnull[j_a/8]) & (1 << (j_a % 8))) >> (j_a % 8)))
					{
						next_tp->isnull[(k / 8)] |= (1 << (k % 8));
					}
					else
					{
						next_tp->isnull[(k / 8)] &= ~(1 << (k % 8));
					}
					++k;
				}
				
				/* Since we increment k below, must decrement now. */
				k--;
				
			}
			else if (pp->base.header->types[k] == DB_INT)
			{
				db_int value;
				tuple_t *temp_tp = &temp_t;
				switch (evaluate_eet(&(pp->exprs[i]), &value, &temp_tp,
					&(pp->child->header), 0, mmp))
				{
					case 2:
						/* Set this attributes isnull bit to 1. */
						next_tp->isnull[(k / 8)] |= (1 << (k % 8));
						break;
					case 1:
						/* Make sure this attributes isnull bit is 0. */
						next_tp->isnull[(k / 8)] &= ~(1 << (k % 8));
						*((db_int*)(&next_tp->bytes[pp->base.header->offsets[k]])) =
							value;
						break;
					case -1:
					default:
						close_tuple(&temp_t, mmp);
						return -1;
				}
			}
			else if (pp->base.header->types[k] == DB_STRING)
			{
				db_int j;
				char *value;
				tuple_t *temp_tp = &temp_t;
				switch (evaluate_eet(&(pp->exprs[i]), &value, &temp_tp,
					&(pp->child->header), 0, mmp))
				{
					case 2:
						/* Set this attributes isnull bit to 1. */
						next_tp->isnull[(k / 8)] |= (1 << (k % 8));
						break;
					case 1:
						/* Make sure this attributes isnull bit is 0. */
						next_tp->isnull[(k / 8)] &= ~(1 << (k % 8));
						for (j = 0; j < strlen(value); j++)
						{
							next_tp->bytes[pp->base.header->offsets[k] + j] = (value[j]);
						}
						next_tp->bytes[pp->base.header->offsets[k] + strlen(value)] = '\0';
						break;
					case -1:
					default:
						close_tuple(&temp_t, mmp);
						return -1;
				}
			}
			/* TODO: Implement future types here. */
		}
		close_tuple(&temp_t, mmp);
		return 1;
	}
}

/* Close the projection operator. */
void close_project(project_t *pp, db_query_mm_t *mmp)
{
	/* Free up header's properties */
	DB_QMM_BFREE(mmp, pp->base.header->size_name);
	DB_QMM_BFREE(mmp, pp->base.header->names);
	DB_QMM_BFREE(mmp, pp->base.header->types);
	DB_QMM_BFREE(mmp, pp->base.header->offsets);
	DB_QMM_BFREE(mmp, pp->base.header->sizes);
	
	/* Free up header */
	DB_QMM_BFREE(mmp, pp->base.header);
}
