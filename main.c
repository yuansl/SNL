#include <stdio.h>
#include <stdlib.h>
#include "snl.h"

#define BUFFER 100

int linum = 0;			/* number of line */
enum STATUS status = START;
struct token *head = NULL;
struct syntax_tree_node *tree = NULL;

int main(void)
{
	char buf[BUFFER];
	FILE *fpr;
	struct token *x;

	if ((fpr = freopen("example.snl", "r", stdin)) == NULL) {
		fprintf(stderr, "can't not open the source file\n");
		exit(EXIT_FAILURE);
	}
	while (fgets(buf, BUFFER, stdin) != NULL) {
		linum++;
		if (status == DONE) 
			break;
		if (head == NULL) {
			head = process_line(buf);
			x = head;
		} else
			x->next = process_line(buf);
		if (x == NULL) 
			continue;
		while (x->next != NULL)
			x = x->next;
	}
	printf("token:\n");
	print_token(head);
	printf("\nrecursive-descend:\n");
	tree = parse();
	print_tree(tree);

	fclose(fpr);
	return 0;
}
