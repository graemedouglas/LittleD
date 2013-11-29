/******************************************************************************/
/**
@file		scan.c
@author		Graeme Douglas
@brief		The scan operator implementation.
@see		For all external function definitions, reference
		@ref scan.h.
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

#include "../dbstorage/dbstorage.h"
#include "scan.h"
#include "db_ops.h"

/* Initialize the scan operator. */
db_int init_scan(scan_t *sp, char* relationName, db_query_mm_t *mmp)
{
	if (sp == NULL || 1 != getrelationheader(&(sp->base.header), relationName, mmp))
		return -1;
	
	sp->base.type = DB_SCAN;
	
	sp->indexon = -1;
	
	sp->tuple_start = 1;
	int i;
	for (i = 0; i < (db_int)sp->base.header->num_attr; ++i)
	{
		sp->tuple_start += 4;	/* constant sized info */
		sp->tuple_start += (long)(sp->base.header->size_name[i]);
	}
	
	sp->relation = db_openreadfile(relationName);
	rewind_scan(sp, mmp);
	
	/* Build up index info. */
	char metaname[9+strlen(relationName)];
	sprintf(metaname, "DB_IDXM_%s", relationName);
	db_fileref_t idxmetafile = db_openreadfile(metaname);
	
	if (DB_STORAGE_NOFILE == idxmetafile)
	{
		sp->idx_meta_data.num_idx = 0;
		return 1;
	}
	
	db_fileread(idxmetafile, &(sp->idx_meta_data.num_idx), sizeof(db_uint8));
	sp->idx_meta_data.len_names = DB_QMM_BALLOC(mmp, sizeof(db_uint8)*(sp->idx_meta_data.num_idx));
	sp->idx_meta_data.names = DB_QMM_BALLOC(mmp, sizeof(char*)*(sp->idx_meta_data.num_idx));
	sp->idx_meta_data.num_expr = DB_QMM_BALLOC(mmp, sizeof(db_uint8)*(sp->idx_meta_data.num_idx));
	sp->idx_meta_data.exprs = DB_QMM_BALLOC(mmp, sizeof(db_eet_t*)*(sp->idx_meta_data.num_idx));
	
	int j;
	for (i = 0; i < sp->idx_meta_data.num_idx; ++i)
	{
		// TODO: Make sure all pointers passed to read function make sense.
		db_fileread(idxmetafile, &(sp->idx_meta_data.len_names[i]), sizeof(db_uint8));
		sp->idx_meta_data.names[i] = DB_QMM_BALLOC(mmp, sp->idx_meta_data.len_names[i]);
		db_fileread(idxmetafile, (unsigned char*)(sp->idx_meta_data.names[i]), sp->idx_meta_data.len_names[i]);
		db_fileread(idxmetafile, &(sp->idx_meta_data.num_expr[i]), sizeof(db_uint8));
		sp->idx_meta_data.exprs[i] = DB_QMM_BALLOC(mmp, sizeof(db_eet_t)*(sp->idx_meta_data.num_expr[i]));
		db_fileread(idxmetafile, (unsigned char*)(sp->idx_meta_data.exprs[i]), sizeof(db_eet_t)*(sp->idx_meta_data.num_expr[i]));
		
		for (j = 0; j < sp->idx_meta_data.num_expr[i]; ++j)
		{
			sp->idx_meta_data.exprs[i][j].nodes = DB_QMM_BALLOC(mmp, (sp->idx_meta_data.exprs[i][j].size));
			db_fileread(idxmetafile, (unsigned char*)(sp->idx_meta_data.exprs[i][j].nodes), (sp->idx_meta_data.exprs[i][j].size));
		}
	}
	
	db_fileclose(idxmetafile);
	
	return 1;
}

/* This method assumes the scan is already initialized */
db_int rewind_scan(scan_t *sp, db_query_mm_t *mmp)
{
	/* Go to beginning, skip over header information. */
	db_filerewind(sp->relation);
	db_fileseek(sp->relation, sp->tuple_start);
	return 1;
}

/* Retrieve the next tuple from the relation. */
db_int next_scan(scan_t *sp, tuple_t *next_tp, db_query_mm_t *mmp)
{
	db_int bit_arr_size = ((db_int)(sp->base.header->num_attr)) / 8;
	if (((db_int)(sp->base.header->num_attr)) % 8 > 0)
	{
		bit_arr_size++;
	}
	if (bit_arr_size == db_fileread(sp->relation, (unsigned char*)next_tp->isnull, SIZE_BYTE * bit_arr_size))
	{
		if ((size_t)(sp->base.header->tuple_size) == db_fileread(sp->relation, (unsigned char*)next_tp->bytes, SIZE_BYTE * (db_int)(sp->base.header->tuple_size)))
		{
			// TODO: Right now, this assumes we only use ints for index.
			/* Only check stop condition if it exists. */
			if (sp->indexon > -1)
			{
				db_int val;
				val = getintbypos(next_tp, sp->indexon, sp->base.header);
				if (val >= sp->stopat)	return 0;
			}
			#if 0
			#endif
			
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/* Close the operator. */
void close_scan(scan_t *sp, db_query_mm_t *mmp)
{
	/* Close the file stream. */
	db_fileclose(sp->relation);
	
	freerelationheader(sp->base.header, mmp);
	
	/* Free indexing information, if necessary. */
	if (sp->idx_meta_data.num_idx > 0)
	{
		int i, j;
		for (i = 0; i < (db_int)sp->idx_meta_data.num_idx; i++)
		{
			DB_QMM_BFREE(mmp, sp->idx_meta_data.names[i]);
			for (j = 0; j < (db_int)sp->idx_meta_data.num_expr[i]; j++)
				DB_QMM_BFREE(mmp, sp->idx_meta_data.exprs[i][j].nodes);
			DB_QMM_BFREE(mmp, sp->idx_meta_data.exprs[i]);
		}
		DB_QMM_BFREE(mmp, sp->idx_meta_data.len_names);
		DB_QMM_BFREE(mmp, sp->idx_meta_data.names);
		DB_QMM_BFREE(mmp, sp->idx_meta_data.num_expr);
		DB_QMM_BFREE(mmp, sp->idx_meta_data.exprs);
	}
}
