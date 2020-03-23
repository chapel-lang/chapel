/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "astutil.h"
#include "flags.h"
#include "library.h"
#include "type.h"

#include <bitset>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#ifdef HAVE_LLVM
// Forward declare MDNode.
namespace llvm
{
  class MDNode;
  class Function;
}
#endif

class BasicBlock;
class BlockStmt;
class DefExpr;
class FnSymbol;
class Immediate;
class IteratorInfo;
class Stmt;
class SymExpr;

const int INTENT_FLAG_IN          = 0x01;
const int INTENT_FLAG_OUT         = 0x02;
const int INTENT_FLAG_CONST       = 0x04;
const int INTENT_FLAG_REF         = 0x08;
const int INTENT_FLAG_PARAM       = 0x10;
const int INTENT_FLAG_TYPE        = 0x20;
const int INTENT_FLAG_BLANK       = 0x40;
const int INTENT_FLAG_MAYBE_CONST = 0x80;

// If this enum is modified, ArgSymbol::intentDescrString()
// and intentDescrString(IntentTag) should also be updated to match
enum IntentTag {
  INTENT_IN              = INTENT_FLAG_IN,
  INTENT_OUT             = INTENT_FLAG_OUT,
  INTENT_INOUT           = INTENT_FLAG_IN          | INTENT_FLAG_OUT,
  INTENT_CONST           = INTENT_FLAG_CONST,
  INTENT_CONST_IN        = INTENT_FLAG_CONST       | INTENT_FLAG_IN,
  INTENT_REF             = INTENT_FLAG_REF,
  INTENT_CONST_REF       = INTENT_FLAG_CONST       | INTENT_FLAG_REF,
  INTENT_REF_MAYBE_CONST = INTENT_FLAG_MAYBE_CONST | INTENT_FLAG_REF,
  INTENT_PARAM           = INTENT_FLAG_PARAM,
  INTENT_TYPE            = INTENT_FLAG_TYPE,
  INTENT_BLANK           = INTENT_FLAG_BLANK
};

typedef std::bitset<NUM_FLAGS> FlagSet;

/*
enum ForallIntentTag : task- or forall-intent tags

TFI_IN_PARENT
  The compiler adds this shadow var during resolution for each TFI_IN
  and TFI_CONST_IN. A TFI_IN_PARENT represents the task function's formal
  that the corresponding TFI_IN or TFI_CONST_IN is to be initialized from.

TFI_REDUCE
  This shadow var replaces the uses of the outer variable in the loop body
  in case of a 'reduce' intent. This is done in parsing and scopeResolve.
  It is analogous to the TFI_IN shadow var for an 'in' intent.
  A TFI_REDUCE var represents the current task's accumulation state.

TFI_REDUCE_*
  The compiler adds one each of these shadow vars during resolution
  for each TFI_REDUCE. They represent:

  TFI_REDUCE_OP        - the current task's reduction OP
  TFI_REDUCE_PARENT_AS - the parent task's Accumulation State
  TFI_REDUCE_PARENT_OP - the parent task's reduction OP

  The *PARENT* vars, like TFI_IN_PARENT, are the task function's formals.

The remaining tags should be self-explanatory.
*/
enum ForallIntentTag {
  // user-specified intents
  TFI_DEFAULT,  // aka TFI_BLANK
  TFI_CONST,                       // ShadowVarSymbol nicknames:
  TFI_IN,                          //   SI
  TFI_CONST_IN,                    //   "
  TFI_REF,                         //   SR
  TFI_CONST_REF,                   //   "
  TFI_REDUCE,                      //   AS    (for Accumulation State)
  TFI_TASK_PRIVATE,                //   TPV
  // compiler-added helpers
  TFI_IN_PARENT,                   //   INP
  TFI_REDUCE_OP,                   //   RP    (for Reduce oP)
  TFI_REDUCE_PARENT_AS,            //   PAS
  TFI_REDUCE_PARENT_OP,            //   PRP
};

const char* forallIntentTagDescription(ForallIntentTag tfiTag);

// for task intents and forall intents
ArgSymbol* tiMarkForForallIntent(ShadowVarSymbol* svar);

// parser support
enum ShadowVarPrefix {
  SVP_CONST,
  SVP_IN,
  SVP_CONST_IN,
  SVP_REF,
  SVP_CONST_REF,
  SVP_VAR,
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class Symbol : public BaseAST {
public:
  // Interface for BaseAST
  virtual GenRet     codegen();
          bool       inTree();
  virtual QualifiedType qualType();
  virtual void       verify();

  // Note: copy may add copied Symbols to the supplied map
  virtual Symbol*    copy(SymbolMap* map      = NULL,
                          bool       internal = false)           = 0;
  virtual void       replaceChild(BaseAST* oldAst,
                                  BaseAST* newAst)               = 0;

  virtual bool       isConstant()                              const;
  virtual bool       isConstValWillNotChange();
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

  bool               isKnownToBeGeneric();
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
  // Same, considering only defs under 'parent'.
  SymExpr*           getSingleDefUnder(Symbol* parent)         const;

  // The compiler really ought to view a call to `init` that
  // constructs a const record as the single "def". However it
  // might consider it a "use" for various reasons. This method
  // is useful for finding such cases.
  // This function finds the statement expression that is responsible
  // for initializing this symbol.
  // It can return NULL if it's unable to make sense of the AST pattern.
  Expr*              getInitialization()                       const;

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
bool isBytes(Symbol* symbol);

/************************************* | **************************************
*                                                                             *
* This class's role is to serve as a common abstract base class for           *
* VarSymbol and ArgSymbol.                                                    *
*                                                                             *
************************************** | *************************************/

class LcnSymbol : public Symbol {
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class VarSymbol : public LcnSymbol {
public:
  // Note that string immediate values are stored
  // with C escapes - that is newline is 2 chars \ n
  Immediate   *immediate;

  //changed isconstant flag to reflect var, const, param: 0, 1, 2
  VarSymbol(const char* init_name, Type* init_type = dtUnknown);
  VarSymbol(const char* init_name, QualifiedType qType);
  virtual ~VarSymbol();

  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(VarSymbol);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual bool       isConstant()                              const;
  virtual bool       isConstValWillNotChange();
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

protected:
  // for subclasses
  VarSymbol(AstTag astTag, const char* initName, Type* initType);

public:
#ifdef HAVE_LLVM
  llvm::MDNode *llvmDIGlobalVariable;
  llvm::MDNode *llvmDIVariable;
#else
  void* llvmDIGlobalVariable;
  void* llvmDIVariable;
#endif
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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
  virtual bool    isConstValWillNotChange();
  virtual bool    isParameter()                             const;

  virtual bool    isVisible(BaseAST* scope)                 const;

  bool            requiresCPtr();
  const char*     intentDescrString();

  GenRet          codegenType();

  std::string     getPythonType(PythonFileType pxd);
  std::string     getPythonDefaultValue();
  std::string     getPythonArgTranslation();

  IntentTag       intent;
  IntentTag       originalIntent; // stores orig intent after resolve intents
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class ShadowVarSymbol : public VarSymbol {
public:
  ShadowVarSymbol(ForallIntentTag iIntent,
                  const char* iName,
                  SymExpr* outerVar,
                  Expr* iSpec = NULL);

  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_SYMBOL_COPY(ShadowVarSymbol);

  virtual void    replaceChild(BaseAST* oldAst, BaseAST* newAst);
  virtual bool    isConstant()                              const;
  virtual bool    isConstValWillNotChange();

  const char* intentDescrString() const;
  bool        isReduce()          const { return intent == TFI_REDUCE;       }
  bool        isTaskPrivate()     const { return intent == TFI_TASK_PRIVATE; }

  static ShadowVarSymbol* buildForPrefix(ShadowVarPrefix prefix,
                                         Expr* name, Expr* type, Expr* init);
  static ShadowVarSymbol* buildFromReduceIntent(Expr* ovar, Expr* riExpr);

  // The outer variable or NULL if not applicable.
  Symbol* outerVarSym()    const;

  // Returns the EXPR in "with (EXPR reduce x)".
  Expr*  reduceOpExpr()    const;

  BlockStmt* initBlock()   const { return svInitBlock; }
  BlockStmt* deinitBlock() const { return svDeinitBlock; }

  // Convert between TFI_[CONST]_IN and TFI_IN_PARENT svars.
  ShadowVarSymbol* ParentvarForIN() const;
  ShadowVarSymbol* INforParentvar() const;
  // Convert between TFI_REDUCE and TFI_REDUCE_* svars.
  ShadowVarSymbol* ReduceOpForAccumState() const;
  ShadowVarSymbol* AccumStateForReduceOp() const;
  ShadowVarSymbol* ReduceOpForParentRP()   const;
  ShadowVarSymbol* AccumStateForParentAS() const;

  // Remove no-longer-needed references to outside symbols when lowering.
  void     removeSupportingReferences();

  // The intent for this variable.
  ForallIntentTag intent;

  // Reference to the outer variable. NULL for task-private variables.
  SymExpr* outerVarSE;

  // For a reduce intent, the reduce expression, wrapped in a block.
  // Otherwise NULL.
  BlockStmt* specBlock;

  // Corresponding actions to be performed at task startup and teardown.
  BlockStmt* svInitBlock;      // always present
  BlockStmt* svDeinitBlock;    //  "

  // Once pruning is no longer needed, this should be removed.
  bool pruneit;
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
  llvm::MDNode* llvmTbaaTypeDescriptor;       // scalar type descriptor
  llvm::MDNode* llvmTbaaAccessTag;            // scalar access tag
  llvm::MDNode* llvmConstTbaaAccessTag;       // scalar const access tag
  llvm::MDNode* llvmTbaaAggTypeDescriptor;    // aggregate type descriptor
  llvm::MDNode* llvmTbaaAggAccessTag;         // aggregate access tag
  llvm::MDNode* llvmConstTbaaAggAccessTag;    // aggregate const access tag
  llvm::MDNode* llvmTbaaStructCopyNode;       // tbaa.struct for memcpy
  llvm::MDNode* llvmConstTbaaStructCopyNode;  // const tbaa.struct
  llvm::MDNode* llvmDIType;
#else
  // Keep same layout so toggling HAVE_LLVM
  // will not lead to build errors without make clean
  void* llvmType;
  void* llvmTbaaTypeDescriptor;
  void* llvmTbaaAccessTag;
  void* llvmConstTbaaAccessTag;
  void* llvmTbaaAggTypeDescriptor;
  void* llvmTbaaAggAccessTag;
  void* llvmConstTbaaAggAccessTag;
  void* llvmTbaaStructCopyNode;
  void* llvmConstTbaaStructCopyNode;
  void* llvmDIType;
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
  // TBAA metadata for complex types
  void codegenCplxMetadata();
  // TBAA metadata for aggregates
  void codegenAggMetadata();

  const char* doc;

  BlockStmt* instantiationPoint;
  astlocT userInstantiationPointLoc;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

#include "FnSymbol.h"

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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

#include "ModuleSymbol.h"

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Checks whether a string is valid in UTF8 encoding
bool isValidString(std::string str);

// Processes a char* to replace any escape sequences with the actual bytes
std::string unescapeString(const char* const str, BaseAST* astForError);

// Creates a new string literal with the given value.
VarSymbol *new_StringSymbol(const char *s);
//
// Creates a new bytes literal with the given value.
VarSymbol *new_BytesSymbol(const char *s);
//
// Creates a new string or bytes literal with the given value.
VarSymbol *new_StringOrBytesSymbol(const char *s, AggregateType *at);

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

VarSymbol *new_CommIDSymbol(int64_t b);

VarSymbol *new_ImmediateSymbol(Immediate *imm);

// Get an Immediate stored in a VarSymbol or an EnumSymbol.
// When called on an EnumSymbol, requires that the enum type is already
// resolved.
Immediate *getSymbolImmediate(Symbol* sym);

void createInitStringLiterals();
void resetTempID();
FlagSet getRecordWrappedFlags(Symbol* s);
VarSymbol* newTemp(const char* name = NULL, Type* type = dtUnknown);
VarSymbol* newTemp(Type* type);
VarSymbol* newTemp(const char* name, QualifiedType qt);
VarSymbol* newTemp(QualifiedType qt);
VarSymbol* newTempConst(const char* name = NULL, Type* type = dtUnknown);
VarSymbol* newTempConst(Type* type);
VarSymbol* newTempConst(const char* name, QualifiedType qt);
VarSymbol* newTempConst(QualifiedType qt);

const char* intentDescrString(IntentTag intent);

// cache some popular strings
extern const char* astrSassign; // =
extern const char* astrSdot;    // .
extern const char* astrSeq;     // ==
extern const char* astrSne;     // !=
extern const char* astrSgt;     // >
extern const char* astrSgte;    // >=
extern const char* astrSlt;     // <
extern const char* astrSlte;    // <=
extern const char* astrSswap;   // <=>
extern const char* astr_cast;
extern const char* astr_defaultOf;
extern const char* astrInit;
extern const char* astrInitEquals;
extern const char* astrNew;
extern const char* astrDeinit;
extern const char* astrTag;
extern const char* astrThis;
extern const char* astrSuper;
extern const char* astr_chpl_cname;
extern const char* astr_chpl_forward_tgt;
extern const char* astr_chpl_manager;
extern const char* astr_chpl_statementLevelSymbol;
extern const char* astr_chpl_waitDynamicEndCount;
extern const char* astr_forallexpr;
extern const char* astr_forexpr;
extern const char* astr_loopexpr_iter;
extern const char* astrPostfixBang;
extern const char* astrBorrow;
extern const char* astr_init_coerce_tmp;

void initAstrConsts();

// Return true if the arg must use a C pointer whether or not
// pass-by-reference intents are used.
bool argMustUseCPtr(Type* t);

// Is 'expr' a SymExpr for the outerVar of some ShadowVarSymbol?
bool isOuterVarOfShadowVar(Expr* expr);

// The source of the PRIM_MOVE whose destination is this temp.
Expr* getDefOfTemp(SymExpr* origSE);

// Parser support.
void addForallIntent(CallExpr* fi, ShadowVarSymbol* svar);
void addTaskIntent(CallExpr* ti, ShadowVarSymbol* svar);

extern bool localTempNames;

extern HashMap<Immediate*, ImmHashFns, VarSymbol*> uniqueConstantsHash;
extern HashMap<Immediate*, ImmHashFns, VarSymbol*> stringLiteralsHash;

extern StringChainHash uniqueStringHash;

extern Symbol *gNil;
extern Symbol *gUnknown;
extern Symbol *gMethodToken;
// Pass this to a return-by-ref formal when the result is not needed.
// Used when inlining iterators for ForallStmts.
extern Symbol *gDummyRef;
extern Symbol *gTypeDefaultToken;
extern Symbol *gLeaderTag, *gFollowerTag, *gStandaloneTag;
extern Symbol *gModuleToken;
extern Symbol *gNoInit;
extern Symbol *gSplitInit;
extern Symbol *gVoid;
extern Symbol *gNone;
extern Symbol *gStringC;
extern Symbol *gOpaque;
extern Symbol *gTimer;
extern Symbol *gTaskID;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;
extern VarSymbol *gBoundsChecking;
extern VarSymbol *gCastChecking;
extern VarSymbol *gNilChecking;
extern VarSymbol *gOverloadSetsChecks;
extern VarSymbol *gDivZeroChecking;
extern VarSymbol *gPrivatization;
extern VarSymbol *gLocal;
extern VarSymbol *gWarnUnstable;
extern VarSymbol *gIteratorBreakToken;
extern VarSymbol *gNodeID;
extern VarSymbol *gModuleInitIndentLevel;
extern VarSymbol *gInfinity;
extern VarSymbol *gNan;
extern VarSymbol *gUninstantiated;

extern Symbol *gSyncVarAuxFields;
extern Symbol *gSingleVarAuxFields;

extern FnSymbol* chplUserMain;

namespace llvmStageNum {
typedef enum {
       // The first options here refer to high-level Chapel LLVM optimization
       NOPRINT = 0,
       NONE,
       BASIC,
       FULL,
       EVERY, // after every optimization if possible
       // These options allow instrumenting the pass pipeline
       // and match ExtensionPointTy in PassManagerBuilder
       EarlyAsPossible,
       ModuleOptimizerEarly,
       LoopOptimizerEnd,
       ScalarOptimizerLate,
       OptimizerLast,
       VectorizerStart,
       EnabledOnOptLevel0,
       Peephole,
       // Updating these? Be sure to leave LAST as the last
       // element and update llvmStageName to reflect this order.
       LAST,
     } llvmStageNum_t;
}
using llvmStageNum::llvmStageNum_t;

extern llvmStageNum_t llvmPrintIrStageNum;

const char *llvmStageNameFromLlvmStageNum(llvmStageNum_t stageNum);
llvmStageNum_t llvmStageNumFromLlvmStageName(const char* stageName);

void addNameToPrintLlvmIr(const char* name);
void addCNameToPrintLlvmIr(const char* name);

bool shouldLlvmPrintIrName(const char* name);
bool shouldLlvmPrintIrCName(const char* name);
bool shouldLlvmPrintIrFn(FnSymbol* fn);

#ifdef HAVE_LLVM
void printLlvmIr(const char* name, llvm::Function *func, llvmStageNum_t numStage);
#endif

void preparePrintLlvmIrForCodegen();
void completePrintLlvmIrStage(llvmStageNum_t numStage);

const char* toString(ArgSymbol* arg);
const char* toString(VarSymbol* var);

#endif
