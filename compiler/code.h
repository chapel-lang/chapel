/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _code_h_
#define _code_h_

#include "extern.h"

class Label;
class AST;
class Prim;
class PNode;
class Sym;

enum Code_kind { 
  Code_SUB = 0, Code_MOVE, Code_SEND, Code_IF, Code_LABEL, 
  Code_GOTO, Code_SEQ, Code_CONC, Code_NOP
};

#define CPP_IS_LAME { "SUB", "MOVE", "SEND", "IF", "LABEL", "GOTO", "SEQ", "CONC", "NOP" }
EXTERN char *code_string[] EXTERN_INIT(CPP_IS_LAME);
#undef CPP_IS_LAME

class Code : public gc {
 public:
  Code_kind	kind;
  Vec<Sym *> 	rvals;
  Vec<Sym *> 	lvals;
  Label		*label[2];
  Vec<Code *> 	sub;
  AST		*ast;
  Prim		*prim;

  unsigned int	dead:1;	// used by if1.cpp
  Code		*cont;	// used by cfg.cpp
  PNode		*pn;	// used by cfg.cpp

  Code(Code_kind k = Code_SUB) { memset(this, 0, sizeof *this); kind = k; }
  int is_group() { return kind == Code_SUB || kind == Code_SEQ || kind == Code_CONC; }
};
#define forv_Code(_c, _v) forv_Vec(Code, _c, _v)

class Label : public gc {
 public:
  int			id;
  unsigned int		live:1;
  Code			*code;			// used by fun.cpp

  Label() { memset(this, 0, sizeof *this); }
};


#endif
