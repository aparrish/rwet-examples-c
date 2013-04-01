/*
 * prints every line matching a given string, from the beginning of that
 * string up to the end of the line. (a more general version of forfinder.py
 * in the original RWET class notes)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap_getline.h"

int main(int argc, char** argv) {
	char* target_pos;
	char* line = NULL;
	int len;

	if (argc != 2) {
		fprintf(stderr, "please supply target string\n");
		exit(-1);
	}

	while (!feof(stdin)) {
		len = ap_getline(&line, stdin);
		target_pos = strstr(line, argv[1]);
		if (target_pos) printf("%s", target_pos);
		free(line);
	}

	return 0;
}
