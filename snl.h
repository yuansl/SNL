#ifndef SNL_H_
#define SNL_H_

#define RESERVEWD 23
#define NTSIZE 67     /* Not-terminal symbol */
#define LEXNO 42
#define LEN 80

enum STATUS {
	START, INASSIGN, INCOMMENT, INNUM, INID, INCHAR, INRANGE, DONE
};

/* lex type of language SNL */
enum LEXTYPE {
	ENDFILE, ERROR, 
	/* reserve-word */
	PROGRAM, PROCEDURE, TYPE, VAR, IF, THEN,
	ELSE, FI, WHILE, DO, ENDWH, BEGIN, END, READ, WRITE,
	ARRAY, OF, RECORD, RETURN,
	INTEGER, CHAR,
	/* constant character symbol */
	ID, NUM, CHARACTER, 
	/* special symbol */
	ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, 
	LPAREN, RPAREN,
	DOT, COLON, SEMI, COMMA,
	LMIDPAREN, RMIDPAREN, 
	UNDERANGE
};

enum N_TERMINAL {
	programt, program_headt, program_namet, declare_partt, 
	type_decpartt, type_dect, type_declistt, type_decmoret,
	type_idt, type_deft, base_typet, struct_typet,
	array_typet, lowt, topt, record_typet, field_declistt,
	field_decmoret,	id_listt, id_moret, var_decpartt, var_dect,
	var_declistt, var_decmoret, var_id_listt, var_id_moret,
	proc_decpartt, proc_dect, proc_decmoret, procnamet, paramlistt,
	param_declistt,	param_moret, paramt, form_listt, fid_moret,
	proc_decpt, proc_bodyt, program_bodyt, stmt_listt, stmt_moret, 
	stmtt, assign_callt, assign_restt, conditional_stmtt,
	loop_stmtt, input_stmtt, invart, output_stmtt, return_stmtt,
	call_restt, act_param_listt, act_param_moret,
	rel_expt, other_relet, exprt, other_termt, termt, other_factort, 
	factort, variablet, vari_moret, field_vart, field_varmoret, 
	cmp_opt, add_opt, mult_opt
};

struct reservewd {
	char reservewd[LEN];
	int name;
};

struct token {
	int linum;
	enum LEXTYPE syntax;
	char semantic[LEN];
	struct token *next;
};

enum SYNTAX_NODE_KIND {
	PROK, PHEADK, TYPEK, VARK, PROCDECK, STMTLK, DECK, STMTK, EXPRK
};

enum DECLARATION_KIND {
	ARRAYK = 9, CHARK, INTEGERK, RECORDK, IDK
};

enum STATEMENT_KIND {
	IFK = 14, WHILEK, ASSIGNK, READK, WRITEK, CALLK, RETURNK
};

enum EXPRESSION_KIND {
	optionk = 21, constk, idk
};

enum TYPECHECK {
	VOIDT = 24, INTEGERT, BOOLEANT
};

enum VARKIND {
	IDV = 27, ARRAYMEMBV, FIELDMEMBV
};

enum VARPARAMTYPE {
	VALPARAM = 30, VARPARAM
};

struct array_attr {
	int low;
	int top;
	int child_type;
};

struct record_attr {
	struct syntax_tree_node *field;
};

struct procedure_attr {
	enum VARPARAMTYPE paramt;
};

struct express_attr {
	enum LEXTYPE op;     /* LT, EQ, PLUS, MINUS, TIMES, OVER */
	int val; 
	enum VARKIND varkind;
	enum TYPECHECK type;
};

struct attribute {
	struct array_attr *arr_attr;
	struct record_attr *recor_attr;
	struct procedure_attr *proc_attr;
	struct express_attr *exp_attr;
};

struct symtable {
};

struct id_name {
	char name[LEN];
	struct id_name *next;
};

struct syntax_tree_node {
	struct syntax_tree_node *child[3];
	struct syntax_tree_node *sibling;
	int linum;
	enum SYNTAX_NODE_KIND nkind;
	union {
		enum DECLARATION_KIND dec;
		enum STATEMENT_KIND stmt;
		enum EXPRESSION_KIND exp;
	} kind;
	int id_counter;
	struct id_name *id_name;
	struct symtable **table;
	/*type_name;*/
	struct attribute *attr;
};

union symb_element {
	enum LEXTYPE terminal;
	void (*fp) (void);       
};

struct symbol_table {
	int is_nterminal;    /* flag for the node: either op or N_TERMINAL */
	union symb_element symb;
	enum N_TERMINAL ntnum;       /* not-terminal number */
	struct symbol_table *next;
};

struct stack_syntax_tree {
	struct syntax_tree_node **t;
	struct stack_syntax_tree *next;
};

/* + - * / < = */
struct operation {
	struct operation *next;
};

/* Number */
struct operand {
	struct operand *next;
};

extern struct syntax_tree_node *tree;
extern struct token *curr_token;
extern enum STATUS status;
extern struct token *head;
extern char syn_t[][LEN];
extern int linum;

struct syntax_tree_node *creat_tree_node(void);
int is_match(enum LEXTYPE LEX);
void save_id_name(struct syntax_tree_node *t);
void next_token(void);
void match(enum LEXTYPE expect);
void print_tree(struct syntax_tree_node *t);

void push_symb(union symb_element top, int flag, int ntnum);
struct symbol_table *pop_symb(void);
struct symbol_table *top_symb(void);
void push_operation(struct operation *);
struct operation *pop_operation(void);
struct operation *top_operation(void);
void push_operand(struct operand *);
struct operand *pop_operand(void);
struct operand *top_operand(void);

void push_tree_node(struct syntax_tree_node **t);
struct syntax_tree_node **pop_tree_node(void);

struct token *get_token(const char *word);
enum LEXTYPE is_reservewd(const char *identifier);
void print_token(struct token *tken);
void erron(const char *err_msg, int lin);
struct token *process_line(const char *src);

/* recursive-descend */
struct syntax_tree_node *parse(void);
struct syntax_tree_node *program(void);
struct syntax_tree_node *program_head(void);
struct syntax_tree_node *declare_part(void);
struct syntax_tree_node *type_dec(void);
struct syntax_tree_node *type_declaration(void);
struct syntax_tree_node *type_declist(void);
struct syntax_tree_node *type_decmore(void);
void type_id(struct syntax_tree_node *t);
void type_def(struct syntax_tree_node *t);
void base_type(struct syntax_tree_node *t);
void struct_type(struct syntax_tree_node *t);
void array_type(struct syntax_tree_node *t);
void record_type(struct syntax_tree_node *t);
struct syntax_tree_node *field_declist(void);
struct syntax_tree_node *field_decmore(void);
void id_list(struct syntax_tree_node *t);
void id_more(struct syntax_tree_node *t);
struct syntax_tree_node *var_dec(void);
struct syntax_tree_node *var_declaration(void);
struct syntax_tree_node *var_declist(void);
struct syntax_tree_node *var_decmore(void);
void var_id_list(struct syntax_tree_node *t);
void var_id_more(struct syntax_tree_node *t);
struct syntax_tree_node *proc_dec(void);
struct syntax_tree_node *proc_declaration(void);
struct syntax_tree_node *proc_decmore(void);
void paramlist(struct syntax_tree_node *t);
struct syntax_tree_node *param_declist(void);
struct syntax_tree_node *param_more(void);
struct syntax_tree_node *param(void);
void form_list(struct syntax_tree_node *t);
void fid_more(struct syntax_tree_node *t);
struct syntax_tree_node *proc_decpart(void);
struct syntax_tree_node *proc_body(void);
struct syntax_tree_node *program_body(void);
struct syntax_tree_node *stmt_list(void);
struct syntax_tree_node *stmt_more(void);
struct syntax_tree_node *stmt(void);
struct syntax_tree_node *assign_call(void);
void assign_stmt_rest(struct syntax_tree_node *t);
struct syntax_tree_node *conditional_stmt(void);
struct syntax_tree_node *loop_stmt(void);
struct syntax_tree_node *input_stmt(void);
struct syntax_tree_node *output_stmt(void);
struct syntax_tree_node *return_stmt(void);
struct syntax_tree_node *call_stmt_rest(void);
struct syntax_tree_node *actual_param_list(void);
struct syntax_tree_node *actual_param_more(void);
struct syntax_tree_node *rel_exp(void);
struct syntax_tree_node *expr(void);
struct syntax_tree_node *term(void);
struct syntax_tree_node *factor(void);
struct syntax_tree_node *variable(void);
void variable_more(struct syntax_tree_node *t);
struct syntax_tree_node *field_var(void);
void field_var_more(struct syntax_tree_node *t);
/* ll1 parse */

struct syntax_tree_node *parsell(void);
void creat_parse_table(void);
void predict(int pnum);
int priority(enum LEXTYPE oprand);
/* 1. Program ::= ProgramHead DeclarePart ProgramBody {program} */
void process1(void);
/* 2. ProgramHead ::= PROGRAM ProgramName {program} */
void process2(void);
/* 3. ProgramName ::= ID {ID} */
void process3(void);
/* 4. DeclarePart ::= TypeDecpart VarDecPart ProcDecPart {TYPE,VAR, PROCEDURE, BEGIN} */
void process4(void);
/*5. TypeDecPart ::= $ {} */
void process5(void);
/*6. 	           | TypeDec */
void process6(void);
/* 7. TypeDec ::= TYPE TypeDecList */
void process7(void);
/* 8. TypeDecList ::= TypeId = TypeDef; TypeDecMore */
void process8(void);
/* 9. TypeDecMore ::= $ */
void process9(void);
/* 10.              | TypeDecList */
void process10(void);
/* 11. TypeId ::= ID */
void process11(void);
/* 12. TypeDef ::= BaseType */
void process12(void);
/* 13.		 | StructreType */
void process13(void);
/* 14.	         | ID*/
void process14(void);
/* 15. BaseType ::= INTEGER */
void process15(void);
/* 16.		  | CHAR */
void process16(void);
/* 17. StructureType ::= ArrayType */
void process17(void);
/* 18.		       | RecType */
void process18(void);
/* 19. ArrayType ::= ARRAY[Low..Top] OF BaseType */
void process19(void);
/* 20. Low ::= INTC */
void process20(void);
/* 21. Top ::= INTC */
void process21(void);
/* 22. RecType ::= RECORD FieldDecList END */
void process22(void);
/* 23. FielDecList ::= BaseType IdList; FieldDecMore */
void process23(void);
/* 24.   	     | ArrayType IdList; FieldDecMore */
void process24(void);
/* 25. FieldDecMore ::= $ */
void process25(void);
/* 26.		      | FieldDecList */
void process26(void);
/* 27. IdList ::= ID IdMore */
void process27(void);
/* 28. IdMore ::= $ */
void process28(void);
/* 29.		| , IdList */
void process29(void);
/* 30. VarDecPart ::= $ */
void process30(void);
/* 31.		    | VarDec */
void process31(void);
/* 32. VarDec ::= VAR VarDecList */
void process32(void);
/* 33. VarDecList ::= TypeDef VarIdList; VarDecMore */
void process33(void);
/* 34. VarDecMore ::= $ */
void process34(void);
/* 35. 		    | VarDecList */
void process35(void);
/* 36. VarIdList ::= ID VarIdMore */
void process36(void);
/* 37. VarIdMore ::= $ */
void process37(void);
/*38.		   | , VarIdList */
void process38(void);
/*39. ProcDecPart ::= $ */
void process39(void);
/*40.		    | ProcDec */
void process40(void);
/*41. ProcDec ::= PROCEDURE
	       ProcName (ParamList);
	       ProcDecP
	       ProcBody
	       ProcDecMore
*/
void process41(void);
/* 42. ProcDecMore ::= $ p*/
void process42(void);
/* 43.		     | ProcDecLaration */
void process43(void);
/* 44. ProcName ::= ID */
void process44(void);
/* 45. ParamList ::= $ */
void process45(void);
/* 46.		   | ParamDecList */
void process46(void);
/* 47. ParamDecList ::= Param ParamMore */
void process47(void);
/* 48. ParamMore ::= $ */
void process48(void);
/* 49.		   | ; ParamDecList */
void process49(void);
/* 50. Param ::= TypeDef FormList*/
void process50(void);
/* 51.	       | VAR TypeDef FormList */
void process51(void);
/* 52. FormList :: ID FidMore */
void process52(void);
/* 53. FidMore ::= $ */
void process53(void);
/* 54.		 | . FormList */
void process54(void);
/* 55. ProcDecPt ::= DecLarePart */
void process55(void);
/* 56. ProcBody ::= ProgramBody */ 
void process56(void);
/* 57. ProgramBody ::= BEGIN StmList END */
void process57(void);
/* 58. StmList ::= Stm StmMore */
void process58(void);
/* 59. StmMore ::= $ */
void process59(void);
/* 60. 	         | ; StmList */
void process60(void);
/* 61. Stm ::= ConditionalStm */
void process61(void);
/*62. 	     | LoopStm */
void process62(void);
/*63.	     | InputStm */
void process63(void);
/*64. 	     | OutputStm */
void process64(void);
/*65. 	     | ReturnStm */
void process65(void);
/*66. 	     | ID AssCall */
void process66(void);
/**67. AssCall ::= AssignmentRest */
void process67(void);
/*68.		 | CallStmRest */
void process68(void);
/*69. AssignmentRest ::= VariMore := Exp */
void process69(void);
/*70. ConditionalStm ::= IF RelExcp THEN StmList ELSE StmList FI */
void process70(void);
/*71. LoopStm ::= WHILE RelExp DO StmList ENDWH */
void process71(void);
/*72. InputStm ::= READ ( Invar ) */
void process72(void);
/*73. Invar ::= ID */
void process73(void);
/* 74. OutputStm ::= WRITE ( Exp ) */
void process74(void);
/*75. ReturnStm ::= RETURN */
void process75(void);
/*76. CallStmRest ::= ( ActParamList ) */
void process76(void);
/*77. ActParamList ::= $ */
void process77(void);
/*78. 		     | Exp ActParaMore */
void process78(void);
/*79. ActParamMore ::= $ */
void process79(void);
/*80.		     | , ActParamList */
void process80(void);
/*81. RelExp ::= Exp OthreRelE */
void process81(void);
/*82. OtherRelE ::= CmpOp Exp */
void process82(void);
/*83. Exp ::= Term OtherTerm */
void process83(void);
/*84. OtherTerm ::= $ */
void process84(void);
/*85.		  | AddOp Exp */
void process85(void);
/*86. Term ::= Factor OtherFactor */
void process86(void);
/*87. OtherFactor ::= $ */
void process87(void);
/*88.		    | MultOp Term */
void process88(void);
/*89. Factor ::= ( Exp ) */
void process89(void);
/*90.	       | INTC */
void process90(void);
/*91.	       | Variable */
void process91(void);
/*92. Variable ::= ID VariMore */
void process92(void);
/*93. VariMore ::= $ */
void process93(void);
/*94.       	 | [ Exp ] */
void process94(void);
/*95.		 | . FieldVarMore */
void process95(void);
/*96. FieldVar ::= ID FieldVarMore */
void process96(void);
/*97. FieldVarMore ::= $ */
void process97(void);
/*98. 		     | [ Exp ] */
void process98(void);
/*99. 
 * CmpOp ::= < 
*/
void process99(void);
/*100.
           | = 
*/
void process100(void);
/*101.
 * AddOp ::= + 
 */
void process101(void);
/*102.	      
 *         | - 
*/
void process102(void);
/*103.
 * MultOp ::= * 
*/
void process103(void);
/*104.	     
            | / 
*/
void process104(void);

#endif

