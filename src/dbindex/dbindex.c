/******************************************************************************/
/**
@file		dbindex.c
@author		Graeme Douglas
@brief		A generic relation index interface.
@see		For more information, please refer to @ref dbindex.h and
		@ref dbindex_types.h.
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

#include "dbindex.h"

db_int init_index(db_index_t *indexp, char *name)
{
	/* Prepare for ugly. */
	char realname[8 + strlen(name)];
	sprintf(realname, "DB_IDX_%s", name);
	
	indexp->indexref = db_openreadfile(realname);
	if (DB_STORAGE_NOFILE == indexp->indexref || 1!=db_fileread(indexp->indexref, &(indexp->type), 1))
	{
		return 0;
	}
	
	return 1;
}

db_int close_index(db_index_t *indexp)
{
	return db_fileclose(indexp->indexref);
}

db_int scan_find(scan_t *sp, db_uint8 indexon, db_eet_t *searchfor,
		db_tuple_t *comparator_tp, relation_header_t *comparator_hp,
		db_query_mm_t *mmp)
{
	db_index_offset_t offset =
				db_index_getoffset(sp, indexon,
					searchfor,
					comparator_tp,
					comparator_hp,
					mmp);
	
	if (-1 >= offset)
	{
		return 0;
	}
	else
	{
		db_filerewind(sp->relation);
		db_fileseek(sp->relation, offset);
		return 1;
	}
}

// FIXME: For now, this assumes equality for each of the expressions.
db_index_offset_t db_index_getoffset(scan_t *sp, db_uint8 indexon,
				db_eet_t *searchfor,
				db_tuple_t *comparator_tp,
				relation_header_t *comparator_hp,
				db_query_mm_t *mmp)
{
	if (sp->idx_meta_data.num_idx <= indexon)
		return -1;
	
	db_index_t index;
	if (1!=init_index(&index, sp->idx_meta_data.names[indexon]))
	{
		return -1;
	}
	
	if (DB_INDEX_TYPE_INLINE == index.type)
	{
		long first = sp->tuple_start;
		size_t total_size = sp->base.header->tuple_size + (sp->base.header->num_attr / 8);
		total_size += (sp->base.header->num_attr) % 8 > 0 ? 1 : 0;
		long last;
		if (sizeof(long)!=db_fileread(index.indexref, (unsigned char*)&(last), sizeof(long)))
		{
			return -1;
		}
		if (last > 0)
			last = first + (total_size * (last - 1));
		else
			last = first;
		long imin  = 0;
		long imax  = (last - first) / total_size;
		long imid;
		
		db_uint8 order[sp->idx_meta_data.num_expr[indexon]];
		int result;
		for (result = 0; result < sp->idx_meta_data.num_expr[indexon]; ++result)
			order[result] = DB_TUPLE_ORDER_ASC;
		
		db_tuple_t temp;
		init_tuple(&temp, sp->base.header->tuple_size, sp->base.header->num_attr, mmp);
		rewind_scan(sp, mmp);
		
		long i = -1;
		
		/* We binary search on expressions for first occurence. */
		while (imin <= imax)
		{
			imid = imin + ((imax - imin) / 2);
			
			db_filerewind(sp->relation);
			db_fileseek(sp->relation, (imid*(total_size))+first);
			next_scan(sp, &temp, mmp);
			
			/* arr[imid], key */
			if (NULL == comparator_hp)	/* FIXME: quick hack to let indexed scans work. */
			{
				result = getintbypos(&temp, ((db_int)searchfor), sp->base.header) - ((db_int)comparator_tp);
			}
			else
				result = cmp_tuple(&temp, comparator_tp,
					sp->base.header, comparator_hp,
					sp->idx_meta_data.exprs[indexon],
					searchfor,
					sp->idx_meta_data.num_expr[indexon],
					order, 1, mmp);
			
			if (result < 0)
				imin = imid + 1;
			else if (result > 0)
				imax = imid - 1;
			else if (imin != imid)
				imax = imid;
			else
			{
				i = imid;
				break;
			}
				
		}
		
		if (i <= -1)	i = imin;
		i = (first + (i*total_size));
		
		db_filerewind(sp->relation);
		db_fileseek(sp->relation, i);
		next_scan(sp, &temp, mmp);
		
		/* FIXME: quick hack to let indexed scans work. (first part of the condition) */
		if (NULL != comparator_hp && 0!=cmp_tuple(&temp, comparator_tp,
				sp->base.header, comparator_hp,
				sp->idx_meta_data.exprs[indexon],
				searchfor,
				sp->idx_meta_data.num_expr[indexon],
				order, 1, mmp))
			i = -1;
		
		close_tuple(&temp, mmp);
		close_index(&index);
		
		return i;
	}
	
	return -1;
}
