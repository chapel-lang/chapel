#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "baseAST.h"
#include "type.h"

#define TRAVERSABLE_SYMBOL(name)                                                        \
 public:                                                                                \
  void traverse(name* &_this, Traversal* traversal, bool atTop = true) {        \
    if (isNull()) return;                                                               \
    if (traversal->processTop || !atTop) traversal->preProcessSymbol((Symbol*&)_this);  \
    if (atTop || traversal->exploreChildSymbols) _this->traverseSymbol(traversal);      \
    if (traversal->processTop || !atTop) traversal->postProcessSymbol((Symbol*&)_this); \
  };                                                                                    \
  void traverseList(name* &_this, Traversal* traversal, bool atTop = true) {    \
    if (isNull()) return;                                                               \
    TRAVERSE(_this, traversal, atTop);                                                  \
    TRAVERSE_LS(_this->next, traversal, atTop);                                         \
  }

class Stmt;
class ASymbol;
class SymScope;

enum varType {
  VAR_NORMAL,
  VAR_CONFIG,
  VAR_STATE
};

class Symbol : public BaseAST {
  TRAVERSABLE_SYMBOL(Symbol);
 public:
  char* name;
  char* cname; /* Name of symbol for generating C code */
  Type* type;

  SymScope* parentScope;
  ASymbol *asymbol;

  Symbol(astType_t astType, char* init_name, Type* init_type = dtUnknown);
  void setParentScope(SymScope* init_parentScope);
  virtual Symbol* copy(void);
  Symbol* copyList(void);

  bool isNull(void);

  virtual void traverseSymbol(Traversal* traverse);

  void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  void printDefList(FILE* outfile, char* separator);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  void codegenDefList(FILE* outfile, char* separator);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


extern Symbol* nilSymbol;


class UnresolvedSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(UnresolvedSymbol);
 public:
  UnresolvedSymbol(char* init_name);
  virtual Symbol* copy(void);

  void codegen(FILE* outfile);
};


class VarSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(VarSymbol);
 public:
  varType varClass;
  bool isConst;

  VarSymbol(char* init_name, Type* init_type = dtUnknown, 
	    varType init_varClass = VAR_NORMAL, bool init_isConst = false);
  virtual Symbol* copy(void);

  bool isNull(void);
  
  void printDef(FILE* outfile);
};

extern VarSymbol* nilVarSymbol;


enum paramType {
  PARAM_BLANK = 0,
  PARAM_IN,
  PARAM_INOUT,
  PARAM_OUT,
  PARAM_CONST,

  NUM_PARAM_TYPES
};


class ParamSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(ParamSymbol);
 public:
  paramType intent;

  ParamSymbol(paramType init_intent, char* init_name, 
	      Type* init_type = dtUnknown);
  virtual Symbol* copy(void);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(TypeSymbol);
 public:
  TypeSymbol(char* init_name, Type* init_definition);
  virtual Symbol* copy(void);
};


class FnSymbol;
extern FnSymbol* nilFnSymbol;

class FnSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(FnSymbol);
 public:
  bool exportMe;
  Symbol* formals;
  Symbol* _this;
  Stmt* body;
  SymScope* paramScope;

  FnSymbol* parentFn;
  FnSymbol* overload;

  FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
	   Stmt* init_body, bool init_exportMe=false, 
	   FnSymbol* init_parentFn = nilFnSymbol);
  FnSymbol(char* init_name, FnSymbol* init_parentFn = nilFnSymbol);
  void finishDef(Symbol* init_formals, Type* init_retType, Stmt* init_body,
		 SymScope* init_paramScope, bool init_exportMe=false);
  virtual Symbol* copy(void);

  bool isNull(void);

  void codegenDef(FILE* outfile);

  static FnSymbol* mainFn;
  static void init(void);
};


class EnumSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(EnumSymbol);
 public:
  Expr* init;
  int val;

  EnumSymbol(char* init_name, Expr* init_init, int init_val = 0);
  virtual Symbol* copy(void);
  void set_values(void);
};


class ModuleSymbol : public Symbol {
  TRAVERSABLE_SYMBOL(ModuleSymbol);
 public:
  bool internal;
  Stmt* stmts;
  FnSymbol* initFn;

  SymScope* modScope;

  ModuleSymbol(char* init_name, bool init_internal);
  void setModScope(SymScope* init_modScope);

  void startTraversal(Traversal* traversal);

  void codegenDef(void);
  void createInitFn(void);
  bool isFileModule(void);
};

#endif
