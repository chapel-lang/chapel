/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/

#ifndef _gram_H_
#define _gram_H_

#define EOF_SENTINAL		"\377"
#define NO_PROD			0xFFFFFFFF

struct Production;
struct Rule;
struct Elem;
struct Term;
struct State;
struct ScanState;
struct ScanStateTransition;
struct D_ParserTables;

typedef struct Elem Item;

typedef struct Code {
  char 	*code;
  int	line;
} Code;

typedef struct Goto {
  struct Elem	*elem;
  struct State	*state;
} Goto;
typedef Vec(Goto *) VecGoto;

typedef enum ActionKind { 
  ACTION_ACCEPT, ACTION_SHIFT, ACTION_REDUCE, ACTION_SHIFT_TRAILING
} ActionKind;
typedef struct Action {
  ActionKind		kind;
  struct Term		*term;
  struct Rule 		*rule;
  struct State 		*state;
  uint			index;
  char			*temp_string;
} Action;
typedef Vec(Action *) VecAction;

typedef struct Hint {
  uint		depth;
  struct State	*state;
  struct Rule	*rule;
} Hint;
typedef Vec(Hint *) VecHint;

typedef Vec(struct ScanStateTransition*) VecScanStateTransition;
typedef Vec(struct ScanState *) VecScanState;

typedef struct Scanner {
  VecScanState			states;
  VecScanStateTransition	transitions;
} Scanner;

typedef struct State {
  uint		index;
  uint64	hash;
  Vec(Item*)	items;
  Vec(Item*)	items_hash;
  VecGoto	gotos;
  VecAction	shift_actions;
  VecAction	reduce_actions;
  VecHint	right_epsilon_hints;
  VecHint	error_recovery_hints;
  Scanner	scanner;
  uint		accept:1;
  uint		scanner_code:1;
  uint		goto_on_token:1;
  uint		scan_kind:3;
  uint		trailing_context:1;
  uint8		*goto_valid;
  int		goto_table_offset;
  struct State	*same_shifts;
  struct State  *reduces_to;
  struct Rule   *reduces_with;
  struct Rule   *reduces_to_then_with;
} State;

#define ASSOC_LEFT   	0x0001
#define ASSOC_RIGHT   	0x0002
#define ASSOC_NARY   	0x0004
#define ASSOC_UNARY  	0x0008
#define ASSOC_BINARY 	0x0010

typedef enum AssocKind {
  ASSOC_NONE		= 0,
  ASSOC_NARY_LEFT 	= (ASSOC_NARY|ASSOC_LEFT),
  ASSOC_NARY_RIGHT 	= (ASSOC_NARY|ASSOC_RIGHT),
  ASSOC_UNARY_LEFT 	= (ASSOC_UNARY|ASSOC_LEFT),
  ASSOC_UNARY_RIGHT 	= (ASSOC_UNARY|ASSOC_RIGHT),
  ASSOC_BINARY_LEFT	= (ASSOC_BINARY|ASSOC_LEFT), 
  ASSOC_BINARY_RIGHT	= (ASSOC_BINARY|ASSOC_RIGHT),
  ASSOC_NO		= 0x0020
} AssocKind;
#define IS_RIGHT_ASSOC(_x)	((_x) & ASSOC_RIGHT)
#define IS_LEFT_ASSOC(_x)	((_x) & ASSOC_LEFT)
#define IS_NARY_ASSOC(_x)	((_x) & ASSOC_NARY)
#define IS_BINARY_ASSOC(_x)	((_x) & ASSOC_BINARY)
#define IS_UNARY_ASSOC(_x)	((_x) & ASSOC_UNARY)
#define IS_UNARY_BINARY_ASSOC(_x) (IS_BINARY_ASSOC(_x) || IS_UNARY_ASSOC(_x))
#define IS_BINARY_NARY_ASSOC(_x) (IS_BINARY_ASSOC(_x) || IS_NARY_ASSOC(_x))
/* not valid for NARY */
#define IS_EXPECT_RIGHT_ASSOC(_x) ((_x) && (_x) != ASSOC_UNARY_LEFT)
#define IS_EXPECT_LEFT_ASSOC(_x) ((_x) && _x != ASSOC_UNARY_RIGHT)
typedef struct Rule {
  uint			index;
  struct Production	*prod;
  int			op_priority;
  AssocKind		op_assoc;
  int			rule_priority;
  AssocKind		rule_assoc;
  Vec(struct Elem*)	elems;
  struct Elem		*end;
  Code			speculative_code;
  Code			final_code;
  Vec(Code*)		pass_code;
  int			action_index;
  struct Rule		*same_reduction;
} Rule;

typedef enum TermKind {
  TERM_STRING, TERM_REGEX, TERM_CODE, TERM_TOKEN
} TermKind;
typedef struct Term {
  TermKind		kind;
  uint			index;
  int			term_priority;
  char			*term_name;
  AssocKind		op_assoc;
  int			op_priority;
  char			*string;
  int			string_len;
  uint			scan_kind:3;
  uint			ignore_case:1;
  uint			trailing_context:1;
  struct Production	*regex_production;
} Term;

typedef Vec(Term *) TermVec;

typedef enum DeclarationKind {
  DECLARE_TOKENIZE, DECLARE_LONGEST_MATCH, DECLARE_ALL_MATCHES, 
  DECLARE_SET_OP_PRIORITY, DECLARE_STATES_FOR_ALL_NTERMS, 
  DECLARE_STATE_FOR, DECLARE_WHITESPACE, DECLARE_SAVE_PARSE_TREE, DECLARE_NUM
} DeclarationKind;
typedef struct Declaration {
  struct Elem *	elem;
  uint		kind;
  uint		index;
} Declaration;

typedef enum InternalKind {
  INTERNAL_NOT, INTERNAL_HIDDEN, INTERNAL_CONDITIONAL, INTERNAL_STAR, INTERNAL_PLUS
} InternalKind;

typedef struct Production {
  char			*name;
  uint			name_len;
  Vec(Rule *)		rules;
  uint			index;
  uint			regex:1;
  uint			in_regex:1;
  uint			internal:3;	/* production used for EBNF */
  uint			live:1;
  Rule			*nullable;	/* shortest rule for epsilon reduction */
  struct Production 	*declaration_group[DECLARE_NUM];
  struct Declaration 	*last_declaration[DECLARE_NUM];
  State			*state;	/* state for independent parsing of this productions*/
  struct Elem		*elem;	/* base elem for the item set of the above state */
  struct Term		*regex_term;	/* regex production terminal */
  char			*regex_term_name;
  struct Production	*next;
} Production;

typedef enum ElemKind {
  ELEM_NTERM, ELEM_TERM, ELEM_UNRESOLVED, ELEM_END
} ElemKind;
typedef struct Elem {
  ElemKind	kind;
  uint		index;
  Rule		*rule;
  union {
    Production	*nterm;
    Term	*term;
    void	*term_or_nterm;
    struct Unresolved {
      char	*string;
      uint	len;
    } unresolved;
  } e;
} Elem;

typedef struct Grammar {
  char			*pathname;
  Vec(Production *)	productions;
  Vec(Term *)		terminals;
  Vec(State *)		states;
  Code			scanner;
  Code			*code;
  int			ncode;
  Vec(Declaration *)	declarations;
  Vec(D_Pass *)		passes;
  char			*default_white_space;
  /* grammar construction options */
  int			set_op_priority_from_rule;
  int			right_recursive_BNF;
  int			states_for_whitespace;
  int			states_for_all_nterms;
  int			tokenizer;
  int			longest_match;
  int			save_parse_tree;
  /* grammar writing options */
  char			grammar_ident[256];
  int			scanner_blocks;
  int			scanner_block_size;
  int			write_line_directives;
  int			write_header;
  int			token_type;
  int			write_cpp;
  char			write_extension[256];
  /* temporary variables for grammar construction */
  struct Production *	p;
  struct Rule *		r;
  struct Elem *		e;
  int			action_index;
  int			action_count;
  int			pass_index;
  int			rule_index;
  int			write_line;
  char			*write_pathname;
} Grammar;

/* automatically add %op_XXX to rightmost token of %XXX rule, default off */

Grammar *new_D_Grammar(char *pathname);
void free_D_Grammar(Grammar *g);
int build_grammar(Grammar *g);
int parse_grammar(Grammar *g, char *pathname, D_ParserTables *t, int sizeof_ParseNode_User);
void print_grammar(Grammar *g);
void print_states(Grammar *g);
void print_rule(Rule *r);
void print_term(Term *t);
Production *lookup_production(Grammar *g, char *name, int len);

/* for creating grammars */
#define last_elem(_r) ((_r)->elems.v[(_r)->elems.n-1])

Rule *new_rule(Grammar *g, Production *p);
Elem *new_elem_nterm(Production *p, Rule *r);
void new_declaration(Grammar *g, Elem *e, uint kind);
Production *new_production(Grammar *g, char *name);
Elem *new_string(Grammar *g, char *s, char *e, Rule *r);
Elem *new_ident(char *s, char *e, Rule *r);
void new_token(Grammar *g, char *s, char *e);
Elem *new_code(Grammar *g, char *s, char *e, Rule *r);
void add_global_code(Grammar *g, char *start, char *end, int line);
Production *new_internal_production(Grammar *g, Production *p);
Elem * dup_elem(Elem *e, Rule *r);
void add_declaration(Grammar *g, char *start, char *end, uint kind, uint line);
void add_pass(Grammar *g, char *start, char *end, uint kind, uint line);
void add_pass_code(Grammar *g, Rule *r, char *pass_start, char *pass_end,
		   char *code_start, char *code_end, uint line, uint pass_line);
D_Pass *find_pass(Grammar *g, char *start, char *end);
void conditional_EBNF(Grammar *g); /* applied to g->e,g->r,g->p */
void star_EBNF(Grammar *g); /* ditto */
void plus_EBNF(Grammar *g); /* ditto */
void initialize_productions(Grammar *g);
void finalize_productions(Grammar *g);
int state_for_declaration(Grammar *g, int iproduction);

#endif
