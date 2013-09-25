#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <bitset>
#include <stdint.h>
#include "alist.h"
#include "baseAST.h"
#include "bitVec.h"
#include "flags.h"
#include "type.h"

#include "genret.h"
#include <vector>

//
// The function that represents the compiler-generated entry point
//
extern FnSymbol* chpl_gen_main;

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
  INTENT_CONST_IN,
  INTENT_CONST_REF,
  INTENT_REF,
  INTENT_PARAM,
  INTENT_TYPE
};

enum ModTag {
  MOD_INTERNAL,  // an internal module that the user shouldn't know about
  MOD_STANDARD,  // a standard module from the Chapel libraries
  MOD_USER,      // a module found along the user's search path
  MOD_MAIN       // a module from a file listed on the compiler command line
};

typedef std::bitset<NUM_FLAGS> FlagSet;


class Symbol : public BaseAST {
 public:
  const char* name;
  const char* cname; // Name of symbol for generating C code
  Type* type;
  DefExpr* defPoint; // Point of definition
  FlagSet flags;

  Symbol(AstTag astTag, const char* init_name, Type* init_type = dtUnknown);
  virtual ~Symbol();
  virtual Symbol* copy(SymbolMap* map = NULL, bool internal = false) = 0;
  virtual Symbol* copyInner(SymbolMap* map) = 0;
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast) = 0;

  virtual void verify(); 
  virtual bool inTree();
  virtual Type* typeInfo(void);

  virtual bool isConstant(void);
  virtual bool isParameter(void);

  virtual GenRet codegen();
  virtual void codegenDef();
  virtual void codegenPrototype(); // ie type decl

  virtual FnSymbol* getFnSymbol(void);
  virtual bool isImmediate();

  bool hasFlag(Flag flag);
  void addFlag(Flag flag);
  void copyFlags(Symbol* other);
  void removeFlag(Flag flag);

  bool hasEitherFlag(Flag aflag, Flag bflag);
};
#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)


class VarSymbol : public Symbol {
 public:
  // Note that string immediate values are stored
  // with C escapes - that is newline is 2 chars \ n
  Immediate   *immediate;

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(const char* init_name, Type* init_type = dtUnknown);
  ~VarSymbol();
  void verify(); 
  DECLARE_SYMBOL_COPY(VarSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool isConstant(void);
  bool isParameter(void);
  const char* doc;

  GenRet codegen();
  void codegenDefC(bool global = false);
  void codegenDef();
  // global vars are different ...
  void codegenGlobalDef();
  
  bool isImmediate();
};


class ArgSymbol : public Symbol {
 public:
  IntentTag intent;
  BlockStmt* typeExpr;  // A type expression for the argument type, or NULL.
  BlockStmt* defaultExpr;
  BlockStmt* variableExpr;
  Type* instantiatedFrom;
  bool instantiatedParam;
  bool markedGeneric;

  ArgSymbol(IntentTag iIntent, const char* iName, Type* iType,
            Expr* iTypeExpr = NULL, Expr* iDefaultExpr = NULL,
            Expr* iVariableExpr = NULL);

  void verify(); 
  DECLARE_SYMBOL_COPY(ArgSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool requiresCPtr(void);
  bool isConstant(void);
  bool isParameter(void);
  const char* intentDescrString(void);

  GenRet codegen();
  GenRet codegenType();
};


class TypeSymbol : public Symbol {
 public:
  // We need to know whether or not the definition
  // for this type has already been codegen'd
  // and cache it if it has.
#ifdef HAVE_LLVM
  llvm::Type* llvmType;
  llvm::MDNode* llvmTbaaNode;
  llvm::MDNode* llvmConstTbaaNode;
  llvm::MDNode* llvmTbaaStructNode;
  llvm::MDNode* llvmConstTbaaStructNode;
#else
  // Keep same layout so toggling HAVE_LLVM
  // will not lead to build errors without make clean
  void* llvmType;
  void* llvmTbaaNode;
  void* llvmConstTbaaNode;
  void* llvmTbaaStructNode;
  void* llvmConstTbaaStructNode;
#endif
  bool codegenned;

  TypeSymbol(const char* init_name, Type* init_type);
  void verify(); 
  DECLARE_SYMBOL_COPY(TypeSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  GenRet codegen();
  void codegenDef();
  void codegenPrototype();
  // This function is used to code generate the LLVM TBAA metadata
  // after all of the types have been defined.
  void codegenMetadata();
};


class FnSymbol : public Symbol {
 public:
  AList formals;
  DefExpr* setter; // implicit setter argument to var functions
  Type* retType;
  BlockStmt* where;
  BlockStmt* retExprType;
  BlockStmt* body;
  IntentTag thisTag;
  RetTag retTag;
  IteratorInfo* iteratorInfo;
  Symbol* _this;
  Symbol* _outer;
  FnSymbol *instantiatedFrom;
  SymbolMap substitutions;
  BlockStmt* instantiationPoint; // point of instantiation
  std::vector<BasicBlock*>* basicBlocks;
  Vec<CallExpr*>* calledBy;
  const char* userString;
  FnSymbol* valueFunction; // pointer to value function (created in
                           // resolve and used in cullOverReferences)
  int codegenUniqueNum;
  const char *doc;

  FnSymbol(const char* initName);
  ~FnSymbol();
           
  void verify(); 
  DECLARE_SYMBOL_COPY(FnSymbol);
  FnSymbol* getFnSymbol(void);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  // Returns an LLVM type or a C-cast expression
  GenRet codegenFunctionType(bool forHeader);
  GenRet codegenCast(GenRet fnPtr);

  void codegenHeaderC();
  void codegenPrototype();
  void codegenDef();
  GenRet codegen();

  void printDef(FILE* outfile);

  void insertAtHead(Expr* ast);
  void insertAtTail(Expr* ast);
  void insertAtHead(const char* format, ...);
  void insertAtTail(const char* format, ...);

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
  void verify(); 
  DECLARE_SYMBOL_COPY(EnumSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef();
  
  bool isParameter(void);
  Immediate* getImmediate(void);
};


struct ExternBlockInfo;

class ModuleSymbol : public Symbol {
 public:
  ModTag modTag;
  BlockStmt* block;
  FnSymbol* initFn;
  const char* filename;
  Vec<ModuleSymbol*> modUseList;
  Vec<ModuleSymbol*> modUseSet;
  const char *doc;
  // These are used for extern C blocks.
  ExternBlockInfo* extern_info;

  ModuleSymbol(const char* iName, ModTag iModTag, BlockStmt* iBlock);
  ~ModuleSymbol();
  void verify(); 
  DECLARE_SYMBOL_COPY(ModuleSymbol);
  Vec<VarSymbol*> getConfigVars();
  Vec<FnSymbol*> getFunctions();
  Vec<ModuleSymbol*> getModules();
  Vec<ClassType*> getClasses();
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef();
};


class LabelSymbol : public Symbol {
 public:
  GotoStmt* iterResumeGoto;
  LabelSymbol(const char* init_name);
  void verify(); 
  DECLARE_SYMBOL_COPY(LabelSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef();
};


// Creates a new string literal with the given value.
VarSymbol *new_StringSymbol(const char *s);

// Creates a new boolean literal with the given value and bit-width.
VarSymbol *new_BoolSymbol(bool b, IF1_bool_type size=BOOL_SIZE_SYS);

// Creates a new (signed) integer literal with the given value and bit-width.
VarSymbol *new_IntSymbol(int64_t b, IF1_int_type size=INT_SIZE_64);

// Creates a new unsigned integer literal with the given value and bit-width.
VarSymbol *new_UIntSymbol(uint64_t b, IF1_int_type size=INT_SIZE_64);

// Creates a new real literal with the given value and bit-width.
// n is used for the cname of the new symbol,
// but only if the value has not already been cached.
VarSymbol *new_RealSymbol(const char *n, long double b,
                          IF1_float_type size=FLOAT_SIZE_64);

// Creates a new imaginary literal with the given value and bit-width.
// n is used for the cname of the new symbol,
// but only if the value has not already been cached.
VarSymbol *new_ImagSymbol(const char *n, long double b,
                          IF1_float_type size=FLOAT_SIZE_64);

// Creates a new complex literal with the given value and bit-width.
// n is used for the cname of the new symbol,
// but only if the value has not already been cached.
VarSymbol *new_ComplexSymbol(const char *n, long double r, long double i,
                             IF1_complex_type size=COMPLEX_SIZE_128);

VarSymbol *new_ImmediateSymbol(Immediate *imm);
void resetTempID();
FlagSet getRecordWrappedFlags(Symbol* s);
FlagSet getSyncFlags(Symbol* s);
VarSymbol* newTemp(const char* name = NULL, Type* type = dtUnknown);
VarSymbol* newTemp(Type* type);

// Return true if the arg must use a C pointer whether or not
// pass-by-reference intents are used.
bool argMustUseCPtr(Type* t);

extern bool localTempNames;

extern HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;
extern StringChainHash uniqueStringHash;

extern ModuleSymbol* rootModule;
extern ModuleSymbol* theProgram;
extern ModuleSymbol* mainModule;
extern ModuleSymbol* baseModule;
extern ModuleSymbol* standardModule;
extern Symbol *gNil;
extern Symbol *gUnknown;
extern Symbol *gMethodToken;
extern Symbol *gTypeDefaultToken;
extern Symbol *gLeaderTag, *gFollowerTag;
extern Symbol *gModuleToken;
extern Symbol *gVoid;
extern Symbol *gStringC;
extern Symbol *gFile;
extern Symbol *gOpaque;
extern Symbol *gTimer;
extern Symbol *gTaskID;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;
extern VarSymbol *gTryToken; // try token for conditional function resolution
extern VarSymbol *gBoundsChecking;
extern VarSymbol *gPrivatization;
extern VarSymbol *gLocal;
extern VarSymbol *gNodeID;
extern FnSymbol *gChplHereAlloc;
extern FnSymbol *gChplHereFree;
extern Symbol *gCLine, *gCFile;

extern Symbol *gSyncVarAuxFields;
extern Symbol *gSingleVarAuxFields;

extern Symbol *gTaskList;

extern Map<FnSymbol*,int> ftableMap;
extern Vec<FnSymbol*> ftableVec;

//
// The virtualMethodTable maps types to their arrays of methods.  The
// virtualMethodMap maps methods to their indexes into these arrays.
// The virtualChildrenMap maps methods to all of the methods that
// could be called when they are called.  The virtualRootsMap maps
// methods to the root methods that it overrides.  Note that multiple
// inheritance will require more virtual method tables, one for each
// path up the class hierarchy to each class root.
//
extern Map<Type*,Vec<FnSymbol*>*> virtualMethodTable;
extern Map<FnSymbol*,int> virtualMethodMap;
extern Map<FnSymbol*,Vec<FnSymbol*>*> virtualChildrenMap;
extern Map<FnSymbol*,Vec<FnSymbol*>*> virtualRootsMap;
extern const char* intentDescrName[];

#endif
