/**
@author		Graeme Douglas
@brief
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

#include <stdio.h>
#include <stdlib.h>
#include "../CuTest.h"
#include "../../ref.h"
#include "../../dbmacros.h"
#include "../../dbmm/db_query_mm.h"

void test_dbqmm_1(CuTest *tc)
{
	// General Variable declaration.
	char mem_seg1000[1000];
	db_query_mm_t qmm;
	db_int size;
	db_int intResult;
	db_int prefix_sum = 0;	// Used as a convenient size sum.  Trying to avoid total ugliness.
	void *ptr = NULL;
	void *ptr2 = NULL;
	void *ptr3 = NULL;
	
	puts("***************************************************************************");
	puts("Test 1: Memory segment = 1000 bytes. Test only front allocation.");
	// Initialize the query memory manager.
	size = 1000;
	init_query_mm(&qmm, mem_seg1000, size);
	
	// Verify memory addresses initialized properly.
	printf("Address of the memory segment is: %p\n", mem_seg1000);
	CuAssertTrue(tc, qmm.next_front == mem_seg1000);
	CuAssertTrue(tc, qmm.last_back == (mem_seg1000 + size));
	
	prefix_sum += sizeof(db_int);
	
	// Allocate 100 bytes, verify it was allocated correctly.
	ptr = db_qmm_falloc(&qmm, 100);
	CuAssertTrue(tc, DB_QMM_SIZEOF_FTOP(&qmm) == 100);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.next_front: %p\n", qmm.next_front);
	CuAssertTrue(tc, ptr != NULL);
	//printf("Difference between addresses: %d\n", (db_int)(((char*)ptr)-mem_seg1000));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-mem_seg1000) == prefix_sum);
	prefix_sum += (100 + sizeof(void*));
	CuAssertTrue(tc, (qmm.next_front) == (mem_seg1000 + prefix_sum));
	//printf("Address of beginning of node: %p\n", (((char*)ptr)-sizeof(int)));
	//printf("Address of beginning of node: %p\n", *((void**)(((char*)(qmm.next_front)-sizeof(void*)))));
	CuAssertTrue(tc, *((void**)(((char*)(qmm.next_front)-sizeof(void*)))) == (((char*)ptr)-sizeof(int)));
	// Write out data to the array.
	int i;
	for (i = 0; i < 100; i++)
	{
		((db_uint8*)ptr)[i] = 1;
	}
	ptr2 = ptr;
	
	// Allocate 20 bytes, verify it was allocated correctly.
	ptr = db_qmm_falloc(&qmm, 20);
	CuAssertTrue(tc, DB_QMM_SIZEOF_FTOP(&qmm) == 20);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.next_front: %p\n", qmm.next_front);
	CuAssertTrue(tc, ptr != NULL);
	CuAssertTrue(tc, (db_int)(((char*)ptr)-mem_seg1000) == prefix_sum + sizeof(db_int));
	prefix_sum += (20 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, (qmm.next_front) == (mem_seg1000 + prefix_sum));
	CuAssertTrue(tc, *((void**)(((char*)(qmm.next_front)-sizeof(void*)))) == (((char*)ptr)-sizeof(int)));;
	// Write out data to the array.
	for (i = 0; i < 20; i++)
	{
		((db_uint8*)ptr)[i] = 113;
	}
	// Check that previous allocated data still good.
	for (i = 0; i < 100; i++)
	{
		CuAssertTrue(tc, ((db_uint8*)ptr2)[i] == 1);
	}
	ptr2 = ptr;
	
	// Allocate 35 bytes, verify it was allocated correctly.
	ptr = db_qmm_falloc(&qmm, 35);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.next_front: %p\n", qmm.next_front);
	CuAssertTrue(tc, ptr != NULL);
	CuAssertTrue(tc, (db_int)(((char*)ptr)-mem_seg1000) == prefix_sum + sizeof(db_int));
	prefix_sum += (35 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, (qmm.next_front) == (mem_seg1000 + prefix_sum));
	CuAssertTrue(tc, *((void**)(((char*)(qmm.next_front)-sizeof(void*)))) == (((char*)ptr)-sizeof(int)));
	// Write out data to the array.
	for (i = 0; i < 35; i++)
	{
		((db_uint8*)ptr)[i] = 76;
	}
	// Check that previous allocated data still good.
	for (i = 0; i < 20; i++)
	{
		CuAssertTrue(tc, ((db_uint8*)ptr2)[i] == 113);
	}
	ptr2 = ptr;
	
	// Allocate 0 bytes, verify it was allocated correctly.
	ptr = db_qmm_falloc(&qmm, 0);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.next_front: %p\n", qmm.next_front);
	CuAssertTrue(tc, ptr != NULL);
	CuAssertTrue(tc, (db_int)(((char*)ptr)-mem_seg1000) == prefix_sum + sizeof(db_int));
	prefix_sum += (0 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, (qmm.next_front) == (mem_seg1000 + prefix_sum));
	CuAssertTrue(tc, *((void**)(((char*)(qmm.next_front)-sizeof(void*)))) == (((char*)ptr)-sizeof(int)));
	// Check that previous allocated data still good.
	for (i = 0; i < 35; i++)
	{
		CuAssertTrue(tc, ((db_uint8*)ptr2)[i] == 76);
	}
	ptr2 = ptr;
	
	// Free memory in order we declared it.
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= (0 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, qmm.next_front == (((char*)(qmm.segment))+prefix_sum));
	
	// Set ptr back to previous allotment.  Ignore the fact that this is really not ideal. :D
	ptr = ((void*)(((char*)ptr) - 35 - sizeof(db_int) - sizeof(void*)));
	
	// (Continue to) free memory in order we declared it.
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= (35 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, qmm.next_front == (((char*)(qmm.segment))+prefix_sum));
	
	// Set ptr back to previous allotment.
	ptr = ((void*)(((char*)ptr) - 20 - sizeof(db_int) - sizeof(void*)));
	
	// (Continue to) free memory in order we declared it.
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= (20 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, qmm.next_front == (((char*)(qmm.segment))+prefix_sum));
	
	// Set ptr back to previous allotment.
	ptr = ((void*)(((char*)ptr) - 100 - sizeof(db_int) - sizeof(void*)));
	
	// (Continue to) free memory in order we declared it.
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= (100 + sizeof(void*) + sizeof(db_int));
	CuAssertTrue(tc, qmm.next_front == (((char*)(qmm.segment))+prefix_sum));
	
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Attempt to allocate more memory than possible (just by 1).
	ptr = db_qmm_falloc(&qmm, 1000-sizeof(db_int)-sizeof(void*)+1);
	CuAssertTrue(tc, ptr == NULL);
	CuAssertTrue(tc, -1 == qmm.errcode);
	
	// Allocate the maximum amount of memory, then deallocate it.
	ptr = db_qmm_falloc(&qmm, 1000-sizeof(db_int)-sizeof(void*));
	CuAssertTrue(tc, ptr != NULL);
	CuAssertTrue(tc, NULL == db_qmm_falloc(&qmm, 1));
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	//// Test fragmentation handling logic.
	// Allocate three seperate sections of memory.
	ptr = db_qmm_falloc(&qmm, 128);
	ptr2 = db_qmm_falloc(&qmm, 16);
	ptr3 = db_qmm_falloc(&qmm, 256);
	
	// Free ptr2, then ptr, then ptr3.
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory.
	ptr = db_qmm_falloc(&qmm, 127);
	ptr2 = db_qmm_falloc(&qmm, 13);
	ptr3 = db_qmm_falloc(&qmm, 259);
	
	// Free ptr, then ptr2, then ptr3.
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory.
	ptr = db_qmm_falloc(&qmm, 500);
	ptr2 = db_qmm_falloc(&qmm, 41);
	ptr3 = db_qmm_falloc(&qmm, 96);
	
	// Free ptr2, then ptr3, then ptr.
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory.
	ptr = db_qmm_falloc(&qmm, 1);
	ptr2 = db_qmm_falloc(&qmm, 1);
	ptr3 = db_qmm_falloc(&qmm, 1);
	
	// Free ptr3, then ptr, then ptr2.
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory, extending the first one.
	ptr = db_qmm_falloc(&qmm, 3);
	CuAssertTrue(tc, 1 == db_qmm_fextend(&qmm, 3));
	printf("qmm.next_front - qmm.segment: %ld\n", ((char*)qmm.next_front) - ((char*)qmm.segment));
	printf("first allocated size: %d\n", *(db_int*)(qmm.segment));
	fflush(stdout);
	ptr2 = db_qmm_falloc(&qmm, 29);
	ptr3 = db_qmm_falloc(&qmm, 17);
	
	// Free ptr3, then ptr, then ptr2.
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory, extending the second one.
	ptr = db_qmm_falloc(&qmm, 3);
	ptr2 = db_qmm_falloc(&qmm, 29);
	CuAssertTrue(tc, 1 == db_qmm_fextend(&qmm, 80));
	ptr3 = db_qmm_falloc(&qmm, 17);
	
	// Free ptr3, then ptr, then ptr2.
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory, extending the second one.
	ptr = db_qmm_falloc(&qmm, 3);
	ptr2 = db_qmm_falloc(&qmm, 29);
	ptr3 = db_qmm_falloc(&qmm, 17);
	CuAssertTrue(tc, 1 == db_qmm_fextend(&qmm, 110));
	
	// Free ptr3, then ptr, then ptr2.
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	
	// Allocate three seperate sections of memory, extending too far..
	ptr = db_qmm_falloc(&qmm, 3);
	CuAssertTrue(tc, -1 == db_qmm_fextend(&qmm, 3000));
	ptr2 = db_qmm_falloc(&qmm, 29);
	ptr3 = db_qmm_falloc(&qmm, 17);
	
	// Free ptr3, then ptr, then ptr2.
	intResult = db_qmm_ffree(&qmm, ptr3);
	CuAssertTrue(tc, 1 == intResult);
	
	intResult = db_qmm_ffree(&qmm, ptr);
	CuAssertTrue(tc, 0 == intResult);
	CuAssertTrue(tc, -3 == qmm.errcode);
	
	intResult = db_qmm_ffree(&qmm, ptr2);
	CuAssertTrue(tc, 1 == intResult);
	CuAssertTrue(tc, qmm.next_front == qmm.segment);
	puts("***************************************************************************");
}

void test_dbqmm_2(CuTest *tc)
{
	// General Variable declaration.
	char mem_seg3000[3000];
	db_query_mm_t qmm;
	db_int size;
	db_int intResult;
	db_int prefix_sum = 0;	// Used as a convenient size sum.  Trying to avoid total ugliness.
	void *ptr = NULL;
	void *ptr2 = NULL;
	void *ptr3 = NULL;
	
	puts("***************************************************************************");
	puts("Test 2: Memory segment = 3000 bytes. Test only back allocation.");
	size = 3000;
	init_query_mm(&qmm, mem_seg3000, size);
	prefix_sum = 0;
	
	// Verify memory addresses initialized properly.
	printf("Address of the memory segment is: %p\n", mem_seg3000);
	CuAssertTrue(tc, qmm.next_front == mem_seg3000);
	CuAssertTrue(tc, qmm.last_back == (mem_seg3000 + size));
	
	/// Allocate 3 chunks for consumption.
	ptr = db_qmm_balloc(&qmm, 1000);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.last_back: %p\n", qmm.last_back);
	CuAssertTrue(tc, ptr != NULL);
	prefix_sum = 1000 + sizeof(db_int);
	
	printf("Difference between addresses: %d\n", (db_int)(((char*)ptr)-mem_seg3000));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-(mem_seg3000)) == size - prefix_sum + sizeof(db_int));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-(((char*)qmm.last_back))) == sizeof(db_int));
	
	ptr = db_qmm_balloc(&qmm, 100);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.last_back: %p\n", qmm.last_back);
	CuAssertTrue(tc, ptr != NULL);
	prefix_sum += 100 + sizeof(db_int);
	
	printf("Difference between addresses: %d\n", (db_int)(((char*)ptr)-mem_seg3000));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-(mem_seg3000)) == size - prefix_sum + sizeof(db_int));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-(((char*)qmm.last_back))) == sizeof(db_int));
	
	ptr = db_qmm_balloc(&qmm, 10);
	printf("Address of ptr: %p\n", ptr);
	printf("Address of qmm.last_back: %p\n", qmm.last_back);
	CuAssertTrue(tc, ptr != NULL);
	prefix_sum += 10 + sizeof(db_int);
	
	printf("Difference between addresses: %d\n", (db_int)(((char*)ptr)-mem_seg3000));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-(mem_seg3000)) == size - prefix_sum + sizeof(db_int));
	CuAssertTrue(tc, (db_int)(((char*)ptr)-(((char*)qmm.last_back))) == sizeof(db_int));
	
	/// Start popping them off in stack order.
	intResult = db_qmm_bfree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= 10 + sizeof(db_int);
	
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, (db_int)(((char*)(qmm.last_back))-(mem_seg3000)) == size - prefix_sum);
	
	ptr = ((void*)(((char*)ptr)+10+sizeof(db_int)));
	intResult = db_qmm_bfree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= 100 + sizeof(db_int);
	
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, (db_int)(((char*)(qmm.last_back))-(mem_seg3000)) == size - prefix_sum);
	
	ptr = ((void*)(((char*)ptr)+100+sizeof(db_int)));
	intResult = db_qmm_bfree(&qmm, ptr);
	CuAssertTrue(tc, 1 == intResult);
	prefix_sum -= 1000 + sizeof(db_int);
	
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, (db_int)(((char*)(qmm.last_back))-(mem_seg3000)) == size - prefix_sum);
	CuAssertTrue(tc, qmm.last_back == ((void*)(((char*)qmm.segment + qmm.size))));
	
	/// Test boundaries and limits.
	CuAssertTrue(tc, qmm.last_back - qmm.segment == 3000);
	CuAssertTrue(tc, NULL == db_qmm_balloc(&qmm, 3000 - sizeof(db_int) + 1));
	CuAssertTrue(tc, qmm.last_back - qmm.segment == 3000);
	CuAssertTrue(tc, NULL != (ptr = db_qmm_balloc(&qmm, 3000 - sizeof(db_int))));
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr));
	//// Test fragmentation handling.
	// Allocate thrice.
	ptr = db_qmm_balloc(&qmm, 654);
	ptr2 = db_qmm_balloc(&qmm, 999);
	ptr3 = db_qmm_balloc(&qmm, 3);
	
	// Deallocate ptr2, than ptr, than ptr3.
	CuAssertTrue(tc, 0 == db_qmm_bfree(&qmm, ptr2));
	CuAssertTrue(tc, -4 == qmm.errcode);
	CuAssertTrue(tc, 0 == db_qmm_bfree(&qmm, ptr));
	CuAssertTrue(tc, -4 == qmm.errcode);
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr3));
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, qmm.last_back == ((void*)((((char*)(qmm.segment)) + qmm.size))));
	
	ptr = db_qmm_balloc(&qmm, 654);
	ptr2 = db_qmm_balloc(&qmm, 999);
	ptr3 = db_qmm_balloc(&qmm, 3);
	
	// Deallocate ptr, than ptr2, than ptr3.
	CuAssertTrue(tc, 0 == db_qmm_bfree(&qmm, ptr));
	CuAssertTrue(tc, -4 == qmm.errcode);
	CuAssertTrue(tc, 0 == db_qmm_bfree(&qmm, ptr2));
	CuAssertTrue(tc, -4 == qmm.errcode);
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr3));
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, qmm.last_back == ((void*)((((char*)(qmm.segment)) + qmm.size))));
	
	ptr = db_qmm_balloc(&qmm, 654);
	ptr2 = db_qmm_balloc(&qmm, 999);
	ptr3 = db_qmm_balloc(&qmm, 3);
	
	// Deallocate ptr3, than ptr, than ptr2.
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr3));
	CuAssertTrue(tc, 0 == db_qmm_bfree(&qmm, ptr));
	CuAssertTrue(tc, -4 == qmm.errcode);
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr2));
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, qmm.last_back == ((void*)((((char*)(qmm.segment)) + qmm.size))));

	ptr = db_qmm_balloc(&qmm, 650);
	ptr = db_qmm_bextend(&qmm, 4);
	ptr2 = db_qmm_balloc(&qmm, 999);
	ptr3 = db_qmm_balloc(&qmm, 3);
	
	// Deallocate ptr3, than ptr, than ptr2.
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr3));
	CuAssertTrue(tc, 0 == db_qmm_bfree(&qmm, ptr));
	CuAssertTrue(tc, -4 == qmm.errcode);
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr2));
	printf("Difference between addresses: %d\n", (db_int)(((char*)(qmm.last_back))-mem_seg3000));
	CuAssertTrue(tc, qmm.last_back == ((void*)((((char*)(qmm.segment)) + qmm.size))));
	puts("***************************************************************************");

}

void test_dbqmm_3(CuTest *tc)
{
	// General Variable declaration.
	char mem_seg3000[3000];
	db_query_mm_t qmm;
	db_int size;
	void *ptr = NULL;
	void *ptr2 = NULL;
	void *ptr3 = NULL;
	
	puts("***************************************************************************");
	puts("Test 3: Memory segment = 3000 bytes. Test collisions when allocating from\nboth front and back.");
	size = 3000;
	init_query_mm(&qmm, mem_seg3000, size);
	ptr = db_qmm_falloc(&qmm, 2000);
	CuAssertTrue(tc, NULL != ptr);
	ptr2 = db_qmm_balloc(&qmm, 100);
	CuAssertTrue(tc, NULL != ptr2);
	ptr3 = db_qmm_falloc(&qmm, 1000);
	CuAssertTrue(tc, NULL == ptr3);
	CuAssertTrue(tc, -1 == qmm.errcode);
	qmm.errcode = 0;
	ptr3 = ptr2;
	ptr3 = db_qmm_balloc(&qmm, 1000);
	CuAssertTrue(tc, NULL == ptr3);
	CuAssertTrue(tc, -1 == qmm.errcode);
	CuAssertTrue(tc, 1 == db_qmm_ffree(&qmm, ptr));
	ptr3 = db_qmm_falloc(&qmm, 1000);
	CuAssertTrue(tc, NULL != ptr3);
	CuAssertTrue(tc, 1 == db_qmm_bfree(&qmm, ptr2));
	CuAssertTrue(tc, 1 == db_qmm_ffree(&qmm, ptr3));
	puts("***************************************************************************");
}

CuSuite *DBQueryMMGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, test_dbqmm_1);
	SUITE_ADD_TEST(suite, test_dbqmm_2);
	SUITE_ADD_TEST(suite, test_dbqmm_3);
	
	return suite;
}

void runAllTests_dbqmm()
{
	CuString *output = CuStringNew();
	CuSuite* suite = DBQueryMMGetSuite();
	
	//CuSuiteAddSuite(suite, DBQueryMMGetSuite());
	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}
