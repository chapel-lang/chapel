/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _IF1_h_
#define _IF1_h_

#include "Sym.h"
#include "Code.h"

class Scope;
class Primitives;

enum IF1_int_type { 
  IF1_INT_TYPE_8, IF1_INT_TYPE_16, IF1_INT_TYPE_32, IF1_INT_TYPE_64, 
  IF1_INT_TYPE_NUM
};

enum IF1_float_type { 
  IF1_FLOAT_TYPE_16, IF1_FLOAT_TYPE_32, IF1_FLOAT_TYPE_48, IF1_FLOAT_TYPE_64, 
  IF1_FLOAT_TYPE_80, IF1_FLOAT_TYPE_96, IF1_FLOAT_TYPE_112, IF1_FLOAT_TYPE_128, 
  IF1_FLOAT_TYPE_NUM
};

enum Builtin {
#define S(_x) Builtin_##_x,
#include "builtin_symbols.h"
#undef S
  Builtin_MAX
};

extern char *builtin_strings[];

class IF1 : public gc {
 public:
  StringOpenHash	strings;		// unique strings
  Map<char *, Sym *>	symbols;		// language level symbols
  Map<char *, Sym *>	builtins;		// symbols builtin to the compiler
  Map<char *, Sym *>	constants;		// unique constants (by string)
  Vec<Sym *>		allsyms;
  Vec<Label *>		alllabels; 
  Vec<Sym *>		allclosures;
  Sym 			*int_types[IF1_INT_TYPE_NUM][2]; // 0 = unsigned, 1 = signed
  Sym 			*float_types[IF1_FLOAT_TYPE_NUM];
  Sym 			*top;			// main function
  Primitives 		*primitives;
  
  IF1();
};

Sym	*if1_alloc_sym(IF1 *p, char *s = 0, char *e = 0);
Sym 	*if1_const(IF1 *p, Sym *typ, char *value);
Sym 	*if1_make_symbol(IF1 *p, char *name, char *end = 0);
void	if1_set_symbols_type(IF1 *p);
void	if1_set_builtin(IF1 *p, Sym *s, char *name, char *end = 0);
Sym	*if1_get_builtin(IF1 *p, char *name, char *end = 0);

Code	*if1_nop(IF1 *p, Code **c);
void	if1_gen(IF1 *p, Code **t, Code *a);
void	if1_seq(IF1 *p, Code **t, Code *a);
void	if1_conc(IF1 *p, Code **t, Code *a);
Code	*if1_move(IF1 *p, Code **c, Sym *a, Sym *b, AST *ast = 0);
Code	*if1_goto(IF1 *p, Code **t, Label *label = 0);
Label	*if1_alloc_label(IF1 *p);
Label	*if1_label(IF1 *p, Code **code, AST *ast = 0, Label *l = 0);
Sym	*if1_operator(IF1 *p, Code **t, Sym *a1, Sym *a2, Sym *a3); 
Code	*if1_send(IF1 *p, Code **c, int args, int results, ...);
Code	*if1_send1(IF1 *p, Code **c);
void 	if1_add_send_arg(IF1 *p, Code *c, Sym *a);
void	if1_add_send_result(IF1 *p, Code *c, Sym *r);
Code	*if1_if_goto(IF1 *p, Code **t, Sym ifcond, AST *ast = 0);
void	if1_set_goto(IF1 *p, Code *go, Label *label);
void	if1_if_label_true(IF1 *p, Code *ifcode, Label *l, AST *ast = 0);
void	if1_if_label_false(IF1 *p, Code *ifcode, Label *l, AST *ast = 0);
Code	*if1_if(IF1 *p, Code **t, 
		Code *ifcond, Sym *ifcond_var,
		Code *ifif, Sym *if_var, 
		Code *ifthen, Sym *then_var, 
		Sym *result, AST *ast = 0);
Code	*if1_loop(IF1 *p, Code **t, 
		  Label *cont, Label *brk,
		  Sym *cond_var, Code *before, 
		  Code *cond, Code *after, Code *body, AST *ast = 0);
Sym   	*if1_closure(IF1 *p, Sym *f, Code *c, int nargs, Sym **args);

void	if1_set_int_type(IF1 *p, Sym *t, int signd, int size);
void	if1_set_float_type(IF1 *p, Sym *t, int size);
char	*if1_cannonicalize_string(IF1 *p, char *start, char *end = 0);
void	if1_finalize(IF1 *p);
void	if1_write(IF1 *p, char *pathname);

// debugging
void	if1_dump(FILE *fp, Code *c);
void	if1_dump_code(FILE *fp, Code *code, int indent);
void	if1_dump_sym(FILE *fp, Sym *sym);


#endif
