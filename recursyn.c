/* recursive-descent grammar parser */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snl.h"

struct token *curr_token;

struct syntax_tree_node *parse(void)
{
	curr_token = head;
	struct syntax_tree_node *T;

	T = program();
	match(ENDFILE);
	return T;
}

struct syntax_tree_node *program(void)
{
	struct syntax_tree_node *root;

	root = creat_tree_node();
	root->nkind = PROK;
	root->child[0] = program_head();
	root->child[1] = declare_part();
	root->child[2] = program_body();
	match(DOT);
	return root;
}

struct syntax_tree_node *program_head(void)
{
	struct syntax_tree_node *phead = NULL;

	match(PROGRAM);
	phead = creat_tree_node();
	phead->nkind = PHEADK;
	if (is_match(ID))
		save_id_name(phead);
	match(ID);
	return phead;
}

struct syntax_tree_node *declare_part(void)
{
	struct syntax_tree_node *typep = NULL, *varp = NULL, *procp = NULL;

	typep = type_dec();
	varp = var_dec();
	procp = proc_dec();
	if (varp == NULL)
		varp = procp;
	else
		varp->sibling = procp;
	if (typep == NULL)
		typep = varp;
	else
		typep->sibling = varp;
	return typep;
}

struct syntax_tree_node *type_dec(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(VAR) || is_match(PROCEDURE) || is_match(BEGIN))
		return t;
	t = type_declaration();
	return t;
}

struct syntax_tree_node *type_declaration(void)
{
	struct syntax_tree_node *t = NULL;

	match(TYPE);
	t = creat_tree_node();
	t->nkind = TYPEK;
	t->child[0] = type_declist();
	return t;
}

struct syntax_tree_node *type_declist(void)
{
	struct syntax_tree_node *t;

	t = creat_tree_node();
	t->nkind = DECK;
	type_id(t);
	match(EQ);
	type_def(t);
	match(SEMI);
	t->sibling = type_decmore();
	return t;
}

struct syntax_tree_node *type_decmore(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(VAR) || is_match(PROCEDURE) || is_match(BEGIN))
		return t;
	t = type_declist();
	return t;
}

void type_id(struct syntax_tree_node *t)
{
	if (is_match(ID))
		save_id_name(t);
	match(ID);
}

void type_def(struct syntax_tree_node *t)
{
	if (is_match(INTEGER) || is_match(CHAR))
		base_type(t);
	else if (is_match(ARRAY) || is_match(RECORD)) {
		t->attr = (struct attribute *) malloc(sizeof(struct attribute));
		struct_type(t);
	} else if (is_match(ID)) {
		t->kind.dec = IDK;
		save_id_name(t);
		match(ID);
	}
}

void base_type(struct syntax_tree_node *t)
{
	if (is_match(INTEGER)) {
		t->kind.dec = INTEGERK;
		match(INTEGER);
	} else {
		t->kind.dec = CHARK;
		match(CHAR);
	}
}

void struct_type(struct syntax_tree_node *t)
{
	if (is_match(ARRAY)) {
		t->kind.dec = ARRAYK;
		array_type(t);
	} else {
		t->kind.dec = RECORDK;
		record_type(t);
	}
}

void array_type(struct syntax_tree_node *t)
{
	int n;
	char *p;

	match(ARRAY);
	t->attr->arr_attr = (struct array_attr *)
	    malloc(sizeof(struct array_attr));
	match(LMIDPAREN);
	if (is_match(NUM)) {
		for (n = 0, p = curr_token->semantic; *p != '\0'; p++)
			n = 10 * n + (*p - '0');
		t->attr->arr_attr->low = n;
	}
	match(NUM);
	match(UNDERANGE);
	if (is_match(NUM)) {
		for (n = 0, p = curr_token->semantic; *p != '\0'; p++)
			n = 10 * n + (*p - '0');
		t->attr->arr_attr->top = n;
	}
	match(NUM);
	match(RMIDPAREN);
	match(OF);
	base_type(t);
	t->attr->arr_attr->child_type = t->kind.dec;
	t->kind.dec = ARRAYK;
}

void record_type(struct syntax_tree_node *t)
{
	match(RECORD);
	t->kind.dec = RECORDK;
	t->attr->recor_attr = (struct record_attr *) 
		malloc(sizeof(struct record_attr));
	t->attr->recor_attr->field = field_declist();
	match(END);
}

struct syntax_tree_node *field_declist(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(INTEGER) || is_match(CHAR) || is_match(ARRAY)) {
		t = creat_tree_node();
		t->nkind = DECK;
	}
	if (is_match(INTEGER) || is_match(CHAR))
		base_type(t);
	else
		array_type(t);
	id_list(t);
	match(SEMI);
	t->sibling = field_decmore();
	return t;
}

struct syntax_tree_node *field_decmore(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(END))
		return t;
	t = field_declist();
	return t;
}

void id_list(struct syntax_tree_node *t)
{
	if (is_match(ID))
		save_id_name(t);
	match(ID);
	id_more(t);
}

void id_more(struct syntax_tree_node *t)
{
	if (is_match(SEMI))
		return;
	match(COMMA);
	id_list(t);
}

struct syntax_tree_node *var_dec(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(PROCEDURE) || is_match(BEGIN))
		return t;
	t = var_declaration();
	return t;
}

struct syntax_tree_node *var_declaration(void)
{
	struct syntax_tree_node *t = NULL;

	match(VAR);
	t = creat_tree_node();
	t->nkind = VARK;
	t->child[0] = var_declist();
	return t;
}

struct syntax_tree_node *var_declist(void)
{
	struct syntax_tree_node *t;

	t = creat_tree_node();
	t->nkind = DECK;
	type_def(t);
	var_id_list(t);
	match(SEMI);
	t->sibling = var_decmore();
	return t;
}

struct syntax_tree_node *var_decmore(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(PROCEDURE) || is_match(BEGIN))
		return t;
	t = var_declist();
	return t;
}

void var_id_list(struct syntax_tree_node *t)
{
	if (is_match(ID))
		save_id_name(t);
	match(ID);
	var_id_more(t);
}

void var_id_more(struct syntax_tree_node *t)
{
	if (is_match(SEMI))
		return;
	match(COMMA);
	var_id_list(t);
}

struct syntax_tree_node *proc_dec(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(BEGIN))
		return t;
	t = proc_declaration();
	return t;
}

struct syntax_tree_node *proc_declaration(void)
{
	struct syntax_tree_node *t;

	match(PROCEDURE);
	t = creat_tree_node();
	t->nkind = PROCDECK;
	if (is_match(ID))
		save_id_name(t);
	match(ID);
	match(LPAREN);
	paramlist(t);
	match(RPAREN);
	match(SEMI);
	t->child[1] = proc_decpart();
	t->child[2] = proc_body();
	t->sibling = proc_decmore();
	return t;
}

struct syntax_tree_node *proc_decmore(void)
{
	struct syntax_tree_node *t = NULL; 

	if (is_match(BEGIN))
		return t;
	t = proc_declaration();
	return t;
}

void paramlist(struct syntax_tree_node *t)
{
	if (is_match(RPAREN))
		return;
	t->child[0] = param_declist();
}

struct syntax_tree_node *param_declist(void)
{
	struct syntax_tree_node *t, *p;

	t = param();
	p = param_more();
	if (t != NULL)
		t->sibling = p;
	return t;
}

struct syntax_tree_node *param(void)
{
	struct syntax_tree_node *t = NULL;

	t = creat_tree_node();
	t->nkind = DECK;
	t->attr = (struct attribute *) malloc(sizeof(struct attribute));
	t->attr->proc_attr = (struct procedure_attr *)
	    malloc(sizeof(struct procedure_attr));
	if (is_match(INTEGER) || is_match(CHAR) || is_match(RECORD)
            || is_match(ARRAY) || is_match(ID)) {
		t->attr->proc_attr->paramt = VALPARAM;
	} else {
		t->attr->proc_attr->paramt = VARPARAM;
		match(VAR);
	}
	type_def(t);
	form_list(t);
	return t;
}

struct syntax_tree_node *param_more(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(RPAREN))
		return t;
	match(SEMI);
	t = param_declist();
	return t;
}

void form_list(struct syntax_tree_node *t)
{

	if (is_match(ID))
		save_id_name(t);
	match(ID);
	fid_more(t);
}

void fid_more(struct syntax_tree_node *t)
{
	if (is_match(SEMI) || is_match(RPAREN))
		return;
	match(COMMA);
	form_list(t);
}

struct syntax_tree_node *proc_decpart(void)
{
	return declare_part();
}

struct syntax_tree_node *proc_body(void)
{
	return program_body();
}

struct syntax_tree_node *program_body(void)
{
	struct syntax_tree_node *t = NULL;

	match(BEGIN);
	t = creat_tree_node();
	t->nkind = STMTLK;
	t->child[0] = stmt_list();
	match(END);
	return t;
}

struct syntax_tree_node *stmt_list(void)
{
	struct syntax_tree_node *t = NULL;

	t = stmt();
	if (t != NULL)
		t->sibling = stmt_more();
	return t;
}

struct syntax_tree_node *stmt_more(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(ELSE) || is_match(FI) || is_match(END) 
	    || is_match(ENDWH))
		return t;
	match(SEMI);
	t = stmt_list();
	return t;
}

struct syntax_tree_node *stmt(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(IF))
		t = conditional_stmt();
	else if (is_match(WHILE))
		t = loop_stmt();
	else if (is_match(RETURN))
		t = return_stmt();
	else if (is_match(READ))
		t = input_stmt();
	else if (is_match(WRITE))
		t = output_stmt();
	else if (is_match(ID))  /* token == ID */
		t = assign_call();
	else {
		erron("stmt error", curr_token->linum);
		next_token();
	}
	return t;
}

struct syntax_tree_node *assign_call(void)
{
	struct syntax_tree_node *t = NULL, *p;

	t = creat_tree_node();
	t->nkind = STMTK;
	p = creat_tree_node();
	p->nkind = EXPRK;
	p->attr = (struct attribute *)
	    malloc(sizeof(struct attribute));
	p->attr->exp_attr = (struct express_attr *)
	    malloc(sizeof(struct express_attr));
	p->kind.exp = idk;
	if (is_match(ID)) {
		save_id_name(p);
		p->attr->exp_attr->varkind = IDV;
	}
	match(ID);
	t->child[0] = p;
	if (is_match(LPAREN)) {
		t->kind.stmt = CALLK;
		t->child[1] = call_stmt_rest();
	} else {
		t->kind.stmt = ASSIGNK;
		assign_stmt_rest(t);
	}
	return t;
}

void assign_stmt_rest(struct syntax_tree_node *t)
{
	variable_more(t->child[0]);
	match(ASSIGN);
	t->child[1] = expr();
}

struct syntax_tree_node *conditional_stmt(void)
{
	struct syntax_tree_node *t;

	match(IF);
	t = creat_tree_node();
	t->nkind = STMTK;
	t->kind.stmt = IFK;
	t->child[0] = rel_exp();
	match(THEN);
	t->child[1] = stmt_list();
	match(ELSE);
	t->child[2] = stmt_list();
	match(FI);
	return t;
}

struct syntax_tree_node *loop_stmt(void)
{
	struct syntax_tree_node *t = NULL;

	match(WHILE);
	t = creat_tree_node();
	t->nkind = STMTK;
	t->kind.stmt = WHILEK;
	t->child[0] = rel_exp();
	match(DO);
	t->child[1] = stmt_list();
	match(ENDWH);
	return t;
}

struct syntax_tree_node *input_stmt(void)
{
	struct syntax_tree_node *t = NULL;

	match(READ);
	match(LPAREN);
	t = creat_tree_node();
	t->nkind = STMTK;
	t->kind.stmt = READK;
	save_id_name(t);
	match(ID);
	match(RPAREN);
	return t;
}

struct syntax_tree_node *output_stmt(void)
{
	struct syntax_tree_node *t = NULL;

	match(WRITE);
	match(LPAREN);
	t = creat_tree_node();
	t->nkind = STMTK;
	t->kind.stmt = WRITEK;
	t->child[0] = expr();
	match(RPAREN);
	return t;
}

struct syntax_tree_node *return_stmt(void)
{
	struct syntax_tree_node *t = NULL;

	match(RETURN);
	t = creat_tree_node();
	t->nkind = STMTK;
	t->kind.stmt = RETURNK;
	return t;
}

struct syntax_tree_node *call_stmt_rest(void)
{
	struct syntax_tree_node *t = NULL;

	match(LPAREN);
	t = actual_param_list();
	match(RPAREN);
	return t;
}

struct syntax_tree_node *actual_param_list(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(RPAREN))
		return t;
	t = expr();
	t->sibling = actual_param_more();
	return t;
}

struct syntax_tree_node *actual_param_more(void)
{
	struct syntax_tree_node *t = NULL;

	if (is_match(RPAREN))
		return t;
	match(COMMA);
	t = actual_param_list();
	return t;
}

struct syntax_tree_node *rel_exp(void)
{
	struct syntax_tree_node *t = NULL, *p;

	t = expr();
	p = creat_tree_node();
	p->nkind = EXPRK;
	p->kind.exp = optionk;
	p->child[0] = t;
	p->attr = (struct attribute *)
	    malloc(sizeof(struct attribute));
	p->attr->exp_attr = (struct express_attr *)
	    malloc(sizeof(struct express_attr));
	p->attr->exp_attr->op = (is_match(LT) ? LT : EQ);
	if (is_match(LT))
		match(LT);
	else
		match(EQ);
	t = p;
	t->child[1] = expr();
	return t;
}

struct syntax_tree_node *expr(void)
{
	struct syntax_tree_node *t = NULL, *p;

	t = term();
	if (is_match(LT) || is_match(EQ) || is_match(RMIDPAREN)
	    || is_match(THEN) || is_match(ELSE) || is_match(FI)
	    || is_match(DO) || is_match(ENDWH) || is_match(RPAREN)
	    || is_match(END) || is_match(SEMI) || is_match(COMMA))
		return t;
	p = creat_tree_node();
	p->nkind = EXPRK;
	p->kind.exp = optionk;
	p->child[0] = t;
	p->attr = (struct attribute *)
	    malloc(sizeof(struct attribute));
	p->attr->exp_attr = (struct express_attr *)
	    malloc(sizeof(struct express_attr));
	p->attr->exp_attr->op = (is_match(PLUS) ? PLUS : MINUS);
	if (is_match(PLUS))
		match(PLUS);
	else
		match(MINUS);
	t = p;
	t->child[1] = expr();
	return t;
}

struct syntax_tree_node *term(void)
{
	struct syntax_tree_node *t = NULL, *p;

	t = factor();
	if (is_match(PLUS) || is_match(MINUS) || is_match(LT)
	    || is_match(EQ)
	    || is_match(RMIDPAREN) || is_match(THEN) || is_match(ELSE)
	    || is_match(FI) || is_match(DO) || is_match(ENDWH)
	    || is_match(RPAREN) || is_match(END) || is_match(SEMI)
	    || is_match(COMMA))
		return t;
	p = creat_tree_node();
	p->nkind = EXPRK;
	p->kind.exp = optionk;
	p->child[0] = t;
	p->attr = (struct attribute *)
	    malloc(sizeof(struct attribute));
	p->attr->exp_attr = (struct express_attr *)
	    malloc(sizeof(struct express_attr));
	p->attr->exp_attr->op = (is_match(TIMES) ? TIMES : OVER);
	t = p;
	if (is_match(TIMES))
		match(TIMES);
	else
		match(OVER);
	t->child[1] = term();
	return t;
}

struct syntax_tree_node *factor(void)
{
	int n;
	char *p;
	struct syntax_tree_node *t = NULL;

	if (is_match(NUM)) {
		t = creat_tree_node();
		t->nkind = EXPRK;
		t->kind.exp = constk;
		t->attr = (struct attribute *)
		    malloc(sizeof(struct attribute));
		t->attr->exp_attr = (struct express_attr *)
		    malloc(sizeof(struct express_attr));
		for (n = 0, p = curr_token->semantic; *p != '\0'; p++)
			n = 10 * n + (*p - '0');
		t->attr->exp_attr->val = n;
		match(NUM);
	} else if (is_match(ID)) {
		t = variable();
		t->nkind = EXPRK;
		t->attr = (struct attribute *)
			malloc(sizeof(struct attribute));
		t->attr->exp_attr = (struct express_attr *)
			malloc(sizeof(struct express_attr));
		t->kind.exp = idk;
		t->attr->exp_attr->varkind = IDV;
	} else if (is_match(LPAREN)) {     /* ( exp ) */
		match(LPAREN);
		t = expr();
		match(RPAREN);
	} else {
		erron("assign_rest error", curr_token->linum);
		next_token();
	}
	return t;
}

struct syntax_tree_node *variable(void)
{
	struct syntax_tree_node *t = NULL;

	t = creat_tree_node();
	t->nkind = EXPRK;
	t->attr = (struct attribute *)
	    malloc(sizeof(struct attribute));
	t->attr->exp_attr = (struct express_attr *)
	    malloc(sizeof(struct express_attr));
	t->kind.exp = idk;
	t->attr->exp_attr->varkind = IDV;
	if (is_match(ID))
		save_id_name(t);
	match(ID);
	variable_more(t);
	return t;
}

void variable_more(struct syntax_tree_node *t)
{
	if (is_match(ASSIGN) || is_match(TIMES) || is_match(OVER)
	    || is_match(PLUS) || is_match(MINUS) || is_match(LT)
	    || is_match(EQ) || is_match(THEN) || is_match(ELSE)
	    || is_match(FI) || is_match(DO) || is_match(ENDWH)
	    || is_match(RPAREN) || is_match(END) || is_match(SEMI)
	    || is_match(COMMA))
		return;
	t->attr =
		(struct attribute *) malloc(sizeof(struct attribute));
	t->attr->exp_attr = (struct express_attr *)
			malloc(sizeof(struct express_attr));
	t->attr->exp_attr->varkind = IDV;
	if (is_match(LMIDPAREN)) {
		match(LMIDPAREN);
		t->child[0] = expr();
		t->child[0]->attr->exp_attr->varkind = ARRAYMEMBV;
		match(RMIDPAREN);
	} else if (is_match(DOT)) {      /* field */
		match(DOT);
		t->child[0] = field_var();
		t->child[0]->attr->exp_attr->varkind = FIELDMEMBV;
	} else {
		erron("field member error", curr_token->linum);
		next_token();
	}
}

struct syntax_tree_node *field_var(void)
{
	struct syntax_tree_node *t;

	t = creat_tree_node();
	t->nkind = EXPRK;
	t->attr = (struct attribute *)
	    malloc(sizeof(struct attribute));
	t->attr->exp_attr = (struct express_attr *)
	    malloc(sizeof(struct express_attr));
	t->kind.exp = idk;
	t->attr->exp_attr->varkind = FIELDMEMBV;
	if (is_match(ID))
		save_id_name(t);
	match(ID);
	field_var_more(t);
	return t;
}

void field_var_more(struct syntax_tree_node *t)
{
	if (is_match(ASSIGN) || is_match(TIMES) || is_match(OVER) 
	    || is_match(PLUS) || is_match(MINUS) || is_match(LT)
	    || is_match(EQ) || is_match(THEN) || is_match(ELSE) 
	    || is_match(FI) || is_match(DO) || is_match(ENDWH) 
	    || is_match(RPAREN) || is_match(END) || is_match(SEMI)
	    || is_match(COMMA))
		return;
	match(LMIDPAREN);
	t->child[0] = expr();
	t->child[0]->attr = (struct attribute *)
		malloc(sizeof(struct attribute));
	t->child[0]->attr->exp_attr = (struct express_attr *)
		malloc(sizeof(struct express_attr));
	t->child[0]->attr->exp_attr->varkind = ARRAYMEMBV;
	match(RMIDPAREN);
}
