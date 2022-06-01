/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/queries/global-strings.h"
#include "chpl/uast/all-uast.h"
#include <vector>

namespace {

using namespace chpl;
using namespace uast;

struct Visitor {
  std::set<UniqueString> exportedFnNames_;
  std::vector<const AstNode*> parents_;
  Context* context_;
  Builder& builder_;

  Visitor(Context* context, Builder& builder)
    : context_(context),
      builder_(builder) {
  }

  // Create and store an error in the builder (convenience overloads for
  // both errors and warnings below). This factory function is provided
  // because errors pinning on freshly parsed AST cannot be stored in
  // the context at this point.
  void report(const AstNode* node, ErrorMessage::Kind kind,
              const char* fmt,
              va_list vl);
  void error(const AstNode* node, const char* fmt, ...);
  void warn(const AstNode* node, const char* fmt, ...);

  // Get the i'th parent of the currently visited node. For example, the
  // call 'parent(0)' will return the most immediate parent.
  const AstNode* parent(int depth=0) const;

  // Get the number of parents.
  int numParents() const;

  // Search ancestors for the closest parent with a given tag. If found,
  // then 'last' will contain the penultimate parent in the path.
  const AstNode* searchParents(AstTag tag, const AstNode** last);

  // Search ancestors for the closest parent that is a decl.
  const AstNode* searchParentsForDecl(const AstNode** last);

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

  // Checks.
  void checkDomainTypeQueryUsage(const TypeQuery* node);
  void checkNoDuplicateNamedArguments(const FnCall* node);
  void checkNewClassDecorators(const FnCall* node);
  void checkExplicitDeinitCalls(const FnCall* node);
  void checkConstVarNoInit(const Variable* node);
  void checkConfigVar(const Variable* node);
  void checkExportVar(const Variable* node);
  void checkEmptyProcedureBody(const Function* node);
  void checkExternProcedure(const Function* node);
  void checkExportProcedure(const Function* node);
  void checkParenlessMethods(const Function* node);
  void checkProcedureFormalsAgainstRetType(const Function* node);
  void checkFunctionReturnsYields(const Function* node);
  void checkReturnHelper(const Return* node);
  void checkYieldHelper(const Yield* node);
  void checkImplicitModuleSameName(const Module* node);
  void checkIncludeModuleStrictName(const Module* node);
  void checkModuleReturnsYields(const Module* node);
  void checkPointlessUse(const Use* node);

  // Called in the visitor loop to check for duplicate exported names.
  void checkPrivateDecl(const Decl* node);
  void checkExportedName(const NamedDecl* node);

  // Warnings.
  void warnUnstableUnions(const Union* node);
  void warnUnstableLeadingUnderscores(const NamedDecl* node);

  // Visitors.
  void visit(const AstNode* node);
  void visit(const FnCall* node);
  void visit(const Variable* node);
  void visit(const TypeQuery* node);
};

void Visitor::report(const AstNode* node, ErrorMessage::Kind kind,
                     const char* fmt,
                     va_list vl) {
  auto loc = builder_.getLocation(node);
  auto err = ErrorMessage::vbuild(node->id(), loc, kind, fmt, vl);
  builder_.addError(std::move(err));
}

void Visitor::error(const AstNode* node, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  report(node, ErrorMessage::ERROR, fmt, vl);
}

void Visitor::warn(const AstNode* node, const char* fmt, ...) {
  va_list vl;
  va_start(vl, fmt);
  report(node, ErrorMessage::WARNING, fmt, vl);
}

const AstNode* Visitor::parent(int depth) const {
  assert(depth >= 0);
  if (depth >= parents_.size()) return nullptr;
  int idx = parents_.size() - depth - 1;
  assert(idx >= 0);
  auto ret = parents_[idx];
  return ret;
}

int Visitor::numParents() const {
  return parents_.size();
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
Visitor::searchParentsForDecl(const AstNode** last) {
  const AstNode* ret = nullptr;
  const AstNode* lastInWalk = nullptr;

  for (int i = parents_.size() - 1; i >= 0; i--) {
    auto ast = parents_[i];
    if (ast->isDecl()) {
      ret = ast;
      break;
    } else {
      lastInWalk = ast;
    }
  }

  if (last) *last = lastInWalk;
  return ret;
}

// Simple pre-order traversal. Also maintain a chain of parents.
void Visitor::check(const AstNode* node) {

  // First run blanket checks over superclass node types.
  if (auto decl = node->toDecl()) checkPrivateDecl(decl);
  if (auto named = node->toNamedDecl()) checkExportedName(named);

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
    error(node, "Domain query expressions may currently "
                "only be used in formal argument types");
  }
}

void Visitor::checkNoDuplicateNamedArguments(const FnCall* node) {
  std::set<UniqueString> actualNames;

  for (int i = 0; i < node->numActuals(); i++) {
    if (node->isNamedActual(i)) {
      auto name = node->actualName(i);
      assert(!name.isEmpty());

      if (!actualNames.insert(name).second) {
        auto actual = node->actual(i);
        assert(actual);
        error(actual, "The named argument '%s' is used more "
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
  } else {
    assert(false && "Not handled!");
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
void Visitor::checkNewClassDecorators(const FnCall* node) {
  auto calledExpr = node->calledExpression();
  if (!calledExpr) return;

  auto newExpr = calledExpr->toNew();
  if (!newExpr) return;

  auto outerMgt = newExpr->management();
  auto innerMgt = nestedExprManagementStyle(newExpr->typeExpression());
  auto nextCall = node->numActuals()
      ? node->actual(0)->toCall()
      : nullptr;

  while (innerMgt != New::DEFAULT_MANAGEMENT) {
    assert(outerMgt != New::DEFAULT_MANAGEMENT);

    error(node, "Type expression uses multiple class kinds: %s %s",
                New::managementToString(outerMgt),
                New::managementToString(innerMgt));

    // Cycle to the next nested call (if any).
    outerMgt = innerMgt;
    innerMgt = nestedExprManagementStyle(nextCall);
    nextCall = nextCall->numActuals()
        ? nextCall->actual(0)->toCall()
        : nullptr;
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
    error(node, "direct calls to deinit() are not allowed");
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
                  "explicitly initialized");
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

  assert(ret);
  return ret;
}

void Visitor::checkConfigVar(const Variable* node) {
  if (!node->isConfig()) return;

  bool doEmitError = true;

  if (parent(0) && parent(0)->isModule()) {
    doEmitError = false;
  } else if (parent(0)->isMultiDecl()) {
    if (parent(1) && parent(1)->isModule()) doEmitError = false;
  }

  if (doEmitError) {
    const char* varTypeStr = configVarStr(node->kind());
    assert(varTypeStr);

    error(node, "Configuration %s are allowed only at module scope",
                varTypeStr);
  }
}

void Visitor::checkExportVar(const Variable* node) {
  (void) node;
}

void Visitor::checkEmptyProcedureBody(const Function* node) {
  (void) node;
}

void Visitor::checkExternProcedure(const Function* node) {
  (void) node;
}

void Visitor::checkExportProcedure(const Function* node) {
  (void) node;
}

void Visitor::checkParenlessMethods(const Function* node) {
  (void) node;
}

void Visitor::checkProcedureFormalsAgainstRetType(const Function* node) {
  (void) node;
}

void Visitor::checkFunctionReturnsYields(const Function* node) {
  (void) node;
}

void Visitor::checkReturnHelper(const Return* node) {
  (void) node;
}

void Visitor::checkYieldHelper(const Yield* node) {
  (void) node;
}

void Visitor::checkImplicitModuleSameName(const Module* node) {
  (void) node;
}

void Visitor::checkIncludeModuleStrictName(const Module* node) {
  (void) node;
}

void Visitor::checkModuleReturnsYields(const Module* node) {
  (void) node;
}

void Visitor::checkPointlessUse(const Use* node) {
  (void) node;
}

void Visitor::checkPrivateDecl(const Decl* node) {
  if (node->visibility() != Decl::PRIVATE) return;

  if (node->isTypeDecl()) {
    error(node, "Can't apply private to types yet");
  }

  // Fetch the enclosing declaration. If we are top level then return.
  auto enclosingDecl = searchParentsForDecl(nullptr);
  if (!enclosingDecl) return;

  if (enclosingDecl->isFunction()) {
    warn(node, "Private declarations within function bodies are "
               "meaningless");

  } else if (enclosingDecl->isAggregateDecl() && !node->isTypeDecl()) {
    error(node, "Can't apply private to the fields or methods of a class "
                "or record yet");

  // TODO: Might need to adjust the order of the stuff in this branch.
  } else if (auto mod = enclosingDecl->toModule()) {
    if (auto fn = node->toFunction()) {
      if (fn->isMethod()) {
        error(node, "Can't apply private to the fields or methods of a "
                    "class or record yet");
      }
    } else if (parent(0)->isBlock() && !isParentFalseBlock(0)) {
      warn(node, "Private declarations within nested blocks are "
                 "meaningless");
    } else if (parent(0) != mod) {
      warn(node, "Private declarations are meaningless outside of module "
                 "level declarations");
    }
  }
}

void Visitor::checkExportedName(const NamedDecl* node) {
  (void) node;
}

void Visitor::warnUnstableUnions(const Union* node) {
  (void) node;
}

void Visitor::warnUnstableLeadingUnderscores(const NamedDecl* node) {
  (void) node;
}

// Do nothing.
void Visitor::visit(const AstNode* node) {}

void Visitor::visit(const FnCall* node) {
  checkNoDuplicateNamedArguments(node);
  checkNewClassDecorators(node);
  checkExplicitDeinitCalls(node);
}

void Visitor::visit(const Variable* node) {
  checkConstVarNoInit(node);
  checkConfigVar(node);
}

void Visitor::visit(const TypeQuery* node) {
  checkDomainTypeQueryUsage(node);
}

} // end anonymous namespace

namespace chpl {
namespace uast {

void Builder::postParseChecks() {
  auto visitor = Visitor(context_, *this);
  for (auto& ast : topLevelExpressions_) {
    if (ast->isComment()) continue;
    visitor.check(ast.get());
  }
}

} // end namespace uast
} // end namespace chpl

