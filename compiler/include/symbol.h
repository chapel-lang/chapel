#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "alist.h"
#include "baseAST.h"
#include "type.h"

extern FnSymbol* chpl_main;

class SymExpr;
class DefExpr;
class Stmt;
class BlockStmt;
class SymScope;
class Immediate;
class BasicBlock;
class IteratorInfo;

enum FnTag {
  FN_FUNCTION,
  FN_ITERATOR
};

enum RetTag {
  RET_VALUE,
  RET_VAR,
  RET_PARAM,
  RET_TYPE
};

enum IntentTag {
  INTENT_BLANK,
  INTENT_IN,
  INTENT_INOUT,
  INTENT_OUT,
  INTENT_CONST,
  INTENT_REF,
  INTENT_PARAM,
  INTENT_TYPE
};

enum ModTag {
  MOD_STANDARD,
  MOD_USER
};

class Symbol : public BaseAST {
 public:
  const char* name;
  const char* cname; // Name of symbol for generating C code
  Type* type;
  DefExpr* defPoint; // Point of definition
  Symbol* overloadNext;
  Symbol* overloadPrev;
  bool isUserAlias;    // is a user alias via '=>'
  bool isCompilerTemp;
  bool isExprTemp;
  bool isTypeVariable;
  bool canParam;       // can be a parameter (determined during resolution)
  bool canType;        // can be a type (determined during resolution)
  bool isConcurrent;   // can be accessed concurrently
  bool isExtern;       // external to Chapel, implemented in C
  Vec<const char*> pragmas;

  Symbol(AstTag astTag, const char* init_name, Type* init_type = dtUnknown);
  virtual ~Symbol();
  virtual void verify(); 
  void setParentScope(SymScope* init_parentScope);
  virtual bool inTree();
  virtual Type* typeInfo(void);
  COPY_DEF(Symbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual bool isConstant(void);
  virtual bool isParameter(void);

  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual FnSymbol* getFnSymbol(void);
  virtual bool isImmediate();

  const char* hasPragma(const char* str);
  void removePragma(const char* str);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


class VarSymbol : public Symbol {
 public:
  bool isConfig;
  bool isParam;
  bool isConst;
  Immediate   *immediate;

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(const char* init_name, Type* init_type = dtUnknown);
  ~VarSymbol();
  virtual void verify(); 
  COPY_DEF(VarSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool isConstant(void);
  bool isParameter(void);

  void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual bool isImmediate();
};


class ArgSymbol : public Symbol {
 public:
  IntentTag intent;
  BlockStmt* typeExpr;
  BlockStmt* defaultExpr;
  Expr* variableExpr;
  bool isGeneric;
  Type* instantiatedFrom;
  bool instantiatedParam;

  ArgSymbol(IntentTag iIntent, const char* iName, Type* iType,
            Expr* iTypeExpr = NULL, Expr* iDefaultExpr = NULL,
            Expr* iVariableExpr = NULL);

  virtual void verify(); 
  COPY_DEF(ArgSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool requiresCPtr(void);
  bool isConstant(void);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
 public:
  TypeSymbol(const char* init_name, Type* init_type);
  virtual void verify(); 
  COPY_DEF(TypeSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
};


class FnSymbol : public Symbol {
 public:
  AList formals;
  DefExpr* setter; // implicit setter argument to var functions
  Type* retType;
  BlockStmt* where;
  Expr* retExprType;
  BlockStmt* body;
  FnTag fnTag;
  RetTag retTag;
  bool noParens;
  bool defSetGet;
  IteratorInfo* iteratorInfo;
  SymScope* argScope;
  bool isGeneric;
  Symbol* _this;
  Symbol* _outer;
  bool isMethod;
  FnSymbol *instantiatedFrom;
  ASTMap substitutions;
  Vec<FnSymbol *> *instantiatedTo;
  CallExpr* visiblePoint; // global functions copied out see local functions
  SymScope* instantiationPoint; // point of instantiation
  bool visible; // included in visible function list for dispatch
                // compiler generated functions are not visible, e.g.,
                // instantiated functions and wrappers
  bool global; // function is globally visible e.g. class argument
               // WAW: temporary hack to get iterator-created methods
               // visible

  Vec<BasicBlock*>* basicBlocks;
  Vec<CallExpr*>* calledBy;
  bool isWrapper;
  const char* userString;
  FnSymbol* valueFunction; // pointer to value function (created in
                           // resolve and used in cullOverReferences)

  FnSymbol(const char* initName);
  ~FnSymbol();
           
  virtual void verify(); 
  COPY_DEF(FnSymbol);
  virtual FnSymbol* getFnSymbol(void);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  FnSymbol* promotion_wrapper(Map<Symbol*,Symbol*>* promotion_subs, bool isSquare);
  FnSymbol* order_wrapper(Map<Symbol*,Symbol*>* formals_to_formals, bool isSquare);
  FnSymbol* coercion_wrapper(ASTMap* coercion_substitutions, Map<ArgSymbol*,bool>* coercions, bool isSquare);
  FnSymbol* default_wrapper(Vec<Symbol*>* defaults, Map<Symbol*,Symbol*>* paramMap, bool isSquare);
  FnSymbol* instantiate_generic(ASTMap* substitutions, 
                                Map<Symbol*,Symbol*>* paramMap,
                                CallExpr* call);
  void codegenHeader(FILE* outfile);
  void codegenPrototype(FILE* outfile);
  void codegenDef(FILE* outfile);

  void printDef(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);

  void insertBeforeReturn(Expr* ast);
  void insertBeforeReturnAfterLabel(Expr* ast);

  void insertFormalAtHead(BaseAST* ast);
  void insertFormalAtTail(BaseAST* ast);

  Symbol* getReturnSymbol();

  int numFormals();
  ArgSymbol* getFormal(int i); // return ith formal

  bool tag_generic();
};


class EnumSymbol : public Symbol {
 public:
  EnumSymbol(const char* init_name);
  virtual void verify(); 
  COPY_DEF(EnumSymbol);
  void codegenDef(FILE* outfile);
  bool isParameter(void);
};


class ModuleSymbol : public Symbol {
 public:
  ModTag modTag;
  BlockStmt* block;
  FnSymbol* initFn;
  VarSymbol* guard; // keeps initFn from being run more than once

  ModuleSymbol(const char* iName, ModTag iModTag, BlockStmt* iBlock);
  ~ModuleSymbol();
  virtual void verify(); 
  COPY_DEF(ModuleSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef(FILE* outfile);
};


class LabelSymbol : public Symbol {
 public:
  LabelSymbol(const char* init_name);
  virtual void verify(); 
  COPY_DEF(LabelSymbol);
  virtual void codegenDef(FILE* outfile);
};


VarSymbol *new_StringSymbol(const char *s);
VarSymbol *new_IntSymbol(long long int b, IF1_int_type size=INT_SIZE_32);
VarSymbol *new_UIntSymbol(unsigned long long int b, IF1_int_type size=INT_SIZE_32);
VarSymbol *new_RealSymbol(const char *n, long double b, IF1_float_type size=FLOAT_SIZE_64);
VarSymbol *new_ImagSymbol(const char *n, long double b, IF1_float_type size=FLOAT_SIZE_64);
VarSymbol *new_ComplexSymbol(const char *n, long double r, long double i, IF1_complex_type size=COMPLEX_SIZE_128);
VarSymbol *new_ImmediateSymbol(Immediate *imm);
PrimitiveType *immediate_type(Immediate *imm);

int compareSymbol(const void* v1, const void* v2);

extern HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;
extern StringChainHash uniqueStringHash;

extern Symbol *gNil;
extern Symbol *gUnknown;
extern Symbol *gUnspecified;
extern Symbol *gMethodToken;
extern Symbol *gVoid;
extern Symbol *gFile;
extern Symbol *gOpaque;
extern Symbol *gTimer;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;
extern VarSymbol *gBoundsChecking;

extern Symbol *gSyncVarAuxFields;
extern Symbol *gSingleVarAuxFields;

extern Symbol *gTaskList;

void freeWrapperAndInstantiationCaches();

#endif
