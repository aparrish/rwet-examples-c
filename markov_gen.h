#include "utarray.h"
#include "uthash.h"
#include "ut_string_helpers.h"

/* struct relating an n-gram to a list of tokens following that n-gram */
typedef struct {
	char* ngram_key; /* key (space-separated strings) */
	UT_array* ngram; /* list of words in ngram */
	UT_array* possible_nexts; /* list of words that can follow */
	UT_hash_handle hh; /* for uthash */
} ngram_t;

/* struct containing info necessary for markov chain generation */
typedef struct {
	int n;
	int max;
	ngram_t* ngrams;
	UT_array* beginnings;
} markov_gen_t;

markov_gen_t* markov_gen_new(int n, int max);
void markov_gen_feed(markov_gen_t* mg, const char* line);
void markov_gen_dump(markov_gen_t* mg);
int markov_gen_generate_new(markov_gen_t* mg, char** line);
void markov_gen_free(markov_gen_t* mg);

