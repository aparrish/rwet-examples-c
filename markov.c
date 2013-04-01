/*
 * executable wrapper for functions in markov_gen.c. produces a markov chain
 * from a character-level n-gram analysis of input text. run on the command
 * line like so:
 *   ./markov n m <your_input_text
 * where n is the desired length of n-gram and m is the number of lines to
 * generate.
 */

#include <stdio.h>
#include "markov_gen.h"
#include "ap_getline.h"

int main(int argc, char** argv) {

	int len, n, i, line_count;
	char* line;
	markov_gen_t* gen;

	if (argc != 3) {
		fprintf(stderr, "usage: markov n line_count\n");
		exit(-1);
	}

	n = atoi(argv[1]);
	line_count = atoi(argv[2]);

	gen = markov_gen_new(n, 100);
	while (!feof(stdin)) {
		len = ap_getline(&line, stdin);
		markov_gen_feed(gen, line);
		free(line);
	}

	/* uncomment to dump markov chain data */
	/*markov_gen_dump(gen);*/

	for (i = 0; i < line_count; i++) {
		markov_gen_generate_new(gen, &line);
		printf("%s\n", line);
		free(line);
	}

	markov_gen_free(gen);

	return 0;
	
}

