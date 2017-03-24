/*
 * Copyright 2004-2017 Cray Inc.
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
#include <iostream>
#include <vector>
#include <map>

#ifdef HAVE_LLVM
// Forward declare MDNode.
namespace llvm
{
  class MDNode;
}
#endif

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
  RET_CONST_REF,
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
const int INTENT_FLAG_MAYBE_CONST = 0x80;

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
  INTENT_REF_MAYBE_CONST = INTENT_FLAG_MAYBE_CONST | INTENT_FLAG_REF,
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

// for task intents and forall intents
ArgSymbol* tiMarkForIntent(IntentTag intent);
ArgSymbol* tiMarkForTFIntent(int tfIntent);


/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

class Symbol : public BaseAST {
public:
  // Interface for BaseAST
  virtual GenRet     codegen();
  virtual bool       inTree();
  virtual QualifiedType qualType();
  virtual void       verify();

  // New interfaces
  virtual Symbol*    copy(SymbolMap* map      = NULL,
                          bool       internal = false)           = 0;
  virtual void       replaceChild(BaseAST* oldAst,
                                  BaseAST* newAst)               = 0;

  virtual bool       isConstant()                              const;
  virtual bool       isConstValWillNotChange()                 const;
  virtual bool       isImmediate()                             const;
  virtual bool       isParameter()                             const;
          bool       isRenameable()                            const;
          bool       isRef();
          bool       isWideRef();
          bool       isRefOrWideRef();

  virtual void       codegenDef();

  // Returns the scope block in which this symbol is declared.
  BlockStmt*         getDeclarationScope()                     const;

  bool               hasFlag(Flag flag)                        const;
  bool               hasEitherFlag(Flag aflag, Flag bflag)     const;

  void               addFlag(Flag flag);
  void               removeFlag(Flag flag);
  void               copyFlags(const Symbol* other);

  virtual bool       isVisible(BaseAST* scope)                 const;
  bool               noDocGen()                                const;

  // Future: consider merging qual, type into a single
  // field of type QualifiedType
  Qualifier          qual;
  Type*              type;
  FlagSet            flags;
  // Tuples can contain ref fields. In that event, a VarSymbol/ArgSymbol
  // needs to be able to track whether the ref field is ref or
  // const ref. It can depend on the variable for ref to arrays.
  Qualifier*         fieldQualifiers;

  const char*        name;
  const char*        cname;    // Name of symbol for C code

  DefExpr*           defPoint; // Point of definition

  // Managing the list of SymExprs that refer to this Symbol
  // use for_SymbolSymExprs, for_SymbolDefs, for_SymbolUses
  // to traverse these.
  void               addSymExpr(SymExpr* se);
  void               removeSymExpr(SymExpr* se);
  SymExpr*           firstSymExpr()                            const;
  SymExpr*           lastSymExpr()                             const;
  // Get the number of Defs or Uses up to the maximum number
  int                countDefs(int max=INT_MAX)                const;
  int                countUses(int max=INT_MAX)                const;
  // Does the Symbol have any Uses? same as countUses() > 0
  // but may be faster.
  bool               isUsed()                                  const;
  bool               isDefined()                               const;
  // Return the single use of this Symbol, or NULL if there are 0 or >= 2
  SymExpr*           getSingleUse()                            const;
  // Return the single def of this Symbol, or NULL if there are 0 or >= 2
  SymExpr*           getSingleDef()                            const;
protected:
                     Symbol(AstTag      astTag,
                            const char* init_name,
                            Type*       init_type = dtUnknown);

  virtual           ~Symbol();

private:
                     Symbol();

  virtual void       codegenPrototype(); // ie type decl

  SymExpr*           symExprsHead;
  SymExpr*           symExprsTail;
};

#define forv_Symbol(_p, _v) forv_Vec(Symbol, _p, _v)

#define for_SymbolSymExprs(se, symbol)                                  \
  for (SymExpr *se = (symbol)->firstSymExpr(),                          \
         *_se_next = se ? se->symbolSymExprsNext : NULL;                \
       se;                                                              \
       se = _se_next,                                                   \
         _se_next = se ? se->symbolSymExprsNext : NULL)

#define for_SymbolDefs(def, symbol)                                      \
  for_SymbolSymExprs(def, symbol)                                        \
    if ((isDefAndOrUse(def) & 1))

#define for_SymbolUses(use, symbol)                                      \
  for_SymbolSymExprs(use, symbol)                                        \
    if ((isDefAndOrUse(use) & 2))


bool isString(Symbol* symbol);
bool isUserDefinedRecord(Symbol* symbol);

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
  virtual bool       isType()                                  const;

  const char* doc;

  GenRet codegenVarSymbol(bool lhsInSetReference=false);
  GenRet codegen();
  void codegenDefC(bool global = false, bool isHeader = false);
  void codegenDef();
  // global vars are different ...
  void codegenGlobalDef(bool isHeader);

  virtual void printDocs(std::ostream *file, unsigned int tabs);

  void makeField();

private:

  virtual std::string docsDirective();
  bool isField;

public:
#ifdef HAVE_LLVM
  llvm::MDNode *llvmDIGlobalVariable;
  llvm::MDNode *llvmDIVariable;
#else
  void* llvmDIGlobalVariable;
  void* llvmDIVariable;
#endif

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

  virtual bool    isVisible(BaseAST* scope)                 const;

  bool            requiresCPtr();
  const char*     intentDescrString();

  GenRet          codegenType();

  IntentTag       intent;
  BlockStmt*      typeExpr;    // Type expr for arg type, or NULL.
  BlockStmt*      defaultExpr;

  // Stores the expression specified after an ellipsis in vararg formal.
  // This must resolve during function resolution to a parameter expression.
  // It can be omitted for variadic arguments or a query identifier.
  BlockStmt*      variableExpr;

  Type*           instantiatedFrom;

public:
#ifdef HAVE_LLVM
  llvm::MDNode *llvmDIFormal;
#else
  void* llvmDIFormal;
#endif
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
  llvm::MDNode* llvmDIType;
#else
  // Keep same layout so toggling HAVE_LLVM
  // will not lead to build errors without make clean
  void* llvmType;
  void* llvmTbaaNode;
  void* llvmConstTbaaNode;
  void* llvmTbaaStructNode;
  void* llvmConstTbaaStructNode;
  void* llvmDIType;
#endif

  TypeSymbol(const char* init_name, Type* init_type);
  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(TypeSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  void renameInstantiatedMulti(SymbolMap& subs, FnSymbol* fn);
  void renameInstantiatedSingle(Symbol* sym);

  GenRet codegen();
  void codegenDef();
  void codegenPrototype();
  // This function is used to code generate the LLVM TBAA metadata
  // after all of the types have been defined.
  void codegenMetadata();

  const char* doc;

 private:
  void renameInstantiatedStart();
  void renameInstantiatedIndividual(Symbol* sym);
  void renameInstantiatedEnd();
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class FnSymbol : public Symbol {
public:
  // each formal is an ArgSymbol, but the elements are DefExprs
  AList                      formals;

  // The return type of the function. This field is not fully established
  // until function resolution, and could be NULL before then.  Up to that
  // point, return type information is stored in the retExprType field.
  Type*                      retType;

  BlockStmt*                 where;
  BlockStmt*                 retExprType;
  BlockStmt*                 body;
  IntentTag                  thisTag;
  RetTag                     retTag;

  // Attached original (user) iterators before lowering.
  IteratorInfo*              iteratorInfo;

  Symbol*                    _this;
  Symbol*                    _outer;
  FnSymbol*                  instantiatedFrom;
  SymbolMap                  substitutions;
  BlockStmt*                 instantiationPoint;
  std::vector<BasicBlock*>*  basicBlocks;
  Vec<CallExpr*>*            calledBy;
  const char*                userString;

  // pointer to value function (created in function resolution
  // and used in cullOverReferences)
  FnSymbol*                  valueFunction;

  int                        codegenUniqueNum;
  const char*                doc;

  // Used to store the return symbol during partial copying.
  Symbol*                    retSymbol;

  // Number of formals before tuple type constructor formals are added.
  int                        numPreTupleFormals;

#ifdef HAVE_LLVM
  llvm::MDNode*              llvmDISubprogram;
#else
  void*                      llvmDISubprogram;
#endif


                             FnSymbol(const char* initName);
                            ~FnSymbol();

  void                       verify();
  virtual void               accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(FnSymbol);

  FnSymbol*                  copyInnerCore(SymbolMap* map);
  void                       replaceChild(BaseAST* oldAst, BaseAST* newAst);

  FnSymbol*                  partialCopy(SymbolMap* map);
  void                       finalizeCopy();

  // Returns an LLVM type or a C-cast expression
  GenRet                     codegenFunctionType(bool forHeader);
  GenRet                     codegenCast(GenRet fnPtr);

  GenRet                     codegen();
  void                       codegenHeaderC();
  void                       codegenPrototype();
  void                       codegenDef();

  void                       printDef(FILE* outfile);

  void                       insertAtHead(Expr* ast);
  void                       insertAtHead(const char* format, ...);

  void                       insertAtTail(Expr* ast);
  void                       insertAtTail(const char* format, ...);

  void                       insertFormalAtHead(BaseAST* ast);
  void                       insertFormalAtTail(BaseAST* ast);

  void                       insertBeforeEpilogue(Expr* ast);

  // insertIntoEpilogue adds an Expr before the final return,
  // but after the epilogue label
  void                       insertIntoEpilogue(Expr* ast);

  LabelSymbol*               getEpilogueLabel();
  LabelSymbol*               getOrCreateEpilogueLabel();

  Symbol*                    getReturnSymbol();
  Symbol*                    replaceReturnSymbol(Symbol* newRetSymbol,
                                                 Type*   newRetType);

  int                        numFormals()                                const;
  ArgSymbol*                 getFormal(int i);

  void                       collapseBlocks();

  bool                       tagIfGeneric();

  bool                       isResolved()                                const;
  bool                       isMethod()                                  const;
  bool                       isPrimaryMethod()                           const;
  bool                       isSecondaryMethod()                         const;
  bool                       isIterator()                                const;
  bool                       returnsRefOrConstRef()                      const;

  QualifiedType              getReturnQualType()                         const;

  virtual void               printDocs(std::ostream* file,
                                       unsigned int  tabs);

  void                       throwsErrorInit();
  bool                       throwsError()                               const;

  bool                       retExprDefinesNonVoid()                     const;

private:
  virtual std::string        docsDirective();

  int                        hasGenericFormals()                         const;

  bool                       _throwsError;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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
  FnSymbol*            deinitFn;

  Vec<ModuleSymbol*>   modUseList;

  const char*          filename;
  const char*          doc;

  // LLVM uses this for extern C blocks.
#ifdef HAVE_LLVM
  ExternBlockInfo*     extern_info;
  llvm::MDNode* llvmDINameSpace;
#else
  void* extern_info;
  void* llvmDINameSpace;
#endif

  void                 printDocs(std::ostream *file, unsigned int tabs, std::string parentName);
  void                 printTableOfContents(std::ostream *file);
  std::string          docsName();

private:
  void                 getTopLevelConfigOrVariables(Vec<VarSymbol *> *contain, Expr *expr, bool config);
  bool                 hasTopLevelModule();
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

// Processes a char* to replace any escape sequences with the actual bytes
std::string unescapeString(const char* const str, BaseAST* astForError);

// Creates a new string literal with the given value.
VarSymbol *new_StringSymbol(const char *s);

// Creates a new C string literal with the given value.
VarSymbol *new_CStringSymbol(const char *s);

// Creates a new boolean literal with the given value and bit-width.
VarSymbol *new_BoolSymbol(bool b, IF1_bool_type size=BOOL_SIZE_SYS);

// Creates a new (signed) integer literal with the given value and bit-width.
VarSymbol *new_IntSymbol(int64_t b, IF1_int_type size=INT_SIZE_64);

// Creates a new unsigned integer literal with the given value and bit-width.
VarSymbol *new_UIntSymbol(uint64_t b, IF1_int_type size=INT_SIZE_64);

// Creates a new real literal with the given value and bit-width.
// n should be a string argument containing a Chapel decimal or hexadecimal
// floating point literal. It will be copied and the floating point
// value will be computed. The resulting symbol will have a cname
// equal to a fixed-up n, or to an n previously passed to this
// function that has the same value.
VarSymbol *new_RealSymbol(const char *n,
                          IF1_float_type size=FLOAT_SIZE_64);

// Creates a new imaginary literal with the given value and bit-width.
// n should be a string argument containing a Chapel decimal or hexadecimal
// floating point literal. It will be copied and the floating point
// value will be computed. The resulting symbol will have a cname
// equal to a fixed-up n, or to an n previously passed to this
// function that has the same value.
VarSymbol *new_ImagSymbol(const char *n,
                          IF1_float_type size=FLOAT_SIZE_64);

// Creates a new complex literal with the given value and bit-width.
// n is used for the cname of the new symbol,
// but only if the value has not already been cached.
VarSymbol *new_ComplexSymbol(const char *n, long double r, long double i,
                             IF1_complex_type size=COMPLEX_SIZE_128);

VarSymbol *new_ImmediateSymbol(Immediate *imm);

void createInitStringLiterals();
void resetTempID();
FlagSet getRecordWrappedFlags(Symbol* s);
VarSymbol* newTemp(const char* name = NULL, Type* type = dtUnknown);
VarSymbol* newTemp(Type* type);
VarSymbol* newTemp(const char* name, QualifiedType qt);
VarSymbol* newTemp(QualifiedType qt);

// for use in an English sentence
const char* retTagDescrString(RetTag retTag);
const char* modTagDescrString(ModTag modTag);
const char* intentDescrString(IntentTag intent);

// Return true if the arg must use a C pointer whether or not
// pass-by-reference intents are used.
bool argMustUseCPtr(Type* t);

//
// Used to pass information from partialCopy() to finalizeCopy().
//
class PartialCopyData {
 public:
  // Used to keep track of symbol substitutions during partial copying.
  SymbolMap partialCopyMap;
  // Source of a partially copied function.
  FnSymbol* partialCopySource;
  // Vararg formal to be replaced with individual formals, or NULL.
  ArgSymbol* varargOldFormal;
  // Individual formals to replace varargOldFormal.
  std::vector<ArgSymbol*> varargNewFormals;

  PartialCopyData() : partialCopySource(NULL), varargOldFormal(NULL) { }
  ~PartialCopyData() { partialCopyMap.clear(); varargNewFormals.clear(); }
};

PartialCopyData* getPartialCopyInfo(FnSymbol* fn);
PartialCopyData& addPartialCopyInfo(FnSymbol* fn);
void clearPartialCopyInfo(FnSymbol* fn);
void clearPartialCopyFnMap();
void checkEmptyPartialCopyFnMap();

void substituteVarargTupleRefs(BlockStmt* ast, int numArgs, ArgSymbol* formal,
                               std::vector<ArgSymbol*>& varargFormals);

// Parser support.
class ForallIntents;
void addForallIntent(ForallIntents* fi, Expr* var, IntentTag intent, Expr* ri);
void addTaskIntent(CallExpr* ti,        Expr* var, IntentTag intent, Expr* ri);

extern bool localTempNames;

extern HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;
extern HashMap<Immediate *, ImmHashFns, VarSymbol *> stringLiteralsHash;
extern StringChainHash uniqueStringHash;

extern ModuleSymbol* rootModule;
extern ModuleSymbol* theProgram;
extern ModuleSymbol* mainModule;
extern ModuleSymbol* baseModule;
extern ModuleSymbol* stringLiteralModule;
extern FnSymbol* initStringLiterals;
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
extern Symbol *gCVoidPtr;
extern Symbol *gFile;
extern Symbol *gOpaque;
extern Symbol *gTimer;
extern Symbol *gTaskID;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;
extern VarSymbol *gTryToken; // try token for conditional function resolution
extern VarSymbol *gBoundsChecking;
extern VarSymbol *gCastChecking;
extern VarSymbol *gDivZeroChecking;
extern VarSymbol *gPrivatization;
extern VarSymbol *gLocal;
extern VarSymbol *gNodeID;
extern VarSymbol *gModuleInitIndentLevel;
extern FnSymbol *gPrintModuleInitFn;
extern FnSymbol *gAddModuleFn;
extern FnSymbol *gChplHereAlloc;
extern FnSymbol *gChplHereFree;
extern FnSymbol *gChplDoDirectExecuteOn;
extern FnSymbol *gGenericTupleTypeCtor;
extern FnSymbol *gGenericTupleInit;
extern FnSymbol *gGenericTupleDestroy;
extern FnSymbol *gChplDeleteError;

// These global symbols point to generic functions that
// will be instantiated.
extern FnSymbol *gBuildTupleType;
extern FnSymbol *gBuildStarTupleType;
extern FnSymbol *gBuildTupleTypeNoRef;
extern FnSymbol *gBuildStarTupleTypeNoRef;

extern Symbol *gSyncVarAuxFields;
extern Symbol *gSingleVarAuxFields;

extern std::map<FnSymbol*,int> ftableMap;
extern std::vector<FnSymbol*> ftableVec;

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
