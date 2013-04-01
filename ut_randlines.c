/*
 * a version of randlines.c that uses uthash's utarray instead of a homegrown
 * linked list.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ap_getline.h"
#include "utarray.h"

int shuffle(const void* a, const void* b) {
	return rand() % 2 == 0 ? 1 : -1;
}

int main(int argc, char* argv[]) {

	/* create ut array, use built-in str array helper type */
	UT_array* lines;
	utarray_new(lines, &ut_str_icd);

	srand(time(NULL));

	/* read in lines */
	while (!feof(stdin)) {
		int len = 0;
		char* line = NULL;

		/* ap_getline populates line with pointer to malloced char* */
		len = ap_getline(&line, stdin);
		utarray_push_back(lines, &line);

		/* push_back copies the string, so we can free ours here */
		free(line); 
	}

	/* shuffle the lines */
	utarray_sort(lines, shuffle);

	/* iterate through the lines */
	char** current = NULL;
	while ((current = (char**)utarray_next(lines, current))) {
		printf("%s", *current);
	}

	utarray_free(lines);
	return 0;
}

