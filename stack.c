#include <stdio.h>
#include <stdlib.h>
#include "snl.h"

static struct symbol_table *top_sym = NULL;
static struct operation *top_option = NULL;
static struct operand *top_oprand = NULL;
static struct stack_syntax_tree *top_tree = NULL;

void push_symb(union symb_element top, int flag, int ntnum)
{
	struct symbol_table *x = (struct symbol_table *) malloc(sizeof(*x));
	x->next = NULL;
	if (top_sym == NULL)
		top_sym = x;
	else {
		x->next = top_sym;
		top_sym = x;
	}
	top_sym->is_nterminal = flag;
	top_sym->symb = top;
	top_sym->ntnum = ntnum;
}

int symb_stack_is_empty(void)
{
	return top_sym == NULL;
}

struct symbol_table *pop_symb(void)
{
	struct symbol_table *tmp = top_sym;

	if (top_sym != NULL)
		top_sym = top_sym->next;
	else
		return NULL;
	return tmp;
}

struct symbol_table *top_symb(void)
{
	return top_sym;
}

void push_operation(struct operation *option)
{
	struct operation *x = (struct operation *) malloc(sizeof(*x));
	x->next = NULL;

	if (top_option == NULL)
		top_option = x;
	else {
		x->next = top_option;
		top_option = x;
	}
}

struct operation *pop_operation(void)
{
	struct operation *tmp = top_option;

	if (top_option != NULL)
		top_option = top_option->next;
	else
		return NULL;
	return tmp;
}

struct operation *top_operation(void)
{
	return top_option;
}

void push_operand(struct operand *operand)
{
	struct operand *x = (struct operand *) malloc(sizeof(*x));
	x->next = NULL;

	if (top_oprand == NULL)
		top_oprand = x;
	else {
		x->next = top_oprand;
		top_oprand = x;
	}
}

struct operand *pop_operand(void)
{
	struct operand *tmp = top_oprand;

	if (top_oprand != NULL)
		top_oprand = top_oprand->next;
	else
		return NULL;
	return tmp;
}

struct operand *top_operand(void)
{
	return top_oprand;
}

void push_tree_node(struct syntax_tree_node **t)
{
	struct stack_syntax_tree *x = 
		(struct stack_syntax_tree *) malloc(sizeof(*x));
	x->next = NULL;
	if (top_tree == NULL) 
		top_tree = x;
	else {
		x->next = top_tree;
		top_tree = x;
	}
	top_tree->t = t;
}

struct syntax_tree_node **pop_tree_node(void)
{
	struct stack_syntax_tree *tmp;

	tmp = top_tree;
	if (top_tree != NULL)
		top_tree = top_tree->next;
	return tmp->t;
}
