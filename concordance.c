/*
 * counts the number of times each word occurs in stream supplied to standard
 * input. prints words in ascending order of frequency.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ap_getline.h"
#include "uthash.h"

/* a struct to store word -> count mappings */
typedef struct {
	char* word;
	int count;
	UT_hash_handle hh;
} word_count_t;

/* function to compare two word_count_t structs by count */
int compare_count(word_count_t* a, word_count_t* b) {
	return (a->count - b->count);
}

int main() {

	word_count_t* concordance = NULL;
	word_count_t* word_count = NULL;
	word_count_t* tmp = NULL;
	char* line;
	int len;

	while (!feof(stdin)) {
		len = ap_getline(&line, stdin);

		/* iterate through each word in line */
		char* word;
		word = strtok(line, " \n");
		while (word != NULL) {
			/* find word count in hash */
			HASH_FIND_STR(concordance, word, word_count);
			/* if found, update count */
			if (word_count != NULL) {
				word_count->count++;
			}
			/* otherwise, alloc a new structure and add it to the hash */
			else {
				word_count = (word_count_t*)malloc(sizeof(word_count_t));
				/* malloc word, copy into it */
				word_count->word = (char*)malloc(strlen(word)+1);
				strcpy(word_count->word, word);
				word_count->count = 1;
				HASH_ADD_KEYPTR(hh, concordance, word_count->word,
						strlen(word_count->word), word_count);
			}
			word = strtok(NULL, " \n");
		}

		free(line);
	}

	/* sort by count */
	HASH_SORT(concordance, compare_count);

	/* iterate through the hash, deleting and freeing as we go */
	HASH_ITER(hh, concordance, word_count, tmp) {
		printf("%s: %d\n", word_count->word, word_count->count);
		HASH_DEL(concordance, word_count);
		free(word_count->word);
		free(word_count);
	}

	return 0;

}

