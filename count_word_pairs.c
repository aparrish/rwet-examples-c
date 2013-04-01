/*
 * finds all unique word pairs in stream supplied to standard input, prints
 * them in ascending order by frequency.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ap_getline.h"
#include "ut_string_helpers.h"
#include "uthash.h"
#include "utarray.h"

typedef struct {
	char* pair;
	int count;
	UT_hash_handle hh;
} word_pair_t;

/* function to compare two word_pair_t structs by count */
int compare_count(word_pair_t* a, word_pair_t* b) {
	return (a->count - b->count);
}

int main() {

	char* line;
	int len, i;
	UT_array* words;
	word_pair_t* pairs = NULL;
	word_pair_t* current_pair = NULL;
	word_pair_t* tmp = NULL;

	while (!feof(stdin)) {
		len = ap_getline(&line, stdin);
		words = split_new(line, " \n");

		char** first = NULL;
		char** second = NULL;
		size_t concat_len = 0;
		char* pair_concat;

		/* if there are at least two words on this line... */
		if (utarray_len(words) >= 2) {
			/* get each pair */
			for (i = 0; i < utarray_len(words) - 1; i++) {
				first = (char**)utarray_eltptr(words, i);
				second = (char**)utarray_eltptr(words, i+1);

				/* malloc a new string and sprintf pair into it */
				concat_len = strlen(*first) + 1 + strlen(*second) + 1;
				pair_concat = (char*)malloc(concat_len);
				sprintf(pair_concat, "%s %s", *first, *second);
				printf("pair: '%s'\n", pair_concat);

				/* check if this key is present */
				HASH_FIND_STR(pairs, pair_concat, current_pair);
				if (current_pair != NULL) {
					current_pair->count++;
					free(pair_concat); /* no longer needed */
				}
				else {
					current_pair = (word_pair_t*)malloc(sizeof(word_pair_t));
					current_pair->pair = pair_concat;
					current_pair->count = 1;
					HASH_ADD_KEYPTR(hh, pairs, current_pair->pair,
							strlen(current_pair->pair), current_pair);
				}
			}
		}
		utarray_free(words);
		free(line);
	}

	HASH_SORT(pairs, compare_count);

	HASH_ITER(hh, pairs, current_pair, tmp) {
		printf("%s: %d\n", current_pair->pair, current_pair->count);
		HASH_DEL(pairs, current_pair);
		free(current_pair->pair);
		free(current_pair);
	}

	return 0;

}
