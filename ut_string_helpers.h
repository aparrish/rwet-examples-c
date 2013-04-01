#include "utarray.h"
#include <stdlib.h>

UT_array* split_new(const char* src, const char* delimiter);
UT_array* slice_new(const UT_array* src, int start, int end);
int join_new(char** s, const UT_array* src, const char* separator);

#define utarray_choice(a) ((utarray_eltptr(a, (rand()%((a)->i)))))
