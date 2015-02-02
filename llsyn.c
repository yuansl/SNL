#include <stdio.h>
#include "snl.h"

static int parse_tab[NTSIZE][LEXNO];
extern int symb_stack_is_empty(void);
extern char lex_t[][LEN];

void (*predic[105]) (void) = {
	NULL, process1, process2, process3, process4, process5, process6,
	process7, process8, process9, process10, process11, process12,
	process13, process14, process15, process16, process17, process18,
	process19, process20, process21, process22, process23, process24,
	process25, process26, process27, process28, process29, process30,
	process31, process32, process33, process34, process35, process36,
	process37, process38, process39, process40, process41, process42,
	process43, process44, process45, process46, process47, process48,
	process49, process50, process51, process52, process53, process54,
	process55, process56, process57, process58, process59, process60, 
	process61, process62, process63, process64, process65, process66,
	process67, process68, process69, process70, process71, process72,
	process73, process74, process75, process76, process77, process78,
	process79, process80, process81, process82, process83, process84,
	process85, process86, process87, process88, process89, process90,
	process91, process92, process93, process94, process95, process96,
	process97, process98, process99, process100, process101, process102,
	process103, process104
};

struct syntax_tree_node *parsell(void)
{
	enum N_TERMINAL ntnum;
	int pnum, is_nterminal;
	union symb_element s;
	struct syntax_tree_node *T;
	struct symbol_table *t;

	curr_token = head;
	creat_parse_table();

	T = creat_tree_node();
	T->nkind = PROK;

	s.fp = process1;
	ntnum = programt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);

	while (!symb_stack_is_empty()) {
		t = pop_symb();
		is_nterminal = t->is_nterminal;
		if (is_nterminal) {
			ntnum = t->ntnum;
			pnum = parse_tab[ntnum][curr_token->syntax];
			if (pnum == 0) 
				erron("Not suitable process statement",
				      curr_token->syntax);
			predict(pnum);
		} else 
			match(t->symb.terminal);
	}
	match(ENDFILE);
	return T;
}

void creat_parse_table(void)
{
	enum N_TERMINAL i;
	enum LEXTYPE j;
	for (i = programt; i <= mult_opt; ++i)
		for (j = ENDFILE; j <= UNDERANGE; ++j)
			parse_tab[i][j] = 0;
	parse_tab[programt][PROGRAM] = 1;
	parse_tab[program_headt][PROGRAM] = 2;
	parse_tab[program_namet][ID] = 3;
	parse_tab[declare_partt][TYPE] = 4;
	parse_tab[declare_partt][VAR] = 4;
	parse_tab[declare_partt][PROCEDURE] = 4;
	parse_tab[declare_partt][BEGIN] = 4;
	parse_tab[type_decpartt][VAR] = 5;
	parse_tab[type_decpartt][PROCEDURE] = 5;
	parse_tab[type_decpartt][BEGIN] = 5;
	parse_tab[type_decpartt][TYPE] = 6;
	parse_tab[type_dect][TYPE] = 7;
	parse_tab[type_declistt][ID] = 8;
	parse_tab[type_decmoret][VAR] = 9;
	parse_tab[type_decmoret][PROCEDURE] = 9;
	parse_tab[type_decmoret][BEGIN] = 9;
	parse_tab[type_decmoret][ID] = 10;
	parse_tab[type_idt][ID] = 11;
	parse_tab[type_deft][INTEGER] = 12;
	parse_tab[type_deft][CHAR] = 12;
	parse_tab[type_deft][ARRAY] = 13;
	parse_tab[type_deft][RECORD] = 13;
	parse_tab[type_deft][ID] = 14;
	parse_tab[base_typet][INTEGER] = 15;
	parse_tab[base_typet][CHAR] = 16;
	parse_tab[struct_typet][ARRAY] = 17;
	parse_tab[struct_typet][RECORD] = 18;
	parse_tab[array_typet][ARRAY] = 19;
	parse_tab[lowt][NUM] = 20;
	parse_tab[topt][NUM] = 21;
	parse_tab[record_typet][RECORD] = 22;
	parse_tab[field_declistt][INTEGER] = 23;
	parse_tab[field_declistt][CHAR] = 23;
	parse_tab[field_declistt][ARRAY] = 24;
	parse_tab[field_decmoret][END] = 25;
	parse_tab[field_decmoret][INTEGER] = 26;
	parse_tab[field_decmoret][CHAR] = 26;
	parse_tab[field_decmoret][ARRAY] = 26;
	parse_tab[id_listt][ID] = 27;
	parse_tab[id_moret][SEMI] = 28;
	parse_tab[id_moret][COMMA] = 29;
	parse_tab[var_decpartt][PROCEDURE] = 30;
	parse_tab[var_decpartt][BEGIN] = 30;
	parse_tab[var_decpartt][VAR] = 31;
	parse_tab[var_dect][VAR] = 32;
	parse_tab[var_declistt][INTEGER] = 33;
	parse_tab[var_declistt][CHAR] = 33;
	parse_tab[var_declistt][ARRAY] = 33;
	parse_tab[var_declistt][RECORD] = 33;
	parse_tab[var_declistt][ID] = 33;
	parse_tab[var_decmoret][PROCEDURE] = 34;
	parse_tab[var_decmoret][BEGIN] = 34;
	parse_tab[var_decmoret][INTEGER] = 35;
	parse_tab[var_decmoret][CHAR] = 35;
	parse_tab[var_decmoret][ARRAY] = 35;
	parse_tab[var_decmoret][RECORD] = 35;
	parse_tab[var_decmoret][ID] = 35;
	parse_tab[var_id_listt][ID] = 36;
	parse_tab[var_id_moret][SEMI] = 37;
	parse_tab[var_id_moret][COMMA] = 38;
	parse_tab[proc_decpartt][BEGIN] = 39;
	parse_tab[proc_decpartt][PROCEDURE] = 40;
	parse_tab[proc_dect][PROCEDURE] = 41;
	parse_tab[proc_decmoret][BEGIN] = 42;
	parse_tab[proc_decmoret][PROCEDURE] = 43;
	parse_tab[procnamet][ID] = 44;
	parse_tab[paramlistt][RPAREN] = 45;
	parse_tab[paramlistt][INTEGER] = 46;
	parse_tab[paramlistt][CHAR] = 46;
	parse_tab[paramlistt][ARRAY] = 46;
	parse_tab[paramlistt][RECORD] = 46;
	parse_tab[paramlistt][ID] = 46;
	parse_tab[paramlistt][VAR] = 46;
	parse_tab[param_declistt][INTEGER] = 47;
	parse_tab[param_declistt][CHAR] = 47;
	parse_tab[param_declistt][ARRAY] = 47;
	parse_tab[param_declistt][RECORD] = 47;
	parse_tab[param_declistt][ID] = 47;
	parse_tab[param_declistt][VAR] = 47;
	parse_tab[param_moret][RPAREN] = 48;
	parse_tab[param_moret][SEMI] = 49;
	parse_tab[paramt][INTEGER] = 50;
	parse_tab[paramt][CHAR] = 50;
	parse_tab[paramt][ARRAY] = 50;
	parse_tab[paramt][RECORD] = 50;
	parse_tab[paramt][ID] = 50;
	parse_tab[paramt][VAR] = 51;
	parse_tab[form_listt][ID] = 52;
	parse_tab[fid_moret][SEMI] = 53;
	parse_tab[fid_moret][RPAREN] = 53;
	parse_tab[fid_moret][COMMA] = 54;
	parse_tab[proc_decpt][TYPE] = 55;
	parse_tab[proc_decpt][VAR] = 55;
	parse_tab[proc_decpt][PROCEDURE] = 55;
	parse_tab[proc_decpt][BEGIN] = 55;
	parse_tab[proc_bodyt][BEGIN] = 56;
	parse_tab[program_bodyt][BEGIN] = 57;
	parse_tab[stmt_listt][ID] = 58;
	parse_tab[stmt_listt][IF] = 58;
	parse_tab[stmt_listt][WHILE] = 58;
	parse_tab[stmt_listt][RETURN] = 58;
	parse_tab[stmt_listt][READ] = 58;
	parse_tab[stmt_listt][WRITE] = 58;
	parse_tab[stmt_moret][ELSE] = 59;
	parse_tab[stmt_moret][FI] = 59;
	parse_tab[stmt_moret][END] = 59;
	parse_tab[stmt_moret][ENDWH] = 59;
	parse_tab[stmt_moret][SEMI] = 60;
	parse_tab[stmtt][IF] = 61;
	parse_tab[stmtt][WHILE] = 62;
	parse_tab[stmtt][READ] = 63;
	parse_tab[stmtt][WRITE] = 64;
	parse_tab[stmtt][RETURN] = 65;
	parse_tab[stmtt][ID] = 66;
	parse_tab[assign_callt][ASSIGN] = 67;
	parse_tab[assign_callt][LPAREN] = 68;
	parse_tab[assign_restt][LMIDPAREN] = 69;
	parse_tab[assign_restt][DOT] = 69;
	parse_tab[assign_restt][ASSIGN] = 69;
	parse_tab[conditional_stmtt][IF] = 70;
	parse_tab[loop_stmtt][WHILE] = 71;
	parse_tab[input_stmtt][READ] = 72;
	parse_tab[invart][ID] = 73;
	parse_tab[output_stmtt][WRITE] = 74;
	parse_tab[return_stmtt][RETURN] = 75;
	parse_tab[call_restt][LPAREN] = 76;
	parse_tab[act_param_listt][RPAREN] = 77;
	parse_tab[act_param_listt][LPAREN] = 78;
	parse_tab[act_param_listt][NUM] = 78;
	parse_tab[act_param_listt][ID] = 78;
	parse_tab[act_param_moret][RPAREN] = 79;
	parse_tab[act_param_moret][COMMA] = 80;
	parse_tab[rel_expt][LPAREN] = 81;
	parse_tab[rel_expt][NUM] = 81;
	parse_tab[rel_expt][ID] = 81;
	parse_tab[other_relet][LT] = 82;
	parse_tab[other_relet][EQ] = 82;
	parse_tab[exprt][LPAREN] = 83;
	parse_tab[exprt][NUM] = 83;
	parse_tab[exprt][ID] = 83;
	parse_tab[other_termt][LT] = 84;
	parse_tab[other_termt][EQ] = 84;
	parse_tab[other_termt][RMIDPAREN] = 84;
	parse_tab[other_termt][THEN] = 84;
	parse_tab[other_termt][ELSE] = 84;
	parse_tab[other_termt][FI] = 84;
	parse_tab[other_termt][DO] = 84;
	parse_tab[other_termt][ENDWH] = 84;
	parse_tab[other_termt][RPAREN] = 84;
	parse_tab[other_termt][END] = 84;
	parse_tab[other_termt][SEMI] = 84;
	parse_tab[other_termt][COMMA] = 84;
	parse_tab[other_termt][PLUS] = 85;
	parse_tab[other_termt][MINUS] = 85;
	parse_tab[termt][LPAREN] = 86;
	parse_tab[termt][NUM] = 86;
	parse_tab[termt][ID] = 86;
	parse_tab[other_factort][PLUS] = 87;
	parse_tab[other_factort][MINUS] = 87;
	parse_tab[other_factort][LT] = 87;
	parse_tab[other_factort][EQ] = 87;
	parse_tab[other_factort][RMIDPAREN] = 87;
	parse_tab[other_factort][THEN] = 87;
	parse_tab[other_factort][ELSE] = 87;
	parse_tab[other_factort][FI] = 87;
	parse_tab[other_factort][DO] = 87;
	parse_tab[other_factort][ENDWH] = 87;
	parse_tab[other_factort][RPAREN] = 87;
	parse_tab[other_factort][END] = 87;
	parse_tab[other_factort][SEMI] = 87;
	parse_tab[other_factort][COMMA] = 87;
	parse_tab[other_factort][TIMES] = 88;
	parse_tab[other_factort][OVER] = 88;
	parse_tab[factort][LPAREN] = 89;
	parse_tab[factort][NUM] = 90;
	parse_tab[factort][ID] = 91;
	parse_tab[variablet][ID] = 92;
	parse_tab[vari_moret][ASSIGN] = 93;
	parse_tab[vari_moret][TIMES] = 93;
	parse_tab[vari_moret][OVER] = 93;
	parse_tab[vari_moret][PLUS] = 93;
	parse_tab[vari_moret][MINUS] = 93;
	parse_tab[vari_moret][LT] = 93;
	parse_tab[vari_moret][EQ] = 93;
	parse_tab[vari_moret][THEN] = 93;
	parse_tab[vari_moret][ELSE] = 93;
	parse_tab[vari_moret][FI] = 93;
	parse_tab[vari_moret][DO] = 93;
	parse_tab[vari_moret][ENDWH] = 93;
	parse_tab[vari_moret][RPAREN] = 93;
	parse_tab[vari_moret][END] = 93;
	parse_tab[vari_moret][SEMI] = 93;
	parse_tab[vari_moret][COMMA] = 93;
	parse_tab[vari_moret][LMIDPAREN] = 94;
	parse_tab[vari_moret][DOT] = 95;
	parse_tab[field_vart][ID] = 96;
	parse_tab[field_varmoret][ASSIGN] = 97;
	parse_tab[field_varmoret][TIMES] = 97;
	parse_tab[field_varmoret][OVER] = 97;
	parse_tab[field_varmoret][PLUS] = 97;
	parse_tab[field_varmoret][MINUS] = 97;
	parse_tab[field_varmoret][LT] = 97;
	parse_tab[field_varmoret][EQ] = 97;
	parse_tab[field_varmoret][THEN] = 97;
	parse_tab[field_varmoret][ELSE] = 97;
	parse_tab[field_varmoret][FI] = 97;
	parse_tab[field_varmoret][DO] = 97;
	parse_tab[field_varmoret][ENDWH] = 97;
	parse_tab[field_varmoret][RPAREN] = 97;
	parse_tab[field_varmoret][END] = 97;
	parse_tab[field_varmoret][SEMI] = 97;
	parse_tab[field_varmoret][COMMA] = 97;
	parse_tab[field_varmoret][LMIDPAREN] = 98;
	parse_tab[cmp_opt][LT] = 99;
	parse_tab[cmp_opt][EQ] = 100;
	parse_tab[add_opt][PLUS] = 101;
	parse_tab[add_opt][MINUS] = 102;
	parse_tab[mult_opt][TIMES] = 103;
	parse_tab[mult_opt][OVER] = 104;
}

void predict(int pnum)
{
	predic[pnum]();
}

/* for oprand */
int priority(enum LEXTYPE symb)
{
	int prior = -1;

	if (symb == END)
		prior = 0;
	else if (symb == LT || symb == EQ)
		prior = 1;
	else if (symb == PLUS || symb == MINUS)
		prior = 2;
	else if (symb == TIMES || symb == OVER)
		prior = 3;
	return prior;
}

/* 1. Program ::= ProgramHead DeclarePart ProgramBody  */
void process1(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = DOT;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process56;
	ntnum = program_bodyt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process4;
	ntnum = declare_partt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process2;
	ntnum = program_headt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 2. ProgramHead ::= PROGRAM ProgramName {program} */
void process2(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process3;
	ntnum = program_namet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = PROGRAM;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 3. ProgramName ::= ID {ID} */
void process3(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 4. DeclarePart ::= TypeDecpart VarDecPart ProcDecPart */
void process4(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process39;
	ntnum = proc_decpartt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process30;
	ntnum = var_decpartt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process5;
	ntnum = type_decpartt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*5. TypeDecPart ::= $  */
void process5(void)
{
}

/*6. 	           | TypeDec */
void process6(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process7;
	ntnum = type_dect;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 7. TypeDec ::= TYPE TypeDecList */
void process7(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process8;
	ntnum = type_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = TYPE;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);

}

/* 8. TypeDecList ::= TypeId = TypeDef; TypeDecMore */
void process8(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process9;
	ntnum = type_decmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process12;
	ntnum = type_deft;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = EQ;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process11;
	ntnum = type_idt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 9. TypeDecMore ::= $ */
void process9(void)
{
}

/* 10.              | TypeDecList */
void process10(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process8;
	ntnum = type_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 11. TypeId ::= ID */
void process11(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 12. TypeDef ::= BaseType */
void process12(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process12;
	ntnum = base_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 13.		 | StructreType */
void process13(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process13;
	ntnum = struct_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 14.	         | ID*/
void process14(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 15. BaseType ::= INTEGER */
void process15(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = INTEGER;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 16.		  | CHAR */
void process16(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = CHAR;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 17. StructureType ::= ArrayType */
void process17(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process19;
	ntnum = array_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 18.		       | RecType */
void process18(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process22;
	ntnum = record_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 19. ArrayType ::= ARRAY[Low..Top] OF BaseType */
void process19(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process15;
	ntnum = base_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = OF;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = RMIDPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process21;
	ntnum = topt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = UNDERANGE;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process20;
	ntnum = lowt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LMIDPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ARRAY;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 20. Low ::= INTC */
void process20(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = NUM;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 21. Top ::= INTC */
void process21(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = NUM;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 22. RecType ::= RECORD FieldDecList END */
void process22(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = END;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process23;
	ntnum = field_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = RECORD;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 23. FielDecList ::= BaseType IdList; FieldDecMore */
void process23(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process25;
	ntnum = field_decmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process27;
	ntnum = id_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process15;
	ntnum = base_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 24.   	     | ArrayType IdList; FieldDecMore */
void process24(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process25;
	ntnum = field_decmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process27;
	ntnum = id_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process19;
	ntnum = array_typet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 25. FieldDecMore ::= $ */
void process25(void)
{
}

/* 26.		      | FieldDecList */
void process26(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process23;
	ntnum = field_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 27. IdList ::= ID IdMore */
void process27(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process28;
	ntnum = id_moret;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 28. IdMore ::= $ */
void process28(void)
{
}

/* 29.		| , IdList */
void process29(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process27;
	ntnum = id_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = COMMA;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 30. VarDecPart ::= $ */
void process30(void)
{
}

/* 31.		    | VarDec */
void process31(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process32;
	ntnum = var_dect;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 32. VarDec ::= VAR VarDecList */
void process32(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process33;
	ntnum = var_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = VAR;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 33. VarDecList ::= TypeDef VarIdList; VarDecMore */
void process33(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process34;
	ntnum = var_decmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process36;
	ntnum = var_id_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process12;
	ntnum = type_deft;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 34. VarDecMore ::= $ */
void process34(void)
{
}

/* 35. 		    | VarDecList */
void process35(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process33;
	ntnum = var_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 36. VarIdList ::= ID VarIdMore */
void process36(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process37;
	ntnum = var_id_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 37. VarIdMore ::= $ */
void process37(void)
{
}

/*38.		   | , VarIdList */
void process38(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process36;
	ntnum = var_id_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = COMMA;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*39. ProcDecPart ::= $ */
void process39(void)
{
}

/*40.		    | ProcDec */
void process40(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process41;
	ntnum = proc_dect;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*41. ProcDec ::= PROCEDURE
	       ProcName (ParamList);
	       ProcDecPt
	       ProcBody
	       ProcDecMore
*/
void process41(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process42;
	ntnum = proc_decmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process56;
	ntnum = proc_bodyt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process55;
	ntnum = proc_decpt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = RPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process45;
	ntnum = paramlistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process44;
	ntnum = procnamet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = PROCEDURE;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 42. ProcDecMore ::= $ */
void process42(void)
{
}

/* 43.		     | ProcDec */
void process43(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process41;
	ntnum = proc_dect;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 44. ProcName ::= ID */
void process44(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 45. ParamList ::= $ */
void process45(void)
{
}

/* 46.		   | ParamDecList */
void process46(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process47;
	ntnum = param_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 47. ParamDecList ::= Param ParamMore */
void process47(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process48;
	ntnum = param_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process50;
	ntnum = paramt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 48. ParamMore ::= $ */
void process48(void)
{
}

/* 49.		   | ; ParamDecList */
void process49(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process47;
	ntnum = param_declistt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 50. Param ::= TypeDef FormList*/
void process50(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process52;
	ntnum = form_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process12;
	ntnum = type_deft;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 51.	       | VAR TypeDef FormList */
void process51(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process52;
	ntnum = form_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process12;
	ntnum = type_deft;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = VAR;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 52. FormList :: ID FidMore */
void process52(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process53;
	ntnum = fid_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 53. FidMore ::= $ */
void process53(void)
{
}

/* 54.		 | . FormList */
void process54(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process52;
	ntnum = form_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = DOT;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 55. ProcDecPt ::= DecLarePart */
void process55(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process4;
	ntnum = declare_partt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 56. ProcBody ::= ProgramBody */
void process56(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process57;
	ntnum = program_bodyt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 57. ProgramBody ::= BEGIN StmList END */
void process57(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = END;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process58;
	ntnum = stmt_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = BEGIN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 58. StmList ::= Stm StmMore */
void process58(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process59;
	ntnum = stmt_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process61;
	ntnum = stmtt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/* 59. StmMore ::= $ */
void process59(void)
{
}

/* 60. 	         | ; StmList */
void process60(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process58;
	ntnum = stmt_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = SEMI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 61. Stm ::= ConditionalStm */
void process61(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process70;
	ntnum = conditional_stmtt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*62. 	     | LoopStm */
void process62(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process71;
	ntnum = loop_stmtt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*63.	     | InputStm */
void process63(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process72;
	ntnum = input_stmtt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*64. 	     | OutputStm */
void process64(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process74;
	ntnum = output_stmtt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*65. 	     | ReturnStm */
void process65(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process75;
	ntnum = return_stmtt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*66. 	     | ID AssCall */
void process66(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process67;
	ntnum = assign_callt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/**67. AssCall ::= AssignmentRest */
void process67(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process69;
	ntnum = assign_restt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*68.		 | CallStmRest */
void process68(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process76;
	ntnum = call_restt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*69. AssignmentRest ::= VariMore := Exp */
void process69(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ASSIGN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process93;
	ntnum = vari_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*70. ConditionalStm ::= IF RelExcp THEN StmList ELSE StmList FI */
void process70(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = FI;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process58;
	ntnum = stmt_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ELSE;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process58;
	ntnum = stmt_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = THEN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process81;
	ntnum = rel_expt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = IF;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*71. LoopStm ::= WHILE RelExp DO StmList ENDWH */
void process71(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = ENDWH;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process58;
	ntnum = stmt_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = DO;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process81;
	ntnum = rel_expt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = WHILE;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*72. InputStm ::= READ ( Invar ) */
void process72(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process73;
	ntnum = invart;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = READ;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*73. Invar ::= ID */
void process73(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/* 74. OutputStm ::= WRITE ( Exp ) */
void process74(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = WRITE;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*75. ReturnStm ::= RETURN */
void process75(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RETURN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*76. CallStmRest ::= ( ActParamList ) */
void process76(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process77;
	ntnum = act_param_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*77. ActParamList ::= $ */
void process77(void)
{
}

/*78. 		     | Exp ActParaMore */
void process78(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process79;
	ntnum = act_param_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*79. ActParamMore ::= $ */
void process79(void)
{
}

/*80.		     | , ActParamList */
void process80(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process77;
	ntnum = act_param_listt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = COMMA;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*81. RelExp ::= Exp OthreRelE */
void process81(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process82;
	ntnum = other_relet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}


/*82. OtherRelE ::= CmpOp Exp */
void process82(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process99;
	ntnum = cmp_opt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*83. Exp ::= Term OtherTerm */
void process83(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process84;
	ntnum = other_termt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process86;
	ntnum = termt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*84. OtherTerm ::= $ */
void process84(void)
{
}

/*85.		  | AddOp Exp */
void process85(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process101;
	ntnum = add_opt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*86. Term ::= Factor OtherFactor */
void process86(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process87;
	ntnum = other_factort;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process89;
	ntnum = factort;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*87. OtherFactor ::= $ */
void process87(void)
{
}

/*88.		    | MultOp Term */
void process88(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process86;
	ntnum = termt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process103;
	ntnum = mult_opt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*89. Factor ::= ( Exp ) */
void process89(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*90.	       | INTC */
void process90(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = NUM;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*91.	       | Variable */
void process91(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process92;
	ntnum = variablet;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
}

/*92. Variable ::= ID VariMore */
void process92(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process93;
	ntnum = vari_moret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*93. VariMore ::= $ */
void process93(void)
{
}

/*94.       	 | [ Exp ] */
void process94(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RMIDPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LMIDPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*95.		 | . FieldVarMore */
void process95(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process97;
	ntnum = field_varmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = DOT;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*96. FieldVar ::= ID FieldVarMore */
void process96(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.fp = process97;
	ntnum = field_varmoret;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = ID;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*97. FieldVarMore ::= $ */
void process97(void)
{
}

/*98. 		     | [ Exp ] */
void process98(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = RMIDPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
	s.fp = process83;
	ntnum = exprt;
	is_nterminal = 1;
	push_symb(s, is_nterminal, ntnum);
	s.terminal = LMIDPAREN;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}

/*99. 
 * CmpOp ::= < 
*/
void process99(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = LT;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);


}

/*100.
           | = 
*/
void process100(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = EQ;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);


}

/*101.
 * AddOp ::= + 
 */
void process101(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = PLUS;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);


}

/*102.	      
 *         | - 
*/
void process102(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = MINUS;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);


}

/*103.
 * MultOp ::= * 
*/
void process103(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = TIMES;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);


}

/*104.	     
            | / 
*/
void process104(void)
{
	enum N_TERMINAL ntnum;
	int is_nterminal;
	union symb_element s;

	s.terminal = OVER;
	ntnum = -1;
	is_nterminal = 0;
	push_symb(s, is_nterminal, ntnum);
}
