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
  Symbol* overloadNext;
  Symbol* overloadPrev;
  bool isUserAlias;    // is a user alias via '=>'
  bool isCompilerTemp;
  bool isExprTemp;
  bool isTypeVariable;
  bool canParam;       // can be a parameter (determined during resolution)
  bool canType;        // can be a type (determined during resolution)
  bool isConcurrent;   // can be accessed concurrently
  Vec<SymExpr*> defs;
  Vec<SymExpr*> uses;
  Vec<char*> pragmas;

  Symbol(astType_t astType, const char* init_name, Type* init_type = dtUnknown);
  virtual ~Symbol();
  virtual void verify(); 
  virtual void clean();
  void setParentScope(SymScope* init_parentScope);
  virtual bool inTree();
  virtual Type* typeInfo(void);
  COPY_DEF(Symbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual bool isConst(void);
  virtual bool isParam(void);
  bool isThis(void);

  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual FnSymbol* getFnSymbol(void);
  virtual Symbol* getSymbol(void);
  virtual bool isImmediate();

  char* hasPragma(char* str);
  void removePragma(char* str);
  char* hasPragmaPrefix(char* str);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


class UnresolvedSymbol : public Symbol {
 public:
  UnresolvedSymbol(const char* init_name);
  virtual void verify(); 
  COPY_DEF(UnresolvedSymbol);
  void codegen(FILE* outfile);
};


class VarSymbol : public Symbol {
 public:
  varType      varClass;
  consType     consClass;
  Immediate   *immediate;
  VarSymbol   *refc;         // number of outstanding references to
  VarSymbol   *refcMutex;    // guard refc

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(char* init_name, Type* init_type = dtUnknown,
            varType  init_varClass = VAR_NORMAL, 
            consType init_consClass = VAR_VAR);
  ~VarSymbol();
  virtual void verify(); 
  COPY_DEF(VarSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool isConst(void);
  bool isParam(void);

  void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual bool isImmediate();
};


class ArgSymbol : public Symbol {
 public:
  intentTag intent;
  Expr* defaultExpr;
  Expr* variableExpr;
  bool isGeneric;
  Type* instantiatedFrom;
  bool instantiatedParam;

  ArgSymbol(intentTag iIntent, char* iName, Type* iType,
            Expr* iDefaultExpr = NULL, Expr* iVariableExpr = NULL);
  virtual void verify(); 
  COPY_DEF(ArgSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool requiresCPtr(void);
  bool isConst(void);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class TypeSymbol : public Symbol {
 public:
  TypeSymbol(char* init_name, Type* init_type);
  virtual void verify(); 
  COPY_DEF(TypeSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
};


class FnSymbol : public Symbol {
 public:
  AList* formals;
  DefExpr* setter; // implicit setter argument to var functions
  Type* retType;
  BlockStmt* where;
  Expr* retExprType;
  BlockStmt* body;
  fnType fnClass;
  bool noParens;
  bool retRef;
  bool defSetGet;
  bool isParam;
  IteratorInfo* iteratorInfo;
  SymScope* argScope;
  bool isGeneric;
  Symbol* _this;
  bool isMethod;
  FnSymbol *instantiatedFrom;
  ASTMap substitutions;
  Vec<FnSymbol *> *instantiatedTo;
  CallExpr* visiblePoint; // point of instantiation is in visible lookup
                          // global functions copied out see local functions
  bool visible; // included in visible function list for dispatch
                // compiler generated functions are not visible, e.g.,
                // instantiated functions and wrappers
  bool global; // function is globally visible e.g. class argument
               // WAW: temporary hack to get iterator-created methods
               // visible

  Vec<BasicBlock*>* basicBlocks;
  Vec<CallExpr*>* calledBy;
  bool isWrapper;
  bool normalizedOnce;

  FnSymbol(char* initName);
  ~FnSymbol();
           
  virtual void verify(); 
  COPY_DEF(FnSymbol);
  virtual FnSymbol* getFnSymbol(void);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  FnSymbol* promotion_wrapper(Map<Symbol*,Symbol*>* promotion_subs, bool isSquare);
  FnSymbol* order_wrapper(Map<Symbol*,Symbol*>* formals_to_formals);
  FnSymbol* coercion_wrapper(ASTMap* coercion_substitutions);
  FnSymbol* default_wrapper(Vec<Symbol*>* defaults);
  FnSymbol* instantiate_generic(ASTMap* substitutions);
  void codegenHeader(FILE* outfile);
  void codegenPrototype(FILE* outfile);
  void codegenDef(FILE* outfile);

  void printDef(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);

  void insertAtHead(AList* ast);
  void insertAtTail(AList* ast);

  void insertBeforeReturn(Expr* ast);
  void insertBeforeReturnAfterLabel(Expr* ast);

  void insertFormalAtHead(BaseAST* ast);
  void insertFormalAtTail(BaseAST* ast);

  Symbol* getReturnSymbol();
  Symbol* getReturnLabel();

  ArgSymbol* getFormal(int i); // return ith formal
};


class EnumSymbol : public Symbol {
 public:
  EnumSymbol(char* init_name);
  virtual void verify(); 
  COPY_DEF(EnumSymbol);
  void codegenDef(FILE* outfile);
  bool isParam(void);
};


enum modType {
  MOD_STANDARD,
  MOD_USER
};


class ModuleSymbol : public Symbol {
 public:
  modType modtype;
  BlockStmt* block;
  FnSymbol* initFn;
  Vec<ModuleSymbol*> subModules;

  ModuleSymbol(char* init_name, modType init_modtype);
  ~ModuleSymbol();
  virtual void verify(); 
  COPY_DEF(ModuleSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef(FILE* outfile);
};


class LabelSymbol : public Symbol {
 public:
  LabelSymbol(char* init_name);
  virtual void verify(); 
  COPY_DEF(LabelSymbol);
  virtual void codegenDef(FILE* outfile);
};


VarSymbol *new_StringSymbol(char *s);
VarSymbol *new_IntSymbol(long long int b, IF1_int_type size=INT_SIZE_32);
VarSymbol *new_UIntSymbol(unsigned long long int b, IF1_int_type size=INT_SIZE_32);
VarSymbol *new_RealSymbol(char *n, long double b, IF1_float_type size=FLOAT_SIZE_64);
VarSymbol *new_ImagSymbol(char *n, long double b, IF1_float_type size=FLOAT_SIZE_64);
VarSymbol *new_ComplexSymbol(char *n, long double r, long double i, IF1_complex_type size=COMPLEX_SIZE_128);
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
extern Symbol *gTimer;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;
extern VarSymbol *gBoundsChecking;

extern Symbol *gMutex_p;
extern Symbol *gCondVar_p;

#endif
