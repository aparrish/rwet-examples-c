/*
 * my own stupid getline() implementation, needed because there's no
 * vendor-supplied posix getline() on osx 10.6 (which I am still using.)
 */
#include <stdlib.h>
#include "ap_getline.h"

/* call with a pointer to a NULL pointer, which will be malloced as
 * appropriate */
size_t ap_getline(char** linep, FILE* fp) {
	int current_index = 0;
	int length = 80;

	*linep = (char*)malloc(length);
	if (*linep == NULL) return -1;

	while (1) {
		int ch;
		ch = getc(fp);
		if (ch == EOF) {
			break;
		}
		/* if the string would be longer than the allocated buffer */
		/* + 2: one because current_index is zero-based, one for trailing \0 */
		if (current_index + 2 > length) {
			/* eighty more chars */
			size_t new_length = length + 80;
			char *new_linep;
			new_linep = (char*)realloc(*linep, new_length);
			if (new_linep == NULL) return -1;

			*linep = new_linep;
			length = new_length;
		}
		(*linep)[current_index] = ch;
		current_index++;
		if (ch == '\n') break;
	}
	(*linep)[current_index] = '\0';
	return current_index;

}
