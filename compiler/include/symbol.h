#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "baseAST.h"
#include "type.h"
#include "analysis.h"


class Stmt;
class ASymbol;
class SymScope;
class MPosition;

enum varType {
  VAR_NORMAL,
  VAR_CONFIG,
  VAR_STATE
};

class Symbol : public BaseAST {
 public:
  char* name;
  char* cname;   /* Name of symbol for generating C code */
  Type* type;
  Stmt* defPoint; /* Point of definition in AST */

  SymScope* parentScope;
  ASymbol *asymbol;

  Symbol(astType_t astType, char* init_name, Type* init_type = dtUnknown);
  void setParentScope(SymScope* init_parentScope);

  bool isNull(void);

  Symbol* copyList(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  Symbol* copy(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseSymbol(Traversal* traverse);
  virtual void traverseDefSymbol(Traversal* traverse);

  virtual bool isConst(void);

  void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  void printDefList(FILE* outfile, char* separator);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  void codegenDefList(FILE* outfile, char* separator);
  void setDefPoint(Stmt* stmt);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


extern Symbol* nilSymbol;


class UnresolvedSymbol : public Symbol {
 public:
  UnresolvedSymbol(char* init_name, char* init_cname = NULL);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void traverseDefSymbol(Traversal* traverse);

  void codegen(FILE* outfile);
};


class VarSymbol : public Symbol {
 public:
  varType varClass;
  bool isConstant;
  Expr* init;

  VarSymbol(char* init_name, Type* init_type = dtUnknown,
	    Expr* init_expr = nilExpr,
	    varType init_varClass = VAR_NORMAL, bool init_isConstant = false);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual void traverseDefSymbol(Traversal* traverse);

  bool initializable(void);
  bool isConst(void);

  virtual void codegenDef(FILE* outfile);

  bool isNull(void);
  
  void printDef(FILE* outfile);
};

extern VarSymbol* nilVarSymbol;


class ParamSymbol : public Symbol {
 public:
  paramType intent;
  Expr* init;

  ParamSymbol(paramType init_intent, char* init_name, 
	      Type* init_type = dtUnknown, Expr* init_init = nilExpr);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, 
			     CloneCallback* analysis_clone);

  virtual void traverseDefSymbol(Traversal* traverse);

  bool requiresCPtr(void);
  bool requiresCopyBack(void);
  bool requiresCTmp(void);
  bool isConst(void);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
 public:
  TypeSymbol(char* init_name, Type* init_definition);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  virtual void traverseDefSymbol(Traversal* traverse);
  virtual void codegenDef(FILE* outfile);
};


class FnSymbol;
extern FnSymbol* nilFnSymbol;

typedef enum __method_type {
  NON_METHOD,
  PRIMARY_METHOD,
  SECONDARY_METHOD
} _method_type;

class FnSymbol : public Symbol {
 public:
  bool exportMe;
  Symbol* formals;
  Type* retType;
  Symbol* _this;
  Stmt* body;
  Symbol* classBinding;
  _method_type method_type;
  SymScope* paramScope;

  FnSymbol* overload;

  FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
	   Stmt* init_body, bool init_exportMe=false,
	   Symbol* init_classBinding = nilSymbol);
  FnSymbol(char* init_name, Symbol* init_classBinding = nilSymbol);
  void finishDef(Symbol* init_formals, Type* init_retType, Stmt* init_body,
		 SymScope* init_paramScope, bool init_exportMe=false);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  virtual void traverseDefSymbol(Traversal* traverse);

  FnSymbol* clone(CloneCallback* clone_callback, Map<BaseAST*,BaseAST*>* map);
  FnSymbol* order_wrapper(Map<MPosition *, MPosition *> *formals_to_actuals) 
    { assert(!"not implemented"); return 0; }
  FnSymbol* coercion_wrapper(Map<MPosition *, Symbol *> *coercion_substitutions);
  FnSymbol* default_wrapper(Vec<MPosition *> *defaults) 
    { assert(!"not implemented"); return 0; }
  FnSymbol* instantiate_generic(Map<Type *, Type *> *generic_substitutions) 
    { assert(!"not implemented"); return 0; }

  bool isNull(void);

  void codegenHeader(FILE* outfile);
  void codegenDef(FILE* outfile);

  static FnSymbol* mainFn;
  static void init(void);
};


class EnumSymbol : public Symbol {
 public:
  Expr* init;
  int val;

  EnumSymbol(char* init_name, Expr* init_init, int init_val = 0);
  virtual Symbol* copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  virtual void traverseDefSymbol(Traversal* traverse);
  void set_values(void);
  void codegenDef(FILE* outfile);
};


class ModuleSymbol : public Symbol {
 public:
  bool internal;
  Stmt* stmts;
  FnSymbol* initFn;

  SymScope* modScope;

  ModuleSymbol(char* init_name, bool init_internal);
  void setModScope(SymScope* init_modScope);
  virtual void traverseDefSymbol(Traversal* traverse);
  void startTraversal(Traversal* traversal);

  void codegenDef(void);
  void createInitFn(void);
  bool isFileModule(void);
};

#endif
