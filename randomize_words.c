/*
 * for each line of standard input, split into words, then print those words
 * in random order.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ap_getline.h"
#include "ut_string_helpers.h"
#include "utarray.h"

int shuffle(const void* a, const void* b) {
	return rand() % 2 == 0 ? 1 : -1;
}

int main() {

	UT_array* line_words;
	char* line, * randomized_line;
	int len;

	srand(time(NULL));

	while (!feof(stdin)) {
		len = ap_getline(&line, stdin);

		utarray_new(line_words, &ut_str_icd);

		/* tokenize line, pushing into array */
		char* word;
		word = strtok(line, " \n");
		while (word != NULL) {
			utarray_push_back(line_words, &word);
			word = strtok(NULL, " \n");
		}

		/* shuffle */
		utarray_sort(line_words, shuffle);

		join_new(&randomized_line, line_words, " ");
		printf("%s\n", randomized_line);

		utarray_free(line_words);
		free(randomized_line);
		free(line);

	}

	return 0;
}
