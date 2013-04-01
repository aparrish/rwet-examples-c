/*
 * construct a linked list of lines read from standard input, then print them
 * out in random order.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ap_getline.h"

/* struct for linked list of strings */
typedef struct line {
	char *line;
	int line_no;
	int len;
	struct line* next;
} line_t;
int line_count = 0;

int main(int argc, char* argv[]) {

	srand(time(NULL));

	/* read in lines */
	line_t* head;
	line_t* last = NULL;
	line_t* current = NULL;
	while (!feof(stdin)) {
		/* malloc a new line_t */
		current = malloc(sizeof(line_t));
		current->next = NULL;
		current->line_no = line_count;

		/* ap_getline populates current->line with pointer to malloced char* */
		current->len = ap_getline(&(current->line), stdin);

		/* set this line_t as the 'next' of previous line_t */
		if (last == NULL) {
			head = current;
		}
		else {
			last->next = current;
		}
		last = current;
		line_count++;
	}

	/* print lines out randomly, removing from linked list as it goes */
	int offset;
	int i;
	line_t* current_line = NULL;
	line_t* prev_line = NULL;
	while (line_count > 0) {
		prev_line = NULL;
		current_line = head;
		offset = rand() % line_count; /* random offset from head of list */

		/* traverse the list until we get to offset, keeping track of previous
 		 * node (so we can remove the node at offset) */
		for (i = 0; i < offset; i++) {
			prev_line = current_line;
			current_line = current_line->next;
		}
		printf("%s", current_line->line);

		/* if offset was zero, the next line is the new head of the list */
		if (offset == 0) 
			head = current_line->next;

		/* the previous line's next now points to the current line's next */
		if (prev_line != NULL)
			prev_line->next = current_line->next;

		/* free the line and the struct */
		free(current_line->line);
		free(current_line);

		line_count--;
	}

	return 0;
}

