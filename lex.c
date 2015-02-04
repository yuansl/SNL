/* A lexical analysis parser */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snl.h"

static struct reservewd reservewd[RESERVEWD] = {
	{"", 0},
	{"", 0},
	{"program", PROGRAM},
	{"procedure", PROCEDURE},
	{"type", TYPE},
	{"var", VAR},
	{"if", IF},
	{"then", THEN},
	{"else", ELSE},
	{"fi", FI},
	{"while", WHILE},
	{"do", DO},
	{"endwh", ENDWH},
	{"begin", BEGIN},
	{"end", END},
	{"read", READ},
	{"write", WRITE},
	{"array", ARRAY},
	{"of", OF},
	{"record", RECORD},
	{"return", RETURN},
	{"integer", INTEGER},
	{"char", CHAR}
};

char lex_t[LEXNO][LEN] = {
	"ENDFILE", "ERROR", "PROGRAM", "PROCEDURE", "TYPE", "VAR", "IF",
	"THEN", "ELSE", "FI", "WHILE", "DO", "ENDWH", "BEGIN", "END",
	"READ", "WRITE", "ARRAY", "OF", "RECORD", "RETURN", "INTEGER",
	"CHAR", "ID", "NUM", "CHARACTER", "ASSIGN", "EQ", "LT", "PLUS",
	"MINUS",
	"TIMES", "OVER", "LPAREN", "RPAREN", "DOT", "COLON", "SEMI",
	"COMMA",
	"LMIDPAREN", "RMIDPAREN", "UNDERANGE"
};

struct token *get_token(const char *word)
{
	char buf[LEN];
	const char *tmp = word;
	struct token *tken = NULL, *x;
	int ct, c;

	while (*tmp != '\0') {
		if (status != INCOMMENT) {
			if (tken == NULL) {
				tken = (struct token *)
				    malloc(sizeof(*tken));
				x = tken;
			} else {
				x = (x->next = (struct token *)
				     malloc(sizeof(*x)));
			}
			x->next = NULL;
			x->linum = linum;
			x->semantic[0] = '\0';
			/* ID */
			if (isalpha(*tmp)) {
				status = INID;
				ct = 0;
				while (isalnum(*tmp)) {
					buf[ct] = *tmp;
					ct++;
					tmp++;
				}
				buf[ct] = '\0';
				if ((x->syntax = is_reservewd(buf)) == -1) {
					x->syntax = ID;
					strcpy(x->semantic, buf);
				}
				continue;
				/* NUM */
			} else if (isdigit(*tmp)) {
				status = INNUM;
				ct = 0;
				while (isdigit(*tmp)) {
					buf[ct] = *tmp;
					ct++;
					tmp++;
				}
				buf[ct] = '\0';
				x->syntax = NUM;
				if (*tmp != '\0' && !isspace(*tmp) &&
				    *tmp != '.' && *tmp != ';' &&
				    *tmp != ']' && *tmp != ')' &&
				    *tmp != ',' && *tmp != '<' &&
				    *tmp != '=' && *tmp != '+' &&
				    *tmp != '-' && *tmp != '*' && *tmp != '/') {
					x->syntax = ERROR;
					strcpy(x->semantic, "NUM");
				} else
					strcpy(x->semantic, buf);
				continue;
				/* CHARC */
			} else if (*tmp == '\'') {
				status = INCHAR;
				ct = 0;
				while (*++tmp != '\'' && isalnum(*tmp))
					buf[ct++] = *tmp;
				buf[ct] = '\0';
				x->syntax = CHARACTER;
				if (ct != 1) {
					x->syntax = ERROR;
					strcpy(x->semantic, "CHAR");
				} else
					strcpy(x->semantic, buf);
			} else if (*tmp == '+') {
				x->syntax = PLUS;
			} else if (*tmp == '-') {
				x->syntax = MINUS;
			} else if (*tmp == '*') {
				x->syntax = TIMES;
			} else if (*tmp == '/') {
				x->syntax = OVER;
			} else if (*tmp == '(') {
				x->syntax = LPAREN;
			} else if (*tmp == ')') {
				x->syntax = RPAREN;
			} else if (*tmp == '.') {	/* .. */
				if (*(tmp + 1) == '.') {
					x->syntax = UNDERANGE;
					tmp++;
				} else {
					x->syntax = DOT;
					tmp++;
					if ((c = getchar()) == EOF) {
						status = DONE;
						break;
					} else {
						ungetc(c, stdin);
						continue;
					}
				}
			} else if (*tmp == '[') {
				x->syntax = LMIDPAREN;
			} else if (*tmp == ']') {
				x->syntax = RMIDPAREN;
			} else if (*tmp == ';') {
				x->syntax = SEMI;
			} else if (*tmp == ':') {	/* ASSIGN */
				if (*(tmp + 1) == '=') {
					x->syntax = ASSIGN;
					tmp++;
				} else
					x->syntax = COLON;
			} else if (*tmp == ',') {
				x->syntax = COMMA;
			} else if (*tmp == '<') {
				x->syntax = LT;
			} else if (*tmp == '=') {
				x->syntax = EQ;
			} else if (*tmp == '{') {
				status = INCOMMENT;
			} else {	/* ERROR : e.g  " ' _ \ */
				x->syntax = ERROR;
				ct = 0;
				buf[ct++] = *tmp;
				buf[ct] = '\0';
				strcpy(x->semantic, buf);
			}
		} else if (*tmp == '}')
			status = START;
		tmp++;
	}
	if (status == DONE) {
		x = (x->next = (struct token *)malloc(sizeof(*x)));
		x->linum = linum;
		x->syntax = ENDFILE;
		x->semantic[0] = '\0';
		x->next = NULL;
	}
	return tken;
}

void print_token(struct token *tken)
{
	struct token *curr = tken;

	while (curr != NULL) {
		printf("%d: %s : %s\n",
		       curr->linum, lex_t[curr->syntax], curr->semantic);
		curr = curr->next;
	}
}

enum LEXTYPE is_reservewd(const char *identifier)
{
	int i;

	for (i = 2; i < RESERVEWD; i++)
		if (strcmp(identifier, reservewd[i].reservewd) == 0)
			return i;
	return -1;
}

struct token *process_line(const char *src)
{
	char buf[LEN];
	struct token *tk = NULL, *t;

	while (isspace(*src++)) ;
	while (sscanf(src, "%s", buf) != EOF) {
		if (tk == NULL) {
			tk = get_token(buf);
			t = tk;
		} else
			t->next = get_token(buf);
		src += strlen(buf) + 1;
		if (t == NULL)
			continue;
		while (t->next != NULL)
			t = t->next;
	}
	return tk;
}
