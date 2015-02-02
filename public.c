#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snl.h"

#define SYNO 32
extern char lex_t[][LEN];

char syn_t[SYNO][LEN] = {
	"PROK", "PHEADK", "TYPEK", "VARK", "PROCDECK", "STMTLK",
	"DECK", "STMTK", "EXPRK",
	"ARRAYK", "CHARK", "INTEGERK", "RECORDK", "IDK",
	"IFK", "WHILEK", "ASSIGNK", "READK", "WRITEK", "CALLK", "RETURNK",
	"optionk", "constk", "idk",
	"VOIDT", "INTEGERT", "BOOLEANT",
	"IDV", "ARRAYMEMBV", "FIELDMEMBV",
	"VALPARAM", "VARPARAM"
};

void match(enum LEXTYPE LEX)
{
	if (is_match(LEX)) {
		next_token();
	} else
		erron(lex_t[LEX], curr_token->linum);

}

void erron(const char *err_msg, int lin)
{
	fprintf(stderr, "syntax error: %s at line: %d\n", err_msg, lin);
	exit(EXIT_FAILURE);
}

void next_token(void)
{
	curr_token = curr_token->next;
}

struct syntax_tree_node *creat_tree_node(void)
{
	struct syntax_tree_node *t;

	if ((t = (struct syntax_tree_node *) malloc(sizeof(*t))) == NULL)
		perror("malloc");
	else {
		t->child[0] = t->child[1] = t->child[3] = NULL;
		t->sibling = NULL;
		if (curr_token == NULL)
			return t;
		t->linum = curr_token->linum;
		t->id_counter = 0;
		t->id_name = NULL;
		t->table = NULL;
		t->attr = NULL;
	}
	return t;
}

int is_match(enum LEXTYPE LEX)
{
	return curr_token->syntax == LEX;
}

void save_id_name(struct syntax_tree_node *t)
{
	struct id_name *prev = t->id_name;

	if (prev == NULL) {
		t->id_name =
			(struct id_name *) malloc(sizeof(struct id_name));
		prev = t->id_name;
	} else {
		while (prev->next != NULL)
			prev = prev->next;
		prev = (prev->next = (struct id_name *)
			malloc(sizeof(struct id_name)));
	}
	prev->next = NULL;
	strcpy(prev->name, curr_token->semantic);
	t->id_counter++;
}

void print_tree(struct syntax_tree_node *t)
{
	if (t != NULL) {
		struct id_name *tmp = t->id_name;
		struct syntax_tree_node *p = NULL;
		printf("%s ", syn_t[t->nkind]);
		if (t->nkind == STMTK)
			printf("%s", syn_t[t->kind.stmt]);
		else if (t->nkind == EXPRK) {
			printf("%s ", syn_t[t->kind.exp]);
			if (t->kind.exp == constk)
				printf("%d ", t->attr->exp_attr->val);
			else if (t->kind.exp == optionk)
				printf("%s ", lex_t[t->attr->exp_attr->op]);
			else if (t->kind.exp == idk)
				printf("%s ", syn_t[t->attr->exp_attr->varkind]);
		} else if (t->nkind == DECK)
			printf("%s ", syn_t[t->kind.dec]);
		while (tmp != NULL) {
			printf(" %s", tmp->name);
			tmp = tmp->next;
		}
		
		printf("\n");
		print_tree(t->child[0]);
		print_tree(t->child[1]);
		print_tree(t->child[2]);
		p = t;
		print_tree(p->sibling);
	}
}
