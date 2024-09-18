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
#include "ResolveScope.h"
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "WhileDoStmt.h"
#include "build.h"
#include "config.h"
#include "global-ast-vecs.h"
#include "metadata.h"
#include "optimizations.h"
#include "parser.h"
#include "resolution.h"
#include "stmt.h"

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/global-strings.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/assertions.h"
#include "chpl/util/string-escapes.h"

#include "convert-help.h"

#include "llvm/ADT/SmallPtrSet.h"

#include <iostream>

using namespace chpl;
using namespace resolution;
using namespace uast;

// converts resolved uAST + types to typed AST
struct TConverter final : UastConverter {

  /// Nested Types ///
  using RV = ResolvedVisitor<TConverter>;

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

  // When converting a statement, where should we put new nodes?
  BlockStmt* curBlock = nullptr;

  bool trace = false;
  bool haveSetupModules = false;
  bool haveConvertedFunctions = false;
  // these are updated as we are converting different things
  ModTag topLevelModTag = MOD_USER;
  const ResolvedFunction* currentResolvedFunction = nullptr;

  // the untyped converter (temporarily, this is used for non-user code)
  owned<UastConverter> untypedConverter;

  // which modules / submodules to convert
  std::vector<ID> modulesToConvertVec;
  std::unordered_set<ID> modulesToConvert;

  // which functions to convert with types
  CalledFnsSet functionsToConvertWithTypes;

  // keeps track of which block we are currently in the process of creating
  std::vector<BlockStmt*> blockStack;
  // this block is where we store any result created at the top-level
  BlockStmt* scratchSpaceBlock = nullptr;

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

  TConverter(Context* context) : context(context) {
    SET_LINENO(rootModule);
    scratchSpaceBlock = new BlockStmt();
    curBlock = scratchSpaceBlock;

    untypedConverter = createUntypedConverter(context);
  }

  ~TConverter();

  // supporting UastConverter methods
  void setModulesToConvert(const std::vector<ID>& vec) override {
    modulesToConvert.clear();
    // save the vector
    modulesToConvertVec = vec;
    // add the modules to the set
    for (const ID& id : modulesToConvert) {
      modulesToConvert.insert(id);
    }

    // also tell the untyped converter about it
    untypedConverter->setModulesToConvert(vec);
  }

  void setFunctionsToConvertWithTypes(const CalledFnsSet& calledFns) override
  {
    functionsToConvertWithTypes = calledFns;
    untypedConverter->setFunctionsToConvertWithTypes(calledFns);
  }

  void useModuleWhenConverting(const chpl::ID& modId, ModuleSymbol* modSym) override {
    INT_FATAL("useModuleWhenConverting not expected for TConverter");
    modSyms[modId] = modSym;
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

  // blockStack helpers
  BlockStmt* pushBlock() {
    auto newBlockStmt = new BlockStmt();
    blockStack.push_back(newBlockStmt);
    curBlock = newBlockStmt;
    return newBlockStmt;
  }
  void popBlock() {
    CHPL_ASSERT(blockStack.size() > 0);
    blockStack.pop_back();
    if (blockStack.size() > 0)
      curBlock = blockStack.back();
    else
      curBlock = scratchSpaceBlock;
  }

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
  //Expr* convertTypeExpression(const AstNode* node);
  //Expr* convertTypeExpressionOrNull(const AstNode* node);
  //ShadowVarSymbol* convertTaskVar(const TaskVar* node);


  // visit functions
  bool enter(const NamedDecl* ast, RV& rv);
  void exit(const NamedDecl* ast, RV& rv);

  bool enter(const OpCall* ast, RV& rv);
  void exit(const OpCall* ast, RV& rv);

  bool enter(const FnCall* ast, RV& rv);
  void exit(const FnCall* ast, RV& rv);

  bool enter(const Return* ast, RV& rv);
  void exit(const Return* ast, RV& rv);

  bool enter(const Throw* ast, RV& rv);
  void exit(const Throw* ast, RV& rv);

  bool enter(const Yield* ast, RV& rv);
  void exit(const Yield* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const uast::Conditional* node, RV& rv);
  void exit(const uast::Conditional* node, RV& rv);

  bool enter(const uast::Select* node, RV& rv);
  void exit(const uast::Select* node, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

TConverter::~TConverter() { }

Expr* TConverter::convertAST(const AstNode* node) {
  CHPL_UNIMPL("convertAST");
  return nullptr;
}

ModuleSymbol* TConverter::convertToplevelModule(const Module* mod,
                                                ModTag modTag) {
  if (!haveSetupModules) {
    // create empty modules so we have somewhere to put the functions!
    setupModulesToConvert();
    haveSetupModules = true;
  }

  astlocMarker markAstLoc(mod->id());

  CHPL_UNIMPL("convertToplevelModule");
  return nullptr;
}

void TConverter::postConvertApplyFixups() {
  CHPL_UNIMPL("postConvertApplyFixups");
}

void TConverter::setupModulesToConvert() {
  // create each module now since we might add a function to it
  // the order in which this is done is not particularly
  // important since they will be filled-in in module init order
  // via calls to convertToplevelModule.
  for (const ID& modId : modulesToConvertVec) {
    setupModule(modId);
  }
}


void TConverter::convertFunctionsToConvert() {
  // remember to sort them by depth!
}

ModuleSymbol* TConverter::setupModule(ID modId) {
  astlocMarker markAstLoc(modId);

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
  UniqueString unused;
  bool isFromLibraryFile = context->moduleIsInLibrary(modId, unused);

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

  attachSymbolAttributes(context, mod, modSym, isFromLibraryFile);

  // Save the empty module for later filling-in
  modSyms[modId] = modSym;

  printf("Created module[%i] for %s\n", modSym->id, modId.str().c_str());

  return modSym;
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}
