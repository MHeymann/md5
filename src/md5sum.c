#include <stdio.h>
#include <stdint.h>
#include "md5.h"

void print_usage();

/*** Main Routine ********************************************************/

int main(int argc, char *argv[])
{
	uint32_t sum[4];
	FILE *file = NULL;

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} 

	file = fopen(argv[1], "r");

	if (!file) {
		fprintf(stderr, "file %s couldn't be opened\n", argv[1]);
	}

	if (get_md5sum(file, sum)) {
		printf("some error in md5sum");
		return 0;
	}
	printf("%08x%08x%08x%08x  %s\n", 
			sum[0], sum[1], sum[2], sum[3], argv[1]);

	fclose(file);
	file = NULL;
	return 0;

}

void print_usage()
{
	printf("blablabla\n");
}
