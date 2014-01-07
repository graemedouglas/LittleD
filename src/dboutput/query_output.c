/******************************************************************************/
/**
@file		query_output.c
@author		Graeme Douglas
@brief		Query result formatting implementation.
@see		For more information, refer to @ref query_output.h.
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

#include "query_output.h"

#if 0
void* calloc(int num, int size)
{
	unsigned char *toret = malloc(num*size);
	int i;
	if (NULL == toret) return NULL;
	for (i = 0; i < num*size; ++i)
		toret[i] = 0;
	
	return (void*)toret;
}
#endif

/* A small utility method for calculating the maximum widths of database
 integers. */
/**
@brief		Count the number of columns needed to represent a value.
@details	This includes both digits and a possible '-' sign.
@param		value		The value we are trying to represent.
				Set to DB_INT_MAX in order to get number
				of columns necessary.
@returns	The number of columns needed to represented @p value.
*/
db_int numdbintdigits(db_int value)
{
	db_int width = 1;
	while (value)
	{
		width++;
		value /= 10;
	}
	return width;
}

/* Get the size of the query. */
db_int sizeQuery(db_op_base_t *op, db_tuple_t *next_t, db_query_mm_t *mmp)
{
	db_int* widths = calloc((db_int) (op->header->num_attr), sizeof(db_int));
	computeWidths(op, widths);
	db_int i;
	int sizeRow = sizeOfRow(op, widths);
	
	db_int requiredMem = 1 + 3*sizeRow;	/* Null-byte + first row
						   separator, attribute names row,
						   last row separatr.*/
	for (i = 0; i < (db_int) op->header->num_attr; ++i)
	{
		requiredMem += widths[i];
	}

	while (1 == next(op, next_t, mmp))
	{
		requiredMem += 2*sizeRow; /* Row separator and actual row. */
	}
	free(widths);
	rewind_dbop(op, mmp);

	return requiredMem;
}

void printQuery(db_op_base_t *op, db_query_mm_t *mmp)
{
	char *output = formatQuery(op, mmp);
	printf("%s\n", output);
	free(output);
}

char* formatQuery(db_op_base_t *op, db_query_mm_t *mmp)
{
	db_tuple_t next_t;
	init_tuple(&next_t, op->header->tuple_size, op->header->num_attr, mmp);
	db_int i;
	db_int* widths = calloc((db_int) (op->header->num_attr), sizeof(db_int));
	computeWidths(op, widths);
	db_int requiredMem = sizeQuery(op, &next_t, mmp);
	char *out = calloc(requiredMem, sizeof(char));

	/** Print row separator. **/
	char *temp = formatRowSeparator(op, widths);
	strcat(out, temp);
	free(temp);
	/** For each attribute, print name. **/
	strcat(out, "|");
	for (i = 0; i < (db_int) op->header->num_attr; ++i)
	{
		/* Allocate enough space for the spaces, the text, and a null-byte. */
		char *tmp = calloc(widths[i] + 3, sizeof(char));
		
		/* If the name doesn't exist, print type. */
		if (op->header->names == NULL || op->header->names[i] == NULL
				|| op->header->size_name == NULL
				|| op->header->size_name[i] <= 0)
		{
			if ((db_uint8) DB_INT == op->header->types[i])
			{
				sprintf(tmp, " %*s ", widths[i], "INTEGER");
				strcat(out, tmp);
			}
			else if ((db_uint8) DB_STRING == op->header->types[i])
			{
				sprintf(tmp, " %*s(%d) ", widths[i]-numdbintdigits((db_int)op->header->sizes[i])-1, "STRING", (db_int)(op->header->sizes[i]) - 1);
				strcat(out, tmp);
			}
		}
		/* Otherwise, print real name. */
		else
		{
			sprintf(tmp, " %*s ", widths[i], op->header->names[i]);
			strcat(out, tmp);
		}
		free(tmp);
		strcat(out, "|");
	}
	strcat(out, "\n");
	/** Print row separator. **/
	temp = formatRowSeparator(op, widths);
	strcat(out, temp);
	free(temp);
	while (1 == next(op, &next_t, mmp))
	{
		temp = formatTuple(&next_t, op, widths);
		strcat(out, temp);
		free(temp);
		/** Print row separator. **/
		temp = formatRowSeparator(op, widths);
		strcat(out, temp);
		free(temp);
	}

	free(widths);
	close_tuple(&next_t, mmp);
	return out;
}

void printTuple(db_tuple_t *toprint, db_op_base_t *op, db_int *widths)
{
	printf("%s\n", formatTuple(toprint, op, widths));
}

char* formatTuple(db_tuple_t *toprint, db_op_base_t *op, db_int *widths)
{
	/* For each attribute, print it. */
	db_int i;
	db_int requiredMem = sizeOfRow(op, widths);
	char *out = calloc(requiredMem+1, sizeof(char));

	strcat(out, "|");

	for (i = 0; i < (db_int) op->header->num_attr; ++i)
	{
		char *tmp = calloc(widths[i]+3, sizeof(char));	/* 2 spaces, null-byte. */
		if (((toprint->isnull[i / 8] & (1 << (i % 8))) >> (i % 8)) == 1)
		{
			sprintf(tmp, " %*s ", widths[i], "NULL");
		}
		else if ((db_uint8) DB_INT == op->header->types[i])
		{
			sprintf(tmp, " %*d ", widths[i],
					getintbypos(toprint, i, op->header));
		}
		else if ((db_uint8) DB_STRING == op->header->types[i])
		{
			sprintf(tmp, " %*s ", widths[i],
					getstringbypos(toprint, i, op->header));
		}
		strcat(out, tmp);
		strcat(out, "|");
		free(tmp);
	}
	strcat(out, "\n");
	return out;
}

/* Print a formatted row separator. */
void printRowSeparator(db_op_base_t *op, db_int *widths)
{
	printf("%s\n", formatRowSeparator(op, widths));
}

/* Make a row separator. */
char* formatRowSeparator(db_op_base_t *op, db_int *widths)
{
	db_int i, j;
	db_int requiredMem = sizeOfRow(op, widths);
	char *out = calloc(requiredMem+1, sizeof(char));
	
	strcat(out, "+");
	for (i = 0; i < (db_int) op->header->num_attr; ++i)
	{
		for (j = 0; j < widths[i] + 2; ++j)
		{
			strcat(out, "-");
		}
		strcat(out, "+");
	}
	strcat(out, "\n");
	return out;
}

/* Find the size of a row separator ("+---+"). */
db_int sizeOfRow(db_op_base_t *op, db_int *widths)
{

	db_int i;
	/* First | or +, newline. */
	db_int requiredMem = 2;
	/* For each attribute... */
	for (i = 0; i < (db_int) op->header->num_attr; ++i)
	{
		/* Spaces on either side and pipe at end. */
		requiredMem += widths[i] + 3;
	}
	return requiredMem;
}

/* Compute the widths of each column to be outputed. */
void computeWidths(db_op_base_t *op, db_int *widths)
{
	db_int max_dbint_width = numdbintdigits(DB_INT_MAX);

	db_int i;
	for (i = 0; i < (db_int) op->header->num_attr; ++i)
	{
		/** First, calculate width of each attribute. **/
		if ((db_uint8) DB_STRING == op->header->types[i])
			widths[i] = (db_int) (op->header->sizes[i]) - 1;
		else
		{
			widths[i] = max_dbint_width;
		}

		/* If the columns name exists and it's name's size is greater
		   than the variables maximum width... */
		if (!(op->header->names == NULL || op->header->names[i] == NULL
				|| op->header->size_name == NULL
				|| op->header->size_name[i] <= 0)
				&& (db_int) (op->header->size_name[i]) > widths[i])
		{
			widths[i] = (db_int) (op->header->size_name[i]);
		}
		else
		{
			/* If the width is less than the number of characters
			   needed to write the the columns type... */
			if ((db_uint8) DB_INT == op->header->types[i])
			{
				if (7 > widths[i])
					widths[i] = 7;
			}
			else if ((db_uint8) DB_STRING == op->header->types[i])
			{
				db_int temp = 8 + max_dbint_width;
				if (temp > widths[i])
					widths[i] = temp;
			}
		}
		/* Since NULL is 4 characters long, no width can be < 4. */
		if (4 > widths[i])
			widths[i] = 4;
	}
}
