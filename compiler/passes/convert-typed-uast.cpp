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

  // What is the module / function we are converting?
  const AstNode* symbol = nullptr; // Module* or Function*

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
    // also tell the untyped converter about them so it can ignore them!
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
  ModuleSymbol* findOrSetupModule(ID modId);
  void convertModuleInit(const Module* mod, ModuleSymbol* modSym);
  void convertFunction(const ResolvedFunction* r);

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
  BlockStmt* pushNewBlock() {
    auto newBlockStmt = new BlockStmt();
    blockStack.push_back(newBlockStmt);
    curBlock = newBlockStmt;
    return curBlock;
  }
  BlockStmt* pushBlock(BlockStmt* block) {
    blockStack.push_back(block);
    curBlock = block;
    return curBlock;
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

  Expr* convertExprOrNull(const AstNode* node) {
    if (node == nullptr)
      return nullptr;

    Expr* ret = convertAST(node);
    INT_ASSERT(ret);
    return ret;
  }

  // helpers we might want to bring back from convert-uast.cpp
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
  /*bool enter(const NamedDecl* ast, RV& rv);
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

  bool enter(const Conditional* node, RV& rv);
  void exit(const Conditional* node, RV& rv);

  bool enter(const Select* node, RV& rv);
  void exit(const Select* node, RV& rv);
  */

  // traversal cases to do nothing
  bool enter(const TypeDecl* ast, RV& rv) { return false; }
  void exit(const TypeDecl* ast, RV& rv) { }
  bool enter(const Use* ast, RV& rv) { return false; }
  void exit(const Use* ast, RV& rv) { }
  bool enter(const Import* ast, RV& rv) { return false; }
  void exit(const Import* ast, RV& rv) { }
  bool enter(const Require* ast, RV& rv) { return false; }
  void exit(const Require* ast, RV& rv) { }
  bool enter(const ExternBlock* ast, RV& rv) { return false; }
  void exit(const ExternBlock* ast, RV& rv) { }
  bool enter(const Implements* ast, RV& rv) { return false; }
  void exit(const Implements* ast, RV& rv) { }
  bool enter(const VisibilityClause* ast, RV& rv) { return false; }
  void exit(const VisibilityClause* ast, RV& rv) { }
  bool enter(const As* ast, RV& rv) { return false; }
  void exit(const As* ast, RV& rv) { }
  bool enter(const Attribute* ast, RV& rv) { return false; }
  void exit(const Attribute* ast, RV& rv) { }
  bool enter(const AttributeGroup* ast, RV& rv) { return false; }
  void exit(const AttributeGroup* ast, RV& rv) { }
  bool enter(const ErroneousExpression* ast, RV& rv) { return false; }
  void exit(const ErroneousExpression* ast, RV& rv) { }
  bool enter(const EmptyStmt* ast, RV& rv) { return false; }
  void exit(const EmptyStmt* ast, RV& rv) { }

  // traversal cases to do something
  bool enter(const Module* node, RV& rv);
  void exit(const Module* node, RV& rv);

  bool enter(const Function* ast, RV& rv);
  void exit(const Function* ast, RV& rv);

  bool enter(const AstNode* node, RV& rv);
  void exit(const AstNode* node, RV& rv);
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

  if (!haveConvertedFunctions) {
    // convert the functions!
    convertFunctionsToConvert();
    haveConvertedFunctions = true;
  }

  astlocMarker markAstLoc(mod->id());

  ModuleSymbol* modSym = findOrSetupModule(mod->id());

  // TODO: remove this block once the resolver is fully operational
  if (modSym->modTag != MOD_USER) {
    return untypedConverter->convertToplevelModule(mod, modTag);
  }

  topLevelModTag = modTag;
  convertModuleInit(mod, modSym);

  return modSym;
}

void TConverter::postConvertApplyFixups() {
  untypedConverter->postConvertApplyFixups();

  // TODO: apply fixups from this converter
}

void TConverter::setupModulesToConvert() {
  // create each module now since we might add a function to it
  // the order in which this is done is not particularly
  // important since they will be filled-in in module init order
  // via calls to convertToplevelModule.
  for (const ID& modId : modulesToConvertVec) {
    findOrSetupModule(modId);
  }
}

struct SortCalledFnsDeeperFirst {
  bool operator()(const std::pair<const ResolvedFunction*, int>& a,
                  const std::pair<const ResolvedFunction*, int>& b) {
    // reverse sort by depth
    if (a.second > b.second) return true;
    if (a.second < b.second) return false;

    // regular sort by ResolvedFunction
    return *a.first < *b.first;
  }
};

void TConverter::convertFunctionsToConvert() {
  // Sort the called functions
  // 1. in descending order by depth (higher depths first)
  // 2. by ID

  auto v = std::vector<std::pair<const ResolvedFunction*, int>>(
                                       functionsToConvertWithTypes.begin(),
                                       functionsToConvertWithTypes.end());

  std::sort(v.begin(), v.end(), SortCalledFnsDeeperFirst());


  printf("Will convert functions in this order:\n");
  for (auto pair : v) {
    printf("  %s depth=%i\n", pair.first->id().str().c_str(), pair.second);
  }

  for (auto pair : v) {
    convertFunction(pair.first);
  }
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

  bool priv = (mod->visibility() == Decl::PRIVATE);
  bool prototype = (mod->kind() == Module::PROTOTYPE ||
                    mod->kind() == Module::IMPLICIT);

  ModuleSymbol* modSym = buildModule(name,
                                     modTag,
                                     new BlockStmt(),
                                     astr(path),
                                     priv,
                                     prototype);

  if (mod->kind() == Module::IMPLICIT) {
    modSym->addFlag(FLAG_IMPLICIT_MODULE);
  }

  attachSymbolAttributes(context, mod, modSym, isFromLibraryFile);

  // Save the empty module for later filling-in
  modSyms[modId] = modSym;

  // also tell the untyped converter about it
  untypedConverter->useModuleWhenConverting(modId, modSym);

  // This code does not add a DefExpr for the new module
  // * for toplevel modules, that happens in the code calling
  //   convertToplevelModule
  // * for submodules, that happens in the visit function for a Module.
  //

  return modSym;
}

ModuleSymbol* TConverter::findOrSetupModule(ID modId) {
  ModuleSymbol* modSym = modSyms[modId];
  if (modSym == nullptr) {
    // It can happen that it wasn't in modulesToConvert but we need to
    // convert it anyway. E.g. with PrintModuleInitOrder.
    modSym = setupModule(modId);
  }

  return modSym;
}

void TConverter::convertModuleInit(const Module* mod, ModuleSymbol* modSym) {
  printf("Converting module init for %s[%i]\n",
         mod->id().str().c_str(), modSym->id);

  // create the module init function
  modSym->initFn = new FnSymbol(astr("chpl__init_", modSym->name));
  modSym->initFn->retType = dtVoid;

  modSym->initFn->addFlag(FLAG_MODULE_INIT);
  modSym->initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);

  // add the init function to the module
  modSym->block->insertAtHead(new DefExpr(modSym->initFn));

  // TODO: handle module deinit functions

  // convert module-level statements into module's init function

  // prepare to traverse
  ID id = mod->id();
  const ResolutionResultByPostorderID& resolved = resolveModule(context, id);
  pushBlock(modSym->initFn->body);
  const AstNode* oldSymbol = symbol;
  symbol = mod;

  // traverse
  ResolvedVisitor<TConverter> rv(context, symbol, *this, resolved);
  symbol->traverse(rv);

  // tidy up after traversal
  symbol = oldSymbol;
  popBlock();
}

void TConverter::convertFunction(const ResolvedFunction* r) {
  printf("Converting function %s\n", r->id().str().c_str());
  // figure out, in which block should we put the DefExpr for the new FnSymbol?

  // here we flatten all functions, so we just need to find the appropriate
  // module.
  ID parentModule = parsing::idToParentModule(context, r->id());
  ModuleSymbol* modSym = findOrSetupModule(parentModule);

  // prepare to visit
  const ResolutionResultByPostorderID& resolved = r->resolutionById();
  const AstNode* ast = parsing::idToAst(context, r->id());
  if (!ast || !ast->isFunction()) {
    CHPL_ASSERT(false && "expected Function");
    return;
  }
  const Function* fn = ast->toFunction();
  pushBlock(modSym->block);
  const AstNode* oldSymbol = symbol;
  symbol = fn;

  // traverse
  ResolvedVisitor<TConverter> rv(context, symbol, *this, resolved);
  symbol->traverse(rv);

  // tidy up after traversal
  symbol = oldSymbol;
  popBlock();
}

bool TConverter::enter(const Module* node, RV& rv) {
  printf("enter %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  if (modulesToConvert.count(node->id()) == 0) {
    // this module should not be converted.
    return false;
  }

  if (node == symbol) {
    // we are visiting it to convert the module initialization function
    // so proceed with the traversal
    return true;
  } else {
    // it is a submodule.
    // Set up to convert the submodule's module init function
    // with a separate call to traverse.
    ModuleSymbol* modSym = findOrSetupModule(node->id());
    convertModuleInit(node, modSym);

    // add the DefExpr now (to match the untyped Converter)
    ID parentModuleId = parsing::idToParentModule(context, node->id());
    ModuleSymbol* parentModule = findOrSetupModule(parentModuleId);
    parentModule->block->insertAtHead(new DefExpr(modSym));

    return false;
  }

  return true;
}
void TConverter::exit(const Module* node, RV& rv) {
  printf("exit %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Function* node, RV& rv) {
  printf("enter %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  if (node == symbol) {
    // we are visiting it to convert it, so proceed with the traversal
    return true;
  }

  return false;
}
void TConverter::exit(const Function* node, RV& rv) {
  printf("exit %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const AstNode* node, RV& rv) {
  printf("enter %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  return true;
}
void TConverter::exit(const AstNode* node, RV& rv) {
  printf("exit %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}
