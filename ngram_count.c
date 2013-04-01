/*
 * a more general version of count_word_pairs.  finds all unique word-level
 * n-grams in stream supplied to standard input, prints them in ascending order
 * by frequency.
 * 
 * specify n on the command line as the first paramater, like so:
 *   ./ngram_count * 4 <../your_input_text
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
	char* gram;
	int count;
	UT_hash_handle hh;
} ngram_count_t;

/* function to compare two word_pair_t structs by count */
int compare_count(ngram_count_t* a, ngram_count_t* b) {
	return (a->count - b->count);
}

int main(int argc, char** argv) {

	char* line;
	char* joined;
	int len, n, i, joined_len;
	UT_array* tokens;
	UT_array* token_slice = NULL;
	ngram_count_t* ngrams = NULL;
	ngram_count_t* current_ngram = NULL;
	ngram_count_t* tmp = NULL;

	if (argc != 2) {
		fprintf(stderr, "missing required param n\n");
		exit(-1);
	}

	n = atoi(argv[1]);

	while (!feof(stdin)) {

		/* read a line, split it into tokens */
		len = ap_getline(&line, stdin);
		tokens = split_new(line, " \n");

		/* if we have enough tokens to find an ngram of length n... */
		if (utarray_len(tokens) >= n) {
			/* iterate over each group of n from beginning to end of list */
			for (i = 0; i < utarray_len(tokens) - n + 1; i++) {

				/* make a new array slice, then join to make key */
				token_slice = slice_new(tokens, i, i+n);
				joined_len = join_new(&joined, token_slice, " ");

				/* if we can find a struct with that key in the hash, increment */
				HASH_FIND_STR(ngrams, joined, current_ngram);
				if (current_ngram != NULL) {
					current_ngram->count++;
					free(joined); /* no longer needed */
				}
				/* otherwise, malloc and add to hash */
				else {
					current_ngram = (ngram_count_t*)malloc(sizeof(ngram_count_t));
					current_ngram->gram = joined; /* free this later */
					current_ngram->count = 1;
					HASH_ADD_KEYPTR(hh, ngrams, current_ngram->gram,
							strlen(current_ngram->gram), current_ngram);
				}

				utarray_free(token_slice);
			}
		}

		utarray_free(tokens);
		free(line);

	}

	HASH_SORT(ngrams, compare_count);

	HASH_ITER(hh, ngrams, current_ngram, tmp) {
		printf("%s: %d\n", current_ngram->gram, current_ngram->count);
		HASH_DEL(ngrams, current_ngram);
		free(current_ngram->gram);
		free(current_ngram);
	}

	return 0;

}

