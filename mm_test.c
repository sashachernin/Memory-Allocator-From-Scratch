/* Assignment 5
 * Operating Systems
 * Kinneret College on the Sea of Galilee
 * Semester 1 5781
 * 
 * Submitted by: Alexandr Chernin & Shimon Yaish
 *
 * mm_test.c
 *
 * Testing file for memory allocation and deallocation.
 */

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MM_GLIBC_ALLOC
#include "mm_alloc.h"
#endif

/*theres not much to comment. with the functions below we are testsing the mm_malloc, mm_free and mm_realloc functions.*/

/*the testing flow: recieving memory -> writing to it -> reading it -> realloc it -> reading it and checking that the data wasnt destroyed -> freeing memory*/


/*
Additional information: In case of allocation, a writing and reading test will be done. If reading test is successful, 'done' will be written next to the allocation attempt, and 'failed' otherwise. If writing test is successful, 'ok' will be written next to the allocation attempt, and 'fail' otherwise. In case of Reallocation, a reading test will be done, to check if the content is preserved, if successful, 'ok' will be written next to the reallocation attempt. 'fail' otherwise"
*/

void basic() {
	puts("-- basic() --");
	
	printf("Allocating 100 bytes... ");
	char* text = (char*)mm_malloc(100);
	if (text)
		puts("ok");
	else
		puts("fail");
	
	printf("Writing \"Flesh and the Power it holds\" into allocation... ");
	strcpy(text, "Flesh and the Power it holds");
	if (strcmp(text, "Flesh and the Power it holds") == 0)
		puts("ok");
	else
		puts("fail");
	
	printf("Freeing the allocation... ");
	mm_free(text);
	puts("ok");
	
	printf("Trying to allocate 0 bytes... ");
	if (mm_malloc(0) == NULL) 
		puts("ok (NULL)");
	else
		puts("fail (not-NULL)");
	
	printf("Trying to free(NULL)... ");
	mm_free(NULL);
	puts("ok");
	
	printf("Reallocating from 0... ");
	void* ptr = NULL;
	ptr = mm_realloc(ptr, 8);
	if (ptr)
		puts("ok");
	else
		puts("fail");
	
	printf("Reallocating to 0... ");
	mm_realloc(ptr, 0);
	puts("ok");
	
	puts("-- end basic() --");
	
	return;
}

void secondTest() {
	int malloc_flag=0, realloc_flag=0;
	puts("-- secondTest() --");
	
	printf("Allocating 10 times... \n");
	char* textArray[10];
	for (int i = 0; i<10; i++){
		textArray[i] = mm_malloc(64);
		printf("%i ", i);
		if (textArray[i])
			printf("done ");
		else
			{printf("failed "); malloc_flag++;}
		strcpy(textArray[i], "Sample text");
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("ok \n");
		else
			{printf("fail \n"); malloc_flag++;}
	}
	if (malloc_flag==0) 
		puts("Allocated 10 times successfully");
	else 
		puts("Allocating 10 times failed");
	
	malloc_flag=0;
	
	printf("Freeing 5 times... \n");
	for (int i = 0; i < 5; i++){
		mm_free(textArray[i]);
		printf("%i ok \n", i);
	}
	puts("Freeing 5 times done");
	
	printf("Reallocating 10 times... \n");
	for (int i = 0; i < 10; i++){
		if (i < 5)
			textArray[i] = mm_realloc(NULL, 128);
		else
			textArray[i] = mm_realloc(textArray[i], 128);
		if (i > 5){
			if (strcmp(textArray[i], "Sample text") == 0)
				printf("%i ok \n", i);
			else
				{printf("%i fail \n", i); realloc_flag++;}
		}else{
			
			if (textArray[i])
				printf("%i ok \n", i);
			else
				{printf("%i fail \n", i); realloc_flag++;}
		}
	}
	if (realloc_flag==0)
		puts("Reallocted 10 times successfully");
	else
		puts("Reallocting 10 times failed");
	realloc_flag=0;
	
	printf("Freeing 5 times... \n");
	for (int i = 5; i < 10; i++){
		mm_free(textArray[i]);
		printf("%i ok \n", i);
	}
	puts("Freeing 5 times done");
	
	printf("Allocating 10 times... \n");
	for (int i = 0; i<10; i++){
		textArray[i] = mm_malloc(64);
		printf("%i ", i);
		if (textArray[i])
			printf("done ");
		else
			{printf("failed "); malloc_flag++;}
		strcpy(textArray[i], "Sample text");
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("ok \n");
		else
			{printf("fail \n"); malloc_flag++;}
	}
	if (malloc_flag==0)
		puts("Allocated 10 times successfully");
	else
		puts("Allocating 10 times failed");
	malloc_flag=0;
	
	puts("-- end secondTest() --");
	
	return;
}

void thirdTest() {
	int malloc_flag=0, realloc_flag=0;
	char* textArray[40];
	puts("-- thirdTest() --");
	
	printf("Allocating 20 times... \n");
	for (int i = 0; i<20; i++){
		textArray[i] = mm_malloc(64);
		printf("%i ", i);
		if (textArray[i])
			printf("done ");
		else
			{printf("failed ");malloc_flag++;}
		strcpy(textArray[i], "Sample text");
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("ok \n");
		else
			{printf("fail \n");malloc_flag++;}
	}
	if (malloc_flag==0)
		puts("Allocated 20 times successfully");
	else
		puts("Allocating 20 times failed");		
	malloc_flag=0;
	
	printf("Freeing 5 times... \n");
	for (int i = 0; i < 5; i++){
		mm_free(textArray[i]);
		printf("%i ok \n", i);
	}
	puts("Freeing 5 times done");
	
	printf("Allocating 20 times... \n");
	for (int i = 20; i<40; i++){
		textArray[i] = mm_malloc(64);
		printf("%i ", i);
		if (textArray[i])
			printf("done ");
		else
			{printf("failed ");malloc_flag++;}
		strcpy(textArray[i], "Sample text");
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("ok \n");
		else
			{printf("fail \n");malloc_flag++;}
	}
	
	if (malloc_flag==0)
		puts("Allocated 20 times successfully");
	else
		puts("Allocating 20 times failed");
	malloc_flag=0;	
	
	printf("Reallocating 20 times... \n");
	for (int i = 20; i < 40; i++){
		textArray[i] = mm_realloc(textArray[i], 128);
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("%i ok \n", i);
		else
			{printf("%i fail \n", i); realloc_flag++;}
	}
	if (realloc_flag==0)
		puts("Reallocated 20 times successfully");
	else
		puts("Reallocating 20 times failed");
	realloc_flag=0;	
	
	printf("Freeing 15 times... \n");
	for (int i = 5; i < 20; i++){
		mm_free(textArray[i]);
		printf("%i ok \n", i);
	}
	puts("Freeing 15 times done");
	
	printf("Allocating 10 times... \n");
	for (int i = 0; i<10; i++){
		textArray[i] = mm_malloc(64);
		printf("%i ", i);
		if (textArray[i])
			printf("done ");
		else
			{printf("failed "); malloc_flag++;}
		strcpy(textArray[i], "Sample text");
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("ok \n");
		else
			{printf("fail \n"); malloc_flag++;}
	}
	
	if (malloc_flag==0)
		puts("Allocated 10 times successfully");
	else
		puts("Allocating 10 times failed");		
	malloc_flag=0;
	
	printf("Reallocating 10 times... \n");
	for (int i = 0; i < 10; i++){
		textArray[i] = mm_realloc(textArray[i], 128);
		if (strcmp(textArray[i], "Sample text") == 0)
			printf("%i ok \n", i);
		else
			{printf("%i fail \n", i); realloc_flag++;}
	}
	if (realloc_flag==0)
		puts("Reallocated 10 times successfully");
	else
		puts("Reallocating 10 times failed");
	realloc_flag=0;
	
	puts("-- end thirdTest() --");
	
	return;
}

void ownTest(){
	puts("-- ownTest() --");
	printf("Allocating 5 bytes... ");
	char* text = mm_malloc(5);
	if (text)
		puts("ok");
	else
		puts("fail");
	
	printf("Writing \"Test\" into allocation... ");
	strcpy(text, "Test");
	if (strcmp(text, "Test") == 0)
		puts("ok");
	else 
		puts("fail");
	
	printf("Reallocating with additional 2 bytes... ");
	text = mm_realloc(text, 7);
	if (strcmp(text, "Test") == 0)
		puts("ok");
	else
		puts("fail");
	
	printf("Writing \"Test++\" into allocation... ");
	strcpy(text, "Test++");
	if (strcmp(text, "Test++") == 0)
		puts("ok");
	else
		puts("fail");
	
	printf("Freeing allocation... ");
	mm_free(text);
	puts("ok");
	
	puts("-- end ownTest() --");
	
	return;
};

int main() {
	basic();
	secondTest();
	thirdTest();
	ownTest();
	return 0;	
}
