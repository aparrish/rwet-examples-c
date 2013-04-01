/*
 * simple implementation of cat.
 */
#include <stdio.h>
#include <stdlib.h>
#include "ap_getline.h"

int main() {
	char* line = NULL;
	int len = 0;
	while (!feof(stdin)) {
		len = ap_getline(&line, stdin);
		printf("%s", line);
		free(line);
	}
	return 0;
}
