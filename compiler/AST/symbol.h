#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "baseAST.h"
#include "traversal.h"
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
  Type* type;
  SymScope* scope;
  ASymbol *asymbol;
  
  Symbol(astType_t astType, char* init_name, Type* init_type = dtUnknown);

  bool isNull(void);

  void traverse(Traversal* traverse);
  virtual void traverseSymbol(Traversal* traverse);

  void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  void printDefList(FILE* outfile, char* separator);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  void codegenDefList(FILE* outfile, char* separator);
  int getTypes(Vec<BaseAST *> &stmts);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


extern Symbol* nilSymbol;


class UseBeforeDefSymbol : public Symbol {
 public:
  UseBeforeDefSymbol(char* init_name);
};


class VarSymbol : public Symbol {
 public:
  varType varClass;
  bool isConst;

  VarSymbol(char* init_name, Type* init_type = dtUnknown, 
	    varType init_varClass = VAR_NORMAL, bool init_isConst = false);

  bool isNull(void);
  
  void printWithType(FILE* outfile);
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

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
 public:
  TypeSymbol(char* init_name, Type* init_definition);
};


class ClassSymbol : public TypeSymbol {
 public:
  ClassSymbol(char* init_name, ClassType* init_class);

  bool isNull();

  ClassType* getType(void);
};

extern ClassSymbol* nilClassSymbol;


class ReduceSymbol : public ClassSymbol {
 public:
  ReduceSymbol(char* init_name, ClassType* init_class);
};


class FnSymbol : public Symbol {
 public:
  bool exportMe;
  Symbol* formals;
  Stmt* body;

  FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
	    Stmt* init_body, bool init_exportMe=false);

  void traverseSymbol(Traversal*);

  void codegenDef(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &stmts);
  int getStmts(Vec<BaseAST *> &stmts);
};


class EnumSymbol : public Symbol {
 public:
  int val;

  EnumSymbol(char* init_name, int init_val);
};

#endif
