/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "astutil.h"
#include "baseAST.h"
#include "chpl/framework/ID.h"
#include "chpl/resolution/resolution-types.h"
#include "flags.h"
#include "intents.h"
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
  class FunctionType;
}
#endif

class BasicBlock;
class BlockStmt;
class DefExpr;
class FnSymbol;
class IteratorInfo;
class Stmt;
class SymExpr;
struct InterfaceReps;

typedef std::bitset<NUM_FLAGS> FlagSet;

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

// Ensures consistent iteration order over std::map<const char*,...>
struct CharStarComparator {
  bool operator()(const char* lhs, const char* rhs) const {
    return strcmp(lhs, rhs) < 0;
  }
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class Symbol : public BaseAST {
public:
  // Interface for BaseAST
  GenRet         codegen()   override;
  bool           inTree()    override;
  QualifiedType  qualType()  override;
  void           verify()    override;

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

  std::string deprecationMsg;
  const char* getDeprecationMsg() const;
  void maybeGenerateDeprecationWarning(Expr* context);


  std::string unstableMsg;
  const char* getUnstableMsg() const;
  void maybeGenerateUnstableWarning(Expr* context);

  const char* getSanitizedMsg(std::string msg) const;

protected:
                     Symbol(AstTag      astTag,
                            const char* init_name,
                            Type*       init_type = dtUnknown);

                    ~Symbol() override;

private:
  virtual void       codegenPrototype(); // ie type decl
  virtual Symbol*    copyInner(SymbolMap* map) = 0;


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

           ~LcnSymbol() override = default;

private:

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
 ~VarSymbol() override;

  void   verify()                                            override;
  void   accept(AstVisitor* visitor)                         override;
  DECLARE_SYMBOL_COPY(VarSymbol);
  VarSymbol* copyInner(SymbolMap* map)                       override;

  void   replaceChild(BaseAST* old_ast, BaseAST* new_ast)    override;

  bool   isConstant()                                  const override;
  bool   isConstValWillNotChange()                           override;
  bool   isImmediate()                                 const override;
  bool   isParameter()                                 const override;
  bool   isType()                                               const;

  GenRet codegenVarSymbol(bool lhsInSetReference=false);
  GenRet codegen()                                           override;
  void   codegenDefC(bool global = false, bool isHeader = false);
  void   codegenDef()                                        override;
  // global vars are different ...
  void   codegenGlobalDef(bool isHeader);

  void makeField();

private:
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

class ArgSymbol final : public LcnSymbol {
public:
  ArgSymbol(IntentTag   iIntent,
            const char* iName,
            Type*       iType,
            Expr*       iTypeExpr     = NULL,
            Expr*       iDefaultExpr  = NULL,
            Expr*       iVariableExpr = NULL);


  // Interface for BaseAST
  GenRet codegen()                                       override;

  void   verify()                                        override;
  void   accept(AstVisitor* visitor)                     override;
  DECLARE_SYMBOL_COPY(ArgSymbol);
  ArgSymbol* copyInner(SymbolMap* map)                   override;


  // Interface for Symbol
  void   replaceChild(BaseAST* oldAst, BaseAST* newAst)  override;
  bool   isConstant()                              const override;
  bool   isConstValWillNotChange()                       override;
  bool   isParameter()                             const override;

  bool   isVisible(BaseAST* scope)                 const override;

  bool            requiresCPtr();
  const char*     intentDescrString() const;

  GenRet          codegenType();

  std::string     getPythonType(PythonFileType pxd);
  std::string     getPythonDefaultValue();
  std::string     getPythonArgTranslation();

  std::string     demungeVarArgName(std::string* num=NULL);

  IntentTag       intent;
  IntentTag       originalIntent; // stores orig intent after resolve intents
  bool            typeExprFromDefaultExpr;
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

class ShadowVarSymbol final : public VarSymbol {
public:
  ShadowVarSymbol(ForallIntentTag iIntent,
                  const char* iName,
                  SymExpr* outerVar,
                  Expr* iSpec = NULL);

  void  verify()                                           override;
  void  accept(AstVisitor* visitor)                        override;
  DECLARE_SYMBOL_COPY(ShadowVarSymbol);
  ShadowVarSymbol* copyInner(SymbolMap* map)               override;


  void  replaceChild(BaseAST* oldAst, BaseAST* newAst)     override;
  bool  isConstant()                                 const override;
  bool  isConstValWillNotChange()                          override;

  const char* intentDescrString() const;
  bool  isReduce()          const { return intent == TFI_REDUCE;       }
  bool  isTaskPrivate()     const { return intent == TFI_TASK_PRIVATE; }
  bool  isCompilerAdded()   const;

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

  // This svar is for a task intent or TPV that is explicit in user code.
  bool svExplicit;
};

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/


// These map from Chapel function types to LLVM function types. They
// live here rather than in 'llvmUtil.h' because of a name conflict
// between 'Type' and 'llvm::Type'.
#ifdef HAVE_LLVM
bool llvmMapUnderlyingFunctionType(FunctionType* k, llvm::FunctionType* v);
llvm::FunctionType* llvmGetUnderlyingFunctionType(FunctionType* t);
#endif

class TypeSymbol final : public Symbol {
 public:
  // We need to know whether or not the definition
  // for this type has already been codegen'd
  // and cache it if it has.
#ifdef HAVE_LLVM
  llvm::Type* llvmImplType;
  llvm::MDNode* llvmTbaaTypeDescriptor;       // scalar type descriptor
  llvm::MDNode* llvmTbaaAccessTag;            // scalar access tag
  llvm::MDNode* llvmConstTbaaAccessTag;       // scalar const access tag
  llvm::MDNode* llvmTbaaAggTypeDescriptor;    // aggregate type descriptor
  llvm::MDNode* llvmTbaaAggAccessTag;         // aggregate access tag
  llvm::MDNode* llvmConstTbaaAggAccessTag;    // aggregate const access tag
  llvm::MDNode* llvmTbaaStructCopyNode;       // tbaa.struct for memcpy
  llvm::MDNode* llvmConstTbaaStructCopyNode;  // const tbaa.struct
  llvm::MDNode* llvmDIType;
  llvm::Type* getLLVMStructureType();         // get structure type for class
  llvm::Type* getLLVMType();                  // get pointer to structure type for class
#else
  // Keep same layout so toggling HAVE_LLVM
  // will not lead to build errors without make clean
  void* llvmImplType;
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
  void  verify()                                          override;
  void  accept(AstVisitor* visitor)                       override;
  DECLARE_SYMBOL_COPY(TypeSymbol);
  TypeSymbol* copyInner(SymbolMap* map)                   override;

  void  replaceChild(BaseAST* old_ast, BaseAST* new_ast)  override;

  GenRet codegen()                                        override;
  void   codegenDef()                                     override;
  void   codegenPrototype()                               override;

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
* An InterfaceSymbol represents an interface declaration. An IfcConstraint or *
* an ImplementsStmt reference the interface being constrained or implemented  *
* via a SymExpr referring to the InterfaceSymbol.                             *
*                                                                             *
* An interface declaration looks like this:                                   *
*   interface InterfaceName(InterfaceFormal...) {                             *
*     proc requiredFunction(args...);                                         *
*     proc requiredFunctionWithDefaultImplementation(args...) {...}           *
*   }                                                                         *
*                                                                             *
************************************** | *************************************/

class InterfaceSymbol final : public Symbol {
public:
  static DefExpr* buildDef(const char* name, CallExpr* formals, BlockStmt* body);
  static DefExpr* buildFormal(const char* name, IntentTag intent);
  InterfaceSymbol(const char* name, BlockStmt* body);
 ~InterfaceSymbol() override;

  DECLARE_SYMBOL_COPY(InterfaceSymbol);
  InterfaceSymbol* copyInner(SymbolMap* map)             override;
  void  verify()                                         override;
  void  accept(AstVisitor* visitor)                      override;

  void  replaceChild(BaseAST* oldAst, BaseAST* newAst)   override;

  int   numFormals()   const { return ifcFormals.length; }
  int   numAssocCons() const { return associatedConstraints.size(); }

  // the DefExprs for each interface formal
  AList      ifcFormals;

  // the body block of the interface declaration, always non-null
  BlockStmt* ifcBody;

  // maps name to the ConstrainedType for an associated type
  // their DefExprs are in ifcBody
  std::map<const char*, ConstrainedType*, CharStarComparator> associatedTypes;

  // constraints to be checked for each implementation
  std::vector<IfcConstraint*> associatedConstraints;

  // representatives for the symbols in associatedConstraints' interfaces
  std::vector<InterfaceReps*> ifcReps;

  // each FnSymbol for the interface's required function is mapped
  //  - to itself, if there is a default implementation
  //  - to gDummyWitness, otherwise
  SymbolMap  requiredFns;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class EnumSymbol final : public Symbol {
public:
                  EnumSymbol(const char* initName);

  void  verify()                                        override;
  void  accept(AstVisitor* visitor)                     override;

  DECLARE_SYMBOL_COPY(EnumSymbol);
  EnumSymbol* copyInner(SymbolMap* map)                 override;

  void  replaceChild(BaseAST* oldAst, BaseAST* newAst)  override;
  void  codegenDef()                                    override;

  bool  isParameter()                             const override;

  Immediate* getImmediate();
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

class LabelSymbol final : public Symbol {
public:
  GotoStmt* iterResumeGoto;
  LabelSymbol(const char* init_name);

  void  verify()                                          override;
  void  accept(AstVisitor* visitor)                       override;
  DECLARE_SYMBOL_COPY(LabelSymbol);
  LabelSymbol* copyInner(SymbolMap* map)                  override;

  void  replaceChild(BaseAST* old_ast, BaseAST* new_ast)  override;
  void  codegenDef()                                      override;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

/* This type exists to be used temporarily during convert-uast.
   By using this type, convert-uast code can robustly handle
   AST copies from an AST node referring to something not yet converted.
   */
class TemporaryConversionSymbol final : public Symbol {
public:
  chpl::ID symId;
  const chpl::resolution::TypedFnSignature* sig;

  TemporaryConversionSymbol(chpl::ID symId);
  TemporaryConversionSymbol(const chpl::resolution::TypedFnSignature* sig);

  void  verify()                                          override;
  void  accept(AstVisitor* visitor)                       override;
  DECLARE_SYMBOL_COPY(TemporaryConversionSymbol);
  TemporaryConversionSymbol* copyInner(SymbolMap* map)    override;

  void  replaceChild(BaseAST* old_ast, BaseAST* new_ast)  override;
  void  codegenDef()                                      override;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/



inline bool Symbol::hasFlag(Flag flag) const {
  CHECK_FLAG(flag);
  return flags[flag];
}

inline void Symbol::addFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.set(flag);
}

inline void Symbol::copyFlags(const Symbol* other) {
  flags |= other->flags;
  qual   = other->qual;
}

inline void Symbol::removeFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.reset(flag);
}

inline bool Symbol::hasEitherFlag(Flag aflag, Flag bflag) const {
  return hasFlag(aflag) || hasFlag(bflag);
}

inline bool Symbol::isRef() {
  QualifiedType q = qualType();
  return (type != NULL) && (q.isRef() || type->symbol->hasFlag(FLAG_REF));
}

inline bool Symbol::isWideRef() {
  QualifiedType q = qualType();
  return (q.isWideRef() || type->symbol->hasFlag(FLAG_WIDE_REF));
}

inline bool Symbol::isRefOrWideRef() {
  return isRef() || isWideRef();
}

// Is this a compiler-added helper / not to be reported to user?
inline bool ShadowVarSymbol::isCompilerAdded() const {
  switch (intent) {
    case TFI_IN_PARENT:
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
      return true;
    default:
      return false;
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Checks whether a string is valid in UTF8 encoding
bool isValidString(std::string str, int64_t* numCodepoints);

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

// Creates a new real literal with the given value, where the
// bit-width will be taken as 32 for the 'float' case and 64 for the
// 'double' case.  The resulting symbol will have a cname equal to a
// normalized version of 'val'.
VarSymbol* new_RealSymbol(float val);
VarSymbol* new_RealSymbol(double val);

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
extern const char* astrScolon;  // :
extern const char* astr_defaultOf;
extern const char* astrInit;
extern const char* astrInitEquals;
extern const char* astrNew;
extern const char* astrDeinit;
extern const char* astrPostinit;
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
extern const char* astr_autoCopy;
extern const char* astr_initCopy;
extern const char* astr_coerceCopy;
extern const char* astr_coerceCopy;
extern const char* astr_coerceMove;
extern const char* astr_autoDestroy;

bool isAstrOpName(const char* name);

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
// Used in InterfaceSymbol::requiredFns
extern Symbol *gDummyWitness;
// Pass this to a return-by-ref formal when the result is not needed.
// Used when inlining iterators for ForallStmts.
extern Symbol *gDummyRef;
// used in convert-uast to mark a SymExpr needing future adjustment
extern Symbol *gFixupRequiredToken;
extern VarSymbol *gTrue;
extern VarSymbol *gFalse;
extern VarSymbol *gBoundsChecking;
extern VarSymbol *gCastChecking;
extern VarSymbol *gNilChecking;
extern VarSymbol *gOverloadSetsChecks;
extern VarSymbol *gDivZeroChecking;
extern VarSymbol *gCacheRemote;
extern VarSymbol *gPrivatization;
extern VarSymbol *gLocal;
extern VarSymbol *gWarnUnstable;
extern VarSymbol *gIteratorBreakToken;
extern VarSymbol *gNodeID;
extern VarSymbol *gModuleInitIndentLevel;
extern VarSymbol *gInfinity;
extern VarSymbol *gNan;
extern VarSymbol *gUninstantiated;
extern VarSymbol *gUseIOSerializers;

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
       ASM,
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
std::vector<std::string> gatherPrintLlvmIrCNames();

#ifdef HAVE_LLVM
void printLlvmIr(const char* name, llvm::Function *func, llvmStageNum_t numStage);
#endif

void preparePrintLlvmIrForCodegen();
void completePrintLlvmIrStage(llvmStageNum_t numStage);

const char* toString(ArgSymbol* arg, bool withTypeAndIntent);
const char* toString(VarSymbol* var, bool withType);
const char* toString(Symbol* sym, bool withTypeAndIntent);

struct SymbolMapKeyValue {
  Symbol *key, *value;
  SymbolMapKeyValue(Symbol* k, Symbol* v): key(k), value(v) { }
};
typedef std::vector<SymbolMapKeyValue> SymbolMapVector;
SymbolMapVector sortedSymbolMapElts(const SymbolMap& map);

#endif
