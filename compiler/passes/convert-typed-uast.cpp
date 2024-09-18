/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

/* This file supports conversion of uAST+types to the older AST used by
   the rest of the compiler. It uses recursive functions to perform
   the conversion.
 */

#include <iostream>

#include "convert-uast.h"

#include "CForLoop.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "DoWhileStmt.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "LoopExpr.h"
#include "ParamForLoop.h"
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "config.h"
#include "global-ast-vecs.h"
#include "optimizations.h"
#include "parser.h"
#include "resolution.h"
#include "ResolveScope.h"
#include "metadata.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/string-escapes.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/util/assertions.h"
#include "stmt.h"

#include "convert-help.h"

#include "llvm/ADT/SmallPtrSet.h"

using namespace chpl;
using namespace resolution;
using namespace uast;

// converts resolved uAST + types to typed AST
struct TConverter final : UastConverter {
  /// Nested Types ///

  // When converting variables etc. with @assertOnGpu or @blockSize,
  // we don't just create a DefExpr; we also create an enclosing block which
  // contains calls to primitives that implement @assertOnGpu and @blockSize.
  //
  // This data structure contains pointers to both.
  struct VariableDefInfo {
    DefExpr* variableDef;
    Expr* entireExpr;

    /**
      Helper for code that calls 'convertVariable' but doesn't expect to handle
      blocks with additional primitives, which can be introduced by that call
      for GPU attributes that need to be propagated to init expressions.
     */
    Expr* requireDefOnly() const {
      CHPL_ASSERT(entireExpr == variableDef);
      return variableDef;
    }
  };

  // State required to mimic the desugaring of multi-declarations into
  // regular ones.
  struct MultiDeclState {
    // The result of computing the target locale, stored in a temp, used
    // for remote variables.
    Symbol* localeTemp = nullptr;

    Symbol* typeTemp = nullptr;
    Symbol* prev = nullptr;
    Expr* prevTypeExpr = nullptr;
    Expr* prevInitExpr = nullptr;

    void reset() {
      typeTemp = nullptr;
      prev = nullptr;
      prevTypeExpr = nullptr;
      prevInitExpr = nullptr;
    }

    // For remote variables, these helpers don't modify the def point, since
    // def point is quite different (it's an invocation of a remote variable
    // wrapper builder with extra arguments).

    void replaceTypeExpr(Expr* newExpr) {
      if (localeTemp) {
        prevTypeExpr->replace(newExpr);
      } else {
        prev->defPoint->exprType = newExpr;
      }
      prevTypeExpr = nullptr;
    }

    void replaceInitExpr(Expr* newExpr) {
      if (localeTemp) {
        prevInitExpr->replace(newExpr);
      } else {
        prev->defPoint->init = newExpr;
      }
    }
  };


  /// Fields ///
  Context* context = nullptr;
  bool trace = false;
  bool haveConvertedFunctions = false;
  // these are updated as we are converting different things
  ModTag topLevelModTag = MOD_USER;
  const ResolvedFunction* currentResolvedFunction = nullptr;

  // which modules / submodules to convert
  std::unordered_set<ID> modulesToConvert;

  // which functions to convert with types
  CalledFnsSet functionsToConvertWithTypes;

  // to keep track of symbols that have been converted & fixups needed
  std::unordered_map<ID, ModuleSymbol*> modSyms;
  std::unordered_map<ID, Symbol*> syms;
  std::unordered_map<const ResolvedFunction*, FnSymbol*> fns;

  std::vector<std::pair<SymExpr*, ID>> identFixups;
  std::vector<std::pair<SymExpr*, const ResolvedFunction*>> callFixups;

  // stores type fixups that are needed
  std::vector<Symbol*> typeFixups;

  // stores a mapping from chpl::Type* to Type*
  std::unordered_map<const types::Type*, Type*> convertedTypes;


  /// Methods ///

  TConverter(Context* context)
    : context(context) {
  }

  ~TConverter();

  // supporting UastConverter methods
  void clearModulesToConvert() override {
    modulesToConvert.clear();
  }

  void addModuleToConvert(ID id) override {
    modulesToConvert.insert(std::move(id));
  }

  void setFunctionsToConvertWithTypes(CalledFnsSet calledFns) override
  {
    functionsToConvertWithTypes.swap(calledFns);
  }


  Expr* convertAST(const AstNode* node) override;

  ModuleSymbol* convertToplevelModule(const Module* mod,
                                      ModTag modTag) override;

  void postConvertApplyFixups() override;


  // main entry points for converting
  void setupModulesToConvert();
  void convertFunctionsToConvert();

  ModuleSymbol* setupModule(ID modId);
  ModuleSymbol* convertModule(const Module* mod);
  FnSymbol* convertFunction(const ResolvedFunction* r);

  // general functions for converting
  Type* convertType(const types::Type* t);
  Symbol* convertParam(types::QualifiedType qt);

  // type conversion helpers
  Type* helpConvertType(const types::Type* t);
  Type* convertClassType(const types::ClassType* t);
  Type* convertCPtrType(const types::CPtrType* t);
  Type* convertEnumType(const types::EnumType* t);
  Type* convertExternType(const types::ExternType* t);
  Type* convertFunctionType(const types::FunctionType* t);
  Type* convertBasicClassType(const types::BasicClassType* t);
  Type* convertRecordType(const types::RecordType* t);
  Type* convertTupleType(const types::TupleType* t);
  Type* convertUnionType(const types::UnionType* t);
  Type* convertBoolType(const types::BoolType* t);
  Type* convertComplexType(const types::ComplexType* t);
  Type* convertImagType(const types::ImagType* t);
  Type* convertIntType(const types::IntType* t);
  Type* convertRealType(const types::RealType* t);
  Type* convertUintType(const types::UintType* t);

  // methods to help track what has been converted & handle fixups
  void noteConvertedSym(const AstNode* ast, Symbol* sym);
  void noteConvertedFn(const ResolvedFunction* r, FnSymbol* fn);
  Symbol* findConvertedSym(ID id, bool neverTrace=false);
  Symbol* findConvertedFn(const ResolvedFunction* r, bool neverTrace=false);
  void noteIdentFixupNeeded(SymExpr* se, ID id);
  void noteCallFixupNeeded(SymExpr* se, const ResolvedFunction* r);
  void noteTypeFixupNeeded(Symbol* sym);

  void noteAllContainedFixups(BaseAST* ast, int depth);

  // helper methods
  Expr* errNotDirectly() {
    // call this from a visit method to indicate it shouldn't be called
    // directly; rather the processing of this form should happen in
    // something that contains it, syntactically.
    INT_FATAL("Should not be called directly!");
    return nullptr;
  }

  bool shouldConvertWithoutTypes(const ResolvedFunction* r) {
    return functionsToConvertWithTypes.count(r) == 0;
  }
  bool shouldConvertModInitWithoutTypes(const Module* mod) {
    // TODO: remove this constraint
    return topLevelModTag != MOD_USER;
  }

  Expr* convertExprOrNull(const AstNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
  }

  //Expr* resolvedIdentifier(const Identifier* node);
  //BlockStmt* convertExplicitBlock(AstListIteratorPair<AstNode> stmts,
  //                                bool flattenTopLevelScopelessBlocks);
  //BlockStmt* convertImplicitBlock(AstListIteratorPair<AstNode> stmts);
  //BlockStmt* createBlockWithStmts(AstListIteratorPair<AstNode> stmts,
             //                     BlockStyle style,
             //                     bool flattenTopLevelScopelessBlocks=true);
  //Expr* singleExprFromStmts(AstListIteratorPair<AstNode> stmts);
  // CallExpr* convertWithClause(const WithClause* node, const AstNode* parent);
  //CallExpr* convertNewManagement(const New* node);
  //Expr* convertScanReduceOp(const AstNode* node);
  //Expr* convertLoopIndexDecl(const Decl* node);
  //bool isBracketLoopMaybeArrayType(const BracketLoop* node);
  //Expr* convertBracketLoopExpr(const BracketLoop* node);
  //Expr* tryExtractFilterCond(const IndexableLoop* node, Expr*& cond);
  //Expr* convertForallLoopExpr(const Forall* node);
  //Expr* convertCalledKeyword(const AstNode* node, const Call* inCall);
  //Expr* convertSparseKeyword(const FnCall* node);
  //CallExpr* convertModuleDotCall(const FnCall* node);
  //Expr* convertDmappedOp(const OpCall* node);
  //Expr* convertTupleExpand(const OpCall* node);
  //Expr* convertReduceAssign(const OpCall* node);
  //Expr* convertToNilableChecked(const OpCall* node);
  //Expr* convertLogicalAndAssign(const OpCall* node);
  //Expr* convertLogicalOrAssign(const OpCall* node);
  //Expr* convertTupleAssign(const OpCall* node);
  //Expr* convertRegularBinaryOrUnaryOp(const OpCall* node,
  //                                    const char* name=nullptr);
  //BlockStmt* convertTupleDeclComponents(const TupleDecl* node);
  //Expr* convertLifetimeClause(const AstNode* node);
  //CallExpr* convertLifetimeIdent(const Identifier* node);
  //FnSymbol* convertFunction(const Function* node);
  //FnSymbol* convertFunctionSignature(const FunctionSignature* node);
  //CallExpr* convertArrayType(const BracketLoop* node);
  //VariableDefInfo convertVariable(const Variable* node,
  //                         bool useLinkageName,
  //                         MultiDeclState* multiState = nullptr);
  //DefExpr* convertEnumElement(const EnumElement* node);
  //template <typename Iterable>
  //void convertInheritsExprs(const Iterable& iterable,
  //                          std::vector<Expr*>& inherits,
  //                          bool& inheritMarkedGeneric);
  //Expr* convertAggregateDecl(const AggregateDecl* node);

 
  // can some of these be shared with the other Converter?
  //const char* convertLinkageNameAstr(const Decl* node);
  //Flag convertFlagForDeclLinkage(const AstNode* node);
  //Flag convertPragmaToFlag(PragmaTag pragma);
  //void attachSymbolAttributes(const Decl* node, Symbol* sym);
  //void attachSymbolVisibility(const Decl* node, Symbol* sym);
  //UnresolvedSymExpr* reservedWordToInternalName(UniqueString name);
  //Expr* reservedWordRemapForIdent(UniqueString name) {
  // LLVMMetadataList extractLlvmAttributesAndRejectOthers(const Loop* node);
  // static RetTag convertRetTag(Function::ReturnIntent returnIntent);
  //static bool isAssignOp(UniqueString name);
  //static const char* createAnonymousRoutineName(const Function* node);
  //static const char*
  //convertFunctionNameAndAstr(const Function* node);
  //const char* constructUserString(const Function* node);
  //const char* constructUserString(const FunctionSignature* node);
  //static IntentTag convertFormalIntent(Formal::Intent intent);
  //Expr* convertTypeExpression(const AstNode* node);
  //Expr* convertTypeExpressionOrNull(const AstNode* node);
  //ShadowVarPrefix convertTaskVarIntent(const TaskVar* node);
  //ShadowVarSymbol* convertTaskVar(const TaskVar* node);
  //const char* sanitizeVarName(const char* name) {
  //void attachSymbolStorage(const Variable::Kind kind, Symbol* vs);
  //void attachSymbolStorage(const TupleDecl::IntentOrKind iok, Symbol* vs);
  //void attachSymbolStorage(const Qualifier kind, Symbol* vs);
  //static bool isEnsureDomainExprCall(Expr* expr);
  //AggregateTag convertAggregateDeclTag(const AggregateDecl* node) {
 

  // visit functions

  Expr* visit(const Comment* node) { return nullptr; }
  Expr* visit(const AttributeGroup* node) { return errNotDirectly(); }
  Expr* visit(const Attribute* node) { return errNotDirectly(); }
  Expr* visit(const ErroneousExpression* node);

  /// SimpleBlockLikes ///
  Expr* visit(const Begin* node);
  BlockStmt* visit(const Block* node);
  Expr* visit(const Defer* node);
  Expr* visit(const Local* node);
  Expr* visit(const Manage* node);
  BlockStmt* visit(const On* node);
  BlockStmt* visit(const Serial* node);
  CondStmt* visit(const When* node);

  /// Use/Import/etc ///
  Expr* visit(const Include* node) { return nullptr; }
  Expr* visit(const Import* node) { return nullptr; }
  Expr* visit(const As* node) { return nullptr; }
  BlockStmt* visit(const Use* node) { return nullptr; }
  Expr* visit(const VisibilityClause* node) { return nullptr; }

  /// Statements ///
  Expr* visit(const Implements* node);
  BlockStmt* visit(const Delete* node);
  Expr* visit(const ExternBlock* node);
  Expr* visit(const Require* node);
  Expr* visit(const Init* node);
  Expr* visit(const Let* node);

  /// Control Flow Statements ///
  BlockStmt* visit(const Break* node);
  CatchStmt* visit(const Catch* node);
  Expr* visit(const Conditional* node);
  BlockStmt* visit(const Continue* node);
  Expr* visit(const Label* node);
  CallExpr* visit(const Return* node);
  BlockStmt* visit(const Select* node);
  CallExpr* visit(const Throw* node);
  CallExpr* visit(const Yield* node);
  Expr* visit(const Try* node);

  /// Non-Loop Parallelism Support ///
  Expr* visit(const Cobegin* node);
  BlockStmt* visit(const Sync* node);

  /// Expressions ///

  Expr* visit(const Identifier* node);
  Expr* visit(const Dot* node);
  Expr* visit(const New* node) { return errNotDirectly(); }
  Expr* visit(const WithClause* node) { return errNotDirectly(); }
  DefExpr* visit(const TypeQuery* node);

  /// Loops ///

  BlockStmt* visit(const DoWhile* node);
  BlockStmt* visit(const While* node);

  /// IndexableLoops ///

  // Note that bracket loop statements in type expressions for variables need to
  // be handled by a separate builder, as those are array types.
  Expr* visit(const BracketLoop* node);

  Expr* visit(const Coforall* node);
  Expr* visit(const For* node);
  Expr* visit(const Forall* node);
  Expr* visit(const Foreach* node);

  /// Compound Literals: Array, Domain, Range, Tuple ///

  Expr* visit(const Array* node);
  Expr* visit(const Domain* node);
  CallExpr* visit(const Range* node);
  Expr* visit(const Tuple* node);

  /// Literals ///

  Expr* visit(const BoolLiteral* node);

  /// NumericLiterals ///

  Expr* visit(const ImagLiteral* node);
  Expr* visit(const IntLiteral* node);
  Expr* visit(const RealLiteral* node);
  Expr* visit(const UintLiteral* node);

  /// StringLikeLiterals ///
  Expr* visit(const BytesLiteral* node);
  Expr* visit(const CStringLiteral* node);
  Expr* visit(const StringLiteral* node);

  /// Calls ///

  Expr* visit(const FnCall* node);
  Expr* visit(const OpCall* node);
  Expr* visit(const PrimCall* node);
  // Note that this conversion is for the reduce expression, and not for
  // the reduce intent (see conversion for 'WithClause').
  Expr* visit(const Reduce* node);
  Expr* visit(const ReduceIntent* reduce) { return errNotDirectly(); }
  Expr* visit(const Scan* node);
  Expr* visit(const Zip* node);

  /// Decls ///

  Expr* visit(const MultiDecl* node);
  BlockStmt* visit(const TupleDecl* node);
  Expr* visit(const ForwardingDecl* node);

  /// NamedDecls ///

  Expr* visit(const EnumElement* node) { return errNotDirectly(); }
  Expr* visit(const AnonFormal* node);
  Expr* visit(const FunctionSignature* node);
  Expr* visit(const Function* node);
  Expr* visit(const Interface* node);
  DefExpr* visit(const Module* node);

  /// VarLikeDecls ///

  DefExpr* visit(const Formal* node);
  Expr* visit(const TaskVar* node) { return errNotDirectly(); }
  Expr* visit(const VarArgFormal* node);
  Expr* visit(const Variable* node);

  /// TypeDecls ///

  Expr* visit(const Enum* node);

  /// AggregateDecls

  Expr* visit(const Class* node);
  Expr* visit(const Record* node);
  Expr* visit(const Union* node);
  Expr* visit(const EmptyStmt* node) { return new BlockStmt(); }
};

TConverter::~TConverter() { }

Expr* TConverter::convertAST(const AstNode* node) {
  CHPL_UNIMPL("convertAST");
  return nullptr;
}

ModuleSymbol* TConverter::convertToplevelModule(const Module* mod,
                                                ModTag modTag) {
  CHPL_UNIMPL("convertToplevelModule");
  return nullptr;
}

void TConverter::postConvertApplyFixups() {
  CHPL_UNIMPL("convertToplevelModule");
}

void TConverter::setupModulesToConvert() {
  // sort the modules to convert
  std::vector<ID> mods;
  for (const ID& modId : modulesToConvert) {
    mods.push_back(modId);
  }
  std::sort(mods.begin(), mods.end());

  // create each module now since we might add a function to it
  // the order in which this is done is not particularly
  // important since they will be filled-in in module init order
  // via calls to convertToplevelModule.
  for (const ID& modId : mods) {
    setupModule(modId);
  }
}


void TConverter::convertFunctionsToConvert() {
  // remember to sort them by depth!
}

ModuleSymbol* TConverter::setupModule(ID modId) {
  UniqueString path;
  bool found = context->filePathForId(modId, path);
  INT_ASSERT(found);

  // compute the modTag
  ModTag modTag = MOD_USER;
  if (chpl::parsing::filePathIsInInternalModule(context, path)) {
    modTag = MOD_INTERNAL;
  } else if (chpl::parsing::filePathIsInStandardModule(context, path)) {
    modTag = MOD_STANDARD;
  } else if (chpl::parsing::filePathIsInBundledModule(context, path)) {
    // TODO: this considers code in modules/packages as MOD_STANDARD but
    // we would like this to be MOD_USER.
    // See also issue #24998.
    modTag = MOD_STANDARD;
  }

  const AstNode* ast = parsing::idToAst(context, modId);
  if (ast == nullptr || !ast->isModule()) {
    INT_FATAL("module ID expected");
  }
  const Module* mod = ast->toModule();
  //UniqueString unused;
  //bool isFromLibraryFile = context->moduleIsInLibrary(modId, unused);

  const char* name = astr(mod->name());
  UniqueString pathUstr;
  UniqueString ignoredParentSymPath;

  bool priv = (mod->visibility() == uast::Decl::PRIVATE);
  bool prototype = (mod->kind() == uast::Module::PROTOTYPE ||
                    mod->kind() == uast::Module::IMPLICIT);

  ModuleSymbol* modSym = buildModule(name,
                                     modTag,
                                     new BlockStmt(),
                                     astr(path),
                                     priv,
                                     prototype);

  if (mod->kind() == uast::Module::IMPLICIT) {
    modSym->addFlag(FLAG_IMPLICIT_MODULE);
  }

  // TODO attachSymbolAttributes(mod, modSym);

  // Save the empty module for later filling-in
  modSyms[modId] = modSym;

  return modSym;
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}
