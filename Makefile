CFLAGS=-Wall -g

EXECUTABLES=cat concordance count_word_pairs forfinder markov ngram_count randlines randomize_words ut_randlines

all: $(EXECUTABLES)

cat: ap_getline.o cat.o

concordance: ap_getline.o concordance.o

count_word_pairs: ap_getline.o ut_string_helpers.o count_word_pairs.o

forfinder: ap_getline.o forfinder.o

markov: ut_string_helpers.o markov_gen.o ap_getline.o markov.o

ngram_count: ap_getline.o ut_string_helpers.o ngram_count.o

randlines: ap_getline.o randlines.o

randomize_words: ap_getline.o ut_string_helpers.o randomize_words.o

ut_randlines: ap_getline.o ut_randlines.c

clean:
	rm -f *.o $(EXECUTABLES)
	rm -rf *.dSYM
