/*
 * functions implementing a markov chain text generation algorithm. see
 * markov.c for an example of how to use these functions.
 */

#include <stdlib.h>
#include <stdio.h>

#include "utarray.h"
#include "uthash.h"
#include "ut_string_helpers.h"
#include "markov_gen.h"

UT_icd ptr_icd = {sizeof(void*), NULL, NULL, NULL};

/* initialize and malloc a new markov chain data structure */
markov_gen_t* markov_gen_new(int n, int max) {
	markov_gen_t* mg;
	mg = (markov_gen_t*)malloc(sizeof(markov_gen_t));
	mg->n = n;
	mg->max = max;
	mg->ngrams = NULL;
	utarray_new(mg->beginnings, &ptr_icd);
	return mg;
}

/* feed a line of text, perform n-gram analysis and construct markov chain */
void markov_gen_feed(markov_gen_t* mg, const char* line) {
	UT_array* tokens;
	UT_array* token_slice;
	ngram_t* current_ngram;
	char** next_tok;
	char* joined;
	int i, token_len, joined_len;

	tokens = split_new(line, " \n");
	token_len = utarray_len(tokens);

	/* skip if this line has fewer than n tokens */
	if (token_len < mg->n) {
		utarray_free(tokens);
		return;
	}

	/* iterate over n-length slices of array */
	for (i = 0; i < token_len - (mg->n); i++) {
		token_slice = slice_new(tokens, i, i+(mg->n));
		joined_len = join_new(&joined, token_slice, " ");

		next_tok = (char**)utarray_eltptr(tokens, i+(mg->n));

		HASH_FIND_STR(mg->ngrams, joined, current_ngram);
		/* if this n-gram is already in the struct, push the next token */
		if (current_ngram != NULL) {
			utarray_push_back(current_ngram->possible_nexts, next_tok);
			free(joined);
			utarray_free(token_slice);
		}
		/* otherwise, malloc a new n-gram struct and init with relevant info */
		else {
			current_ngram = (ngram_t*)malloc(sizeof(ngram_t));
			/* need to store slice as well */
			current_ngram->ngram_key = joined;
			current_ngram->ngram = token_slice;
			utarray_new(current_ngram->possible_nexts, &ut_str_icd);
			utarray_push_back(current_ngram->possible_nexts, next_tok);
			HASH_ADD_KEYPTR(hh, mg->ngrams, current_ngram->ngram_key,
					strlen(current_ngram->ngram_key), current_ngram);
		}
		/* if this is the first ngram of the line, add to beginnings */
		if (i == 0) {
			utarray_push_back(mg->beginnings, &current_ngram);
		} 

	}

	utarray_free(tokens);

}

/* generate a string from the markov chain (second param should be an
 * unallocated char**, which will be malloc'ed and returned) */
int markov_gen_generate_new(markov_gen_t* mg, char** line) {

	char* current_key;
	char** possible_next;
	UT_array* output, * next_slice;
	ngram_t* current_ngram;
	int i, output_len, joined_len;

	/* grab a random ngram that occurred at the beginning of a line */
	current_ngram = *((ngram_t**)utarray_choice(mg->beginnings));

	/* make a key by joining the array with a space */
	joined_len = join_new(&current_key, current_ngram->ngram, " ");

	/* new array for output */
	utarray_new(output, &ut_str_icd);

	/* copy from first ngram to output */
	utarray_concat(output, current_ngram->ngram);

	for (i = 0; i < mg->max; i++) {

		/* try to find an ngram with this key */
		HASH_FIND_STR(mg->ngrams, current_key, current_ngram);
		free(current_key); /* previously malloc'ed by join_new */

		/* if we found an ngram */
		if (current_ngram != NULL) {
			/* get a random next word */
			possible_next = (char**)utarray_choice(current_ngram->possible_nexts);
			/* add the word to our output */
			utarray_push_back(output, possible_next);
			/* make a new key from the last n elements of output array */
			output_len = utarray_len(output);
			next_slice = slice_new(output, output_len - (mg->n), output_len);
			joined_len = join_new(&current_key, next_slice, " ");
			utarray_free(next_slice);
		}
		else {
			break;
		}
	}

	/* join output into supplied char* */
	joined_len = join_new(line, output, " ");

	utarray_free(output);
	return joined_len;

}

/* print markov chain data structure to stdout */
void markov_gen_dump(markov_gen_t* mg) {
	ngram_t* current, * tmp;
	char* joined_nexts, * joined_ngram;
	int joined_len, i;

	printf("ngrams:\n");
	HASH_ITER(hh, mg->ngrams, current, tmp) {
		joined_len = join_new(&joined_ngram, current->ngram, " ");
		joined_len = join_new(&joined_nexts, current->possible_nexts, ", ");
		printf("\t%s -> %s\n", joined_ngram, joined_nexts);
		free(joined_ngram);
		free(joined_nexts);
	}

	printf("beginnings:\n");
	for (i = 0; i < utarray_len(mg->beginnings); i++) {
		current = *((ngram_t**)utarray_eltptr(mg->beginnings, i));
		printf("\t%s\n", current->ngram_key);
	}

	printf("a random beginning:\n");
	current = *((ngram_t**)utarray_choice(mg->beginnings));
	printf("\t%s\n", current->ngram_key);

}

/* free a markov generator data structure */
void markov_gen_free(markov_gen_t* mg) {

	ngram_t* current, * tmp;

	/* free individual ngram structs here */
	HASH_ITER(hh, mg->ngrams, current, tmp) {
		HASH_DEL(mg->ngrams, current);
		free(current->ngram_key);
		utarray_free(current->possible_nexts);
		utarray_free(current->ngram);
		free(current);
	}

	utarray_free(mg->beginnings);
	free(mg);
}

