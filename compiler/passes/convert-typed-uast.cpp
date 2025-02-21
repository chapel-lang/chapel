/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "view.h"

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/global-strings.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/util/assertions.h"
#include "chpl/util/string-escapes.h"

#include "convert-help.h"

#include "llvm/ADT/SmallPtrSet.h"

#include <iostream>

// If defined as 1, dump debug output from the converter to stdout.
#define TC_DEBUG_TRACE 0

// Wrapper around 'CHPL_UNIMPL' that also calls 'gdbShouldBreakHere()'.
#define TC_UNIMPL(msg__) do { \
  CHPL_UNIMPL(msg__); \
  gdbShouldBreakHere(); \
} while (0)


using namespace chpl;
using namespace resolution;
using namespace uast;


//
// The 'typed converter' uses the fully resolved uAST provided by the 'dyno'
// frontend in order to generate typed AST used by the rest of the compiler
// that is equivalent to the AST produced at the end of the 'callDestructors'
// pass (with a few deliberate exceptions). It uses recursive functions to
// perform the conversion.
//
// One of the goals of this pass is to (for the functions generated here)
// completely sidestep the use of the legacy normalizer and resolver.
// This means that when deciding to use a legacy function, extra care should
// be taken to make sure that the function does not normalize or resolve.
//
// Right now this pass operates on a function-by-function basis. The untyped
// converter will be invoked on functions in standard/internal modules
// that are not explicitly represented in the call graph.
//
struct TConverter final : UastConverter {

  /// ------------ ///
  /// Nested Types ///
  /// ------------ ///

  // The typed converter declares itself to be a 'resolved visitor'.
  using RV = ResolvedVisitor<TConverter>;

  // This struct is used to implement a debug printing macro below.
  struct DebugPrinter {
    const char* function_ = nullptr;
    int line_ = -1;

    DebugPrinter(const char* function, int line)
      : function_(function), line_(line) {
    }

    void operator()(TConverter* tc, const char* fmt, ...) {
      va_list vl;
      va_start(vl, fmt);
      vdebugf(tc, function_, line_, fmt, vl);
      va_end(vl);
    }

    static void vdebugf(TConverter* tc, const char* function, int line,
                        const char* fmt,
                        va_list vl) {
      std::printf("[%s:%d] ", function, line);
      std::vprintf(fmt, vl);
    }
  };

  // A "debug printf" macro that only prints if 'TC_DEBUG_TRACE' is non-zero.
  #if defined(TC_DEBUG_TRACE) && TC_DEBUG_TRACE != 0
    #define TC_DEBUGF DebugPrinter(__FUNCTION__, __LINE__)
  #else
    // Discard arguments if unused to prevent any 'unused variable' warnings.
    #define TC_DEBUGF(tc__, fmt__, ...) do { \
      if constexpr(false) std::ignore = std::make_tuple(__VA_ARGS__); \
    } while (0)
  #endif

  // State required to mimic the desugaring of multi-declarations into
  // regular ones.
  // TODO: this is a copy from the untyped converter and needs to be
  // modified when multi-decls are supported in the typed converter.
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

  class ActualConverter;

  // This state is used to drive conversion of a module or function.
  // It is swapped in or out of the converter as needed in order to
  // convert different functions.
  struct ConvertedSymbolState {

    // The uAST function or module that we are currently converting.
    const AstNode* symbol = nullptr;

    // When converting functions, this is the AST for the 'FnSymbol*'.
    FnSymbol* fnSymbol = nullptr;

    // The AST for the module containing the symbol we are converting.
    ModuleSymbol* moduleSymbol = nullptr;

    // In functions, this AST is for the return variable (if it exists).
    Symbol* retVar = nullptr;

    // In functions, this AST is for the epilogue jump label (if it exists).
    LabelSymbol* epilogueLabel = nullptr;

    // When converting a uAST function, this is the 'ResolvedFunction*'.
    const ResolvedFunction* resolvedFunction = nullptr;

    // This will only be set when converting a call that has default values.
    ActualConverter* actualConverter = nullptr;

    // Set to 'true' if this module was loaded from a library file.
    bool moduleFromLibraryFile = false;

    // A mapping from 'dyno' symbols to old AST symbols for local variables.
    // TODO: This won't work for variables declared within a 'param' loop!
    std::unordered_map<ID, Symbol*> localSyms;

    // The current insertion point for expressions.
    AList* AList = nullptr;

    // Used to determine if we are in a USER context or not.
    ModTag topLevelModTag = MOD_USER;
  };

  // When a formal has a default-argument value that is used, this type
  // will help to construct the value and pass it in at the callsite.
  class ActualConverter {
   public:
    // The first component is the converted expression. The second is
    // the actual temp, created on demand if needed while converting
    // default arguments.
    //
    // If the first slot is 'nullptr' then no actual was created for it,
    // which is possible if the actual was elided (e.g., the formal is
    // param or a type).
    //
    // If the second slot is 'nullptr' then no actual temp was needed.
    using ActualInfo = std::tuple<Expr*, Symbol*>;

   private:
    // The typed converter.
    TConverter* tc_ = nullptr;

    // Does this call require defaults at all?
    bool callRequiresDefaults_ = false;

    // The state of the called function. This is swapped on and off
    // the converter as needed in order to interleave conversion of
    // default argument values with conversion of call actuals.
    ConvertedSymbolState calledFnState_;

    // The signature of the called function.
    const TypedFnSignature* tfs_ = nullptr;

    // State for actuals. Not all slots may be used.
    std::vector<ActualInfo> actualState_;

    // The uAST representing the call's location.
    const AstNode* astForCall_ = nullptr;

    // Collected actual uASTs. Do not have to be children of 'astForCall'.
    const std::vector<const AstNode*> actualAsts_;

    // The map driving conversion of the call actuals.
    const FormalActualMap& fam_;

    // The resolved visitor for the symbol containing the call.
    RV& rv_;

    // Mapping from a formal's ID to the 'FormalActual' slot.
    std::unordered_map<ID, const FormalActual&> formalIdToFormalActual_;

    // Used to insert actual temps if needed.
    AList* tempInsertionPoint_ = nullptr;

    // Extra setup done only if there are default arguments.
    void prepareFormalConversionState();

    // Convert a single actual considering 'fa'.
    void convertActual(const FormalActual& fa);

   public:
    ActualConverter(TConverter* tc,
                    const AstNode* astForCall,
                    const std::vector<const AstNode*>& actualAsts,
                    const resolution::TypedFnSignature* tfs,
                    const FormalActualMap& fam,
                    RV& rv);

    // Insert converted actuals into the given 'CallExpr'.
    void convertAndInsertActuals(CallExpr* call);

    // When converting a default argument, this is called to replace a
    // reference to a formal with a reference to an actual if needed.
    Symbol* interceptFormalUse(const ID& id);
  };

  /// ------ ///
  /// Fields ///
  /// ------ ///

  #if CHPL_TYPED_CONVERTER_DEBUG_TRACE != 0
    static constexpr bool trace = true;
  #else
    static constexpr bool trace = false;
  #endif

  // The 'dyno' compiler context.
  Context* context = nullptr;

  // State for the current function or module being converted.
  ConvertedSymbolState cur;

  bool haveSetupModules = false;
  bool haveConvertedFunctions = false;

  // the untyped converter (temporarily, this is used for non-user code)
  owned<UastConverter> untypedConverter;

  // which modules / submodules to convert
  std::vector<ID> modulesToConvertVec;
  std::unordered_set<ID> modulesToConvert;

  // which functions to convert with types
  CalledFnsSet functionsToConvertWithTypes;

  // the main module
  ID mainModuleId;

  // keeps track of which block/formals list/actuals list we are currently
  // in the process of creating
  //
  // the Expr* pointer is used to set the parentExpr for anything added
  // to the AList.
  std::vector<std::pair<AList*, Expr*>> aListStack;

  // this block is where we store any result created at the top-level
  BlockStmt* scratchSpaceBlock = nullptr;

  // TODO: Can we just use 'scratchSpaceBlock' for this?
  AList* globalInsertionPoint = nullptr;

  //
  // to keep track of symbols that have been converted & fixups needed
  //

  // Stores functions, modules, module-scope vars, and does not need to
  // be cleaned up when 'CurrentSymbolState' is changed.
  std::unordered_map<ID, Symbol*> globalSyms;

  // this one handles functions in a way that allows for instantiation
  std::unordered_map<const ResolvedFunction*, FnSymbol*> fns;
  std::unordered_map<const ResolvedFunction*, FnSymbol*> newWrappers;
  std::unordered_map<const types::Type*, FnSymbol*> chplTupleInit;

  // stores a mapping from chpl::Type* to Type*
  std::unordered_map<const types::Type*, Type*> convertedTypes;

  // so that TConverter can process one module (or function) at a time,
  // encountering a submodule, if it is to be converted, add it to this
  // list for processing just after the current module.
  std::vector<const uast::Module*> submodulesEncountered;

  // Handles for standard/internal modules (these are not global
  // variables in case we'd like to remove those somewhow later).
  ModuleSymbol* modChapelBase = nullptr;
  ModuleSymbol* modChapelTuple = nullptr;
  ModuleSymbol* modIO = nullptr;

  /// ------- ///
  /// Methods ///
  /// ------- ///

  TConverter(Context* context) : context(context) {
    SET_LINENO(rootModule);
    scratchSpaceBlock = new BlockStmt();
    globalInsertionPoint = &theProgram->block->body;
    cur.AList = &scratchSpaceBlock->body;
    untypedConverter = createUntypedConverter(context);

    setupEssentialModuleGlobalVars();
  }

  ~TConverter();

  void setupEssentialModuleGlobalVars() {
    std::vector<std::tuple<ModuleSymbol**, ModuleSymbol**, const char*>> v = {
      std::tuple { &modChapelBase,  &baseModule,    "ChapelBase"  },
      std::tuple { &modChapelTuple, nullptr,        "ChapelTuple" },
      std::tuple { &modIO,          &ioModule,      "IO"          },
    };

    for (auto [ptr1, ptr2, str] : v) {
      INT_ASSERT(ptr1);
      if (*ptr1) continue;

      auto name = ustr(str);

      // Since this occurs before the call graph is built, make sure the
      // file path for the module has been mapped by parsing it.
      auto uastMod = parsing::getToplevelModule(context, name);

      // The module should exist and be top-level. TODO: Minimal modules?
      INT_ASSERT(uastMod);

      auto mod = findOrSetupModule(ID(name));
      INT_ASSERT(mod);

      // TODO: Handle collisions if necessary...
      INT_ASSERT(mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD);

      // Set the 'TConverter' field pointer.
      *ptr1 = mod;

      // Set the global variable pointer if it exists.
      if (ptr2) *ptr2 = mod;
    }
  }

  // supporting UastConverter methods
  void setModulesToConvert(const std::vector<ID>& vec) override {
    TC_DEBUGF(this, "have %i modules\n", (int)vec.size());

    modulesToConvert.clear();
    // save the vector
    modulesToConvertVec = vec;
    // add the modules to the set
    for (const ID& id : modulesToConvertVec) {
      TC_DEBUGF(this, "Will convert module %s\n", id.str().c_str());
      modulesToConvert.insert(id);
    }

    // also tell the untyped converter about it
    untypedConverter->setModulesToConvert(vec);
  }

  void setFunctionsToConvertWithTypes(const CalledFnsSet& calledFns) override {
    TC_DEBUGF(this, "have %i functions\n", (int)calledFns.size());
    functionsToConvertWithTypes = calledFns;
  }

  void setSymbolsToIgnore(std::unordered_set<ID> ignore) override {
    INT_FATAL("TConverter::setSymbolsToIgnore not implemented");
  }

  void setMainModule(ID mainModule) override {
    TC_DEBUGF(this, "have %s\n", mainModule.str().c_str());
    mainModuleId = mainModule;
  }

  void useModuleWhenConverting(const chpl::ID& modId,
                               ModuleSymbol* modSym) override {
    INT_FATAL("useModuleWhenConverting not expected for TConverter");
    globalSyms[modId] = modSym;
  }

  // This should not be called. To use untyped conversion, call below.
  Expr* convertAST(const AstNode* node) override {
    INT_ASSERT(false && "Should not be called here!");
    return nullptr;
  }

  Expr* convertAstUntyped(const AstNode* node) {
    return untypedConverter->convertAST(node);
  }

  Expr* convertExpr(const AstNode* node, RV& rv,
                    types::QualifiedType* outQt=nullptr);

  Expr* convertExprOrNull(const AstNode* node, RV& rv,
                          types::QualifiedType* outQt=nullptr) {
    if (node == nullptr) return nullptr;
    return convertExpr(node, rv, outQt);
  }

  UniqueString ustr(const char* str) {
    return UniqueString::get(context, str);
  }

  UniqueString ustr(const std::string& str) {
    return UniqueString::get(context, str);
  }

  /// -------------------------- ///
  /// Symbol and Type Conversion ///
  /// -------------------------- ///

  ModuleSymbol* convertModule(const Module* mod);
  ModuleSymbol* convertToplevelModule(const Module* mod,
                                      ModTag modTag) override;

  void postConvertApplyFixups() override;

  void setupModulesToConvert();
  void convertFunctionsToConvert();

  ModuleSymbol* setupModule(ID modId);
  ModuleSymbol* findOrSetupModule(ID modId);

  void convertModuleInit(const Module* mod, ModuleSymbol* modSym);
  void convertFunction(const ResolvedFunction* r);
  FnSymbol* convertNewWrapper(const ResolvedFunction* initFn);
  ArgSymbol* convertFormal(const Formal* node, RV& rv);
  void convertAndInsertVarArgs(const VarArgFormal* node, RV& rv);

  FnSymbol* findOrConvertFunction(const ResolvedFunction* r);
  FnSymbol* findOrConvertNewWrapper(const ResolvedFunction* initFn);
  FnSymbol* findOrConvertTupleInit(const types::TupleType* tt);

  // This gadget will resolve a _frontend-visible_ function using 'ci'
  // and produce the lowered 'FnSymbol*' to use in code generation.
  // If the function does not exist in the original source code or the
  // frontend is not otherwise aware of it, then it will not be found!
  //
  // If 'outRf' is a non-null pointer then it will be set with info
  // about the function that was used.
  FnSymbol*
  convertFunctionForGeneratedCall(const resolution::CallInfo& ci,
                                  const uast::AstNode* inAst,
                                  const ResolvedFunction** outRf=nullptr);

  void createMainFunctions() override;

  ArgSymbol* findOrCreateFormal(const Formal* node);
  VarSymbol* findOrCreateVar(const Variable* node);
  VarSymbol* findOrCreateHiddenVarArg(const VarArgFormal* node, RV& rv);
  Symbol* findConvertedSym(const ID& id);
  Symbol* findOrCreateSym(const ID& id, RV& rv);

  /* Look up a type in the converted types map. Does not convert. */
  Type* findConvertedType(const types::Type* t);

  // Converts a type. If it's an AggregateType, it will also fill in the
  // fields, but only after creating a dummy AggregateType for the map
  // that can be used in the case of recursive data structures.
  Type* convertType(const types::Type* t, bool convertRefType=false);

  void noteConvertedSym(const uast::AstNode* ast, Symbol* sym);

  // a helper to convert the fields contained in composite types
  void helpConvertFields(const types::CompositeType* ct,
                         const ResolvedFields& rf,
                         AggregateType* at);

  Symbol* convertParam(const types::QualifiedType& qt);

  // note: the relevant calls and DefExpr are added to cur.AList;
  // this function returns the VarSymbol*
  VarSymbol* convertVariable(const uast::Variable* node,
                             RV& rv,
                             bool useLinkageName,
                             MultiDeclState* multiState = nullptr);

  /// ------------------ ///
  /// aListStack helpers ///
  /// ------------------ ///

  void pushAList(AList* lst, Expr* expr) {
    aListStack.push_back({lst, expr});
    cur.AList = lst;
  }

  Expr* popAList() {
    CHPL_ASSERT(aListStack.size() > 0);

    Expr* ret = nullptr;
    ret = aListStack.back().second;

    aListStack.pop_back();
    cur.AList = aListStack.size() ? aListStack.back().first
                                  : &scratchSpaceBlock->body;
    return ret;
  }

  BlockStmt* pushNewBlock() {
    auto newBlockStmt = new BlockStmt();
    pushAList(&newBlockStmt->body, newBlockStmt);
    return newBlockStmt;
  }

  BlockStmt* pushBlock(BlockStmt* block) {
    pushAList(&block->body, block);
    return block;
  }

  BlockStmt* popBlock() {
    return toBlockStmt(popAList());
  }

  void enterCallActuals(CallExpr* call) {
    pushAList(&call->argList, call);
  }

  CallExpr* exitCallActuals() {
    return toCallExpr(popAList());
  }

  void enterFormals(FnSymbol* fn) {
    pushAList(&fn->formals, nullptr);
  }

  void exitFormals(FnSymbol* fn) {
    popAList();
  }

  /// ---------- ///
  /// Predicates ///
  /// ---------- ///

  bool shouldElideFormal(const TypedFnSignature* tfs, int formalIdx);
  bool typeExistsAtRuntime(const types::Type* t);
  bool functionExistsAtRuntime(const ResolvedFunction* r);

  /// --------------------- ///
  /// Expression Conversion ///
  /// --------------------- ///

  // Use this any time you are generating AST and reach an error case
  // _instead_ of doing something like 'return nullptr'. All paths
  // through conversion should return _some_ sort of AST in order to
  // maintain the correct shape (to keep the old compiler happy).
  //
  // Do not worry about handling error cases when converting. We can
  // rely on the frontend to have emitted errors for a poorly formed
  // program.
  Expr* placeholder() const {
    auto var = new_IntSymbol(0, INT_SIZE_64);
    return new SymExpr(var);
  }

  // Helper that constructs AST to produce the default value for a type.
  // NOTE: If the type cannot construct a default value then this helper
  //       will produce a potentially bogus expression.
  Expr* defaultValueForType(const types::Type* t);

  // Create a new temporary. The type used for it must be supplied.
  Symbol* makeNewTemp(const types::QualifiedType& qt);

  // Store 'e' in a temporary if it does not already refer to one.
  // The type for the temporary must be provided and cannot easily
  // be retrieved from the converted expression.
  SymExpr* storeInTempIfNeeded(Expr* e, const types::QualifiedType& qt);

  Expr* convertLifetimeClause(const AstNode* node, RV& rv);

  CallExpr* convertLifetimeIdent(const Identifier* node, RV& rv);

  // Only type expressions that are runtime types need to be converted
  // as code; otherwise we can forget the type expression and just use the type.
  // Accordingly, this returns 'nullptr' if the type expression isn't
  // a type with a runtime component.
  Expr* convertRuntimeTypeExpression(const AstNode* node, RV& rv);

  void simplifyEpilogue(FnSymbol* fn);

  ::Qualifier convertQualifier(types::QualifiedType::Kind kind);

  IntentTag convertFormalIntentQt(types::QualifiedType::Kind kind);

  void setVariableType(const uast::VarLikeDecl* v, Symbol* sym, RV& rv);

  // Try to convert a primitive call.
  Expr* convertPrimCallOrNull(const Call* node, RV& rv);

  // Try to convert a 'new' expression (which is considered a special
  // form of call) into a call to a middle-end only '_new' wrapper.
  Expr* convertNewCallOrNull(const Call* node, RV& rv);

  // Try to convert a regular named call that maps to a 'TypedFnSignature'.
  // This branch is taken if the 'ResolvedExpression' for 'node' maps
  // cleanly to a single 'TypedFnSignature'.
  Expr* convertRegularNamedCallOrNull(const Call* node, RV& rv);

  // Try to convert a parenless call.
  Expr* convertParenlessCallOrNull(const Identifier* node, RV& rv);

  // Convert a "special" call that the compiler has handled directly
  // without mapping to a 'TypedFnSignature' in an obvious way. This
  // branch will also handle patterns which do not necessarily map to
  // calls in the lowered AST (e.g., field accesses).
  Expr* convertSpecialNamedCallOrNull(const Call* node, RV& rv);

  // Try to convert the pattern 'tuple[0]'.
  Expr* convertSpecialTupleParamIndexingOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // Try to convert field init points e.g., 'this.x = x' in an initializer.
  Expr* convertSpecialFieldInitOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // The frontend may decide to handle several casts directly as 'builtin',
  // such as casts for primitive types or strings. If the cast expression
  // produces a 'QualifiedType' but does not have a 'TypedFnSignature' set,
  // then it was probably handled directly by the compiler.
  Expr* convertSpecialIntrinsicCastOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv);

  // Try to elide a call to a specific signature at compile-time. If so,
  // generate and return a NOOP. Sets 'outRf' to the results of the
  // lookup process for 'sig' if 'outRf' is a non-null pointer.
  Expr* paramElideCallOrNull(const TypedFnSignature* sig,
                             const PoiScope* poiScope,
                             const ResolvedFunction** outRf=nullptr);

  // Convert using a simple pattern that does not handle pass-by-name.
  template <typename Iter>
  void convertAndInsertPrimCallActuals(CallExpr* call,
                                       const Iter& actuals,
                                       RV& rv);

  // For a given AST, convert and insert actuals into the lowered call 'c'.
  //
  // This process is driven by the 'FormalActualMap' 'fam', so 'fam' must
  // have the correct shape to ensure proper conversion (it is responsible
  // for handling pass-by-name).
  //
  // The vector 'actualAsts' should contain the ASTs that are apparent in
  // the call. Implicit receivers are handled by considering the signature
  // and may rely on the currently converted symbol (e.g., `cur.fnSymbol`).
  void convertAndInsertActuals(CallExpr* c,
                               const AstNode* node,
                               const std::vector<const AstNode*>& actualAsts,
                               const resolution::TypedFnSignature* tfs,
                               const FormalActualMap& fam,
                               RV& rv);

  // This handles tasks such as e.g., inserting 'deinit' calls at the
  // end of the current statement. TODO: Need to wire this up to occur
  // at end of statement and block.
  void handlePostCallActions(CallExpr* c,
                             const Call* node,
                             const resolution::ResolvedExpression* re,
                             RV& rv);

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
  //FnSymbol* convertFunctionSignature(const FunctionSignature* node);
  //CallExpr* convertArrayType(const BracketLoop* node);
  //DefExpr* convertEnumElement(const EnumElement* node);
  //template <typename Iterable>
  //void convertInheritsExprs(const Iterable& iterable,
  //                          std::vector<Expr*>& inherits,
  //                          bool& inheritMarkedGeneric);
  //Expr* convertAggregateDecl(const AggregateDecl* node);
  //Expr* convertTypeExpression(const AstNode* node);
  //Expr* convertTypeExpressionOrNull(const AstNode* node);
  //ShadowVarSymbol* convertTaskVar(const TaskVar* node);

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
  bool enter(const Formal* ast, RV& rv) { return false; }
  void exit(const Formal* ast, RV& rv) { }
  bool enter(const New* ast, RV& rv) { return false; }
  void exit(const New* ast, RV& rv) { }

  // traversal cases to do something
  bool enter(const Module* node, RV& rv);
  void exit(const Module* node, RV& rv);

  bool enter(const Function* node, RV& rv);
  void exit(const Function* node, RV& rv);

  bool enter(const Variable* node, RV& rv);
  void exit(const Variable* node, RV& rv);

  bool enter(const Literal* node, RV& rv);
  void exit(const Literal* node, RV& rv);

  bool enter(const Identifier* node, RV& rv);
  void exit(const Identifier* node, RV& rv);

  bool enter(const Return* node, RV& rv);
  void exit(const Return* node, RV& rv);

  bool enter(const Call* node, RV& rv);
  void exit(const Call* node, RV& rv);

  bool enter(const Conditional* node, RV& rv);
  void exit(const Conditional* node, RV& rv);

  bool enter(const AstNode* node, RV& rv);
  void exit(const AstNode* node, RV& rv);
};

TConverter::~TConverter() { }

Expr* TConverter::convertExpr(const AstNode* node, RV& rv,
                              types::QualifiedType* outQt) {
  astlocMarker markAstLoc(node->id());

  // traverse to add it to cur.AList
  node->traverse(rv);

  CHPL_ASSERT(!cur.AList->empty());

  // Set the expression's type if it was requested.
  if (outQt) {
    if (auto re = rv.byAstOrNull(node)) *outQt = re->type();
  }

  // remove the last thing from the current AList and return that
  auto ret = cur.AList->last()->remove();
  return ret;
}

ModuleSymbol* TConverter::convertModule(const Module* mod) {
  astlocMarker markAstLoc(mod->id());

  ModuleSymbol* modSym = findOrSetupModule(mod->id());

  // TODO: remove this block once the resolver is fully operational
  if (modSym->modTag != MOD_USER) {
    return untypedConverter->convertToplevelModule(mod, modSym->modTag);
  }

  // convert the module init while gathering submodules
  submodulesEncountered.clear();
  convertModuleInit(mod, modSym);

  // Additionally, convert any submodules encountered.
  // This is handled as a follow-on step in order to avoid storing
  // various pieces of TConverter state in a stack.
  for (const uast::Module* submod : submodulesEncountered) {
    ModuleSymbol* subSym = convertModule(submod);
    // add the DefExpr now (to match the untyped Converter)
    modSym->block->insertAtHead(new DefExpr(subSym));
  }
  submodulesEncountered.clear();

  return modSym;
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

  cur.topLevelModTag = modTag;
  return convertModule(mod);
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
  bool operator()(const std::pair<const ResolvedFunction*, CalledFnOrder>& a,
                  const std::pair<const ResolvedFunction*, CalledFnOrder>& b) {
    // reverse sort by depth
    if (a.second.depth > b.second.depth) return true;
    if (a.second.depth < b.second.depth) return false;

    // regular sort by index encountered in call graph
    return a.second.index < b.second.index;
  }
};

void TConverter::convertFunctionsToConvert() {
  // Sort the called functions
  // 1. in descending order by depth (higher depths first)
  // 2. by ID

  auto v = std::vector<std::pair<const ResolvedFunction*, CalledFnOrder>>(
                                       functionsToConvertWithTypes.begin(),
                                       functionsToConvertWithTypes.end());

  std::sort(v.begin(), v.end(), SortCalledFnsDeeperFirst());

  if (trace) {
    TC_DEBUGF(this, "Will convert functions in this order:\n");
    for (auto pair : v) {
      TC_DEBUGF(this, "  %s depth=%i index=%i\n",
                pair.first->id().str().c_str(),
                pair.second.depth,
                pair.second.index);
    }
  }

  for (auto pair : v) {
    convertFunction(pair.first);
  }

  // convert also main and create chpl_gen_main
  createMainFunctions();

  // tell the untyped converter about which functions to ignore
  // this is whatever functions we have converted already here
  std::unordered_set<ID> ignore;
  for (auto pair: fns) {
    const ResolvedFunction* fn = pair.first;
    ignore.insert(fn->id());
  }
  for (auto pair: globalSyms) {
    ignore.insert(pair.first);
  }
  untypedConverter->setSymbolsToIgnore(ignore);
}

ModuleSymbol* TConverter::setupModule(ID modId) {
  astlocMarker markAstLoc(modId);

  UniqueString path;
  bool found = context->filePathForId(modId, path);
  INT_ASSERT(found);

  // compute the modTag
  ModTag modTag = getModuleTag(context, path);

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

  if (parsing::idIsToplevelModule(context, modId)) {
    // Make sure that the module is inserted into 'theProgram'. This
    // call does nothing if the module is already inserted.
    ModuleSymbol::addTopLevelModule(modSym);
  }

  if (mod->kind() == Module::IMPLICIT) {
    modSym->addFlag(FLAG_IMPLICIT_MODULE);
  }

  attachSymbolAttributes(context, mod, modSym, isFromLibraryFile);

  // Save the empty module for later filling-in
  globalSyms[modId] = modSym;

  // also tell the untyped converter about it
  untypedConverter->useModuleWhenConverting(modId, modSym);

  // This code does not add a DefExpr for the new module
  // * for toplevel modules, that happens in the code calling
  //   convertToplevelModule
  // * for submodules, that happens in convertModule for
  //   the containing module.

  return modSym;
}

ModuleSymbol* TConverter::findOrSetupModule(ID modId) {
  Symbol* sym = globalSyms[modId];
  if (sym == nullptr) {
    // It can happen that it wasn't in modulesToConvert but we need to
    // convert it anyway. E.g. with PrintModuleInitOrder.
    return setupModule(modId);
  }

  return toModuleSymbol(sym);
}

void TConverter::convertModuleInit(const Module* mod, ModuleSymbol* modSym) {
  TC_DEBUGF(this, "Converting module init for %s[%i]\n",
            mod->id().str().c_str(), modSym->id);

  // create the module init function
  modSym->initFn = new FnSymbol(astr("chpl__init_", modSym->name));
  modSym->initFn->retType = dtVoid;

  modSym->initFn->addFlag(FLAG_MODULE_INIT);
  modSym->initFn->addFlag(FLAG_RESOLVED);
  modSym->initFn->addFlag(FLAG_RESOLVED_EARLY);
  modSym->initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  modSym->initFn->setNormalized(true);

  // add the init function to the module
  modSym->block->insertAtHead(new DefExpr(modSym->initFn));

  // If the module has the EXPORT_INIT flag then
  // propagate it to the module's init function
  if (modSym->hasFlag(FLAG_EXPORT_INIT) ||
      (fLibraryCompile && modSym->modTag == MOD_USER)) {
    modSym->initFn->addFlag(FLAG_EXPORT);
    modSym->initFn->addFlag(FLAG_LOCAL_ARGS);
  }

  // TODO: handle module deinit functions
  // TODO: call module init functions for other modules


  // convert module-level statements into module's init function

  // prepare to traverse
  ID id = mod->id();
  const ResolutionResultByPostorderID& rr = resolveModule(context, id);

  // don't actually convert for --dyno-resolve-only
  if (fDynoResolveOnly) return;

  ConvertedSymbolState saved = cur;
  cur = {};

  pushBlock(modSym->initFn->body);
  cur.symbol = mod;
  cur.fnSymbol = modSym->initFn;
  cur.moduleSymbol = modSym;
  cur.moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);
  cur.resolvedFunction = nullptr;

  // traverse
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, cur.symbol, *this, rr);
  cur.symbol->traverse(rv);

  // add the 'return _void' at the end of the module init function
  modSym->initFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  // Swap off after traversal.
  std::swap(saved, cur);
}

void TConverter::convertFunction(const ResolvedFunction* rf) {
  // don't bother with functions that don't need to exist at runtime
  if (!functionExistsAtRuntime(rf)) return;

  // don't actually convert for --dyno-resolve-only
  if (fDynoResolveOnly) return;

  auto& id = rf->id();
  TC_DEBUGF(this, "Converting function %s\n", id.str().c_str());

  // figure out, in which block should we put the DefExpr for the new FnSymbol?

  // here we flatten all functions, so we just need to find the appropriate
  // module.
  ID parentModule = parsing::idToParentModule(context, id);
  ModuleSymbol* modSym = findOrSetupModule(parentModule);

  // prepare to visit
  const ResolutionResultByPostorderID& rr = rf->resolutionById();
  const AstNode* ast = parsing::idToAst(context, id);
  if (!ast || !ast->isFunction()) {
    CHPL_ASSERT(false && "expected Function");
    return;
  }

  const Function* fn = ast->toFunction();

  // Clear the current symbol markers (caller should have saved them).
  cur = {};
  cur.symbol = fn;
  cur.resolvedFunction = rf;
  cur.moduleFromLibraryFile = modSym->hasFlag(FLAG_PRECOMPILED);

  pushBlock(modSym->block);

  // Traversing the symbol will use the typed converter as a visitor.
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rv(&rcval, cur.symbol, *this, rr);
  cur.symbol->traverse(rv);

  popBlock();
}

FnSymbol* TConverter::findOrConvertFunction(const ResolvedFunction* rf) {
  // if it was already converted, return that
  auto it = fns.find(rf);
  if (it != fns.end()) {
    return it->second;
  }

  // otherwise, convert the function

  // save state
  // this needs to happen when looking for a function called by
  // some code that wasn't necessarily planned for in the call graph
  // e.g. _cond_test
  ConvertedSymbolState saved = cur;

  // convert the function and return the result
  convertFunction(rf);
  FnSymbol* ret = fns[rf];
  INT_ASSERT(ret);

  // restore state
  cur = saved;

  return ret;
}

FnSymbol* TConverter::convertNewWrapper(const ResolvedFunction* rInitFn) {
  // don't bother with functions that don't need to exist at runtime
  if (!functionExistsAtRuntime(rInitFn)) return nullptr;

  TC_DEBUGF(this, "Converting _new wrapper for %s\n", rInitFn->id().str().c_str());

  // compute the containing Module and ModuleSymbol
  const Module* inModule = nullptr;
  ModuleSymbol* modSym = nullptr;
  {
    ID inModuleId = parsing::idToParentModule(context, rInitFn->id());
    if (const AstNode* inAst = parsing::idToAst(context, inModuleId)) {
      inModule = inAst->toModule();
    }
    INT_ASSERT(inModule);
    modSym = findOrSetupModule(inModuleId);
    INT_ASSERT(modSym);
  }

  // find or convert the init function
  FnSymbol* initFn = findOrConvertFunction(rInitFn);
  AggregateType* type = toAggregateType(initFn->_this->getValType());

  if (initFn->throwsError()) {
    // code below will need adjustment for this case
    TC_UNIMPL("_new that throws");
  }

  // build the new wrapper. this code is based upon buildNewWrapper.
  FnSymbol* fn = new FnSymbol(astrNew);

  BlockStmt* body = fn->body;
  VarSymbol* initTemp = newTemp("initTemp", type);
  CallExpr* innerInit = new CallExpr(initFn, gMethodToken, initTemp);
  VarSymbol* retVar = newTemp("ret", type);
  retVar->addFlag(FLAG_RVV);

  fn->setMethod(true);
  fn->addFlag(FLAG_NEW_WRAPPER);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);
  fn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  fn->addFlag(FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO);
  fn->addFlag(FLAG_LLVM_RETURN_NOALIAS);

  if (initFn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  }
  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_RESOLVED_EARLY);
  fn->setNormalized(true);

  // Add the _new function just after the relevant initFn
  initFn->defPoint->insertAfter(new DefExpr(fn));

  // set up the formals and init call based on the init function
  SymbolMap initToNewMap;
  for_formals(formal, initFn) {
    if (formal != initFn->_this && formal->type != dtMethodToken) {
      ArgSymbol* newArg = formal->copy();
      initToNewMap.put(formal, newArg);
      fn->insertFormalAtTail(newArg);
      innerInit->insertAtTail(new SymExpr(newArg));
    }
  }
  update_symbols(fn, &initToNewMap);

  body->insertAtTail(new DefExpr(initTemp));

  // compute the size and md
  VarSymbol* size = newTemp("size", dtInt[INT_SIZE_64]);
  body->insertAtTail(new DefExpr(size));
  CallExpr* sizeofCall = new CallExpr(PRIM_SIZEOF_BUNDLE, type->symbol);
  body->insertAtTail(new CallExpr(PRIM_MOVE, size, sizeofCall));
  VarSymbol* md = newMemDesc(type); // int(16)

  // allocate and cast
  auto sizeQt = types::QualifiedType(types::QualifiedType::PARAM,
                                     types::IntType::get(context, 64));
  auto mdQt = types::QualifiedType(types::QualifiedType::PARAM,
                                   types::IntType::get(context, 16));
  auto ci = resolution::CallInfo(ustr("chpl_here_alloc"),
                                 /* calledType */ types::QualifiedType(),
                                 /* isMethodCall */ false,
                                 /* hasQuestionArg */ false,
                                 /* isParenless */ false,
                                 {CallInfoActual(sizeQt),
                                  CallInfoActual(mdQt)});
  FnSymbol* allocateFn = convertFunctionForGeneratedCall(ci, inModule);

  VarSymbol* castTmp = newTemp("cast_tmp", dtCVoidPtr);
  body->insertAtTail(new DefExpr(castTmp));
  CallExpr* allocateCall = new CallExpr(allocateFn, size, md);
  body->insertAtTail(new CallExpr(PRIM_MOVE, castTmp, allocateCall));
  CallExpr* castCall = new CallExpr(PRIM_CAST, type->symbol, castTmp);
  body->insertAtTail(new CallExpr(PRIM_MOVE, initTemp, castCall));

  // run the initializer on the allocated class
  body->insertAtTail(innerInit);

  // return initTemp
  body->insertAtTail(new CallExpr(PRIM_MOVE, retVar, initTemp));
  body->insertAtTail(new CallExpr(PRIM_RETURN, retVar));

  return fn;
}

FnSymbol* TConverter::findOrConvertNewWrapper(const ResolvedFunction* rInitFn) {
  // if it was already converted, return that
  auto it = newWrappers.find(rInitFn);
  if (it != newWrappers.end()) {
    return it->second;
  }

  // otherwise, convert the function now
  FnSymbol* ret = convertNewWrapper(rInitFn);
  INT_ASSERT(ret);
  INT_ASSERT(newWrappers[rInitFn] == nullptr);
  newWrappers[rInitFn] = ret;

  return ret;
}

// TODO: Migrate tuples to just use 'init' instead.
FnSymbol* TConverter::findOrConvertTupleInit(const types::TupleType* tt) {
  auto it = chplTupleInit.find(tt);
  if (it != chplTupleInit.end()) return it->second;

  const bool convertRefType = true;
  Type* newType = convertType(tt, convertRefType);

  // Otherwise, construct it.
  FnSymbol* ret = new FnSymbol("chpl__tuple_init");

  auto _this = new ArgSymbol(INTENT_REF, "this", newType);
  _this->addFlag(FLAG_ARG_THIS);
  ret->insertFormalAtTail(_this);
  ret->_this = _this;

  // NOTE: The original code inserts this and adds a substitution, but
  // I don't think we need to bother with this (also, why does the
  // original 'chpl__tuple_init' code do this in the first place? Isn't
  // the substitution for the type sufficient enough?).
  /*
  Type* sizeType = dtInt[INT_SIZE_DEFAULT];
  ArgSymbol* sizeArg = new ArgSymbol(INTENT_IN, "size", sizeType);
  sizeArg->addFlag(FLAG_INSTANTIATED_PARAM);
  ret->insertFormalAtTail(sizeArg);
  */

  int size = tt->numElements();
  for (int i = 0; i < size; i++) {
    const char* name = astr("x", istr(i));
    auto qtElement = tt->elementType(i);
    auto eltType = convertType(qtElement.type());

    // Use the 'in' intent to ensure that we can safely move the formal.
    ArgSymbol* arg = new ArgSymbol(INTENT_IN, name, eltType);
    ret->insertFormalAtTail(arg);

    // Mark the arg as 'no auto destroy' since we will be moving it.
    arg->addFlag(FLAG_NO_AUTO_DESTROY);

    // Move the formal into the field.
    ret->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                   _this,
                                   new_CStringSymbol(name),
                                   new SymExpr(arg)));
  }

  ret->addFlag(FLAG_RESOLVED);
  ret->addFlag(FLAG_RESOLVED_EARLY);
  ret->addFlag(FLAG_ALLOW_REF);
  ret->addFlag(FLAG_COMPILER_GENERATED);
  ret->addFlag(FLAG_LAST_RESORT);
  ret->addFlag(FLAG_INLINE);
  ret->addFlag(FLAG_INVISIBLE_FN);
  ret->addFlag(FLAG_INIT_TUPLE);
  ret->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  ret->addFlag(FLAG_PARTIAL_TUPLE);
  ret->retTag = RET_VALUE;
  ret->retType = newType;

  ret->substitutions.copy(newType->substitutions);
  CallExpr* primReturn = new CallExpr(PRIM_RETURN, _this);
  ret->insertAtTail(primReturn);

  // TODO: Do we need to set this?
  BlockStmt* instantiationPoint = nullptr;
  ret->setInstantiationPoint(instantiationPoint);

  // Insert the definition into the tuple module.
  modChapelTuple->block->insertAtHead(new DefExpr(ret));

  // Emplace it and return it.
  auto& slot = chplTupleInit[tt];
  INT_ASSERT(slot == nullptr);
  slot = ret;

  return ret;
}

// This runs the resolver to figure out which function to call &
// converts that function.
// It does not convert the call itself.
// It works as though the generated call were in the module initializer.
FnSymbol*
TConverter::convertFunctionForGeneratedCall(const resolution::CallInfo& ci,
                                            const uast::AstNode* in,
                                            const ResolvedFunction** outRf) {
  auto scope = scopeForId(context, in->id());
  const PoiScope* poiScope = nullptr;
  auto scopeInfo = CallScopeInfo::forNormalCall(scope, poiScope);
  auto r = resolveGeneratedCall(context, in, ci, scopeInfo);
  const auto& candidate = r.mostSpecific().only();
  const TypedFnSignature* sig = candidate.fn();
  INT_ASSERT(sig);
  INT_ASSERT(sig->untyped()->idIsFunction());

  chpl::resolution::ResolutionContext rcval(context);
  const ResolvedFunction* resolvedFn = resolveFunction(&rcval, sig, poiScope);
  INT_ASSERT(resolvedFn);

  if (outRf) *outRf = resolvedFn;

  return findOrConvertFunction(resolvedFn);
}

void TConverter::createMainFunctions() {
  const AstNode* ast = parsing::idToAst(context, mainModuleId);
  INT_ASSERT(ast);
  const Module* mainMod = ast->toModule();
  INT_ASSERT(mainMod);
  ModuleSymbol* mainModule = findOrSetupModule(mainModuleId);
  INT_ASSERT(mainModule);
  SET_LINENO(mainModule);

  // if 'proc main' was not provided, generate an empty 'proc main'.
  //
  // (if there was a main function there, we would have already converted it,
  //  since it needed to be included in the call graph).
  ID mainFnId = parsing::findProcMainInModule(context, mainModuleId);

  FnSymbol* mainFn = nullptr;
  if (mainFnId.isEmpty()) {
    // there wasn't a 'proc main' so we need to generate one.
    mainFn = new FnSymbol("main");
    mainFn->addFlag(FLAG_RESOLVED);
    mainFn->addFlag(FLAG_RESOLVED_EARLY);
    mainFn->setNormalized(true);
    mainFn->retType = dtVoid;
    mainFn->body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    mainModule->block->insertAtTail(new DefExpr(mainFn));
  } else {
    // otherwise, we should have already converted 'proc main'
    // (since it was added to the call graph). Get the converted version.
    auto resolvedMain = resolveConcreteFunction(context, mainFnId);
    mainFn = toFnSymbol(fns[resolvedMain]);
    INT_ASSERT(mainFn);
  }

  ModuleSymbol::setMainModule(mainModule);
  chplUserMain = mainFn;

  // adjust cname for 'proc main'
  chplUserMain->cname = astr("chpl_user_main");
  if (fIdBasedMunging && !mainModule->astloc.id().isEmpty()) {
    const char* cname = astr(mainModule->astloc.id().symbolPath().c_str(),
                             ".main");
    chplUserMain->cname = cname;
  }

  // TODO: Let's write this in the module code!
  // generate chpl_gen_main
  // TODO: expand this after we have 'class C' and 'record R' converting
#if 0
  bool mainReturnsSomething = mainFn->retType != dtVoid;

  //
  // chpl_gen_main is the entry point for the compiler-generated code.
  // It invokes the user's code.
  //

  ArgSymbol* arg = new ArgSymbol(INTENT_CONST_REF, "_arg", dtMainArgument);

  chpl_gen_main          = new FnSymbol("chpl_gen_main");
  chpl_gen_main->retType = dtInt[INT_SIZE_64];
  chpl_gen_main->cname   = astr("chpl_gen_main");

  chpl_gen_main->insertFormalAtTail(arg);

  chpl_gen_main->addFlag(FLAG_EXPORT);  // chpl_gen_main is always exported.
  chpl_gen_main->addFlag(FLAG_LOCAL_ARGS);
  chpl_gen_main->addFlag(FLAG_COMPILER_GENERATED);
  chpl_gen_main->addFlag(FLAG_GEN_MAIN_FUNC);

  mainModule->block->insertAtTail(new DefExpr(chpl_gen_main));

  VarSymbol* main_ret = newTemp("ret", dtInt[INT_SIZE_64]);
  main_ret->addFlag(FLAG_RVV);
  VarSymbol* endCount = NULL;

  chpl_gen_main->insertAtTail(new DefExpr(main_ret));

  //
  // In --minimal-modules compilation mode, we won't have any
  // parallelism, so no need for end counts (or atomic/sync types to
  // support them).
  //
  if (fMinimalModules == false) {
    // figure out _endCountAlloc

    // TODO: add converter or QualifiedType methods to more
    // easily construct a QualifiedType for common values like param false.
    auto falseQt = types::QualifiedType::makeParamBool(context, false);
    auto ci = resolution::CallInfo(UniqueString::get(context, "_endCountAlloc"),
                                   /* calledType */ types::QualifiedType(),
                                   /* isMethodCall */ false,
                                   /* hasQuestionArg */ false,
                                   /* isParenless */ false,
                                   {CallInfoActual(falseQt)});
    FnSymbol* endCountAlloc = convertFunctionForGeneratedCall(ci, mainMod);

    endCount = newTemp("_endCount");

    chpl_gen_main->insertAtTail(new DefExpr(endCount));
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                             endCount,
                                             new CallExpr(endCountAlloc)));

    chpl_gen_main->insertAtTail(new CallExpr(PRIM_SET_DYNAMIC_END_COUNT, endCount));
  }

  chpl_gen_main->insertAtTail(new CallExpr("chpl_rt_preUserCodeHook"));

  // We have to initialize the main module explicitly.
  // It will initialize all the modules it uses, recursively.
  if (!fMultiLocaleInterop) {
    chpl_gen_main->insertAtTail(new CallExpr(mainModule->initFn));
    // also init other modules mentioned on command line
    forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
      if (mod->hasFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE) &&
          mod != mainModule) {
        chpl_gen_main->insertAtTail(new CallExpr(mod->initFn));
      }
    }

  } else {
    // Create an extern definition for the multilocale library server's main
    // function.  chpl_gen_main needs to call it in the course of its run, so
    // that we correctly set up the runtime.
    FnSymbol* chpl_mli_smain = new FnSymbol("chpl_mli_smain");
    chpl_mli_smain->addFlag(FLAG_EXTERN);
    chpl_mli_smain->addFlag(FLAG_LOCAL_ARGS);
    // Takes the connection information
    ArgSymbol* setup_conn = new ArgSymbol(INTENT_BLANK, "setup_conn",
                                          dtStringC);
    chpl_mli_smain->insertFormalAtTail(setup_conn);

    mainModule->block->insertAtTail(new DefExpr(chpl_mli_smain));

    VarSymbol* connection = newTemp("setup_conn");
    chpl_gen_main->insertAtTail(new DefExpr(connection));
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, connection,
                                             new CallExpr("chpl_get_mli_connection",
                                                          arg)));
    chpl_gen_main->insertAtTail(new CallExpr("chpl_mli_smain", connection));
    //normalize(chpl_mli_smain);
  }

  bool main_ret_set = false;

  if (!fLibraryCompile && !fDynoGenStdLib) {
    SET_LINENO(chpl_gen_main);

    if (mainHasArgs == true) {
      VarSymbol* converted_args = newTemp("_main_args");

      converted_args->addFlag(FLAG_INSERT_AUTO_DESTROY);

      chpl_gen_main->insertAtTail(new DefExpr(converted_args));
      chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                               converted_args,
                                               new CallExpr("chpl_convert_args", arg)));

      if (mainReturnsSomething) {
        chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                                 main_ret,
                                                 new CallExpr("main", converted_args)));
        main_ret_set = true;

      } else {
        chpl_gen_main->insertAtTail(new CallExpr("main", converted_args));
      }

    } else {
      if (mainReturnsSomething) {
        chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                                 main_ret,
                                                 new CallExpr("main")));
        main_ret_set = true;

      } else {
        chpl_gen_main->insertAtTail(new CallExpr("main"));
      }
    }
  }

  if (!main_ret_set) {
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE,
                                             main_ret,
                                             new_IntSymbol(0, INT_SIZE_64)));
  }

  chpl_gen_main->insertAtTail(new CallExpr("chpl_rt_postUserCodeHook"));

  //
  // In --minimal-modules compilation mode, we won't be waiting on an
  // endcount (see comment above)
  //
  if (fMinimalModules == false) {
    chpl_gen_main->insertAtTail(new CallExpr("_waitEndCount", endCount));
    chpl_gen_main->insertAtTail(new CallExpr("chpl_deinitModules"));
  }

  chpl_gen_main->insertAtTail(new CallExpr(PRIM_RETURN, main_ret));
#endif
}

// Finds or creates a Formal
// The ArgSymbol is created on first request here, and filled in
// and the DefExpr created wherever it should be defined.
ArgSymbol* TConverter::findOrCreateFormal(const Formal* node) {
  auto it = cur.localSyms.find(node->id());
  if (it != cur.localSyms.end()) {
    return toArgSymbol(it->second);
  }

  IntentTag intentTag = convertFormalIntent(node->intent());
  astlocMarker markAstLoc(node->id());

  ArgSymbol* ret = new ArgSymbol(intentTag, astr(node->name()),
                                 /* typeExpr */ dtUnknown,
                                 /* initExpr */ nullptr,
                                 /* varargsVariable */ nullptr);
  cur.localSyms[node->id()] = ret;
  return ret;
}

// Finds or creates a Variable
// The VarSymbol is created here on first request, and filled in
// and the DefExpr created wherever it should be defined.
VarSymbol* TConverter::findOrCreateVar(const Variable* node) {
  if (Symbol* sym = findConvertedSym(node->id())) {
    return toVarSymbol(sym);
  }

  // otherwise, create something
  // we need to decide if it should be a module-scope var or not
  // in order to update the appropriate map.
  bool isModuleScope = parsing::idIsModuleScopeVar(context, node->id());

  astlocMarker markAstLoc(node->id());
  VarSymbol* ret = new VarSymbol(astr(node->name()));
  if (isModuleScope) {
    globalSyms[node->id()] = ret;
  } else {
    cur.localSyms[node->id()] = ret;
  }

  return ret;
}

//
// For the following call where 'x', 'y', and 'z' are passed to a single
// varargs formal 'a'...
//
// proc foo(a: ...?n) {}
// var x, y, z: int;
// foo(x, y, z);
//
// (...The compiler generates the following signature for 'foo'...)
//
// proc foo(_e0: int, _e1: int, _e2: int) {}
//
// When the original varargs formal 'x' is used in certain ways that require
// tuple semantics, this function will create a "hidden" temporary tuple out
// of the formals '_e0'..'_e2' that represents 'x'.
//
// This helper function should be called when the "hidden" varargs tuple is
// required, as the first call will create and initialize the temporary.
//
VarSymbol*
TConverter::findOrCreateHiddenVarArg(const VarArgFormal* node, RV& rv) {
  if (Symbol* sym = findConvertedSym(node->id())) return toVarSymbol(sym);

  astlocMarker markAstLoc(node->id());
  VarSymbol* ret = new VarSymbol(astr(node->name()));
  cur.localSyms[node->id()] = ret;

  // Fill in the variable, insert it, and initialize it. This is different
  // from the other cases above because the hidden vararg variable is not
  // filled in at declaration, but rather when a specific use of the formal
  // triggers it to exist.

  // Add flags as per: 'buildTupleVariable'
  if (node->intent() == Formal::TYPE) ret->addFlag(FLAG_TYPE_VARIABLE);
  if (auto ag = node->attributeGroup()) {
    if (!ag->hasPragma(PRAGMA_NO_AUTO_DESTROY)) {
      ret->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }

  auto re = rv.byAstOrNull(node);
  INT_ASSERT(re);

  // Next, set the type and initialize it using the vararg formals.
  auto& qt = re->type();
  auto tt = qt.type() ? qt.type()->toTupleType() : nullptr;
  if (tt) {
    ret->type = convertType(tt);

    if (qt.kind() == types::QualifiedType::CONST_IN ||
        qt.kind() == types::QualifiedType::IN) {
      TC_UNIMPL("Manual move-initialization for varargs tuple");
      return ret;
    }

    auto init = findOrConvertTupleInit(tt);
    auto call = new CallExpr(init);
    int numViewed = 0;

    // TODO: Why does 'chpl__tuple_init' take 'this' only to 'return this'?!
    call->insertAtTail(new SymExpr(ret));

    // TODO: Need to figure out a strategy for injecting auto-copies.
    // TODO: Either dyno handles 'init' or we need to insert naive
    // copy calls here. Probably the latter.
    INT_ASSERT(cur.fnSymbol);
    for_formals(arg, cur.fnSymbol) {
      if (!arg->hasFlag(FLAG_EXPANDED_VARARGS)) continue;
      call->insertAtTail(new SymExpr(arg));
      numViewed += 1;
    }

    INT_ASSERT(numViewed == tt->numElements());

    auto move = new CallExpr(PRIM_MOVE, new SymExpr(ret), call);
    cur.fnSymbol->body->insertAtHead(move);
    cur.fnSymbol->body->insertAtHead(new DefExpr(ret));
  }

  return ret;
}

// Looks for an already-converted Symbol. If none found, returns nullptr.
Symbol* TConverter::findConvertedSym(const ID& id) {
  { // check for local variables with that ID
    auto it = cur.localSyms.find(id);
    if (it != cur.localSyms.end()) {
      return it->second;
    }
  }
  { // check for global symbols with that ID
    auto it = globalSyms.find(id);
    if (it != globalSyms.end()) {
      return it->second;
    }
  }

  return nullptr;
}

// TODO: convert this to a visitor if there end up being a lot of cases
Symbol* TConverter::findOrCreateSym(const ID& id, RV& rv) {
  Symbol* ret = findConvertedSym(id);
  if (ret != nullptr) {
    return ret;
  }

  auto ast = parsing::idToAst(context, id);
  auto parent = ast ? parsing::parentAst(context, ast) : nullptr;
  INT_ASSERT(ast);

  if (cur.symbol && cur.symbol->isFunction() &&
      cur.symbol == parent &&
      cur.actualConverter) {
    INT_ASSERT(ast->isFormal() || ast->isVarArgFormal() ||
               ast->isTupleDecl());
    // It's a formal. Since the 'actualConverter' field is set, then we
    // are converting a default argument and need to let the actual
    // converter intercept the formal to replace it with an actual.
    return cur.actualConverter->interceptFormalUse(id);
  }

  if (auto varargs = ast->toVarArgFormal()) {
    // TODO: Copy logic to determine if we need to create a hidden vararg
    // tuple variable at all, see: 'needVarArgTupleAsWhole'...
    // -OR-...Should we just always insert it and let dead-variable-elim
    // remove it if unused?
    return findOrCreateHiddenVarArg(varargs, rv);

  } else if (auto formal = ast->toFormal()) {
    return findOrCreateFormal(formal);

  } else if (auto var = ast->toVariable()) {
    return findOrCreateVar(var);
  }

  return nullptr;
}

void TConverter::noteConvertedSym(const uast::AstNode* ast, Symbol* sym) {
  if (cur.resolvedFunction == nullptr) {
    globalSyms[ast->id()] = sym;
  } else {
    cur.localSyms[ast->id()] = sym;
  }
}

Type* TConverter::findConvertedType(const types::Type* t) {
  auto it = convertedTypes.find(t);
  if (it != convertedTypes.end()) {
    return it->second;
  }

  return nullptr;
}

static IF1_complex_type getComplexSize(const types::ComplexType* t) {
  if (t->isDefaultWidth())
    return COMPLEX_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 64)  return COMPLEX_SIZE_64;
  else if (width == 128) return COMPLEX_SIZE_128;

  INT_FATAL("should not be reached");
  return COMPLEX_SIZE_DEFAULT;
}

static IF1_float_type getImagSize(const types::ImagType* t) {
  if (t->isDefaultWidth())
    return FLOAT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 32) return FLOAT_SIZE_32;
  else if (width == 64) return FLOAT_SIZE_64;

  INT_FATAL("should not be reached");
  return FLOAT_SIZE_DEFAULT;
}

static IF1_int_type getIntSize(const types::IntType* t) {
  if (t->isDefaultWidth())
    return INT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 8)  return INT_SIZE_8;
  else if (width == 16) return INT_SIZE_16;
  else if (width == 32) return INT_SIZE_32;
  else if (width == 64) return INT_SIZE_64;

  INT_FATAL("should not be reached");
  return INT_SIZE_DEFAULT;
}

static IF1_float_type getRealSize(const types::RealType* t) {
  if (t->isDefaultWidth())
    return FLOAT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 32) return FLOAT_SIZE_32;
  else if (width == 64) return FLOAT_SIZE_64;

  INT_FATAL("should not be reached");
  return FLOAT_SIZE_DEFAULT;
}

static IF1_int_type getUintSize(const types::UintType* t) {
  if (t->isDefaultWidth())
    return INT_SIZE_DEFAULT;

  int width = t->bitwidth();
  if      (width == 8)  return INT_SIZE_8;
  else if (width == 16) return INT_SIZE_16;
  else if (width == 32) return INT_SIZE_32;
  else if (width == 64) return INT_SIZE_64;

  INT_FATAL("should not be reached");
  return INT_SIZE_DEFAULT;
}

void TConverter::helpConvertFields(const types::CompositeType* ct,
                                   const ResolvedFields& rf,
                                   AggregateType* at) {
  int nFields = rf.numFields();
  for (int i = 0; i < nFields; i++) {
    types::QualifiedType qt = rf.fieldType(i);
    if (qt.isType() && !typeExistsAtRuntime(qt.type())) {
      // a 'type' field can be left out at this point
      continue;
    } else if (qt.isParam()) {
      // a 'param' field can be left out at this point
      continue;
    }
    Type* ft = convertType(qt.type());
    UniqueString name = rf.fieldName(i);
    VarSymbol* v = new VarSymbol(astr(name), ft);
    v->qual = convertQualifier(qt.kind());
    v->makeField();
    at->fields.insertAtTail(new DefExpr(v));
  }
}

struct ConvertTypeHelper {
  TConverter* tc_;

  Context* context() { return tc_->context; }

  DefExpr* insertTypeIntoModule(ID idToInsertAt, Type* converted) {
    if (auto def = converted->symbol->defPoint) return def;

    ID inModuleId = idToInsertAt;
    if (!parsing::idIsModule(context(), inModuleId)) {
      inModuleId = parsing::idToParentModule(context(), inModuleId);
    }

    auto mod = inModuleId ? tc_->findOrSetupModule(inModuleId)
                          : rootModule;
    mod->block->insertAtTail(new DefExpr(converted->symbol));

    return converted->symbol->defPoint;
  }

  Type* visit(const types::Type* t) {
    // Check to make sure implementation detail tags did not slip through.
    switch (t->tag()) {
      case types::typetags::START_ManageableType:
      case types::typetags::END_ManageableType:
      case types::typetags::START_BuiltinType:
      case types::typetags::END_BuiltinType:
      case types::typetags::START_DeclaredType:
      case types::typetags::END_DeclaredType:
      case types::typetags::START_CompositeType:
      case types::typetags::END_CompositeType:
      case types::typetags::START_PrimitiveType:
      case types::typetags::END_PrimitiveType:
      case types::typetags::START_IteratorType:
      case types::typetags::END_IteratorType:
      case types::typetags::START_PtrType:
      case types::typetags::END_PtrType:
      case types::typetags::NUM_TYPE_TAGS:
        INT_FATAL("should not be reachable");
        return dtUnknown;
      default: break;
    }

    std::string msg;
    msg += "Converting type tag '";
    msg += chpl::types::typetags::tagToString(t->tag());
    msg += "'";
    TC_UNIMPL(msg.c_str());
    return nullptr;
  }

  // builtin types with their own classes
  Type* visit(const types::AnyType* t) { return dtAny; }
  Type* visit(const types::CStringType* t) { return dtStringC; }
  Type* visit(const types::ErroneousType* t) { return dtUnknown; } // a lie
  Type* visit(const types::NilType* t) { return dtNil; }
  Type* visit(const types::NothingType* t) { return dtNothing; }
  Type* visit(const types::UnknownType* t) { return dtUnknown; }
  Type* visit(const types::VoidType* t) { return dtVoid; }

  // subclasses of IterableType (these return lies)
  Type* visit(const types::FnIteratorType* t) { return dtUnknown; }
  Type* visit(const types::LoopExprIteratorType* t) { return dtUnknown; }
  Type* visit(const types::PromotionIteratorType* t) { return dtUnknown; }

  // concrete builtin types
  Type* visit(const types::CFnPtrType* t) { return dtCFnPtr; }
  Type* visit(const types::CVoidPtrType* t) { return dtCVoidPtr; }
  Type* visit(const types::OpaqueType* t) { return dtOpaque; }
  Type* visit(const types::SyncAuxType* t) { return dtSyncVarAuxFields; }
  Type* visit(const types::TaskIdType* t) { return dtTaskID; }

  // generic builtin types
  Type* visit(const types::AnyComplexType* t) { return dtAnyComplex; }
  Type* visit(const types::AnyEnumType* t) { return dtAnyEnumerated; }
  Type* visit(const types::AnyImagType* t) { return dtAnyImag; }
  Type* visit(const types::AnyIntType* t) { return dtIntegral; } // a lie
  Type* visit(const types::AnyIntegralType* t) { return dtIntegral; }
  Type* visit(const types::AnyIteratorClassType* t) { return dtIteratorClass; }
  Type* visit(const types::AnyIteratorRecordType* t) { return dtIteratorRecord; }
  Type* visit(const types::AnyThunkRecordType* t) { return dtThunkRecord; }
  Type* visit(const types::AnyNumericType* t) { return dtNumeric; }
  Type* visit(const types::AnyOwnedType* t) { return dtOwned; }
  Type* visit(const types::AnyPodType* t) { return dtAnyPOD; }
  Type* visit(const types::AnyRealType* t) { return dtAnyReal; }
  Type* visit(const types::AnyRecordType* t) { return dtAnyRecord; }
  Type* visit(const types::AnySharedType* t) { return dtShared; }
  Type* visit(const types::AnyClassType* t) { return dtAnyManagementNonNilable; }
  Type* visit(const types::AnyUintType t) { return dtIntegral; } // a lie
  Type* visit(const types::AnyUninstantiatedType* t) { return dtUninstantiated; }
  Type* visit(const types::AnyUnionType* t) { return dtUnknown; } // a lie

  // declared types

  Type* visit(const types::ClassType* t) {
    if (auto mt = t->manageableType()) {
      if (mt->isAnyClassType()) {
        // The production compiler represents these as special builtins
        auto dec = t->decorator();
        if (dec.isUnmanaged()) {
          if (dec.isNilable()) return dtUnmanagedNilable;
          if (dec.isNonNilable()) return dtUnmanagedNonNilable;
          return dtUnmanaged;
        } else if (dec.isBorrowed()) {
          if (dec.isNilable()) return dtBorrowedNilable;
          if (dec.isNonNilable()) return dtBorrowedNonNilable;
          return dtBorrowed;
        } else if (dec.isUnknownManagement()) {
          if (dec.isNilable()) return dtAnyManagementNilable;
          if (dec.isNonNilable()) return dtAnyManagementNonNilable;
          return dtAnyManagementAnyNilable;
        } else {
          // fall through
        }
      }
    }

    // convert the basic class type
    const types::BasicClassType* bct = t->basicClassType();
    Type* gotT = tc_->convertType(bct);
    AggregateType* at = toAggregateType(gotT);
    INT_ASSERT(at);

    Type* ret = dtUnknown;

    const types::RecordType* manager = t->managerRecordType(context());
    if (manager == nullptr) {
      ret = at; // unamanged / borrowed is just the class type at this point
    } else {
      // owned/shared should have had a substitution for chpl_t
      CHPL_ASSERT(!manager->substitutions().empty());
      // convert the managed class type
      ret = tc_->convertType(manager);
    }

    return ret;
  }

  Type* visit(const types::BasicClassType* bct) {
    const ResolvedFields& rf =
      fieldsForTypeDecl(context(), bct, DefaultsPolicy::USE_DEFAULTS);

    AggregateType* at = toAggregateType(tc_->findConvertedType(bct));
    INT_ASSERT(at);

    if (at->symbol == nullptr) {
      at->symbol = new TypeSymbol(astr(bct->name()), at);
    }

    auto def = insertTypeIntoModule(bct->id(), at);
    INT_ASSERT(def);

    // convert the superclass as a field 'super'
    if (auto parentClassType = bct->parentClassType()) {
      Type* pt = tc_->convertType(parentClassType);
      VarSymbol* v = new VarSymbol("super", pt);
      v->qual = QUAL_VAL;
      v->makeField();
      at->fields.insertAtTail(new DefExpr(v));
    }

    // convert the fields
    tc_->helpConvertFields(bct, rf, at);

    return at;
  }

  Type* visit(const types::RecordType* t) {
    const ResolvedFields& rf =
      fieldsForTypeDecl(context(), t, DefaultsPolicy::USE_DEFAULTS);

    AggregateType* at = toAggregateType(tc_->findConvertedType(t));
    INT_ASSERT(at);

    if (at->symbol == nullptr) {
      at->symbol = new TypeSymbol(astr(t->name()), at);
    }

    auto def = insertTypeIntoModule(t->id(), at);
    INT_ASSERT(def);

    // convert the fields
    tc_->helpConvertFields(t, rf, at);

    return at;
  }

  Type* visit(const types::UnionType* t) {
    auto& rf = fieldsForTypeDecl(context(), t, DefaultsPolicy::USE_DEFAULTS);

    AggregateType* at = toAggregateType(tc_->findConvertedType(t));
    INT_ASSERT(at);

    if (at->symbol == nullptr) {
      at->symbol = new TypeSymbol(astr(t->name()), at);
    }

    auto def = insertTypeIntoModule(t->id(), at);
    INT_ASSERT(def);

    // convert the fields
    tc_->helpConvertFields(t, rf, at);

    return at;
  }

  // Taken from 'resolution/tuples.cpp'...
  static void makeTupleName(const std::vector<TypeSymbol*>& args,
                            bool isStarTuple,
                            std::string& name,
                            std::string& cname) {
    int size = args.size();
    const char* size_str = istr(size);
    bool omitRef = !developer;

    cname = "_tuple_";
    name = "";
    if (isStarTuple) {
      TypeSymbol* nameTS = args[0];
      if (omitRef)
        nameTS = nameTS->type->getValType()->symbol;
      name += size_str;
      name += "*";
      name += toString(nameTS->type);
      cname += size_str;
      cname += "_star_";
      cname += args[0]->cname;
    } else {
      name += "(";
      cname += size_str;
      for(int i = 0; i < size; i++) {
        TypeSymbol* nameTS = args[i];
        if (omitRef)
          nameTS = nameTS->type->getValType()->symbol;
        cname += "_";
        cname += args[i]->cname;
        if (i != 0 ) name += ",";
        name += toString(nameTS->type);
      }
      name += ")";
    }
  }

  Type* visit(const types::TupleType* t) {
    SET_LINENO(tc_->modChapelTuple);

    auto ret = new AggregateType(AGGREGATE_RECORD);

    // Construct the 'size' field which exists in every tuple.
    auto sizeVar = new VarSymbol("size");
    sizeVar->addFlag(FLAG_PARAM);
    sizeVar->type = dtInt[INT_SIZE_DEFAULT];
    ret->fields.insertAtTail(new DefExpr(sizeVar));
    ret->substitutions.put(sizeVar, new_IntSymbol(t->numElements()));

    std::vector<TypeSymbol*> args;
    bool isStarTuple = t->isStarTuple();
    std::string name, cname;

    // Push fields.
    for (int i = 0; i < t->numElements(); i++) {
      auto qt = t->elementType(i);
      auto sym = tc_->convertType(qt.type())->symbol;
      args.push_back(sym);

      const char* fname = astr("x", istr(i));
      VarSymbol*  var  = new VarSymbol(fname);
      var->type = args[i]->type;
      ret->fields.insertAtTail(new DefExpr(var));

      // TODO: In 'getTupleInfo' we emplace substitutions against the type
      // constructor formals, do we need to bother doing that in this
      // world where the tuple type is fully concrete?
      //
      // ret->substitutions.put(typeCtorArgs[i+1], var->type->symbol);
    }

    makeTupleName(args, isStarTuple, name, cname);

    ret->instantiatedFrom = dtTuple;
    ret->resolveStatus = RESOLVED;

    forv_Vec(AggregateType, t, dtTuple->dispatchParents) {
      AggregateType* at = toAggregateType(t);
      INT_ASSERT(at);
      ret->dispatchParents.add(at);
      at->dispatchChildren.add_exclusive(ret);
    }

    auto newTypeSymbol = new TypeSymbol(name.c_str(), ret);

    // TODO: Does this need to be set?
    newTypeSymbol->instantiationPoint = nullptr;

    newTypeSymbol->cname = astr(cname.c_str());
    newTypeSymbol->addFlag(FLAG_ALLOW_REF);
    newTypeSymbol->addFlag(FLAG_COMPILER_GENERATED);
    newTypeSymbol->addFlag(FLAG_TUPLE);
    newTypeSymbol->addFlag(FLAG_PARTIAL_TUPLE);
    newTypeSymbol->addFlag(FLAG_TYPE_VARIABLE);
    newTypeSymbol->addFlag(FLAG_RESOLVED_EARLY);
    if (isStarTuple) newTypeSymbol->addFlag(FLAG_STAR_TUPLE);

    // Insert all tuple instantiations in the tuple module.
    tc_->modChapelTuple->block->insertAtHead(new DefExpr(newTypeSymbol));

    return ret;
  }

  // pointer types and primitive types

  Type* visit(const types::PtrType* t) {
    // find the pointer type to instantiate
    AggregateType* base = nullptr;
    if (auto ct = t->toCPtrType()) {
      base = ct->isConst() ? dtCPointerConst : dtCPointer;
    } else {
      INT_ASSERT(t->toHeapBufferType());
      base = dtHeapBuffer;
    }

    if (base->symbol == nullptr) {
      INT_ASSERT(false && "Should not happen!");

      const char* name = nullptr;
      if (base == dtCPointer) {
        name = "c_ptr";
      } else if (base == dtCPointerConst) {
        name = "c_ptrConst";
      } else {
        name = "_ddata";
      }
      base->symbol = new TypeSymbol(name, base);
    }

    if (base->numFields() == 0) {
      // fill in the 'type eltType' field
      VarSymbol* v = new VarSymbol(astr("eltType"), dtAny);
      v->qual = QUAL_UNKNOWN;
      v->makeField();
      base->fields.insertAtTail(new DefExpr(v));

      // add to globalSyms so it can be ignored in the untyped converter
      const char* symbolPath = nullptr;
      if (base == dtCPointer) {
        symbolPath = "CTypes.c_ptr";
      } else if (base == dtCPointerConst) {
        symbolPath = "CTypes.c_ptrConst";
      } else {
        symbolPath = "ChapelBase._ddata";
      }

      auto id = ID(tc_->ustr(symbolPath));
      tc_->globalSyms[id] = base->symbol;

      auto def = insertTypeIntoModule(id, base);
      INT_ASSERT(def);
    }

    // handle ptr without an element type
    if (t->eltType() == nullptr) {
      return base;
    }

    // convert the element type
    Type* convertedEltT = tc_->convertType(t->eltType());

    // instantiate it with the element type
    // note: getInstantiation should re-use existing instantiations
    int index = 1;
    Expr* insnPoint = nullptr; // TODO: does this need to be set?
    AggregateType* ret =
      base->getInstantiation(convertedEltT->symbol, index, insnPoint);

    return ret;
  }

  Type* visit(const types::BoolType* t) {
    return dtBool;
  }

  Type* visit(const types::ComplexType* t) {
    return dtComplex[getComplexSize(t)];
  }

  Type* visit(const types::ImagType* t) {
    return dtImag[getImagSize(t)];
  }

  Type* visit(const types::IntType* t) {
    return dtInt[getIntSize(t)];
  }

  Type* visit(const types::RealType* t) {
    return dtReal[getRealSize(t)];
  }

  Type* visit(const types::UintType* t) {
    return dtUInt[getUintSize(t)];
  }
};

Type* TConverter::convertType(const types::Type* t, bool convertRefType) {
  if (t == nullptr) return dtUnknown;

  Type* ret = nullptr;

  if (auto existing = findConvertedType(t)) {
    // (A) Fetch the cached converted type to reuse it if possible.
    ret = existing;

  } else {
    // (B) Convert a type for the first time. Start by checking to see if
    // the type is an aggregate. If it is, check to see if it is a well
    // known type, otherwise make a stub first to support recursion when
    // converting the fields.
    auto selector = [](auto name, AggregateTag tag) {
      auto at = shouldWireWellKnownType(name.c_str());
      auto ret = at ? at : new AggregateType(tag);
      return ret;
    };

    AggregateType* at = nullptr;
    if (auto x = t->toBasicClassType()) {
      // NOTE: 'ClassType' will convert the 'BasicClassType' to get here.
      at = selector(x->name(), AGGREGATE_CLASS);
    } else if (auto x = t->toRecordType()) {
      at = selector(x->name(), AGGREGATE_RECORD);
    } else if (auto x = t->toUnionType()) {
      at = selector(x->name(), AGGREGATE_UNION);
    }

    // It's an aggregate, so emplace a map entry before converting.
    if (at) convertedTypes[t] = at;

    // Invoke the visitor to convert the type.
    ConvertTypeHelper visitor = { this };
    ret = t->dispatch<Type*>(visitor);
    INT_ASSERT(ret);

    // Set the converted type once again.
    convertedTypes[t] = ret;

    return ret;
  }

  // If we need to generate a 'ref' wrapper for the type, do so now.
  if (convertRefType && !ret->refType) {
    SET_LINENO(ret->symbol);

    // TODO: This is "AGGREGATE_RECORD" in "getOrMakeRefTypeDuringCodegen"?
    // But making these early 'ref' wrappers records interferes with a lot
    // of optimization passes that are looking for records, so my guess is
    // that the codegen generated ones should probably be 'CLASS' as well.
    AggregateType* ref = new AggregateType(AGGREGATE_CLASS);
    TypeSymbol* refTs = new TypeSymbol(astr("_ref_", ret->symbol->cname), ref);
    refTs->addFlag(FLAG_REF);
    refTs->addFlag(FLAG_NO_DEFAULT_FUNCTIONS);
    refTs->addFlag(FLAG_NO_OBJECT);
    refTs->addFlag(FLAG_RESOLVED_EARLY);
    globalInsertionPoint->insertAtTail(new DefExpr(refTs));
    ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", ret)));
    ret->refType = ref;
  }

  return ret;
}

struct ConvertDefaultValueHelper {
  TConverter* tc_;

  Expr* visit(const types::Type* t) {
    std::string msg;
    msg += "Constructing default value for type tag '";
    msg += chpl::types::typetags::tagToString(t->tag());
    msg += "'";
    TC_UNIMPL(msg.c_str());
    return tc_->placeholder();
  }

  Expr* visit(const types::BoolType* t) {
    return new SymExpr(new_BoolSymbol(false));
  }

  Expr* visit(const types::IntType* t) {
    auto var = new_IntSymbol(0, getIntSize(t));
    return new SymExpr(var);
  }

  Expr* visit(const types::CPtrType* t) {
    return new SymExpr(gNil);
  }

  Expr* visit(const types::ClassType* t) {
    // Note that we use 'nil' regardless of whether or not the class is
    // non-nilable or not, because non-nilable classes set to 'nil' or
    // default-initialized should have been caught earlier in compilation.
    return new SymExpr(gNil);
  }
};

Expr* TConverter::defaultValueForType(const types::Type* t) {
  ConvertDefaultValueHelper visitor = { this };
  auto ret = t->dispatch<Expr*>(visitor);
  return ret;
}

Symbol* TConverter::makeNewTemp(const types::QualifiedType& qt) {
  auto ret = newTemp();
  ret->addFlag(FLAG_EXPR_TEMP);
  ret->qual = convertQualifier(qt.kind());
  ret->type = convertType(qt.type());
  return ret;
}

SymExpr*
TConverter::storeInTempIfNeeded(Expr* e, const types::QualifiedType& qt) {
  if (SymExpr* se = toSymExpr(e)) {
    return se;
  }

  // otherwise, store the value in a temp
  auto t = makeNewTemp(qt);
  cur.AList->insertAtTail(new DefExpr(t));
  cur.AList->insertAtTail(new CallExpr(PRIM_MOVE, t, e));
  return new SymExpr(t);
}

// note: new_IntSymbol etc already returns existing if already created
Symbol* TConverter::convertParam(const types::QualifiedType& qt) {
  const types::Param* p = qt.param();
  const types::Type* t = qt.type();
  INT_ASSERT(p && t);

  if (auto bp = p->toBoolParam()) {
    return new_BoolSymbol(bp->value());
  } else if (auto cp = p->toComplexParam()) {
    const types::ComplexType* ct = t->toComplexType();
    types::Param::ComplexDouble tmp = cp->value();
    char buf[64];
    // compute the hexadecimal string form for the number
    snprintf(buf, sizeof(buf), "%a+%ai", tmp.re, tmp.im);
    return new_ComplexSymbol(buf, tmp.re, tmp.im, getComplexSize(ct));
  } else if (auto ip = p->toIntParam()) {
    const types::IntType* it = t->toIntType();
    return new_IntSymbol(ip->value(), getIntSize(it));
  } else if (p->isNoneParam()) {
    return gNone;
  } else if (auto rp = p->toRealParam()) {
    char buf[64];
    // compute the hexadecimal string form for the number
    snprintf(buf, sizeof(buf), "%a", rp->value());

    if (auto rt = t->toRealType()) {
      return new_RealSymbol(buf, getRealSize(rt));
    } else if (auto it = t->toImagType()) {
      return new_ImagSymbol(buf, getImagSize(it));
    }
  } else if (auto sp = p->toStringParam()) {
    if (t->isStringType()) {
      return new_StringSymbol(sp->value().c_str());
    } else if (t->isCStringType()) {
      return new_CStringSymbol(sp->value().c_str());
    } else if (t->isBytesType()) {
      return new_BytesSymbol(sp->value().c_str());
    }
  } else if (auto up = p->toUintParam()) {
    const types::UintType* t = qt.type()->toUintType();
    return new_UIntSymbol(up->value(), getUintSize(t));
  }

  INT_FATAL("should not be reached");
  return nullptr;
}

bool TConverter::typeExistsAtRuntime(const types::Type* t) {
  /* TODO */
  return false;
}

bool TConverter::functionExistsAtRuntime(const ResolvedFunction* r) {
  const auto& qt = r->returnType();
  if (qt.isType() && !typeExistsAtRuntime(qt.type())) {
    // a 'type' actual can be left out at this point
    return false;
  } else if (qt.isParam()) {
    // a 'param' actual can be left out at this point
    return false;
  }

  // otherwise, there is something to do.
  return true;
}

Expr* TConverter::convertLifetimeClause(const uast::AstNode* node, RV& rv) {
  astlocMarker markAstLoc(node->id());

  INT_ASSERT(node->isOpCall() || node->isReturn());
  if (auto opCall = node->toOpCall()) {
    INT_ASSERT(opCall->numActuals()==2);
    auto lhsIdent = opCall->actual(0)->toIdentifier();
    auto rhsIdent = opCall->actual(1)->toIdentifier();
    INT_ASSERT(lhsIdent && rhsIdent);
    INT_ASSERT(opCall->op() == USTR("=") ||
               opCall->op() == USTR("<") ||
               opCall->op() == USTR(">") ||
               opCall->op() == USTR("==")||
               opCall->op() == USTR("<=")||
               opCall->op() == USTR(">="));
    Expr* lhs = convertLifetimeIdent(lhsIdent, rv);
    Expr* rhs = convertLifetimeIdent(rhsIdent, rv);
    return new CallExpr(opCall->op().c_str(), lhs, rhs);
  } else if (auto ret = node->toReturn()) {
    INT_ASSERT(ret->value() && ret->value()->isIdentifier());
    auto ident = ret->value()->toIdentifier();

    Expr* val = convertLifetimeIdent(ident, rv);
    return new CallExpr(PRIM_RETURN, val);

  } else {
    // should not arrive here, or else we missed something
    TC_UNIMPL("Unhandled lifetime clause");
    return nullptr;
  }
}

CallExpr* TConverter::convertLifetimeIdent(const uast::Identifier* node, RV& rv)
{
  astlocMarker markAstLoc(node->id());

  auto ident = node->toIdentifier();
  INT_ASSERT(ident);
  CallExpr* callExpr = new CallExpr(PRIM_LIFETIME_OF,
                                    convertExprOrNull(node, rv));
  return callExpr;
}

Expr* TConverter::convertRuntimeTypeExpression(const AstNode* node, RV& rv) {
  // TODO: implement this
  return nullptr;
}

VarSymbol* TConverter::convertVariable(const uast::Variable* node,
                                       RV& rv,
                                       bool useLinkageName,
                                       MultiDeclState* multiState) {
  astlocMarker markAstLoc(node->id());

  bool isStatic = false;
  if (auto ag = node->attributeGroup()) {
    isStatic = ag->getAttributeNamed(USTR("functionStatic"));
  }

  if (node->name() == USTR("_")) {
    // no need to convert _
    return nullptr;
  }

  bool isRemote = node->destination() != nullptr ||
                  (multiState != nullptr && multiState->localeTemp != nullptr);

  auto varSym = findOrCreateVar(node);
  const bool isTypeVar = node->kind() == uast::Variable::TYPE;
  bool moduleScopeVar = false;
  const uast::Module* inModule = cur.symbol->toModule();

  if (inModule != nullptr) {
    moduleScopeVar = parsing::idIsModuleScopeVar(context, node->id());
  }

  if (moduleScopeVar) {
    varSym->addFlag(FLAG_RESOLVED_EARLY);
  }

  if (fIdBasedMunging && node->linkage() == uast::Decl::DEFAULT_LINKAGE) {
    // is it a module-scope variable?
    // adjust the cname for module-scope variables
    if (moduleScopeVar && inModule) {
      varSym->cname = astr(inModule->id().symbolPath().c_str(), ".",
                           varSym->name);
    }
  }
  uast::Variable::Kind symbolKind = node->kind();
  if (isRemote) {
    TC_UNIMPL("remote variable");
  }

  // Adjust the variable according to its kind, e.g. 'const'/'type'.
  attachSymbolStorage(symbolKind, varSym, /* setQual */ true);

  attachSymbolAttributes(context, node, varSym, cur.moduleFromLibraryFile);

  attachSymbolVisibility(node, varSym);
  if (node->isConfig()) {
    varSym->addFlag(FLAG_CONFIG);
  }

  Flag linkageFlag = convertFlagForDeclLinkage(node);
  if (linkageFlag != FLAG_UNKNOWN) {
    varSym->addFlag(linkageFlag);
  }

  // TODO (dlongnecke): Move to new parser (or post-parsing walk).
  if (node->kind() == uast::Variable::PARAM &&
      linkageFlag == FLAG_EXTERN) {
    USR_FATAL(varSym, "external params are not supported");
  }

  if (useLinkageName && node->linkageName()) {
    INT_ASSERT(linkageFlag != FLAG_UNKNOWN);
    varSym->cname = convertLinkageNameAstr(node);
  }
  //Expr* destinationExpr = convertExprOrNull(node->destination(), rv);
  Expr* typeExpr = convertRuntimeTypeExpression(node->typeExpression(), rv);
  Expr* initExpr = nullptr;

  if (const uast::AstNode* ie = node->initExpression()) {
    const uast::BracketLoop* bkt = ie->toBracketLoop();
    if (bkt && isTypeVar) {
      TC_UNIMPL("convert array type");
    } else {
      initExpr = convertExpr(ie, rv);
    }

    if (isStatic) {
      TC_UNIMPL("function-static variables");
    }
  } else {
    initExpr = convertExprOrNull(node->initExpression(), rv);
  }

  if ((!typeExpr && !initExpr) && multiState) {
    TC_UNIMPL("multi-vars");
  }

  DefExpr* def = nullptr;
  if (isRemote) {
    TC_UNIMPL("remote variable");
  } else {
    def = new DefExpr(varSym);
    if (!moduleScopeVar) {
      cur.AList->insertAtTail(def);
    } else {
      // module variables need to be stored outside of module init fn
      INT_ASSERT(cur.moduleSymbol);
      cur.moduleSymbol->block->insertAtTail(def);
    }

    CallExpr* move = nullptr;
    if (varSym->isRef()) {
      move = new CallExpr(PRIM_MOVE, varSym,
                                     new CallExpr(PRIM_ADDR_OF, initExpr));
    } else {
      if (initExpr == nullptr) {
        // compute the default value for this type
        if (const resolution::ResolvedExpression* re = rv.byAstOrNull(node)) {
          types::QualifiedType qt = re->type();
          if (!qt.isUnknown()) {
            initExpr = defaultValueForType(qt.type());
          }
        }
        INT_ASSERT(initExpr);
      }

      move = new CallExpr(PRIM_MOVE, varSym, initExpr);
    }
    cur.AList->insertAtTail(move);
  }

  auto loopFlags = LoopAttributeInfo::fromVariableDeclaration(context, node);
  if (!loopFlags.empty()) {
    TC_UNIMPL("loop attribute info");
    // cur.AList->insertAtTail(new CallExpr(PRIM_GPU_ATTRIBUTE_BLOCK));
    //if (auto primBlock = loopFlags.createPrimitivesBlock(*this)) {
    //  cur.AList->insertAtTail(primBlock);
    //}
  }
  // If the init expression of this variable is a domain and this
  // variable is not const, propagate that information by setting
  // 'definedConst' in the domain to false.
  setDefinedConstForDefExprIfApplicable(def, &def->sym->flags);

  // Fix up the AST based on the type, if it should be known
  setVariableType(node, varSym, rv);

  // Note the variable is converted so we can wire up SymExprs later
  noteConvertedSym(node, varSym);

  return varSym;
}

// this does a micro-optimization to replace
//   GOTO(epilogue)
//   epilogue:
// with relying on the pass-through
void TConverter::simplifyEpilogue(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_NO_FN_BODY)) return;

  if (Expr* labelDef = cur.epilogueLabel->defPoint) {
    if (GotoStmt* g = toGotoStmt(labelDef->prev)) {
      if (g->gotoTag == GOTO_RETURN) {
        // remove the GOTO and rely on fall-through
        g->remove();
      }
    }
  }

  // if the epilogue label is now unused, remove it
  if (cur.epilogueLabel->firstSymExpr() == nullptr) {
    cur.epilogueLabel->defPoint->remove();
  }
}

::Qualifier TConverter::convertQualifier(types::QualifiedType::Kind kind) {
  ::Qualifier q = QUAL_UNKNOWN;
  if      (kind == types::QualifiedType::VAR)       q = QUAL_VAL;
  else if (kind == types::QualifiedType::CONST_VAR) q = QUAL_CONST_VAL;
  else if (kind == types::QualifiedType::CONST_REF) q = QUAL_CONST_REF;
  else if (kind == types::QualifiedType::REF)       q = QUAL_REF;
  else if (kind == types::QualifiedType::IN)        q = QUAL_VAL;
  else if (kind == types::QualifiedType::CONST_IN)  q = QUAL_CONST_VAL;
  else if (kind == types::QualifiedType::OUT)       q = QUAL_VAL;
  else if (kind == types::QualifiedType::INOUT)     q = QUAL_VAL;
  else if (kind == types::QualifiedType::PARAM)     q = QUAL_PARAM;

  return q;
}

IntentTag TConverter::convertFormalIntentQt(types::QualifiedType::Kind kind) {
  IntentTag t = INTENT_BLANK;

  if      (kind == types::QualifiedType::VAR)            t = INTENT_IN;
  else if (kind == types::QualifiedType::CONST_VAR)      t = INTENT_IN;
  else if (kind == types::QualifiedType::CONST_REF)      t = INTENT_CONST_REF;
  else if (kind == types::QualifiedType::REF)            t = INTENT_REF;
  else if (kind == types::QualifiedType::IN)             t = INTENT_IN;
  else if (kind == types::QualifiedType::CONST_IN)       t = INTENT_CONST_IN;
  else if (kind == types::QualifiedType::OUT)            t = INTENT_OUT;
  else if (kind == types::QualifiedType::INOUT)          t = INTENT_INOUT;
  else if (kind == types::QualifiedType::PARAM)          t = INTENT_PARAM;
  else if (kind == types::QualifiedType::TYPE)           t = INTENT_TYPE;
  else if (kind == types::QualifiedType::DEFAULT_INTENT) t = INTENT_BLANK;

  return t;
}

void TConverter::setVariableType(const uast::VarLikeDecl* v,
                                 Symbol* sym,
                                 RV& rv) {
  TC_DEBUGF(this, "id = %s\n", v->id().str().c_str());

  // Get the type of the variable itself
  if (const resolution::ResolvedExpression* rr = rv.byAstOrNull(v)) {
    types::QualifiedType qt = rr->type();
    if (!qt.isUnknown()) {
      if (trace) {
        TC_DEBUGF(this, "qt is \n");
        qt.dump();
      }

      // Set a type for the variable
      sym->type = convertType(qt.type());

      // Set the Qualifier
      auto q = convertQualifier(qt.kind());
      if (q != QUAL_UNKNOWN)
        sym->qual = q;

      // If it's a Formal / ArgSymbol, also set the intent and originalIntent
      ArgSymbol* arg = toArgSymbol(sym);
      const uast::Formal* fml = v->toFormal();
      if (arg != nullptr && fml != nullptr) {
        arg->originalIntent = convertFormalIntent(fml->intent());
        arg->intent = convertFormalIntentQt(qt.kind());
      }

      // Set the param value for the variable in paramMap, if applicable
      /*if (sym->hasFlag(FLAG_MAYBE_PARAM) || sym->hasFlag(FLAG_PARAM)) {
        if (qt.hasParamPtr()) {
          Symbol* val = convertParam(qt);
          paramMap.put(sym, val);
        }
      }*/
    }
  }
}

Expr* TConverter::paramElideCallOrNull(const TypedFnSignature* sig,
                                       const PoiScope* poiScope,
                                       const ResolvedFunction** outRf) {
  if (!sig->untyped()->idIsFunction()) return nullptr;

  // TODO: Do we need to handle nested functions differently here?
  chpl::resolution::ResolutionContext rcval(context);
  auto rf = resolveFunction(&rcval, sig, poiScope);

  if (rf == nullptr) return nullptr;

  Expr* ret = nullptr;

  // Consider the return type of the called function.
  // Do we need to emit this call at all?
  // TODO: If the call is producing a type, do we really need to bother?
  if (!functionExistsAtRuntime(rf)) {
    // don't emit the call, but instead emit a SymExpr for the relevant
    // type or param.
    const auto& qt = rf->returnType();
    if (qt.isType()) {
      Type* t = convertType(qt.type());
      ret = new SymExpr(t->symbol);
    } else {
      Symbol* p = convertParam(qt);
      ret = new SymExpr(p);
    }
  }

  if (outRf) *outRf = rf;

  return ret;
}

Expr* TConverter::convertNewCallOrNull(const Call* node, RV& rv) {
  if (!node->calledExpression() ||
      !node->calledExpression()->isNew()) {
    return nullptr;
  }

  // We need the 'ResolvedExpression' to get the 'NEW_INIT' action.
  auto re = rv.byAstOrNull(node);
  if (!re) return nullptr;

  // Fetch the signature of the 'init()' procedure.
  const TypedFnSignature* init = nullptr;
  for (const auto& action : re->associatedActions()) {
    if (action.action() == AssociatedAction::NEW_INIT) {
      init = action.fn();
      break;
    }
  }

  const ResolvedFunction* rf = nullptr;
  if (paramElideCallOrNull(init, re->poiScope(), &rf)) {
    INT_ASSERT(false && "Should not have param elided initializer call!");
  } else if (!rf) {
    return nullptr;
  }

  // For 'new unmanaged C(...)' it should generate a call to a '_new'
  // function. The actuals are passed along to it down below.
  //
  // TODO: Handle management appropriately here (e.g., 'owned').
  //
  FnSymbol* calledFn = findOrConvertNewWrapper(rf);
  CallExpr* ret = new CallExpr(calledFn);

  // Pieces needed to construct the 'CallInfo'.
  const bool raiseErrors = false;
  std::vector<CallInfoActual> actuals;
  const AstNode* questionArg = nullptr;
  std::vector<const AstNode*> actualAsts;

  // Use 'prepareActuals' instead of 'create' here because the latter does
  // not understand how to handle a 'new' expression, and produces an
  // incorrect 'CallInfo'. A new expression is not actually a normal call,
  // though we represent it with a CI here because it does have actuals
  // (which can be named).
  resolution::CallInfo::prepareActuals(context, node, rv.byPostorder(),
                                       raiseErrors,
                                       actuals,
                                       questionArg,
                                       &actualAsts);

  // Copy over the init formal shapes, except for the receiver formal.
  std::vector<UntypedFnSignature::FormalDetail> formals;
  auto ufsInit = init->untyped();

  INT_ASSERT(ufsInit->isMethod());
  for (int i = 0; i < ufsInit->numFormals(); i++) {
    if (i == 0) continue;
    formals.push_back({ ufsInit->formalName(i),
                        ufsInit->formalDefaultKind(i),
                        /*decl*/ nullptr,
                        ufsInit->formalIsVarArgs(i) });
  }

  // Generate an untyped signature to get at the formal/actual map.
  auto fakeNewUfs = UntypedFnSignature::get(context,
                                        /*id*/ ID(),
                                        /*name*/ ustr("_new"),
                                        /*isMethod*/ false,
                                        /*isTypeConstructor*/ false,
                                        /*isCompilerGenerated*/ true,
                                        /*throws*/ init->untyped()->throws(),
                                        /*idTag*/ uast::asttags::Function,
                                        /*kind*/ uast::Function::PROC,
                                        /*formals*/ std::move(formals),
                                        /*whereClause*/ nullptr,
                                        /*compilerGeneratedOrigin*/ ID());

  // Note that we're creating a CI, but we're not actually going to resolve
  // this generated call, because the '_new' function exists only in the
  // middle-end. Instead, we're just using the CI to generate a formals-to-
  // actuals map.
  auto ci = resolution::CallInfo(ustr("_new"), types::QualifiedType(),
                                 /*isMethodCall*/ false,
                                 /*hasQuestionArg*/ false,
                                 /*isParenless*/ false,
                                 /*actuals*/ std::move(actuals));

  // This mapping drives the conversion of actuals.
  auto fam = FormalActualMap(fakeNewUfs, ci);
  INT_ASSERT(fam.isValid());

  convertAndInsertActuals(ret, node, actualAsts, init, fam, rv);

  return ret;
}

template <typename Iter>
void TConverter::convertAndInsertPrimCallActuals(CallExpr* call,
                                                 const Iter& actuals,
                                                 RV& rv) {
  // use a simple strategy to convert prim calls
  // (assuming no named argument passing)
  // TODO: do some associated actions need to be handled here?
  enterCallActuals(call);
  for (auto ast : actuals) {
    call->insertAtTail(convertExpr(ast, rv));
  }
  exitCallActuals();
}

Expr* TConverter::convertPrimCallOrNull(const Call* node, RV& rv) {
  auto primCall = node->toPrimCall();
  if (!primCall) return nullptr;

  // there should not be associated actions for primitive calls
  auto re = rv.byAstOrNull(node);
  CHPL_ASSERT(!re || !re->hasAssociatedActions());

  CallExpr* ret = new CallExpr(primCall->prim());

  convertAndInsertPrimCallActuals(ret, node->actuals(), rv);

  handlePostCallActions(ret, node, re, rv);

  return ret;
}

// TODO: The frontend should tell us this is happening so that we don't have
// to pattern-match against this. E.g., generate a method for param indexing.
Expr* TConverter::convertSpecialTupleParamIndexingOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // Param tuple indexing e.g., 'tuple[0]'.
  if (ci.name() == USTR("this") && ci.numActuals() == 2) {
    auto qt1 = ci.actual(0).type();
    auto qt2 = ci.actual(1).type();
    auto tt = qt1.type() ? qt1.type()->toTupleType() : nullptr;

    if (tt && qt2.param() && qt2.type() && qt2.type()->isIntType()) {
      auto val = qt2.param()->toIntParam()->value();
      INT_ASSERT(0 <= val && val < tt->numElements());

      // TODO: Replace this sequence with a 'getField' helper.
      Expr* tuple = convertExpr(actualAsts[0], rv);
      auto at = toAggregateType(tuple->typeInfo());
      INT_ASSERT(at);
      auto name = astr("x", istr(val));
      auto field = at->getField(name);
      auto get = new CallExpr(PRIM_GET_MEMBER, tuple, new SymExpr(field));
      Expr* ret = storeInTempIfNeeded(get, {});

      return ret;
    }
  }

  return nullptr;
}

Expr* TConverter::convertSpecialFieldInitOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // If not in an initializer, then 'this.x = ...' cannot be an init point.
  if (!cur.fnSymbol->isInitializer() || ci.name() != USTR("=")) {
    return nullptr;
  }

  // If the call maps directly to a function then it is not initialization.
  if (re->mostSpecific().only()) return nullptr;

  // Search for a relevant initialization action.
  const AssociatedAction* action = nullptr;
  for (const auto& a : re->associatedActions()) {
    if (a.action() == AssociatedAction::COPY_INIT ||
        a.action() == AssociatedAction::INIT_OTHER) {
      action = &a;
      break;
    }
  }

  CallExpr* ret = nullptr;

  // Emit a 'init=' call if an action was found, or a PRIM_MOVE otherwise.
  if (action) {

    // TODO: Encapsulate this whole process of "resolve a call using a TFS"
    // into a separate helper method.
    auto init = action->fn();
    INT_ASSERT(init && init->isInitEquals());

    const ResolvedFunction* rf = nullptr;
    if (paramElideCallOrNull(init, re->poiScope(), &rf)) {
      INT_ASSERT(false && "Should not have param elided initializer call!");
    } else if (!rf) {
      return placeholder();
    }

    auto fn = findOrConvertFunction(rf);
    INT_ASSERT(fn);

    auto fam = FormalActualMap(init->untyped(), ci);
    INT_ASSERT(fam.isValid());

    auto call = new CallExpr(fn);
    convertAndInsertActuals(call, node, actualAsts, init, fam, rv);
    ret = call;

  } else {
    // If no associated action was found then emit a move.
    ret = new CallExpr(PRIM_MOVE);
    convertAndInsertPrimCallActuals(ret, actualAsts, rv);
  }

  INT_ASSERT(ret);

  return ret;
}

Expr* TConverter::convertSpecialIntrinsicCastOrNull(
                                const Call* node,
                                const ResolvedExpression* re,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::CallInfo& ci,
                                RV& rv) {
  // The call is not a cast.
  if (ci.name() != USTR(":")) return nullptr;

  // If the call maps directly to a function then it is a "normal" call.
  if (re->mostSpecific().only()) return nullptr;

  auto& qt = re->type();
  INT_ASSERT(qt.type());

  // The cast was not typed, so the AST is malformed.
  if (qt.isUnknownOrErroneous()) return placeholder();

  if (qt.isParam()) {
    // The param value was not set, so the code is malformed.
    if (!qt.hasParamPtr()) return placeholder();

    // Otherwise, convert the param value and return it.
    auto sym = convertParam(re->type());
    auto ret = new SymExpr(sym);
    return ret;
  }

  // Match casts by actual type(s).
  INT_ASSERT(node->numActuals() == 2);
  auto& qt1 = rv.byAst(node->actual(0)).type();
  auto& qt2 = rv.byAst(node->actual(1)).type();

  // The cast is malformed, the second actual should be a type.
  if (qt2.kind() != types::QualifiedType::TYPE) return placeholder();

  if (qt1.type() == qt2.type()) {
    // TODO: Elide the cast, see '_removed_cast' in old compiler.
    TC_UNIMPL("Eliding cast to same type");
    return placeholder();
  }

  const ResolvedFunction* rf = nullptr;
  if (auto fn = convertFunctionForGeneratedCall(ci, cur.symbol, &rf)) {
    // Otherwise, using the provided CI, try to resolve a call to a cast
    // function. Many times intrinsic casts are still implemented using
    // a standard/internal module function.

    INT_ASSERT(rf && rf->signature());
    auto tfs = rf->signature();

    auto fam = FormalActualMap(tfs, ci);
    INT_ASSERT(fam.isValid());

    auto ret = new CallExpr(fn);
    convertAndInsertActuals(ret, node, actualAsts, tfs, fam, rv);
    return ret;
  }

  // There was no call, so we missed a case! Return a placeholder.
  TC_UNIMPL("Unhandled intrinsic cast");
  return placeholder();
}

Expr* TConverter::convertSpecialNamedCallOrNull(const Call* node, RV& rv) {
  auto re = rv.byAstOrNull(node);

  // Special calls need the info from the 'ResolvedExpression'.
  if (!re) return nullptr;

  // In mose cases the call should be specified in 're->mostSpecific()'.
  auto& candidate = re->mostSpecific().only();

  // If we have a signature, then it's a regular call, not a special one.
  if (candidate.fn()) return nullptr;

  // Reconstruct a 'CallInfo' to help with pattern-matching.
  std::vector<const AstNode*> actualAsts;
  const bool raiseErrors = false;
  auto ci = resolution::CallInfo::create(context, node, rv.byPostorder(),
                                         raiseErrors,
                                         &actualAsts);
  Expr* ret = nullptr;

  ret = convertSpecialTupleParamIndexingOrNull(node, re, actualAsts, ci, rv);
  if (ret) return ret;

  ret = convertSpecialFieldInitOrNull(node, re, actualAsts, ci, rv);
  if (ret) return ret;

  ret = convertSpecialIntrinsicCastOrNull(node, re, actualAsts, ci, rv);
  if (ret) return ret;

  return ret;
}

Expr* TConverter::convertRegularNamedCallOrNull(const Call* node, RV& rv) {
  auto re = rv.byAstOrNull(node);

  // Regular calls need the info from the 'ResolvedExpression'.
  if (!re) return nullptr;

  // In mose cases the call should be specified in 're->mostSpecific()'.
  auto& candidate = re->mostSpecific().only();
  auto sig = candidate.fn();

  // If we don't have a signature, then resolution probably failed.
  if (!sig) return nullptr;

  // Fetch the resolved function and elide the call entirely if possible.
  const ResolvedFunction* rf = nullptr;
  if (Expr* elided = paramElideCallOrNull(sig, re->poiScope(), &rf)) {
    return elided;
  } else if (!rf) {
    return nullptr;
  }

  auto calledFn = findOrConvertFunction(rf);
  CallExpr* ret = new CallExpr(calledFn);

  // Create a 'CallInfo' to populate the actual ASTs as a side-effect.
  std::vector<const AstNode*> actualAsts;
  const bool raiseErrors = true;
  std::ignore = resolution::CallInfo::create(context, node, rv.byPostorder(),
                                             raiseErrors,
                                             &actualAsts);

  // This mapping drives the conversion of actuals.
  auto& fam = candidate.formalActualMap();
  INT_ASSERT(fam.isValid());

  convertAndInsertActuals(ret, node, actualAsts, sig, fam, rv);

  return ret;
}

TConverter::ActualConverter::ActualConverter(
                                TConverter* tc,
                                const AstNode* astForCall,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::TypedFnSignature* tfs,
                                const FormalActualMap& fam,
                                RV& rv)
    : tc_(tc), tfs_(tfs), astForCall_(astForCall),
      actualAsts_(actualAsts),
      fam_(fam),
      rv_(rv),
      tempInsertionPoint_(tc->cur.AList) {
  INT_ASSERT(tempInsertionPoint_);

  // Use the arity provided by 'fam' since it has already accounted for
  // variable-arity formals such as varargs when computing alignment.
  INT_ASSERT(fam_.isValid());
  actualState_.resize(fam_.numFormalsMapped());

  // Determine if the call requires any defaults at all.
  for (auto& fa : fam_.byFormals()) {
    callRequiresDefaults_ |= fa.hasDefault();

    if (auto decl = fa.formal()) {
      // A vararg formal can never be referred to by another formal.
      if (decl->isVarArgFormal()) continue;

      auto it = formalIdToFormalActual_.find(decl->id());
      INT_ASSERT(it == formalIdToFormalActual_.end());
      formalIdToFormalActual_.emplace(decl->id(), fa);
    }
  }

  // Exit early if defaults are not required.
  if (!callRequiresDefaults_) return;

  if (!tfs_) {
    TC_UNIMPL("Converting default arguments without 'TypedFnSignature'!");
    return;
  }

  prepareFormalConversionState();
}

void TConverter::ActualConverter::prepareFormalConversionState() {
  //
  // Otherwise, we prepare to convert default arguments by setting up
  // a temporary "conversion state" for the called function.
  //
  Context* context = tc_->context;

  auto re = rv_.byAstOrNull(astForCall_);
  INT_ASSERT(re);

  ResolutionContext rcval(context);
  auto rf = resolveFunction(&rcval, tfs_, re->poiScope());
  INT_ASSERT(rf);

  ID parentModule = parsing::idToParentModule(context, rf->id());
  auto moduleSymbol = tc_->findOrSetupModule(parentModule);

  // Prepare the state block for the called function.
  calledFnState_ = {
    .symbol                 = parsing::idToAst(context, rf->id()),
    .fnSymbol               = tc_->findOrConvertFunction(rf),
    .moduleSymbol           = moduleSymbol,
    .retVar                 = nullptr,
    .epilogueLabel          = nullptr,
    .resolvedFunction       = rf,
    .actualConverter        = this,
    .moduleFromLibraryFile  = moduleSymbol->hasFlag(FLAG_PRECOMPILED),
    .localSyms              = {},
    .AList                  = nullptr,
    .topLevelModTag         = moduleSymbol->modTag
  };
}

void
TConverter::ActualConverter::convertActual(const FormalActual& fa) {
  if (tc_->shouldElideFormal(tfs_, fa.formalIdx())) return;

  Context* context = tc_->context;

  // One or the other must hold in order to compute the slot index.
  INT_ASSERT(fa.hasDefault() || fa.hasActual());

  int idxSlot = fa.hasActual() ? fa.actualIdx() : fa.formalIdx();
  INT_ASSERT(0 <= idxSlot && idxSlot < actualState_.size());
  auto& slot = actualState_[idxSlot];

  if (!fa.hasDefault()) {
    INT_ASSERT(fa.hasActual());

    // If there is no default argument then an actual is present.
    int idxActual = fa.actualIdx();
    INT_ASSERT(0 <= idxActual && idxActual < actualAsts_.size());

    // And there should be AST there.
    auto astActual = fa.hasActual() ? actualAsts_[idxActual] : nullptr;
    INT_ASSERT(astActual);

    // Convert the actual and leave.
    std::get<Expr*>(slot) = tc_->convertExpr(astActual, rv_);

    return;
  }

  // Otherwise we need a default argument. Get the formal's init expression.
  const Function* fn = calledFnState_.symbol->toFunction();
  const Decl* decl = fn->formal(fa.formalIdx());
  const AstNode* initExpr = nullptr;
  if (auto var = decl->toVarLikeDecl()) {
    initExpr = var->initExpression();
  } else {
    TC_UNIMPL("Unhandled formal when unpacking init expression!");
  }

  // Something went wrong and we failed to get the init expression.
  if (!initExpr) {
    std::get<Expr*>(slot) = tc_->placeholder();
    return;
  }

  //
  // At this point, we are resolving a default argument value. In order to
  // do so, we have to swap on the temporary "conversion state" that we
  // prepared for the called function, run the converter on the formal's
  // init-expr in order to generate AST, and then swap back to the
  // "conversion state" of the function that contains the call. To handle
  // the case where a formal's init-expr refers to another formal, the
  // converter is set up to replace references to formals with actuals
  // "on demand".
  //

  auto rf = calledFnState_.resolvedFunction;
  INT_ASSERT(rf);

  auto& rr = rf->resolutionById();

  // Swap on the called function state.
  std::swap(calledFnState_, tc_->cur);
  tc_->pushBlock(tc_->cur.moduleSymbol->block);

  // Convert the init expression for the formal of interest.
  ResolutionContext rcval(context);
  ResolvedVisitor<TConverter> rvCalledFn(&rcval, fn, *tc_, rr);
  std::get<Expr*>(slot) = tc_->convertExpr(initExpr, rvCalledFn);

  // Swap off the called function state.
  tc_->popBlock();
  std::swap(calledFnState_, tc_->cur);
}

void
TConverter::ActualConverter::convertAndInsertActuals(CallExpr* call) {
  auto aList = tc_->cur.AList;

  tc_->enterCallActuals(call);

  // Convert each actual.
  for (const FormalActual& fa : fam_.byFormals()) {
    this->convertActual(fa);
  }

  tc_->exitCallActuals();

  // Ensure the 'current AST list' matches - converter may swap contexts.
  INT_ASSERT(aList == tc_->cur.AList);

  // Then insert their ASTs into the call.
  for (auto& slot : actualState_) {
    if (auto temp = std::get<Symbol*>(slot)) {
      call->insertAtTail(new SymExpr(temp));
    } else if (auto expr = std::get<Expr*>(slot)) {
      call->insertAtTail(expr);
    }
  }
}

// TODO: How are implicit receivers handled here? There won't be an AST for it?
void TConverter::convertAndInsertActuals(
                                CallExpr* c,
                                const AstNode* node,
                                const std::vector<const AstNode*>& actualAsts,
                                const resolution::TypedFnSignature* tfs,
                                const FormalActualMap& fam,
                                RV& rv) {
  ActualConverter ac(this, node, actualAsts, tfs, fam, rv);
  ac.convertAndInsertActuals(c);
}

Symbol* TConverter::ActualConverter::interceptFormalUse(const ID& id) {
  auto it = formalIdToFormalActual_.find(id);
  if (it != formalIdToFormalActual_.end()) {
    auto& fa = it->second;
    int idxActual = fa.actualIdx();
    auto& slot = actualState_[idxActual];
    auto& temp = std::get<Symbol*>(slot);

    // If a temporary exists, then return it.
    if (temp) return temp;

    // Otherwise, we need to create the temporary.
    auto expr = std::get<Expr*>(slot);
    INT_ASSERT(expr);

    auto ret = newTemp(astr("actual_", istr(idxActual)));
    tempInsertionPoint_->insertAtTail(new DefExpr(ret));

    auto move = new CallExpr(PRIM_MOVE, ret, expr);
    tempInsertionPoint_->insertAtTail(move);

    temp = ret;

    return ret;
  } else {
    INT_ASSERT(false && "Expected entry for formal ID!");
  }
  return nullptr;
}

void TConverter::handlePostCallActions(CallExpr* c,
                                       const Call* node,
                                       const resolution::ResolvedExpression* re,
                                       RV& rv) {
}

ArgSymbol* TConverter::convertFormal(const Formal* fml, RV& rv) {
  auto typeExpr = convertRuntimeTypeExpression(fml->typeExpression(), rv);
  if (typeExpr) {
    TC_UNIMPL("runtime type formal");
  }

  ArgSymbol* ret = findOrCreateFormal(fml);

  attachSymbolAttributes(context, fml, ret, cur.moduleFromLibraryFile);

  setVariableType(fml, ret, rv);

  // Special handling for implicit receiver formal.
  if (fml->name() == USTR("this")) {
    ret->addFlag(FLAG_ARG_THIS);
    if (ret->intent == INTENT_TYPE) setupTypeIntentArg(ret);
  }

  return ret;
}

// See: 'resolution/expandVarArgs.cpp'
//
// Some big benefits to working with the typed converter. First off, we
// don't need to create a dummy tuple to inject into the 'where' clause,
// because the where is already param-evaluated. So we can drop that code.
// Next, we can also drop worrying about the lifetime constraints code.
void TConverter::convertAndInsertVarArgs(const VarArgFormal* fml, RV& rv) {
  auto fn = cur.fnSymbol;

  fn->addFlag(FLAG_EXPANDED_VARARGS);

  auto typeExpr = convertRuntimeTypeExpression(fml->typeExpression(), rv);
  if (typeExpr) {
    TC_UNIMPL("runtime type formal");
  }

  if (auto re = rv.byAstOrNull(fml)) {
    auto& qt = re->type();
    auto tt = qt.type() ? qt.type()->toTupleType() : nullptr;

    if (tt) {
      for (int i = 0; i < tt->numElements(); i++) {
        // See: 'insertFormalsForVarArg'

        auto qtElement = tt->elementType(i);
        auto name = astr("_e", istr(i), "_", fml->name().c_str());

        // Start with 'INTENT_BLANK', but we'll overwrite it below.
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, dtUnknown);

        // Manually set the formal's fields.
        arg->originalIntent = convertFormalIntent(fml->intent());
        arg->intent = convertFormalIntentQt(qtElement.kind());
        arg->variableExpr = nullptr;
        arg->type = convertType(qtElement.type());
        arg->addFlag(FLAG_EXPANDED_VARARGS);

        attachSymbolAttributes(context, fml, arg, cur.moduleFromLibraryFile);

        auto def = new DefExpr(arg);
        fn->insertFormalAtTail(def);
      }
    }
  }
}

bool TConverter::enter(const Module* node, RV& rv) {
  TC_DEBUGF(this, "enter module %s %s\n", node->id().str().c_str(),
            asttags::tagToString(node->tag()));

  if (modulesToConvert.count(node->id()) == 0) {
    // this module should not be converted.
    TC_DEBUGF(this, "Not in modules to convert\n");
    return false;
  }

  if (node == cur.symbol) {
    // we are visiting it to convert the module initialization function
    // so proceed with the traversal
    TC_DEBUGF(this, "Proceeding with module converting\n");
    return true;
  } else {
    TC_DEBUGF(this, "Recording a submodule\n");
    submodulesEncountered.push_back(node);

    return false;
  }
  return true;
}

void TConverter::exit(const Module* node, RV& rv) {
  TC_DEBUGF(this, "exit module %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool
TConverter::shouldElideFormal(const TypedFnSignature* tfs, int formalIdx) {
  INT_ASSERT(tfs && 0 <= formalIdx && formalIdx < tfs->numFormals());
  bool ret = false;

  auto& qt = tfs->formalType(formalIdx);

  if (qt.isType() || qt.isParam()) {
    // In general, always elide 'type' and 'param' formals. The information
    // they provide does not need to be carried around because the frontend
    // has already resolved the program.
    ret = true;

    // But keep around formals with runtime types.
    if (qt.isType() && typeExistsAtRuntime(qt.type())) ret = false;

  } else if (tfs->formalName(formalIdx) == USTR("this") &&
             tfs->isOperator()) {
    // We can skip receivers for operator methods as they should have
    // the 'TYPE' or 'BLANK' intent. If they don't then the frontend
    // should have caught this and emitted an error.
    ret = true;
  }

  return ret;
}

bool TConverter::enter(const Function* node, RV& rv) {
  TC_DEBUGF(this, "enter function %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));


  if (node != cur.symbol) {
    // It's a function, but not the one we are working on.
    // Stop the traversal. This Function will be handled when
    // convertFunction is called.
    return false;
  }

  TC_DEBUGF(this, "Really converting Function %s\n", node->name().c_str());

  astlocMarker markAstLoc(node->id());

  CHPL_ASSERT(cur.resolvedFunction != nullptr);

  FnSymbol* fn = new FnSymbol(astr(node->name()));
  cur.AList->insertAtTail(new DefExpr(fn));
  cur.fnSymbol = fn; // for setting child node's parentSymbol

  // note the correspondence between the ResolvedFunction & what it converts to
  // (for calls & done here to support recursive calls)
  fns[cur.resolvedFunction] = fn;

  fn->userString = constructUserString(node);

  attachSymbolAttributes(context, node, fn, cur.moduleFromLibraryFile);
  attachSymbolVisibility(node, fn);

  if (node->isInline()) {
    fn->addFlag(FLAG_INLINE);
  }

  if (node->isOverride()) {
    fn->addFlag(FLAG_OVERRIDE);
  }

  if (node->isParenless()) {
    fn->addFlag(FLAG_NO_PARENS);
  }

  if (node->isMethod()) {
    fn->addFlag(FLAG_METHOD);
    if (node->isPrimaryMethod()) {
      fn->addFlag(FLAG_METHOD_PRIMARY);
    }
  }

  fn->addFlag(FLAG_RESOLVED);
  fn->addFlag(FLAG_RESOLVED_EARLY);
  fn->setNormalized(true);

  if (cur.resolvedFunction->signature()->instantiatedFrom() != nullptr) {
    // generic instantiations are "invisible"
    fn->addFlag(FLAG_INVISIBLE_FN);
  }

  IntentTag thisTag = INTENT_BLANK;
  ArgSymbol* convertedReceiver = nullptr;

  // Convert the formals

  if (node->numFormals() > 0) {
    // create the formals first & put them in a map
    TC_DEBUGF(this, "Converting formals\n");
    enterFormals(fn);
    // TODO: gather the formals we want to create
    // to un-pack tuples etc.

    int formalIdx = 0;
    for (auto decl : node->formals()) {
      astlocMarker markAstLoc(decl->id());

      // Some formals (e.g., `type`/`param`) can be skipped.
      auto tfs = cur.resolvedFunction->signature();
      if (shouldElideFormal(tfs, formalIdx)) continue;

      // Fill in each ArgSymbol and add the DefExpr
      if (auto fml = decl->toFormal()) {
        auto arg = convertFormal(fml, rv);

        if (arg->hasFlag(FLAG_ARG_THIS)) {
          INT_ASSERT(!convertedReceiver);
          convertedReceiver = arg;
          thisTag = arg->intent;
        }

        auto def = new DefExpr(arg);
        fn->insertFormalAtTail(def);

      } else if (auto va = decl->toVarArgFormal()) {
        convertAndInsertVarArgs(va, rv);

      } else if (decl->toTupleDecl()) {
        // A tuple decl, where components are formals or tuple decls.
        TC_UNIMPL("Unhandled tuple formal");
        /*
        auto castIntent = (uast::Formal::Intent)formal->intentOrKind();
        IntentTag tag = convertFormalIntent(castIntent);
        BlockStmt* tuple = convertTupleDeclComponents(formal);
        INT_ASSERT(tuple);

        Expr* type = convertExprOrNull(formal->typeExpression());
        Expr* init = convertExprOrNull(formal->initExpression());

        // TODO: Move this specialization into visitor? We can just
        // detect if components are formals.
        conv = buildTupleArgDefExpr(tag, tuple, type, init);
        INT_ASSERT(conv);
        */
      } else {
        TC_UNIMPL("Unhandled formal");
      }

      formalIdx += 1;
    }

    exitFormals(fn);
  }

  const char* convName = convertFunctionNameAndAstr(node);

  // used to be buildFunctionSymbol
  fn->cname = fn->name = astr(convName);

  if (fIdBasedMunging && node->linkage() == uast::Decl::DEFAULT_LINKAGE &&
      // ignore things like chpl_taskAddCoStmt
      !fn->hasFlag(FLAG_ALWAYS_RESOLVE)) {
    CHPL_ASSERT(node->id().postOrderId() == -1);
    fn->cname = astr(node->id().symbolPath());
  }

  if (convertedReceiver) {
    fn->thisTag = thisTag;
    fn->_this = convertedReceiver;
    fn->setMethod(true);
    ArgSymbol* mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
    fn->insertFormalAtHead(new DefExpr(mt));
    if (node->isPrimaryMethod()) {
      fn->addFlag(FLAG_METHOD_PRIMARY);
    }
  }

  if (fn->name == astrDeinit)
    fn->addFlag(FLAG_DESTRUCTOR);

  if (isAssignOp(node->name())) {
    fn->addFlag(FLAG_ASSIGNOP);
  }

  RetTag retTag = convertRetTag(node->returnIntent());

  if (node->kind() == uast::Function::ITER) {
    fn->addFlag(FLAG_ITERATOR_FN);

  } else if (node->kind() == uast::Function::OPERATOR) {
    fn->addFlag(FLAG_OPERATOR);

  } else if (node->isAnonymous()) {
    fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
    fn->addFlag(FLAG_ANONYMOUS_FN);
    if (node->kind() == uast::Function::LAMBDA) {
      fn->addFlag(FLAG_LEGACY_LAMBDA);
    }
  }
  Expr* retType = convertRuntimeTypeExpression(node->returnType(), rv);
  Expr* whereClause = nullptr;

  Expr* lifetimeConstraints = nullptr;
  if (node->numLifetimeClauses() > 0) {
    TC_DEBUGF(this, "Converting lifetime clause\n");
    // create a new AList for the lifetime clause
    pushNewBlock();

    for (auto clause : node->lifetimeClauses()) {
      Expr* convertedClause = convertLifetimeClause(clause, rv);
      INT_ASSERT(convertedClause);

      if (lifetimeConstraints == nullptr) {
        lifetimeConstraints = convertedClause;
      } else {
        lifetimeConstraints =
          new CallExpr(",", lifetimeConstraints, convertedClause);
      }
    }
    INT_ASSERT(lifetimeConstraints);

    popBlock();
  }

  setupFunctionDecl(fn, retTag, retType, node->throws(),
                    whereClause,
                    lifetimeConstraints,
                    // pass a body or not to set FLAG_NO_FN_BODY appropriately
                    node->body() ? fn->body : nullptr);

  if (node->linkage() != uast::Decl::DEFAULT_LINKAGE) {
    Flag linkageFlag = convertFlagForDeclLinkage(node);
    Expr* linkageExpr = convertExprOrNull(node->linkageName(), rv);

    // This thing sets the 'cname' if it's a string literal, attaches
    // some flags, sets the return type to 'void' if no type is
    // specified, and attaches a dummy formal for the C name (?).
    setupExternExportFunctionDecl(linkageFlag, linkageExpr, fn);
  }

  const bool convertRefType = true;
  fn->retType = convertType(cur.resolvedFunction->returnType().type(),
                            convertRefType);

  if (fn->retType == dtVoid) {
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }

  // visit the body to convert
  if (node->body()) {
    TC_DEBUGF(this, "Converting body into [%i]\n", fn->body->id);
    pushBlock(fn->body);

    if (fn->retType != dtVoid) {
      // construct the RVV
      cur.retVar = newTemp("ret", fn->retType);
      cur.retVar->addFlag(FLAG_RVV);

      if (fn->retTag == RET_PARAM)      cur.retVar->addFlag(FLAG_PARAM);
      if (fn->retTag == RET_TYPE)       cur.retVar->addFlag(FLAG_TYPE_VARIABLE);
      if (fn->hasFlag(FLAG_MAYBE_TYPE)) cur.retVar->addFlag(FLAG_MAYBE_TYPE);

      fn->insertAtHead(new DefExpr(cur.retVar));
    }

    // construct the epilogue label
    cur.epilogueLabel = new LabelSymbol(astr("_end_", fn->name));
    cur.epilogueLabel->addFlag(FLAG_EPILOGUE_LABEL);
    // note: label is added to the AST later

    for (auto stmt : node->body()->stmts()) {
      astlocMarker markAstLoc(stmt->id());
      stmt->traverse(rv);
    }

    // add the epilogue label to the AST
    fn->insertAtTail(new DefExpr(cur.epilogueLabel));

    // TODO: add deinit calls for the end-of-block actions for the function here

    // add the single return
    if (fn->retType == dtVoid) {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    } else {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, cur.retVar));
    }

    popBlock();
  }

  simplifyEpilogue(fn);

  if (trace) {
    TC_DEBUGF(this, "converted to: ");
    nprint_view(fn);
    TC_DEBUGF(this, "\n");
  }

  // clear return variables so they can't be confused
  cur.retVar = nullptr;
  cur.epilogueLabel = nullptr;

  return false;
}
void TConverter::exit(const Function* node, RV& rv) {
  TC_DEBUGF(this, "exit function %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Variable* node, RV& rv) {
  TC_DEBUGF(this, "enter variable %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  VarSymbol* varSym = convertVariable(node, rv, true);
  INT_ASSERT(varSym);
  // note: convertVariable should have added the DefExpr and any associated
  // stuff to cur.AList.

  // Special handling for extern type variables.
  const bool isTypeVar = node->kind() == uast::Variable::TYPE;
  if (isTypeVar) {
    if (node->linkage() == uast::Decl::EXTERN) {
      TC_UNIMPL("extern type vars");
      /* TODO
      INT_ASSERT(!node->isConfig());
      INT_ASSERT(info.variableDef->sym && isVarSymbol(info.variableDef->sym));
      auto varSym = toVarSymbol(info.variableDef->sym);
      auto linkageName = node->linkageName() ? varSym->cname : nullptr;
      stmts = convertTypesToExtern(stmts, linkageName);

      // fix up convertedSyms since convertTypesToExtern
      // replaced the DefExpr/Symbol
      INT_ASSERT(stmts->body.last() && isDefExpr(stmts->body.last()));
      auto newDef = toDefExpr(stmts->body.last());
      noteConvertedSym(node, newDef->sym);*/
    }
  }

  return false;
}
void TConverter::exit(const Variable* node, RV& rv) {
  TC_DEBUGF(this, "exit variable %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Literal* node, RV& rv) {
  TC_DEBUGF(this, "enter literal %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));


  Expr* se = convertAstUntyped(node);
  cur.AList->insertAtTail(se);

  return false;
}
void TConverter::exit(const Literal* node, RV& rv) {
  TC_DEBUGF(this, "exit literal %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

// TODO: Figure out a way to merge this with 'convertRegularNamedCallOrNull'?
Expr* TConverter::convertParenlessCallOrNull(const Identifier* node, RV& rv) {
  auto re = rv.byAstOrNull(node);
  auto candidate = re->mostSpecific().only();
  auto sig = candidate ? candidate.fn() : nullptr;

  if (!candidate || !sig) return nullptr;

  // Fetch the resolved function and elide the call entirely if possible.
  const ResolvedFunction* rf = nullptr;
  if (Expr* elided = paramElideCallOrNull(sig, re->poiScope(), &rf)) {
    return elided;
  } else if (!rf) {
    return placeholder();
  }

  if (sig->isMethod()) {
    TC_UNIMPL("Handle parenless method call!");
    return placeholder();
  }

  auto calledFn = findOrConvertFunction(rf);
  auto ret = new CallExpr(calledFn);

  // TODO: Add an actual AST for a method receiver.
  std::vector<const AstNode*> actualAsts;

  // This mapping drives the conversion of actuals.
  auto fam = candidate.formalActualMap();
  INT_ASSERT(fam.isValid());

  convertAndInsertActuals(ret, node, actualAsts, sig, fam, rv);

  return ret;
}

bool TConverter::enter(const Identifier* node, RV& rv) {
  TC_DEBUGF(this, "enter identifier %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  // Cases for constants:
  if (node->name() == USTR("nil")) {
    cur.AList->insertAtTail(new SymExpr(gNil));
    return false;
  }

  // Cases requiring inspection of resolution results:
  if (auto re = rv.byAstOrNull(node)) {
    if (Expr* got = convertParenlessCallOrNull(node, rv)) {
      cur.AList->insertAtTail(got);
      return false;

    } else if (auto id = re->toId()) {
      Symbol* sym = findOrCreateSym(id, rv);
      SymExpr* se = new SymExpr(sym);
      cur.AList->insertAtTail(se);
      return false;

    } else if (node->name() == USTR("super")) {
      // TODO: Generalize this branch to be come "implicit field access".
      auto qt = re->type();
      auto ct = qt.type() ? qt.type()->getCompositeType() : nullptr;
      auto fn = cur.fnSymbol;
      if (ct && fn && fn->isMethod()) {
        if (auto at = toAggregateType(fn->getReceiverType())) {
          auto field = at->getField(node->name().c_str());

          // Fetch field by value since 'super' is a class reference.
          auto get = new CallExpr(PRIM_GET_MEMBER_VALUE,
                                  new SymExpr(fn->_this),
                                  new SymExpr(field));
          auto se = storeInTempIfNeeded(get, {});
          cur.AList->insertAtTail(se);
          return false;
        }
      }
    }
  }

  TC_UNIMPL("identifier case not handled");

  return false;
}
void TConverter::exit(const Identifier* node, RV& rv) {
  TC_DEBUGF(this, "exit identifier %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}


bool TConverter::enter(const Return* node, RV& rv) {
  TC_DEBUGF(this, "enter return %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  CallExpr* ret = new CallExpr(PRIM_RETURN);

  cur.AList->insertAtTail(ret);
  enterCallActuals(ret);

  return true;
}
void TConverter::exit(const Return* node, RV& rv) {
  TC_DEBUGF(this, "exit return %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  CallExpr* ret = exitCallActuals();

  // normalize returns to use the Return Value Variable (RVV)
  if (node->value() != nullptr) {
    Expr* retExpr = ret->get(1)->remove();
    CallExpr* move = nullptr;
    if (cur.fnSymbol->returnsRefOrConstRef()) {
      move = new CallExpr(PRIM_MOVE,
                          cur.retVar, new CallExpr(PRIM_ADDR_OF, retExpr));
    } else {
      move = new CallExpr(PRIM_MOVE, cur.retVar, retExpr);
    }
    ret->insertBefore(move);
  }

  // replace with GOTO(epilogue)
  ret->replace(new GotoStmt(GOTO_RETURN, cur.epilogueLabel));
}

bool TConverter::enter(const Call* node, RV& rv) {
  TC_DEBUGF(this, "enter call %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  Expr* expr = nullptr;
  if (Expr* got = convertPrimCallOrNull(node, rv)) {
    expr = got;
  } else if (Expr* got = convertNewCallOrNull(node, rv)) {
    expr = got;
  } else if (Expr* got = convertSpecialNamedCallOrNull(node, rv)) {
    expr = got;
  } else if (Expr* got = convertRegularNamedCallOrNull(node, rv)) {
    expr = got;
  } else {
    TC_UNIMPL("Unhandled type of call");
    return false;
  }

  if (expr) {
    cur.AList->insertAtTail(expr);
  } else {
    // Insert a dummy value just to keep the converter happy.
    cur.AList->insertAtTail(new SymExpr(gFalse));
  }

  return false;
}

void TConverter::exit(const Call* node, RV& rv) {
  TC_DEBUGF(this, "exit call %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

bool TConverter::enter(const Conditional* node, RV& rv) {
  TC_DEBUGF(this, "enter conditional %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));

  auto condRE = rv.byAst(node->condition());
  if (condRE.type().isParamTrue()) {
    // Don't need to process the false branch.
    auto block = pushNewBlock();
    node->thenBlock()->traverse(rv);
    popBlock();
    cur.AList->insertAtTail(block);
    return false;
  } else if (condRE.type().isParamFalse()) {
    if (auto elseBlock = node->elseBlock()) {
      auto block = pushNewBlock();
      elseBlock->traverse(rv);
      popBlock();
      cur.AList->insertAtTail(block);
    }
    return false;
  }

  // Not param-known condition; visit both branches as normal.

  if (node->isExpressionLevel()) {
    INT_ASSERT(node->elseBlock());
    INT_ASSERT(node->thenBlock()->numStmts() == 1);
    INT_ASSERT(node->elseBlock()->numStmts() == 1);

    auto cond = storeInTempIfNeeded(convertExpr(node->condition(), rv), {});

    // Temp stores the result of the if expression.
    // auto temp = makeNewTemp(rv.byAst(node).type());
    auto temp = newTemp();
    cur.AList->insertAtTail(new DefExpr(temp));

    // TODO: Insert conversion if necessary?
    auto thenExpr = convertExpr(node->thenBlock()->stmt(0), rv);
    auto thenMove = new CallExpr(PRIM_MOVE, new SymExpr(temp), thenExpr);
    auto thenBlock = new BlockStmt(thenMove);

    // TODO: Insert conversion if necessary?
    auto elseExpr = convertExpr(node->elseBlock()->stmt(0), rv);
    auto elseMove = new CallExpr(PRIM_MOVE, new SymExpr(temp), elseExpr);
    auto elseBlock = new BlockStmt(elseMove);

    // After normalize the 'IfExpr' is lowered to a 'CondStmt'.
    auto branch = new CondStmt(cond, thenBlock, elseBlock);
    cur.AList->insertAtTail(branch);

    // The result of the 'if-expr' will be stored in 'temp'.
    cur.AList->insertAtTail(new SymExpr(temp));
  } else {
    astlocMarker markAstLoc(node->id());

    Expr* cond = nullptr;
    VarSymbol* ifVarSym = nullptr;
    BlockStmt* thenBlock = nullptr;
    BlockStmt* elseBlock = nullptr;

    if (auto ifVar = node->condition()->toVariable()) {
      // If the condition is an 'if var', we convert it and stick the
      // variable into the 'then' block.
      INT_ASSERT(ifVar->kind() == uast::Variable::CONST ||
                 ifVar->kind() == uast::Variable::VAR);
      INT_ASSERT(ifVar->initExpression());

      uast::Variable::Kind symbolKind = ifVar->kind();
      types::QualifiedType qtInitExpr;
      auto initExpr = convertExpr(ifVar->initExpression(), rv, &qtInitExpr);

      // Generate a call to 'chpl_checkBorrowIfVar'.
      auto qtFalse = types::QualifiedType::createParamBool(context, false);
      auto ci = resolution::CallInfo(ustr("chpl_checkBorrowIfVar"),
                                     /*calledType*/ types::QualifiedType(),
                                     /*isMethodCall*/ false,
                                     /*hasQuestionArg*/ false,
                                     /*isParenless*/ false,
                                     { CallInfoActual(qtInitExpr),
                                       CallInfoActual(qtFalse) });

      auto checkFn = convertFunctionForGeneratedCall(std::move(ci), ifVar);

      // TODO: Need to make sure this temporary is destroyed, and not the
      // inner variable which just contains a '.borrow()' / the results of
      // the 'check...' call.
      auto initSymExpr = storeInTempIfNeeded(initExpr, {});

      // Generate the 'chpl_checkBorrowIfVar' call.
      auto checkCall = new CallExpr(checkFn, initSymExpr, new SymExpr(gFalse));

      // The borrow will be inserted into 'cur.AList'. It is the condition.
      cond = storeInTempIfNeeded(checkCall, {});

      // Now generate the 'if var' itself. Use 'findOrCreateVar' to ensure
      // that code in the 'if' will find it. The variable will be
      // initialized in the 'then' block.
      ifVarSym = findOrCreateVar(ifVar);

      // Set the variable's type.
      ifVarSym->type = cond->typeInfo();

      // Attach storage and attributes. This sets 'const' if needed.
      const bool setQual = true;
      attachSymbolStorage(symbolKind, ifVarSym, setQual);
      attachSymbolAttributes(context, ifVar, ifVarSym,
                             cur.moduleFromLibraryFile);
      attachSymbolVisibility(ifVar, ifVarSym);
    } else {
      cond = convertExpr(node->condition(), rv);
    }
    INT_ASSERT(cond);

    cond = storeInTempIfNeeded(cond, {});

    // if it's not a 'bool', emit a call to '_cond_test'.
    if (auto rr = rv.byAstOrNull(node->condition())) {
      types::QualifiedType qt = rr->type();
      if (!qt.isUnknown()) {
        if (!qt.type()->isBoolType()) {
          // emit a call to '_cond_test' and store the result in a temp
          auto ci = resolution::CallInfo(
                               ustr("_cond_test"),
                               /* calledType */ types::QualifiedType(),
                               /* isMethodCall */ false,
                               /* hasQuestionArg */ false,
                               /* isParenless */ false,
                               {CallInfoActual(qt)});
          FnSymbol* condFn = convertFunctionForGeneratedCall(ci, node);
          CallExpr* condCall = new CallExpr(condFn, cond);
          cond = storeInTempIfNeeded(condCall, {});
        }
      }
    }

    // convert the 'then' block
    {
      astlocMarker markAstLoc(node->thenBlock()->id());
      thenBlock = new BlockStmt();

      if (ifVarSym) {
        // Insert any 'if-var' declaration into the 'then' block.
        auto se = toSymExpr(cond);
        INT_ASSERT(se);

        thenBlock->insertAtTail(new DefExpr(ifVarSym));
        auto ifVarInitExpr = new CallExpr(PRIM_TO_NON_NILABLE_CLASS,
                                          new SymExpr(se->symbol()));
        auto ifVarInitMove = new CallExpr(PRIM_MOVE, new SymExpr(ifVarSym),
                                          ifVarInitExpr);
        thenBlock->insertAtTail(ifVarInitMove);
      }

      pushBlock(thenBlock);
      node->thenBlock()->traverse(rv);
      popBlock();
    }

    if (node->hasElseBlock()) {
      astlocMarker markAstLoc(node->elseBlock()->id());
      elseBlock = new BlockStmt();
      pushBlock(elseBlock);
      node->elseBlock()->traverse(rv);
      popBlock();
    }

    auto ret = new CondStmt(cond, thenBlock, elseBlock);
    cur.AList->insertAtTail(ret);
  }

  return false;
}

void TConverter::exit(const Conditional* node, RV& rv) {
  TC_DEBUGF(this, "exit conditional %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}


bool TConverter::enter(const AstNode* node, RV& rv) {
  TC_DEBUGF(this, "enter ast %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
  return true;
}
void TConverter::exit(const AstNode* node, RV& rv) {
  TC_DEBUGF(this, "exit ast %s %s\n", node->id().str().c_str(), asttags::tagToString(node->tag()));
}

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context) {
  return toOwned(new TConverter(context));
}
