#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "md5.h"


/*** Main Routine ********************************************************/

int main(void)
{
	uint32_t sum[8];
	FILE *file = NULL;
	char path[50];
	char command[60];
	int res = 0;
	

	printf("This is a test\n\n");

	/*** 448 bits exactly ***/

	sprintf(path, "testfiles/448bits.txt");
	file = fopen(path, "r");
	if (file == NULL) {
		printf("file %s not found\n", path);
	} else {
		get_md5sum(file, sum);
		printf("%08x%08x%08x%08x  %s\n", 
				sum[0], sum[1], sum[2], sum[3], path);
		sprintf(command, "md5sum %s", path);
		fclose(file);
		file = NULL;
		res = system(command);
	}
	printf("\n");

	/*** 496 bits ***/

	sprintf(path, "testfiles/496bits.txt");
	file = fopen(path, "r");
	if (file == NULL) {
		printf("file %s not found\n", path);
	} else {
		get_md5sum(file, sum);
		printf("%08x%08x%08x%08x  %s\n", 
				sum[0], sum[1], sum[2], sum[3], path);
		sprintf(command, "md5sum %s", path);
		fclose(file);
		file = NULL;
		res = system(command);
	}
	printf("\n");

	/*** 512 bits ***/

	sprintf(path, "testfiles/512bits.txt");
	file = fopen(path, "r");
	if (file == NULL) {
		printf("file %s not found\n", path);
	} else {
		get_md5sum(file, sum);
		printf("%08x%08x%08x%08x  %s\n", 
				sum[0], sum[1], sum[2], sum[3], path);
		sprintf(command, "md5sum %s", path);
		fclose(file);
		file = NULL;
		res = system(command);
	}
	printf("\n");

	/*** big file --> tale of two cities ***/

	sprintf(path, "testfiles/tale.txt");
	file = fopen(path, "r");
	if (file == NULL) {
		printf("file %s not found\n", path);
	} else {
		get_md5sum(file, sum);
		printf("%08x%08x%08x%08x  %s\n", 
				sum[0], sum[1], sum[2], sum[3], path);
		sprintf(command, "md5sum %s", path);
		fclose(file);
		file = NULL;
		res = system(command);
	}
	printf("\n");

	/*** sentence  ***/
	
	sprintf(path, "testfiles/sentence.txt");
	file = fopen(path, "r");
	if (file == NULL) {
		printf("file %s not found\n", path);
	} else {
		get_md5sum(file, sum);
		printf("%08x%08x%08x%08x  %s\n", 
				sum[0], sum[1], sum[2], sum[3], path);
		sprintf(command, "md5sum %s", path);
		fclose(file);
		file = NULL;
		res = system(command);
	}
	printf("\n");

	/*** sentence with period  ***/
	
	sprintf(path, "testfiles/sentence_period.txt");
	file = fopen(path, "r");
	if (file == NULL) {
		printf("file %s not found\n", path);
	} else {
		get_md5sum(file, sum);
		printf("%08x%08x%08x%08x  %s\n", 
				sum[0], sum[1], sum[2], sum[3], path);
		sprintf(command, "md5sum %s", path);
		fclose(file);
		file = NULL;
		res = system(command);
	}
	printf("\n");


	printf("End of test\n");


	if (res) {
		printf("meh\n");
	}
	
	return 0;
}

