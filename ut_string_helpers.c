/*
 * some helper functions to deal with uthash data structures that contain
 * strings.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ut_string_helpers.h"

/* return a pointer to newly malloc'ed UT_array with string split by delim */
UT_array* split_new(const char* src, const char* delimiter) {
	char* src_copy;
	char* word;
	UT_array* tokens;

	utarray_new(tokens, &ut_str_icd);

	/* don't modify original string */
	src_copy = (char*)malloc(strlen(src)+1);
	strcpy(src_copy, src);
	if (src_copy == NULL) return NULL;

	word = strtok(src_copy, delimiter);
	while (word != NULL) {
		/* utarray_push_back copies the string */
		utarray_push_back(tokens, &word);
		word = strtok(NULL, delimiter);
	}

	free(src_copy);
	return tokens;
}

/* return a newly allocated UT_array that is a (copied) slice of src */
UT_array* slice_new(const UT_array* src, int start, int end) {

	int i = start;
	UT_array* slice = NULL;
	utarray_new(slice, &ut_str_icd);
	char** elem = NULL;

	if (end > utarray_len(src))
		end = utarray_len(src);

	for (i = start; i < end; i++) {
		elem = (char**)utarray_eltptr(src, i);
		utarray_push_back(slice, elem);
	}

	return slice;

}

/* join a UT_array of strings into one newly malloc'ed string */
int join_new(char** s, const UT_array* src, const char* separator) {
	int len, i, separator_len, offset, src_len;
	char** elem = NULL;

	separator_len = strlen(separator);
	src_len = utarray_len(src);

	/* calculate the size for the needed buffer (length of all strings in src,
	   plus src_len * separator_len - 1 */
	len = 0;
	for (i = 0; i < src_len; i++) {
		len += strlen(*(char**)utarray_eltptr(src, i));
	}
	len += (src_len - 1) * separator_len;

	*s = (char*)malloc(len + 1);

	/* s = char**
   * *s = char*
   * **s char */

	/* blit chars into allocated buffer */
	offset = 0;
	for (i = 0; i < src_len; i++) {
		elem = (char**)utarray_eltptr(src, i);	
		if (i < src_len - 1) {
			offset += sprintf((*s) + offset, "%s%s", *elem, separator);
		}
		else {
			sprintf((*s) + offset, "%s", *elem);
		}
	}

	return len;

}
