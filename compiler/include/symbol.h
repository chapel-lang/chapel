/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "baseAST.h"

#include "flags.h"
#include "type.h"

#include <bitset>
#include <vector>

//
// The function that represents the compiler-generated entry point
//
extern FnSymbol* chpl_gen_main;

class BasicBlock;
class BlockStmt;
class DefExpr;
class Immediate;
class IteratorInfo;
class Stmt;
class SymExpr;

// keep in sync with retTagDescrString()
enum RetTag {
  RET_VALUE,
  RET_REF,
  RET_PARAM,
  RET_TYPE
};

const int INTENT_FLAG_IN    = 0x01;
const int INTENT_FLAG_OUT   = 0x02;
const int INTENT_FLAG_CONST = 0x04;
const int INTENT_FLAG_REF   = 0x08;
const int INTENT_FLAG_PARAM = 0x10;
const int INTENT_FLAG_TYPE  = 0x20;
const int INTENT_FLAG_BLANK = 0x40;

// If this enum is modified, ArgSymbol::intentDescrString()
// and intentDescrString(IntentTag) should also be updated to match
enum IntentTag {
  INTENT_IN        = INTENT_FLAG_IN,
  INTENT_OUT       = INTENT_FLAG_OUT,
  INTENT_INOUT     = INTENT_FLAG_IN    | INTENT_FLAG_OUT,
  INTENT_CONST     = INTENT_FLAG_CONST,
  INTENT_CONST_IN  = INTENT_FLAG_CONST | INTENT_FLAG_IN,
  INTENT_REF       = INTENT_FLAG_REF,
  INTENT_CONST_REF = INTENT_FLAG_CONST | INTENT_FLAG_REF,
  INTENT_PARAM     = INTENT_FLAG_PARAM,
  INTENT_TYPE      = INTENT_FLAG_TYPE,
  INTENT_BLANK     = INTENT_FLAG_BLANK
};

// keep in sync with modTagDescrString()
enum ModTag {
  MOD_INTERNAL,  // an internal module that the user shouldn't know about
  MOD_STANDARD,  // a standard module from the Chapel libraries
  MOD_USER,      // a module found along the user's search path
};

typedef std::bitset<NUM_FLAGS> FlagSet;

// for task intents and forall intents, in createTaskFunctions.cpp
ArgSymbol* tiMarkForIntent(IntentTag intent);


/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class Symbol : public BaseAST {
public:
  // Interface for BaseAST
  virtual GenRet     codegen();
  virtual bool       inTree();
  virtual Type*      typeInfo();
  virtual void       verify();

  // New interfaces
  virtual Symbol*    copy(SymbolMap* map      = NULL,
                          bool       internal = false)           = 0;
  virtual void       replaceChild(BaseAST* oldAst,
                                  BaseAST* newAst)               = 0;

  virtual FnSymbol*  getFnSymbol();

  virtual bool       isConstant()                              const;
  virtual bool       isConstValWillNotChange()                 const;
  virtual bool       isImmediate()                             const;
  virtual bool       isParameter()                             const;
          bool       isRenameable()                            const;

  virtual void       codegenDef();

  bool               hasFlag(Flag flag)                        const;
  bool               hasEitherFlag(Flag aflag, Flag bflag)     const;

  void               addFlag(Flag flag);
  void               removeFlag(Flag flag);
  void               copyFlags(const Symbol* other);

  Type*              type;
  FlagSet            flags;

  const char*        name;
  const char*        cname;    // Name of symbol for C code

  DefExpr*           defPoint; // Point of definition

protected:
                     Symbol(AstTag      astTag,
                            const char* init_name,
                            Type*       init_type = dtUnknown);

  virtual           ~Symbol();

private:
                     Symbol();

  virtual void       codegenPrototype(); // ie type decl
};

#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)

/******************************** | *********************************
*                                                                   *
* This class has two roles:                                         *
*    1) A common abstract base class for VarSymbol and ArgSymbol.   *
*    2) Maintain location state as an IPE "optimization".           *
*                                                                   *
********************************* | ********************************/

class LcnSymbol : public Symbol
{
public:
  int       depth()                                            const;
  int       offset()                                           const;

  void      locationSet(int depth, int offset);

protected:
            LcnSymbol(AstTag      astTag,
                      const char* initName,
                      Type*       initType);

  virtual  ~LcnSymbol();

private:
            LcnSymbol();

  int       mDepth;                // Lexical depth relative to root
  int       mOffset;               // Byte offset within frame
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class VarSymbol : public LcnSymbol {
public:
  // Note that string immediate values are stored
  // with C escapes - that is newline is 2 chars \ n
  Immediate   *immediate;

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(const char* init_name, Type* init_type = dtUnknown);
  virtual ~VarSymbol();

  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(VarSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual bool       isConstant()                              const;
  virtual bool       isConstValWillNotChange()                 const;
  virtual bool       isImmediate()                             const;
  virtual bool       isParameter()                             const;

  const char* doc;

  GenRet codegen();
  void codegenDefC(bool global = false);
  void codegenDef();
  // global vars are different ...
  void codegenGlobalDef();
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class ArgSymbol : public LcnSymbol {
public:
  ArgSymbol(IntentTag   iIntent,
            const char* iName,
            Type*       iType,
            Expr*       iTypeExpr     = NULL,
            Expr*       iDefaultExpr  = NULL,
            Expr*       iVariableExpr = NULL);

  // Interface for BaseAST
  virtual GenRet  codegen();

  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(ArgSymbol);

  // Interface for Symbol
  virtual void    replaceChild(BaseAST* oldAst, BaseAST* newAst);


  // New interface
  virtual bool    isConstant()                              const;
  virtual bool    isConstValWillNotChange()                 const;
  virtual bool    isParameter()                             const;

  bool            requiresCPtr();
  const char*     intentDescrString();

  GenRet          codegenType();

  IntentTag       intent;
  BlockStmt*      typeExpr;    // Type expr for arg type, or NULL.
  BlockStmt*      defaultExpr;
  BlockStmt*      variableExpr;
  Type*           instantiatedFrom;

};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

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

  TypeSymbol(const char* init_name, Type* init_type);
  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(TypeSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  GenRet codegen();
  void codegenDef();
  void codegenPrototype();
  // This function is used to code generate the LLVM TBAA metadata
  // after all of the types have been defined.
  void codegenMetadata();
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class FnSymbol : public Symbol {
 public:
  AList formals;
  DefExpr* setter; // implicit setter argument to var functions
  Type* retType; // The return type of the function.  This field is not
                 // fully established until resolution, and could be NULL
                 // before then.  Up to that point, return type information is
                 // stored in the retExprType field.
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

  /// Used to keep track of symbol substitutions during partial copying.
  SymbolMap partialCopyMap;
  /// Source of a partially copied function.
  FnSymbol* partialCopySource;
  /// Used to store the return symbol during partial copying.
  Symbol* retSymbol;
  /// Number of formals before tuple type constructor formals are added.
  int numPreTupleFormals;

                  FnSymbol(const char* initName);
                 ~FnSymbol();

  void            verify();
  virtual void    accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(FnSymbol);

  FnSymbol*       copyInnerCore(SymbolMap* map);
  FnSymbol*       getFnSymbol(void);
  void            replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  FnSymbol*       partialCopy(SymbolMap* map);
  void            finalizeCopy();

  // Returns an LLVM type or a C-cast expression
  GenRet          codegenFunctionType(bool forHeader);
  GenRet          codegenCast(GenRet fnPtr);

  GenRet          codegen();
  void            codegenHeaderC();
  void            codegenPrototype();
  void            codegenDef();

  void            printDef(FILE* outfile);

  void            insertAtHead(Expr* ast);
  void            insertAtHead(const char* format, ...);

  void            insertAtTail(Expr* ast);
  void            insertAtTail(const char* format, ...);

  void            insertBeforeReturn(Expr* ast);
  void            insertBeforeReturnAfterLabel(Expr* ast);
  void            insertBeforeDownEndCount(Expr* ast);

  void            insertFormalAtHead(BaseAST* ast);
  void            insertFormalAtTail(BaseAST* ast);

  Symbol*         getReturnSymbol();
  Symbol*         replaceReturnSymbol(Symbol* newRetSymbol, Type* newRetType);

  int             numFormals()                                 const;
  ArgSymbol*      getFormal(int i); // return ith formal

  void            collapseBlocks();

  bool            tag_generic();
  bool            isResolved()                                 const;
  bool            isMethod()                                   const;
  bool            isPrimaryMethod()                            const;
  bool            isSecondaryMethod()                          const;
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class EnumSymbol : public Symbol {
 public:
                  EnumSymbol(const char* initName);

  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(EnumSymbol);

  virtual void    replaceChild(BaseAST* oldAst, BaseAST* newAst);
  virtual void    codegenDef();

  virtual bool    isParameter()                             const;

  Immediate*      getImmediate();
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

struct ExternBlockInfo;

class ModuleSymbol : public Symbol {
public:
                       ModuleSymbol(const char* iName,
                                    ModTag      iModTag,
                                    BlockStmt*  iBlock);

                      ~ModuleSymbol();

  // Interface to BaseAST
  virtual void         verify();
  virtual void         accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(ModuleSymbol);

  // Interface to Symbol
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void codegenDef();

  // New interface
  Vec<AggregateType*>  getTopLevelClasses();
  Vec<VarSymbol*>      getTopLevelConfigVars();
  Vec<VarSymbol*>      getTopLevelVariables();
  Vec<FnSymbol*>       getTopLevelFunctions(bool includeExterns);
  Vec<ModuleSymbol*>   getTopLevelModules();

  void                 addDefaultUses();
  void                 moduleUseAdd(ModuleSymbol* module);
  void                 moduleUseRemove(ModuleSymbol* module);

  ModTag               modTag;

  BlockStmt*           block;
  FnSymbol*            initFn;

  Vec<ModuleSymbol*>   modUseList;

  const char*          filename;
  const char*          doc;

  // LLVM uses this for extern C blocks.
  ExternBlockInfo*     extern_info;

private:
  void                 getTopLevelConfigOrVariables(Vec<VarSymbol *> *contain, Expr *expr, bool config);

};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class LabelSymbol : public Symbol {
 public:
  GotoStmt* iterResumeGoto;
  LabelSymbol(const char* init_name);
  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(LabelSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef();
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

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

// for use in an English sentence
const char* retTagDescrString(RetTag retTag);
const char* modTagDescrString(ModTag modTag);
const char* intentDescrString(IntentTag intent);

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
extern ModuleSymbol* printModuleInitModule;
extern Symbol *gNil;
extern Symbol *gUnknown;
extern Symbol *gMethodToken;
extern Symbol *gTypeDefaultToken;
extern Symbol *gLeaderTag, *gFollowerTag, *gStandaloneTag;
extern Symbol *gModuleToken;
extern Symbol *gNoInit;
extern Symbol *gVoid;
extern Symbol *gStringC;
extern Symbol *gStringCopy;
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
extern VarSymbol *gModuleInitIndentLevel;
extern FnSymbol *gPrintModuleInitFn;
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

#endif
