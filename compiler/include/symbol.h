#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "baseAST.h"
#include "type.h"

class Stmt;
class ASymbol;
class SymScope;

enum varType {
  VAR_NORMAL,
  VAR_CONFIG,
  VAR_STATE
};

class Symbol : public BaseAST {
 public:
  char* name;
  char* cname; /* Name of symbol for generating C code */
  Type* type;
  SymScope* scope;
  ASymbol *asymbol;

  Symbol(astType_t astType, char* init_name, Type* init_type = dtUnknown);
  virtual Symbol* copy(void);
  Symbol* copyList(void);

  bool isNull(void);

  void traverse(Traversal* traverse, bool atTop = true);
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
 public:
  UnresolvedSymbol(char* init_name);
  virtual Symbol* copy(void);

  void codegen(FILE* outfile);
};


class VarSymbol : public Symbol {
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
  PARAM_IN = 0,
  PARAM_INOUT,
  PARAM_OUT,
  PARAM_CONST,

  NUM_PARAM_TYPES
};


class ParamSymbol : public Symbol {
 public:
  paramType usage;

  ParamSymbol(paramType init_usage, char* init_name, 
	      Type* init_type = dtUnknown);
  virtual Symbol* copy(void);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
 public:
  TypeSymbol(char* init_name, Type* init_definition);
  virtual Symbol* copy(void);
};


class FnSymbol;
extern FnSymbol* nilFnSymbol;

class FnSymbol : public Symbol {
 public:
  bool exportMe;
  Symbol* formals;
  Symbol* _this;
  Stmt* body;

  FnSymbol* parentFn;

  FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
	   Stmt* init_body, bool init_exportMe=false, 
	   FnSymbol* init_parentFn = nilFnSymbol);
  FnSymbol(char* init_name, FnSymbol* init_parentFn = nilFnSymbol);
  void finishDef(Symbol* init_formals, Type* init_retType, Stmt* init_body,
		 bool init_exportMe=false);
  virtual Symbol* copy(void);

  bool isNull(void);

  void codegenDef(FILE* outfile);

  static FnSymbol* mainFn;
  static void init(void);
};


class EnumSymbol : public Symbol {
 public:
  Expr* init;
  int val;

  EnumSymbol(char* init_name, Expr* init_init, int init_val = 0);
  virtual Symbol* copy(void);
  void set_values(void);
};


class ModuleSymbol : public Symbol {
 public:
  bool internal;
  Stmt* stmts;
  FnSymbol* initFn;

  ModuleSymbol(char* init_name, bool init_internal);

  void codegenDef(void);
  void createInitFn(void);
  bool isFileModule(void);
};

#endif
