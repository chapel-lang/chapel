#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "alist.h"
#include "baseAST.h"
#include "analysis.h"
#include "type.h"

extern FnSymbol* chpl_main;

class DefExpr;
class Stmt;
class BlockStmt;
class ASymbol;
class SymScope;
class Immediate;
class BasicBlock;

enum fnType {
  FN_FUNCTION,
  FN_CONSTRUCTOR,
  FN_ITERATOR
};

enum varType {
  VAR_NORMAL,
  VAR_CONFIG,
  VAR_STATE
};

enum consType {
  VAR_VAR,
  VAR_CONST,
  VAR_PARAM
};

class Symbol : public BaseAST {
 public:
  char* name;
  char* cname; // Name of symbol for generating C code
  Type* type;
  DefExpr* defPoint; // Point of definition
  Expr* variableExpr;
  Vec<SymExpr*>* uses;

  ASymbol *asymbol;
  Symbol* overload;
  bool isUnresolved;

  Symbol(astType_t astType, char* init_name, Type* init_type = dtUnknown);
  virtual void verify(); 
  void setParentScope(SymScope* init_parentScope);
  COPY_DEF(Symbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseSymbol(Traversal* traverse);
  virtual void traverseDefSymbol(Traversal* traverse);

  virtual bool isConst(void);
  virtual bool isParam(void);
  bool isThis(void);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual FnSymbol* getFnSymbol(void);
  virtual Symbol* getSymbol(void);
  virtual Type* typeInfo(void);
  int nestingDepth();
  FnSymbol *nestingParent(int i);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


class UnresolvedSymbol : public Symbol {
 public:
  UnresolvedSymbol(char* init_name, char* init_cname = NULL);
  virtual void verify(); 
  COPY_DEF(UnresolvedSymbol);

  virtual void traverseDefSymbol(Traversal* traverse);

  void codegen(FILE* outfile);
};


class VarSymbol : public Symbol {
 public:
  varType varClass;
  consType consClass;
  Immediate *immediate;
  LiteralType *literalType;
  bool noDefaultInit;

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(char* init_name, Type* init_type = dtUnknown,
            varType init_varClass = VAR_NORMAL, 
            consType init_consClass = VAR_VAR);
  virtual void verify(); 
  COPY_DEF(VarSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverseDefSymbol(Traversal* traverse);

  bool isConst(void);
  //Roxana
  bool isParam(void);

  void print(FILE* outfile);
  void printDef(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
};


class ArgSymbol : public Symbol {
 public:
  intentTag intent;
  Expr* defaultExpr;
  Symbol *genericSymbol;
  bool isGeneric;
  bool isExactMatch;

  ArgSymbol(intentTag iIntent, char* iName, Type* iType,
            Expr* iDefaultExpr = NULL, Expr* iVariableExpr = NULL);
  virtual void verify(); 
  COPY_DEF(ArgSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverseDefSymbol(Traversal* traverse);

  bool requiresCPtr(void);
  bool isConst(void);
  bool isRef(void);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
 public:
  Type *definition;

  TypeSymbol(char* init_name, Type* init_definition);
  virtual void verify(); 
  COPY_DEF(TypeSymbol);
  TypeSymbol* clone(ASTMap* map);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverseDefSymbol(Traversal* traverse);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
};


class FnSymbol : public Symbol {
 public:
  TypeSymbol* typeBinding;
  AList<DefExpr>* formals;
  Type* retType;
  BlockStmt* where;
  Expr* retExpr;
  BlockStmt* body;
  fnType fnClass;
  bool noParens;
  bool retRef;

  SymScope* argScope;
  bool isSetter;
  bool isGeneric;
  bool hasVarArgs;
  Symbol* _this;
  Symbol* _setter; // the variable this function sets if it is a setter
  Symbol* _getter; // the variable this function gets if it is a getter
  bool isMethod;
  Vec<Symbol *> genericSymbols;
  FnSymbol *instantiatedFrom;
  ASTMap substitutions;
  Vec<FnSymbol *> *instantiatedTo;
  bool visible; // included in visible function list for dispatch
                // compiler generated functions are not visible, e.g.,
                // instantiated functions, wrappers, cloned functions
  Vec<BasicBlock*>* basicBlocks;
  Vec<CallExpr*>* calledBy;
  Vec<CallExpr*>* calls;

  FnSymbol(char* initName, TypeSymbol* initTypeBinding = NULL);
           
  virtual void verify(); 
  COPY_DEF(FnSymbol);
  virtual FnSymbol* getFnSymbol(void);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverseDefSymbol(Traversal* traverse);

  FnSymbol* clone(ASTMap* map);
  FnSymbol* promotion_wrapper(Map<Symbol*,Symbol*>* promotion_subs);
  FnSymbol* order_wrapper(Map<Symbol*,Symbol*>* formals_to_formals);
  FnSymbol* coercion_wrapper(Map<Symbol*,Symbol*>* coercion_substitutions);
  FnSymbol* default_wrapper(Vec<Symbol*>* defaults);
  bool isPartialInstantiation(ASTMap* generic_substitutions);
  FnSymbol* instantiate_generic(ASTMap* substitutions);
  FnSymbol* clone_generic(ASTMap* formal_types);
  void codegenHeader(FILE* outfile);
  void codegenPrototype(FILE* outfile);
  void codegenDef(FILE* outfile);

  void printDef(FILE* outfile);

  void insertAtHead(BaseAST* ast);
  void insertAtTail(BaseAST* ast);
};


class EnumSymbol : public Symbol {
 public:
  EnumSymbol(char* init_name);
  virtual void verify(); 
  COPY_DEF(EnumSymbol);
  virtual void traverseDefSymbol(Traversal* traverse);
  void codegenDef(FILE* outfile);
};


enum modType {
  MOD_INTERNAL, // intrinsic, prelude (no codegen)
  MOD_STANDARD, // standard modules require codegen, e.g., _chpl_complex
  MOD_USER,
  MOD_INSTANTIATED // for instantiated types, no init function is necessary
};


class ModuleSymbol : public Symbol {
 public:
  modType modtype;
  AList<Stmt>* stmts;
  FnSymbol* initFn;

  SymScope* modScope;

  ModuleSymbol(char* init_name, modType init_modtype);
  virtual void verify(); 
  COPY_DEF(ModuleSymbol);
  void setModScope(SymScope* init_modScope);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverseDefSymbol(Traversal* traverse);
  void startTraversal(Traversal* traversal);

  void codegenDef(void);
  bool isFileModule(void);

  static int numUserModules(Vec<ModuleSymbol*>* moduleList);
};


class LabelSymbol : public Symbol {
 public:
  LabelSymbol(char* init_name);
  virtual void verify(); 
  COPY_DEF(LabelSymbol);
  virtual void codegenDef(FILE* outfile);
};


TypeSymbol *new_UnresolvedTypeSymbol(char *init_name);
VarSymbol *new_StringSymbol(char *s);
VarSymbol *new_IntSymbol(long b);
VarSymbol *new_UIntSymbol(unsigned long long b);
VarSymbol *new_FloatSymbol(char *n, double b);
VarSymbol *new_ComplexSymbol(char *n, double r, double i);
VarSymbol *new_ImmediateSymbol(Immediate *imm);
VarSymbol *new_SymbolSymbol(char *str);
PrimitiveType *immediate_type(Immediate *imm);
int set_immediate_type(Immediate *imm, Type *); // -1 on failure

extern HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;
extern StringChainHash uniqueStringHash;

extern Symbol *gNil;
extern Symbol *gUnknown;
extern Symbol *gUnspecified;
extern Symbol *gVoid;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;

extern Vec<FnSymbol*> new_ast_functions;
extern Vec<TypeSymbol*> new_ast_types;

#endif
