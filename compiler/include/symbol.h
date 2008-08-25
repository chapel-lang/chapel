#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <bitset>
#include "alist.h"
#include "baseAST.h"
#include "bitVec.h"
#include "pragmas.h"
#include "type.h"

extern FnSymbol* chpl_main;

class SymExpr;
class DefExpr;
class Stmt;
class BlockStmt;
class Immediate;
class BasicBlock;
class IteratorInfo;

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
  std::bitset<NUM_FLAGS> flags;

  Symbol(AstTag astTag, const char* init_name, Type* init_type = dtUnknown);
  virtual ~Symbol();
  virtual void verify(); 
  virtual bool inTree();
  virtual Type* typeInfo(void);
  DECLARE_SYMBOL_COPY(Symbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual bool isConstant(void);
  virtual bool isParameter(void);

  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual FnSymbol* getFnSymbol(void);
  virtual bool isImmediate();

  bool hasFlag(Flag flag);
  void addFlag(Flag flag);
  void addFlags(Vec<const char*>* strs);
  void copyFlags(Symbol* other);
  void removeFlag(Flag flag);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


class VarSymbol : public Symbol {
 public:
  Immediate   *immediate;

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(const char* init_name, Type* init_type = dtUnknown);
  ~VarSymbol();
  virtual void verify(); 
  DECLARE_SYMBOL_COPY(VarSymbol);
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
  BlockStmt* variableExpr;
  Type* instantiatedFrom;
  bool instantiatedParam;
  bool markedGeneric;

  ArgSymbol(IntentTag iIntent, const char* iName, Type* iType,
            Expr* iTypeExpr = NULL, Expr* iDefaultExpr = NULL,
            Expr* iVariableExpr = NULL);

  virtual void verify(); 
  DECLARE_SYMBOL_COPY(ArgSymbol);
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
  DECLARE_SYMBOL_COPY(TypeSymbol);
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
  BlockStmt* retExprType;
  BlockStmt* body;
  RetTag retTag;
  IteratorInfo* iteratorInfo;
  Symbol* _this;
  Symbol* _outer;
  FnSymbol *instantiatedFrom;
  SymbolMap substitutions;
  BlockStmt* instantiationPoint; // point of instantiation
  Vec<BasicBlock*>* basicBlocks;
  Vec<CallExpr*>* calledBy;
  const char* userString;
  FnSymbol* valueFunction; // pointer to value function (created in
                           // resolve and used in cullOverReferences)

  FnSymbol(const char* initName);
  ~FnSymbol();
           
  virtual void verify(); 
  DECLARE_SYMBOL_COPY(FnSymbol);
  virtual FnSymbol* getFnSymbol(void);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

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
  DECLARE_SYMBOL_COPY(EnumSymbol);
  void codegenDef(FILE* outfile);
  bool isParameter(void);
};


class ModuleSymbol : public Symbol {
 public:
  ModTag modTag;
  BlockStmt* block;
  FnSymbol* initFn;
  VarSymbol* guard; // keeps initFn from being run more than once
  const char* filename;

  ModuleSymbol(const char* iName, ModTag iModTag, BlockStmt* iBlock);
  ~ModuleSymbol();
  virtual void verify(); 
  DECLARE_SYMBOL_COPY(ModuleSymbol);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef(FILE* outfile);
};


class LabelSymbol : public Symbol {
 public:
  LabelSymbol(const char* init_name);
  virtual void verify(); 
  DECLARE_SYMBOL_COPY(LabelSymbol);
  virtual void codegenDef(FILE* outfile);
};


VarSymbol *new_StringSymbol(const char *s);
VarSymbol *new_BoolSymbol(bool b, IF1_bool_type size=BOOL_SIZE_SYS);
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

extern ModuleSymbol* rootModule;
extern ModuleSymbol* theProgram;
extern ModuleSymbol* baseModule;
extern ModuleSymbol* standardModule;
extern Symbol *gNil;
extern Symbol *gNilRef;
extern Symbol *gUnknown;
extern Symbol *gUnspecified;
extern Symbol *gMethodToken;
extern Symbol *gModuleToken;
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

#endif
