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

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/global-strings.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/parsing/parser-error.h"
#include "chpl/uast/all-uast.h"
#include <vector>

namespace {

using namespace chpl;
using namespace uast;

/***
  TODO: Run the visitor while assigning IDs to avoid a second pass.
*/
struct Visitor {
  std::set<UniqueString> exportedFnNames_;
  std::vector<const AstNode*> parents_;
  Context* context_ = nullptr;
  Builder& builder_;
  bool isUserCode_ = false;

  // Helper to determine if a file path is for user code.
  static bool isUserFilePath(Context* context, UniqueString filepath);

  Visitor(Context* context, Builder& builder, bool isUserCode)
    : context_(context),
      builder_(builder),
      isUserCode_(isUserCode) {
  }

  // Create and store an error in the builder (convenience overloads for
  // both errors and warnings below). This factory function is provided
  // because errors pinning on freshly parsed AST cannot be stored in
  // the context at this point.
  void report(const AstNode* node, ErrorBase::Kind kind,
              const char* fmt,
              va_list vl);
  void error(const AstNode* node, const char* fmt, ...);
  void warn(const AstNode* node, const char* fmt, ...);

  // Return true if a given flag is set.
  bool isFlagSet(CompilerFlags::Name flag) const;

  // Return true if we are visiting user code.
  inline bool isUserCode() const { return isUserCode_; }

  // Get the i'th parent of the currently visited node. For example, the
  // call 'parent(0)' will return the most immediate parent.
  const AstNode* parent(int depth=0) const;

  // Search ancestors for the closest parent with a given tag. If found,
  // then 'last' will contain the penultimate parent in the path.
  const AstNode* searchParents(AstTag tag, const AstNode** last);

  // Search ancestors for the closest parent that is a decl.
  const AstNode* searchParentsForDecl(const AstNode** last, int declDepth=0);

  // Wrapper around 'node->dispatch'.
  void check(const AstNode* node);

  //
  // Returns 'true' if the parent at 'depth' is a false block that is
  // created only to attach scopes for resolution (it does not
  // actually exist in the source code).
  //
  // Note that today we exist in a weird state where many statements
  // do not actually have a block to hold their children. This means
  // the statement itself stores the children and a "block style"
  // (curly braces, do keyword, etc).
  //
  // TODO
  //
  // Future work should rework blocks entirely so that every statement
  // has a block containing children, and the block itself stores its
  // block style. In such a world we could just check to see if the
  // block uses explicit curly braces.
  //
  bool isParentFalseBlock(int depth=0) const;

  bool isNamedThisAndNotReceiverOrFunction(const NamedDecl* node);
  bool isNameReservedWord(const NamedDecl* node);

  // Checks.
  void checkDomainTypeQueryUsage(const TypeQuery* node);
  void checkNoDuplicateNamedArguments(const FnCall* node);
  bool handleNestedDecoratorsInNew(const FnCall* node);
  bool handleNestedDecoratorsInTypeConstructors(const FnCall* node);
  void checkForNestedClassDecorators(const FnCall* node);
  void checkExplicitDeinitCalls(const FnCall* node);
  void checkConstVarNoInit(const Variable* node);
  void checkConfigVar(const Variable* node);
  void checkExportVar(const Variable* node);
  void checkOperatorNameValidity(const Function* node);
  void checkEmptyProcedureBody(const Function* node);
  void checkExternProcedure(const Function* node);
  void checkExportProcedure(const Function* node);
  void checkProcedureRequiresParens(const Function* node);
  void checkOverrideNonMethod(const Function* node);
  void checkFormalsForTypeOrParamProcs(const Function* node);
  void checkNoReceiverClauseOnPrimaryMethod(const Function* node);
  void checkLambdaReturnIntent(const Function* node);
  void checkProcTypeFormalsAreAnnotated(const FunctionSignature* node);
  void checkProcDefFormalsAreNamed(const Function* node);

  /*
  TODO
  void checkProcedureFormalsAgainstRetType(const Function* node);
  void checkFunctionReturnsYields(const Function* node);
  void checkReturnHelper(const Return* node);
  void checkYieldHelper(const Yield* node);
  void checkImplicitModuleSameName(const Module* node);
  void checkIncludeModuleStrictName(const Module* node);
  void checkModuleReturnsYields(const Module* node);
  void checkPointlessUse(const Use* node);
  */

  // Called in the visitor loop to check against superclass types.
  void checkPrivateDecl(const Decl* node);
  void checkExportedName(const NamedDecl* node);
  void checkReservedSymbolName(const NamedDecl* node);
  void checkLinkageName(const NamedDecl* node);

  // Warnings.
  void warnUnstableUnions(const Union* node);
  void warnUnstableSymbolNames(const NamedDecl* node);

  // Visitors.
  inline void visit(const AstNode* node) {} // Do nothing by default.
  void visit(const FnCall* node);
  void visit(const Variable* node);
  void visit(const TypeQuery* node);
  void visit(const Function* node);
  void visit(const FunctionSignature* node);
  void visit(const Union* node);
};

// Note that even though we pass in the IDs for error messages here, the
// locations map is not actually populated for the user until after the
// builder wraps up and produces a builder result.
void Visitor::report(const AstNode* node, ErrorBase::Kind kind,
                     const char* fmt,
                     va_list vl) {
  auto err = GeneralError::vbuild(context_, kind, node->id(), fmt, vl);
  CHPL_POSTPARSE_REPORT_SIMPLE(builder_, err->message());
}

void Visitor::error(const AstNode* node, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  report(node, ErrorBase::ERROR, fmt, vl);
}

void Visitor::warn(const AstNode* node, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  report(node, ErrorBase::WARNING, fmt, vl);
}

bool Visitor::isFlagSet(CompilerFlags::Name flag) const {
  return chpl::isCompilerFlagSet(context_, flag);
}

const AstNode* Visitor::parent(int depth) const {
  CHPL_ASSERT(depth >= 0);
  if (((size_t) depth) >= parents_.size()) return nullptr;
  int idx = parents_.size() - depth - 1;
  CHPL_ASSERT(idx >= 0);
  auto ret = parents_[idx];
  return ret;
}

const AstNode*
Visitor::searchParents(AstTag tag, const AstNode** last) {
  const AstNode* ret = nullptr;
  const AstNode* lastInWalk = nullptr;

  for (int i = parents_.size() - 1; i >= 0; i--) {
    auto ast = parents_[i];
    if (ast->tag() == tag) {
      ret = ast;
      break;
    } else {
      lastInWalk = ast;
    }
  }

  if (last) *last = lastInWalk;
  return ret;
}

const AstNode*
Visitor::searchParentsForDecl(const AstNode** last, int declDepth) {
  const AstNode* ret = nullptr;
  const AstNode* lastInWalk = nullptr;
  int countDeclDepth = 0;

  for (int i = parents_.size() - 1; i >= 0; i--) {
    auto ast = parents_[i];
    if (ast->isDecl()) {
      if (countDeclDepth >= declDepth) {
        ret = ast;
        break;
      }
      countDeclDepth++;
    }
    lastInWalk = ast;
  }

  if (last && ret) *last = lastInWalk;
  return ret;
}

// Simple pre-order traversal. Also maintain a chain of parents.
void Visitor::check(const AstNode* node) {

  // First run blanket checks over superclass node types.
  if (auto decl = node->toDecl()) checkPrivateDecl(decl);
  if (auto named = node->toNamedDecl()) {
    checkExportedName(named);
    checkReservedSymbolName(named);
    warnUnstableSymbolNames(named);
    checkLinkageName(named);
  }

  // Now run checks via visitor and recurse to children.
  node->dispatch<void>(*this);
  parents_.push_back(node);
  for (auto child : node->children()) check(child);
  parents_.pop_back();
}

// TODO: Adjust me to handle all the cases where false blocks occur.
bool Visitor::isParentFalseBlock(int depth) const {
  auto parent = this->parent(depth);
  auto grandparent = this->parent(depth + 1);
  if (!parent || !grandparent || !parent->isBlock()) return false;
  if (auto loop = grandparent->toLoop()) {
    return parent == loop->body();
  }
  return false;
}

void Visitor::checkDomainTypeQueryUsage(const TypeQuery* node) {
  if (!parent(0) || !parent(1)) return;

  // Only care about the form '[?d]', leave otherwise.
  auto dom = parent(0)->toDomain();
  auto bkt = parent(1)->toBracketLoop();
  if (!dom || !bkt || (dom != bkt->iterand())) return;

  const AstNode* lastInWalk = nullptr;
  bool doEmitError = true;

  // If we are descended from the formal's type expression, OK!
  if (auto foundFormal = searchParents(asttags::Formal, &lastInWalk)) {
    auto formal = foundFormal->toFormal();
    if (lastInWalk == formal->typeExpression()) doEmitError = false;
  }

  if (doEmitError) {
    error(node, "domain query expressions may currently "
                "only be used in formal argument types.");
  }
}

void Visitor::checkNoDuplicateNamedArguments(const FnCall* node) {
  std::set<UniqueString> actualNames;

  for (int i = 0; i < node->numActuals(); i++) {
    if (node->isNamedActual(i)) {
      auto name = node->actualName(i);
      CHPL_ASSERT(!name.isEmpty());

      if (!actualNames.insert(name).second) {
        auto actual = node->actual(i);
        CHPL_ASSERT(actual);
        error(actual, "the named argument '%s' is used more "
                      "than once in the same function call.",
                      name.c_str());
      }
    }
  }
}

static
New::Management nestedExprManagementStyle(const AstNode* node) {
  if (!node) return New::DEFAULT_MANAGEMENT;

  auto ret = New::DEFAULT_MANAGEMENT;

  if (auto call = node->toCall()) {
    if (auto calledExpr = call->calledExpression()) {
      if (auto ident = calledExpr->toIdentifier()) {
        ret = New::stringToManagement(ident->name());
      }
    }
  } else if (auto ident = node->toIdentifier()) {
    ret = New::stringToManagement(ident->name());
  }

  return ret;
}

//
// For the expression 'new owned shared borrowed unmanaged C()', three
// errors will be emitted, one for 'owned shared', another for 'shared
// borrowed', and lastly for 'borrowed unmanaged'.
//
// TODO: Just generate a single error?
//
// The AST structure for such an expression looks like:
//
// test2@8 FnCall
// test2@1  New
// test2@0    Identifier shared
// test2@7  FnCall
// test2@2    Identifier borrowed
// test2@6    FnCall
// test2@3      Identifier unmanaged
// test2@5      FnCall
// test2@4        Identifier C
//
// That is, the first duplicate management is stored in the type
// expression of the 'New' expression, and all subsequent duplicates are
// nested in a chain starting from the first actual.
//
// Adjust this comment if our parsing strategy changes!
//
bool Visitor::handleNestedDecoratorsInNew(const FnCall* node) {
  auto calledExpr = node->calledExpression();
  if (!calledExpr) return false;

  auto newExpr = calledExpr->toNew();
  if (!newExpr) return false;

  const auto defMgt = New::DEFAULT_MANAGEMENT;
  auto outerMgt = newExpr->management();
  auto innerMgt = nestedExprManagementStyle(newExpr->typeExpression());

  // No collision if the outer management is empty/default.
  if (outerMgt == defMgt) return true;

  const AstNode* outerPin = newExpr;
  const AstNode* innerPin = newExpr->typeExpression();
  auto nextCall = node->numActuals()
      ? node->actual(0)->toCall()
      : nullptr;

  while (innerMgt != defMgt) {
    CHPL_ASSERT(outerMgt != defMgt);

    // TODO: Also error about 'please use class? instead of %s?'...
    error(outerPin, "type expression uses multiple class kinds: %s %s.",
                    New::managementToString(outerMgt),
                    New::managementToString(innerMgt));

    // Cycle _once_, to try and catch something like 'new owned owned'.
    // Note that if a third pair of duplicate decorators exists, then
    // the check for type constructors will handle that. This case has
    // to be handled because the second decorator is stored in the
    // new expression (a bit awkward).
    outerPin = innerPin;
    innerPin = nextCall;
    outerMgt = innerMgt;
    innerMgt = nextCall ? nestedExprManagementStyle(nextCall) : defMgt;
    nextCall = nullptr;
  }

  return true;
}

bool
Visitor::handleNestedDecoratorsInTypeConstructors(const FnCall* node) {
  if (node->numActuals() != 1) return false;
  if (!node->calledExpression()) return false;

  const auto defMgt = New::DEFAULT_MANAGEMENT;
  auto outerMgt = nestedExprManagementStyle(node);
  auto innerMgt = nestedExprManagementStyle(node->actual(0));
  if (outerMgt == defMgt) return false;

  // Do not loop, as recursion will handle other decorator pairs.
  if (innerMgt != defMgt) {
    CHPL_ASSERT(outerMgt != defMgt);

    // TODO: Also error about 'please use class? instead of %s?'...
    error(node, "type expression uses multiple class kinds: %s %s.",
               New::managementToString(outerMgt),
               New::managementToString(innerMgt));
  }

  return true;
}

void Visitor::checkForNestedClassDecorators(const FnCall* node) {
  if (!handleNestedDecoratorsInNew(node)) {
    std::ignore = handleNestedDecoratorsInTypeConstructors(node);
  }
}

void Visitor::checkExplicitDeinitCalls(const FnCall* node) {
  auto calledExpr = node->calledExpression();
  if (!calledExpr) return;

  bool doEmitError = false;

  if (auto ident = calledExpr->toIdentifier()) {
    doEmitError = ident->name() == USTR("deinit");
  } else if (auto dot = calledExpr->toDot()) {
    doEmitError = dot->field() == USTR("deinit");
  }

  // Check if we are being called from the delete implementation.
  if (doEmitError) {
    if (auto foundFn = searchParents(asttags::Function, nullptr)) {
      auto fn = foundFn->toFunction();
      if (fn->name() == "chpl__delete") doEmitError = false;
    }
  }

  if (doEmitError) {
    error(node, "direct calls to deinit() are not allowed.");
  }
}

// TODO: Extend to all 'VarLikeDecl' instead of just variables?
void Visitor::checkConstVarNoInit(const Variable* node) {
  if (!node->initExpression()) return;

  if (node->kind() != Variable::CONST_REF &&
      node->kind() != Variable::CONST) {
    return;
  }

  if (auto ident = node->initExpression()->toIdentifier()) {
    if (ident->name() == USTR("noinit")) {
      error(node, "const variables specified with noinit must be "
                  "explicitly initialized.");
    }
  }
}

static const char* configVarStr(Variable::Kind kind) {
  const char* ret = nullptr;
  switch (kind) {
    case Variable::PARAM:
      ret = "parameters";
      break;
    case Variable::CONST_REF:
    case Variable::CONST:
      ret = "constants";
      break;
    case Variable::TYPE:  // TODO: Add a case for types?
    default:
      ret = "variables";
      break;
  }

  CHPL_ASSERT(ret);
  return ret;
}

void Visitor::checkConfigVar(const Variable* node) {
  if (!node->isConfig()) return;

  bool doEmitError = true;
  if (parent(0) && parent(0)->isModule()) {
    doEmitError = false;
  } else if (parent(0)->isMultiDecl() || parent(0)->isTupleDecl()) {

    // Find first non tuple/multi decl...
    for (int i = 0; ((size_t) i) < parents_.size(); i++) {
      auto up = parent(i);
      if (up->isMultiDecl() || up->isTupleDecl()) continue;
      if (up->isModule()) doEmitError = false;
      break;
    }
  }

  if (doEmitError) {
    const char* varTypeStr = configVarStr(node->kind());
    CHPL_ASSERT(varTypeStr);

    error(node, "configuration %s are allowed only at module scope.",
                varTypeStr);
  }
}

void Visitor::checkExportVar(const Variable* node) {
  if (node->linkage() == Decl::EXPORT) {
    error(node, "export variables are not yet supported.");
  }
}

void Visitor::checkOperatorNameValidity(const Function* node) {
  if (node->kind() == Function::Kind::OPERATOR) {
    // operators must have valid operator names
    if (!isOpName(node->name())) {
      error(node, "'%s' is not a legal operator name.", node->name().c_str());
    }
  } else {
    // functions with operator names must be declared as operators
    if (isOpName(node->name())) {
      error(node, "operators cannot be declared without the operator keyword.",
            node->name().c_str());
    }
  }
}

void Visitor::checkEmptyProcedureBody(const Function* node) {
  if (!node->body() && node->linkage() != Decl::EXTERN) {
    auto decl = searchParentsForDecl(nullptr);
    if (!decl || !decl->isInterface()) {
      error(node, "no-op procedures are only legal for extern functions.");
    }
  }
}

void Visitor::checkExternProcedure(const Function* node) {
  if (node->linkage() != Decl::EXTERN) return;

  if (node->body()) {
    error(node, "extern functions cannot have a body.");
  }

  if (node->throws()) {
    error(node, "extern functions cannot throw errors.");
  }

  if (node->kind() == Function::ITER) {
    error(node, "'iter' is not legal with 'extern'.");
  }
}

void Visitor::checkExportProcedure(const Function* node) {
  if (node->linkage() != Decl::EXPORT) return;

  if (node->whereClause()) {
    error(node, "exported functions cannot have where clauses.");
  }
}

// TODO: Should this be confirming that the function is a method?
void Visitor::checkProcedureRequiresParens(const Function* node) {
  if (node->name() == "this" && node->isParenless()) {
    error(node, "method 'this' must have parentheses.");
  }

  if (node->name() == "these" && node->isParenless()) {
    error(node, "method 'these' must have parentheses.");
  }
}

void Visitor::checkOverrideNonMethod(const Function* node) {
  if (!node->isMethod() && node->isOverride()) {
    error(node, "'override' cannot be applied to non-method '%s'.",
                node->name().c_str());
  }
}

void Visitor::checkFormalsForTypeOrParamProcs(const Function* node) {
  if (node->returnIntent() != Function::PARAM &&
      node->returnIntent() != Function::TYPE) {
    return;
  }

  const char* returnIntentStr =
    node->returnIntent() == Function::TYPE ? "type" : "param";

  for (auto decl : node->formals()) {
    const char* formalIntentStr = nullptr;
    bool doEmitError = false;

    if (auto formal = decl->toFormal()) {
      if (formal->intent() == Formal::OUT ||
          formal->intent() == Formal::INOUT) {
        formalIntentStr = formal->intent() == Formal::OUT ? "out" : "inout";
        doEmitError = true;
      }
    }

    if (doEmitError) {
      CHPL_ASSERT(formalIntentStr);
      error(decl, "cannot use '%s' intent in a function returning "
                  "with '%s' intent.",
                  formalIntentStr,
                  returnIntentStr);
    }
  }
}

void Visitor::checkNoReceiverClauseOnPrimaryMethod(const Function* node) {
  // Currently, the uAST sets the primaryMethod field according to
  // whether or not there was a receiver type, so
  //   record R { proc R.bad() { } }
  // will have primaryMethod=false even though it is contained in the Record.

  if (const Formal* receiver = node->thisFormal()) {
    if (!node->isPrimaryMethod()) {
      const AstNode* last = nullptr;
      auto parentDecl = searchParentsForDecl(&last);
      if (parentDecl->isAggregateDecl()) {
        // stringify the receiver type uAST for use in the error message
        std::string receiverTypeStr = "<unknown>";
        if (auto receiverType = receiver->typeExpression()) {
          std::ostringstream ss;
          receiverType->stringify(ss, StringifyKind::CHPL_SYNTAX);
          receiverTypeStr = ss.str();
        }

        error(node, "type binding clauses ('%s.' in this case) are not "
                    "supported in declarations within a class, record "
                    "or union.",
                    receiverTypeStr.c_str());
      }
    }
  }
}

void Visitor::checkLambdaReturnIntent(const Function* node) {
  if (node->kind() != Function::LAMBDA) return;

  switch (node->returnIntent()) {
    case Function::CONST_REF:
    case Function::REF:
      error(node, "'ref' return types are not allowed in lambdas.");
      break;
    case Function::PARAM:
      error(node, "'param' return types are not allowed in lambdas.");
      break;
    case Function::TYPE:
      error(node, "'type' return types are not allowed in lambdas.");
      break;
    default:
      break;
  }
}

void
Visitor::checkProcTypeFormalsAreAnnotated(const FunctionSignature* node) {
  bool isProcType = !parent() || !parent()->isFunction();
  if (!isProcType) return;

  for (auto ast : node->formals())
    if (auto anon = ast->toAnonFormal())
      CHPL_REPORT(context_, ProcTypeUnannotatedFormal, node, anon);
}

void Visitor::checkProcDefFormalsAreNamed(const Function* node) {
  for (auto ast : node->formals()) {
    CHPL_ASSERT(!ast->isAnonFormal());

    // All procedure formals must have names.
    if (auto formal = ast->toFormal())
      if (formal->isExplicitlyAnonymous())
        CHPL_REPORT(context_, ProcDefExplicitAnonFormal, node, formal);
  }
}

void Visitor::checkPrivateDecl(const Decl* node) {
  if (node->visibility() != Decl::PRIVATE) return;

  if (node->isTypeDecl()) {
    error(node, "can't apply private to types yet.");
    return;
  }

  if (auto var = node->toVariable()) {
    if (var->kind() == Variable::TYPE) {
      error(node, "can't apply private to types yet.");
      return;
    }
  }

  // Fetch the enclosing declaration. If we are top level then return.
  auto enclosingDecl = searchParentsForDecl(nullptr);
  if (!enclosingDecl) return;

  if (enclosingDecl->isFunction()) {
    warn(node, "private declarations within function bodies are "
               "meaningless.");

  } else if (enclosingDecl->isAggregateDecl() && !node->isTypeDecl()) {
    error(node, "can't apply private to the fields or methods of a class "
                "or record yet.");

  // TODO: Might need to adjust the order of the stuff in this branch.
  } else if (auto mod = enclosingDecl->toModule()) {
    if (auto fn = node->toFunction()) {
      if (fn->isMethod()) {
        error(node, "can't apply private to the fields or methods of a "
                    "class or record yet.");
      }

    } else if (parent(0)->isBlock() && !isParentFalseBlock(0)) {
      warn(node, "private declarations within nested blocks are "
                 "meaningless.");

    } else if (parent(0) != mod) {
      warn(node, "private declarations are meaningless outside of module "
                 "level declarations.");
    }
  }
}

// TODO: This is a global operation, where to store the state?
void Visitor::checkExportedName(const NamedDecl* node) {
  (void) node;
}

bool Visitor::isNamedThisAndNotReceiverOrFunction(const NamedDecl* node) {
  if (node->name() != USTR("this")) return false;
  if (node->isFunction()) return false;
  if (node->isFormal())
    if (auto decl = searchParentsForDecl(nullptr))
      if (auto fn = decl->toFunction())
        if (fn->thisFormal() == node)
          return false;
  return true;
}

bool Visitor::isNameReservedWord(const NamedDecl* node) {
  auto name = node->name();
  if (isNamedThisAndNotReceiverOrFunction(node)) return true;
  if (name == "none") return true;
  if (name == "false") return true;
  if (name == "true") return true;
  return false;
}

// TODO: Maybe migrate this to a common place?
static bool isNameReservedType(UniqueString name) {
  if (name == USTR("bool")      ||
      name == USTR("int")       ||
      name == USTR("uint")      ||
      name == USTR("real")      ||
      name == USTR("imag")      ||
      name == USTR("complex")   ||
      name == USTR("bytes")     ||
      name == USTR("string")    ||
      name == USTR("sync")      ||
      name == USTR("single")    ||
      name == USTR("owned")     ||
      name == USTR("shared")    ||
      name == USTR("borrowed")  ||
      name == USTR("unmanaged") ||
      name == USTR("domain")    ||
      name == USTR("index")     ||
      name == USTR("locale")    ||
      name == USTR("nothing")   ||
      name == USTR("void"))
    return true;
  return false;
}

// TODO: May have to restrict errors to fire only for functions/formals.
void Visitor::checkReservedSymbolName(const NamedDecl* node) {
  auto name = node->name();

  // TODO: Do we really want this sort of exception?
  if (auto mod = node->toModule())
    if (mod->kind() == Module::IMPLICIT)
      return;

  // TODO: Specialize warnings for reduce intents, see...
  // test/parallel/forall/checks/with-this.chpl
  if (node->isTaskVar()) return;

  if (isNameReservedWord(node)) {
    error(node, "attempt to redefine reserved word '%s'.", name.c_str());
  } else if (isNameReservedType(name)) {
    error(node, "attempt to redefine reserved type '%s'.", name.c_str());
  }
}

void Visitor::checkLinkageName(const NamedDecl* node) {
  auto linkageName = node->linkageName();
  if (!linkageName) return;

  CHPL_ASSERT(node->linkage() != Decl::DEFAULT_LINKAGE);

  // Functions accept complex expressions for their linkage name.
  if (node->isFunction()) return;

  if (!linkageName->isStringLiteral()) {
    error(linkageName, "the linkage name for '%s' must be a string literal.",
                       node->name().c_str());
  }
}

// TODO: This relies on the "warn unstable" flag that we do not have.
void Visitor::warnUnstableUnions(const Union* node) {
  if (!isFlagSet(CompilerFlags::WARN_UNSTABLE)) return;
  warn(node, "unions are currently unstable and are expected to change "
             "in ways that will break their current uses.");
}

void Visitor::warnUnstableSymbolNames(const NamedDecl* node) {
  if (!isFlagSet(CompilerFlags::WARN_UNSTABLE)) return;
  if (!isUserCode()) return;

  auto name = node->name();

  if (name.startsWith("_")) {
    warn(node, "symbol names with leading underscores (%s) are unstable.",
               name.c_str());
  }

  if (name.startsWith("chpl_")) {
    warn(node, "symbol names beginning with 'chpl_' (%s) are unstable.",
               name.c_str());
  }
}

void Visitor::visit(const FnCall* node) {
  checkNoDuplicateNamedArguments(node);
  checkForNestedClassDecorators(node);
  checkExplicitDeinitCalls(node);
}

void Visitor::visit(const Variable* node) {
  checkConstVarNoInit(node);
  checkConfigVar(node);
  checkExportVar(node);
}

void Visitor::visit(const TypeQuery* node) {
  checkDomainTypeQueryUsage(node);
}

void Visitor::visit(const Function* node) {
  checkOperatorNameValidity(node);
  checkEmptyProcedureBody(node);
  checkExternProcedure(node);
  checkExportProcedure(node);
  checkProcedureRequiresParens(node);
  checkOverrideNonMethod(node);
  checkFormalsForTypeOrParamProcs(node);
  checkNoReceiverClauseOnPrimaryMethod(node);
  checkLambdaReturnIntent(node);
  checkProcDefFormalsAreNamed(node);
}

void Visitor::visit(const FunctionSignature* node) {
  checkProcTypeFormalsAreAnnotated(node);
}

void Visitor::visit(const Union* node) {
  warnUnstableUnions(node);
}

// Duplicate the contents of 'idIsInBundledModule', while skipping the
// call to 'filePathForId', because at this point the `setFilePathForId`
// setter query may not have been run yet.
bool Visitor::isUserFilePath(Context* context, UniqueString filepath) {
  UniqueString modules = chpl::parsing::bundledModulePath(context);
  auto ret = filepath.startsWith(modules);
  return ret;
}

} // end anonymous namespace

namespace chpl {
namespace uast {

void Builder::postParseChecks() {
  if (topLevelExpressions_.size() == 0) return;

  bool isUserCode = Visitor::isUserFilePath(context_, filepath_);
  auto v = Visitor(context_, *this, isUserCode);

  for (auto& ast : topLevelExpressions_) {
    if (ast->isComment()) continue;
    v.check(ast.get());
  }
}

} // end namespace uast
} // end namespace chpl

