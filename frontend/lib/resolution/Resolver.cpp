/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "Resolver.h"

#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Qualifier.h"
#include "chpl/uast/all-uast.h"
#include "call-init-deinit.h"

#include "InitResolver.h"
#include "VarScopeVisitor.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

#include <bitset>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

namespace {
  struct IterDetails {
    // Iterators will be resolved for each bit set in descending order.
    // Resolution stops when the first iterator with a valid yield type
    // is found. TODO: Stop the moment we find a signature instead?
    //
    // If the 'LEADER_FOLLOWER' bit is set, then the 'FOLLOWER' bit will
    // be ignored, and the 'leaderYieldType' will be computed from the
    // resolved leader signature. Any provided value for 'leaderYieldType'
    // will be overwritten.
    enum Priority {
      NONE            = 0b0000,
      STANDALONE      = 0b0001,
      LEADER_FOLLOWER = 0b0010,
      SERIAL          = 0b1000,
    };

    // When an iter is resolved these pieces of the process will be stored.
    struct Pieces {
      const IteratorType* iterType = nullptr;
      TheseResolutionResult resolutionResult;
    };
    Pieces standalone;
    Pieces leader;
    Pieces follower;
    Pieces serial;

    // This is the iterator that resolution stopped at if it succeeded.
    // In that case, the type in 'idxType' will be set to the yield type
    // of this iterator.
    Priority succeededAt = NONE;

    // If the 'LEADER_FOLLOWER' bit was set, then this will always be the
    // yield type of the resolved leader iterator. Otherwise, it will be
    // the type the user provided.
    QualifiedType leaderYieldType;

    // This will be set to the yield type of the first iterator to succeed.
    QualifiedType idxType;

    Pieces& piecesForIterKind(const Function::IteratorKind kind) {
      switch (kind) {
        case Function::STANDALONE: return standalone;
        case Function::LEADER: return leader;
        case Function::FOLLOWER: return follower;
        case Function::SERIAL: return serial;
        default:
          CHPL_ASSERT(false && "shouldn't happen");
          return serial;
      }
    }
  };
}

class IterandComponent;

// Helper to resolve a specified iterator signature and its yield type.
static QualifiedType
resolveIterTypeWithTag(Resolver& rv,
                       IterDetails::Pieces& outIterPieces,
                       const IterandComponent& ic,
                       Function::IteratorKind iterKind,
                       const QualifiedType& followThisFormal);

// Resolve iterators according to the policy set in 'mask' (see the type
// 'IterDetails::Policy'). Resolution stops the moment an iterator is
// found with a usable yield type.
static IterDetails resolveNonZipExpression(Resolver& rv,
                                           const AstNode* astForErr,
                                           const AstNode* iterand,
                                           const QualifiedType& leaderYieldType,
                                           int mask);

Resolver::~Resolver() {
  if (didPushFrame) {
    CHPL_ASSERT(rc != &emptyResolutionContext);
    rc->popFrame(this);
  }
}

const PoiScope*
Resolver::poiScopeOrNull(Context* context,
                         const TypedFnSignature* sig,
                         const Scope* inScope,
                         const PoiScope* inPoiScope) {
  const PoiScope* ret = nullptr;
  for (auto up = sig; up; up = up->parentFn()) {
    if (up->instantiatedFrom()) {
      ret = pointOfInstantiationScope(context, inScope, inPoiScope);
      break;
    }
  }
  return ret;
}

static QualifiedType::Kind qualifiedTypeKindForId(Context* context, ID id) {
  if (parsing::idIsParenlessFunction(context, id))
    return QualifiedType::PARENLESS_FUNCTION;

  auto tag = parsing::idToTag(context, id);
  if (isFunction(tag)) {
    return QualifiedType::FUNCTION;
  } else if (isModule(tag) || isInclude(tag)) {
    return QualifiedType::MODULE;
  } else if (isTypeDecl(tag)) {
    return QualifiedType::TYPE;
  } else if (asttags::isEnumElement(tag)) {
    return QualifiedType::CONST_VAR;
  }

  return QualifiedType::UNKNOWN;
}

static QualifiedType::Kind
qualifiedTypeKindForDecl(Context* context, const NamedDecl* decl) {
  if (const VarLikeDecl* vd = decl->toVarLikeDecl()) {
    Qualifier storageKind = vd->storageKind();
    return storageKind;
  }

  QualifiedType::Kind ret = qualifiedTypeKindForId(context, decl->id());
  CHPL_ASSERT(ret != QualifiedType::UNKNOWN && "case not handled");
  return ret;
}

// This class can gather up the IDs of contained fields or formals
struct GatherFieldsOrFormals {
  std::set<ID> fieldOrFormals;

  bool enter(const NamedDecl* decl) {
    // visit type declarations
    // is it a field or a formal?
    bool isField = false;
    if (auto var = decl->toVariable())
      if (var->isField())
        isField = true;

    bool isFieldOrFormal = isField || decl->isFormal();

    if (isFieldOrFormal && decl->name() != USTR("this"))
      fieldOrFormals.insert(decl->id());

    return decl->isAggregateDecl() || decl->isFunction();
  }
  void exit(const NamedDecl* decl) { }

  // go in to TupleDecl and MultiDecl
  bool enter(const TupleDecl* d) { return true; }
  void exit(const TupleDecl* d) { }
  bool enter(const MultiDecl* d) { return true; }
  void exit(const MultiDecl* d) { }

  // don't go in to anything else
  bool enter(const AstNode* ast) { return false; }
  void exit(const AstNode* ast) { }
};


Resolver::ParenlessOverloadInfo
Resolver::ParenlessOverloadInfo::fromMatchingIds(Context* context,
                                                 const MatchingIdsWithName& ids)
{
  bool anyMethodOrField = false;
  bool anyNonMethodOrField = false;

  for (auto idIt = ids.begin(); idIt != ids.end(); ++idIt) {
    if (idIt.curIdAndFlags().isMethodOrField()) {
      anyMethodOrField = true;
    } else {
      anyNonMethodOrField = true;
    }

    // TODO: should this check NOT_PARENFUL_FUNCTION ?
    if (!parsing::idIsParenlessFunction(context, idIt.curIdAndFlags().id())) {
      return {};
    }
  }
  return Resolver::ParenlessOverloadInfo(anyMethodOrField, anyNonMethodOrField);
}

Resolver
Resolver::createForModuleStmt(ResolutionContext* rc, const Module* mod,
                              const AstNode* modStmt,
                              ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(rc->context(), mod, byId, nullptr);
  ret.curStmt = modStmt;
  ret.byPostorder.setupForSymbol(mod);
  ret.rc = rc;
  rc->pushFrame(&ret, ResolutionContext::Frame::MODULE);
  ret.didPushFrame = true;
  return ret;
}

Resolver
Resolver::createForInterfaceStmt(ResolutionContext* rc,
                                 const uast::Interface* interface,
                                 const types::InterfaceType* ift,
                                 const ImplementationWitness* witness,
                                 const uast::AstNode* stmt,
                                 ResolutionResultByPostorderID& byPostorder) {
  const AstNode* symbol = interface;
  const Block* fnBody = nullptr;
  if (auto fn = stmt->toFunction()) {
    symbol = fn;
    fnBody = fn->body();
  }

  auto ret = Resolver(rc->context(), symbol, byPostorder, nullptr);
  ret.curStmt = stmt;
  ret.byPostorder.setupForSymbol(symbol);
  ret.rc = rc;
  ret.signatureOnly = true;
  ret.fnBody = fnBody;
  rc->pushFrame(ift, witness);
  ret.didPushFrame = true;
  return ret;
}

Resolver
Resolver::createForScopeResolvingModuleStmt(
                              Context* context, const Module* mod,
                              const AstNode* modStmt,
                              ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, mod, byId, nullptr);
  ret.curStmt = modStmt;
  ret.byPostorder.setupForSymbol(mod);
  ret.scopeResolveOnly = true;
  return ret;
}

Resolver
Resolver::createForInitialSignature(ResolutionContext* rc, const Function* fn,
                                    ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(rc->context(), fn, byId, nullptr);
  ret.signatureOnly = true;
  ret.fnBody = fn->body();
  ret.byPostorder.setupForSignature(fn);

  ret.rc = rc;

  if (fn->isMethod()) {
    fn->thisFormal()->traverse(ret);
    auto receiverType = ret.byPostorder.byAst(fn->thisFormal()).type();
    if (receiverType.hasTypePtr()) {
      if (auto ct = receiverType.type()->getCompositeType()) {
        ret.inCompositeType = ct;
      }
      ret.allowReceiverScopes = true;
    }
  }

  return ret;
}

Resolver
Resolver::createForInstantiatedSignature(Context* context,
                                         const Function* fn,
                                         const SubstitutionsMap& substitutions,
                                         const PoiScope* poiScope,
                                         ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, fn, byId, poiScope);
  ret.substitutions = &substitutions;
  ret.signatureOnly = true;
  ret.fnBody = fn->body();
  ret.byPostorder.setupForSignature(fn);

  if (fn->isMethod()) {
    ret.allowReceiverScopes = true;
  }

  return ret;
}

static void setFormalTypeUsingSignature(Resolver& rv, const TypedFnSignature* sig, int i) {
  const UntypedFnSignature* uSig = sig->untyped();
  const Decl* decl = uSig->formalDecl(i);
  const auto& qt = sig->formalType(i);

  ResolvedExpression& re = rv.byPostorder.byAst(decl);
  re.setType(qt);

  // TODO: Aren't these results already computed when we traverse formals
  // in resolution-queries?
  if (auto formal = decl->toFormal())
    rv.resolveTypeQueriesFromFormalType(formal, qt);
  if (auto formal = decl->toVarArgFormal())
    rv.resolveTypeQueriesFromFormalType(formal, qt);
  if (auto td = decl->toTupleDecl())
    rv.resolveTupleUnpackDecl(td, qt);
}

static void setFormalTypesUsingSignature(Resolver& rv) {
  // set the resolution results for the formals according to
  // the typedFnSignature
  const TypedFnSignature* sig = rv.typedSignature;
  int nFormals = rv.typedSignature->numFormals();
  for (int i = 0; i < nFormals; i++) {
    setFormalTypeUsingSignature(rv, sig, i);
  }
}

Resolver
Resolver::createForFunction(ResolutionContext* rc,
                            const Function* fn,
                            const PoiScope* poiScope,
                            const TypedFnSignature* typedFnSignature,
                            ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(rc->context(), fn, byId, poiScope);
  ret.typedSignature = typedFnSignature;
  ret.signatureOnly = false;
  ret.fnBody = fn->body();
  ret.rc = rc;

  // TODO: Stop copying these back in.
  ret.outerVariables = typedFnSignature->outerVariables();

  // Push a 'ResolutionContext::Frame' for the function we are resolving.
  ret.rc->pushFrame(&ret, ResolutionContext::Frame::FUNCTION);
  ret.didPushFrame = true;

  CHPL_ASSERT(typedFnSignature);
  CHPL_ASSERT(typedFnSignature->untyped());

  ret.byPostorder.setupForFunction(fn);

  // First, set the formal types using the types in the signature.
  setFormalTypesUsingSignature(ret);

  // Then, re-resolve the formals to set e.g., init-exprs.
  int nFormals = ret.typedSignature->numFormals();
  for (int i = 0; i < nFormals; i++) {
    const Decl* decl = ret.typedSignature->untyped()->formalDecl(i);
    CHPL_ASSERT(decl);
    decl->traverse(ret);

    // Set the formal types again since we already know what they are
    // from the 'TypedFnSignature', and they may have changed when
    // they were resolved again (the real process of resolving formals
    // in e.g., 'instantiateSignature' is much more complex and one
    // traversal will not produce the same results).
    //
    // Do this incrementally so that full formal type info is available to
    // subsequent formals.
    //
    // TODO: Ideally we preserve the types of formal sub-expressions by
    // some other means or make it so that re-resolving them in this
    // manner does not emit errors (perhaps we set a bool flag in the
    // Resolver to indicate as such).
    setFormalTypeUsingSignature(ret, ret.typedSignature, i);
  }

  if (typedFnSignature->isMethod()) {
    // allow computing the receiver scope from the typedSignature.
    ret.allowReceiverScopes = true;
  }

  return ret;
}

Resolver
Resolver::createForInitializer(ResolutionContext* rc,
                               const uast::Function* fn,
                               const PoiScope* poiScope,
                               const TypedFnSignature* typedFnSignature,
                               ResolutionResultByPostorderID& byPostorder) {
  auto ret = createForFunction(rc, fn, poiScope, typedFnSignature,
                               byPostorder);
  ret.initResolver = InitResolver::create(rc->context(), ret, fn);
  return ret;
}

Resolver
Resolver::createForScopeResolvingFunction(Context* context,
                                          const Function* fn,
                                          ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, fn, byId, nullptr);
  ret.typedSignature = nullptr; // re-set below
  ret.signatureOnly = true; // re-set below
  ret.scopeResolveOnly = true;
  ret.fnBody = fn->body();

  ret.byPostorder.setupForFunction(fn);

  // scope-resolve the formal types but not the body, yet
  // (particularly relevant for computing the method receiver type
  //  if it is an identifier)
  fn->traverse(ret);

  // copy the formal types to create a TypedFnSignature
  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, fn->id());
  std::vector<QualifiedType> formalTypes = ret.getFormalTypes(fn);
  auto whereTbd = TypedFnSignature::WhereClauseResult::WHERE_TBD;
  const TypedFnSignature* sig =
    TypedFnSignature::get(context, uSig,
                          std::move(formalTypes),
                          whereTbd,
                          /* needsInstantiation */ false,
                          /* instantiatedFrom */ nullptr,
                          /* parentFn */ nullptr,
                          /* formalsInstantiated */ Bitmap(),
                          /* outerVariables */ ret.outerVariables);

  ret.typedSignature = sig;
  ret.signatureOnly = false;

  CHPL_ASSERT(sig);
  CHPL_ASSERT(sig->untyped());

  // set the resolution results for the formals according to
  // the typedFnSignature (which just has UnknownType in it for all args
  // here)
  int nFormals = sig->numFormals();
  for (int i = 0; i < nFormals; i++) {
    const Decl* decl = uSig->formalDecl(i);
    const auto& qt = sig->formalType(i);

    ResolvedExpression& r = ret.byPostorder.byAst(decl);
    r.setType(qt);
  }

  return ret;
}

Resolver
Resolver::createForScopeResolvingField(Context* context,
                                 const uast::AggregateDecl* ad,
                                 const AstNode* fieldStmt,
                                 ResolutionResultByPostorderID& byPostorder) {
  auto ret = Resolver(context, ad, byPostorder, nullptr);
  ret.scopeResolveOnly = true;
  ret.curStmt = fieldStmt;
  ret.byPostorder.setupForSymbol(ad);

  return ret;
}

Resolver
Resolver::createForScopeResolvingEnumConstant(Context* context,
                                       const uast::Enum* ed,
                                       const uast::AstNode* fieldStmt,
                                       ResolutionResultByPostorderID& byPostorder) {
  auto ret = Resolver(context, ed, byPostorder, nullptr);
  ret.scopeResolveOnly = true;
  ret.curStmt = fieldStmt;
  ret.byPostorder.setupForSymbol(ed);

  return ret;
}


// set up Resolver to initially resolve field declaration types
Resolver
Resolver::createForInitialFieldStmt(ResolutionContext* rc,
                                    const AggregateDecl* decl,
                                    const AstNode* fieldStmt,
                                    const CompositeType* compositeType,
                                    ResolutionResultByPostorderID& byId,
                                    DefaultsPolicy defaultsPolicy) {
  auto ret = Resolver(rc->context(), decl, byId, nullptr);
  ret.rc = rc;
  ret.curStmt = fieldStmt;
  ret.inCompositeType = compositeType;
  ret.defaultsPolicy = defaultsPolicy;
  ret.byPostorder.setupForSymbol(decl);
  ret.fieldTypesOnly = true;
  ret.allowReceiverScopes = true;
  return ret;
}

// set up Resolver to resolve instantiated field declaration types
Resolver
Resolver::createForInstantiatedFieldStmt(Context* context,
                                         const AggregateDecl* decl,
                                         const AstNode* fieldStmt,
                                         const CompositeType* compositeType,
                                         const PoiScope* poiScope,
                                         ResolutionResultByPostorderID& byId,
                                         DefaultsPolicy defaultsPolicy) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.curStmt = fieldStmt;
  ret.inCompositeType = compositeType;
  ret.substitutions = &compositeType->substitutions();
  ret.defaultsPolicy = defaultsPolicy;
  ret.byPostorder.setupForSymbol(decl);
  ret.fieldTypesOnly = true;
  ret.allowReceiverScopes = true;
  return ret;
}

Resolver
Resolver::createForEnumElements(Context* context,
                                const uast::Enum* enumNode,
                                ResolutionResultByPostorderID& byPostorder) {
  auto ret = Resolver(context, enumNode, byPostorder, nullptr);
  ret.byPostorder.setupForSymbol(enumNode);
  return ret;
}

// set up Resolver to resolve instantiated field declaration types
// without knowing the CompositeType
Resolver
Resolver::createForInstantiatedSignatureFields(Context* context,
                                     const AggregateDecl* decl,
                                     const SubstitutionsMap& substitutions,
                                     const PoiScope* poiScope,
                                     ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.substitutions = &substitutions;
  ret.defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
  ret.byPostorder.setupForSymbol(decl);
  ret.fieldTypesOnly = true;
  return ret;
}


// set up Resolver to resolve a parent class type expression
Resolver
Resolver::createForParentClass(Context* context,
                               const AggregateDecl* decl,
                               const AstNode* inheritExpr,
                               const SubstitutionsMap& substitutions,
                               const PoiScope* poiScope,
                               ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.curInheritanceExpr = inheritExpr;
  ret.substitutions = &substitutions;
  ret.defaultsPolicy = DefaultsPolicy::USE_DEFAULTS;
  ret.byPostorder.setupForSymbol(decl);
  return ret;
}

// set up Resolver to scope resolve a parent class type expression
Resolver
Resolver::createForParentClassScopeResolve(Context* context,
                                           const AggregateDecl* decl,
                                           const AstNode* inheritExpr,
                                           ResolutionResultByPostorderID& byId)
{
  auto ret = Resolver(context, decl, byId, /* poiScope */ nullptr);
  ret.curInheritanceExpr = inheritExpr;
  ret.defaultsPolicy = DefaultsPolicy::USE_DEFAULTS;
  ret.byPostorder.setupForSymbol(decl);
  ret.scopeResolveOnly = true;
  return ret;
}


// set up Resolver to resolve a param loop
Resolver
Resolver::paramLoopResolver(Resolver& parent,
                            const For* loop,
                            ResolutionResultByPostorderID& bodyResults) {
  auto ret = Resolver(parent.context, loop, bodyResults, parent.poiScope);
  ret.parentResolver = &parent;
  ret.declStack = parent.declStack;
  ret.byPostorder.setupForParamLoop(loop, parent.byPostorder);
  ret.typedSignature = parent.typedSignature;

  return ret;
}

const AstNode* Resolver::nearestCalledExpression() const {
  if (callNodeStack.empty()) return nullptr;

  return callNodeStack.back()->calledExpression();
}

void Resolver::setCompositeType(const CompositeType* ct) {
  CHPL_ASSERT(this->inCompositeType == nullptr);
  this->inCompositeType = ct;
}

std::vector<types::QualifiedType>
Resolver::getFormalTypes(const Function* fn) {
  std::vector<types::QualifiedType> formalTypes;
  for (auto formal : fn->formals()) {
    QualifiedType t = byPostorder.byAst(formal).type();
    // compute concrete intent
    bool isThis = false;
    bool isInit = fn->name() == USTR("init") || fn->name() == USTR("init=");
    if (auto namedDecl = formal->toNamedDecl()) {
      isThis = namedDecl->name() == USTR("this");
    }
    Qualifier intent = resolveIntent(t, isThis, isInit);
    if (auto attributes = formal->attributeGroup()) {
      if (attributes->hasPragma(PRAGMA_INTENT_REF_MAYBE_CONST_FORMAL)) {
        intent = Qualifier::REF_MAYBE_CONST;
      }
    }
    t = QualifiedType(intent, t.type(), t.param());

    formalTypes.push_back(std::move(t));
  }
  return formalTypes;
}

types::QualifiedType Resolver::typeErr(const uast::AstNode* ast,
                                       const char* msg)
{
  context->error(ast, "%s", msg);
  auto t = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
  return t;
}

static bool
isOuterVariable(Resolver& rv, const Identifier* ident, const ID& target) {
  if (!target || !ident || target.isSymbolDefiningScope()) return false;
  const ID& mention = ident->id();

  Context* context = rv.context;
  ID targetParentSymbolId = target.parentSymbolId(context);
  ID mentionParentSymbolId = mention.parentSymbolId(context);

  // Need both mention and target parent IDs to make a decision.
  if (!targetParentSymbolId || !mentionParentSymbolId) return false;

  // No match if the target's parent symbol is what we're resolving.
  if (rv.symbol && targetParentSymbolId == rv.symbol->id()) return false;

  // The mention and the target should not live in the same symbol.
  if (mentionParentSymbolId == targetParentSymbolId) return false;

  auto tag = parsing::idToTag(context, targetParentSymbolId);

  if (tag == asttags::Function || tag == asttags::Interface) return true;

  if (tag == asttags::Module) {
    // Module-scope variables are not considered outer-variables. However,
    // variables declared in a module initializer statement can be, e.g.,
    /**
      module M {
        // Nested block within a module.
        {
          var someVar = 42;
          proc f() { writeln(someVar); }
          f();
        }
      }
    */
    // Return 'false' if the module is not the most immediate parent AST.
    auto enclosingMutliDecl = parsing::idToContainingMultiDeclId(context, target);
    auto targetParentAstId = parsing::idToParentId(context, enclosingMutliDecl);
    return targetParentSymbolId != targetParentAstId;
  }

  if (tag == asttags::Class || tag == asttags::Record ||
      tag == asttags::Union) {
    // In this case the mention is of a field. It could possibly be an
    // outer variable, because we could be reading a field value from a
    // non-local receiver. E.g.,
    /*
      record r {
        type T;
        proc foo() {
          // Here, 'bar' is not a method and 'T' is read from the
          // implicit receiver of 'foo' (which can also be used as
          // a 'this' as well).
          proc bar(f: T) {}
          var x: T;
          bar(x);
        }
      }
    */
    ID& m = mentionParentSymbolId;
    bool isMentionInNested = parsing::idIsNestedFunction(context, m);
    bool isMentionInMethod = parsing::idIsMethod(context, m);
    bool isTargetField = parsing::idIsField(context, target);

    // Not sure what else it could be in this case...
    CHPL_ASSERT(isTargetField);

    if (isTargetField && isMentionInNested) {

      // If we aren't in a method, then the receiver has to be non-local.
      if (!isMentionInMethod) return true;

      // In a method, check if the type ID of the closest receiver matches
      // the parent of the target.
      //
      // TODO: Is it even legal to reference fields from two different
      // receivers? Shouldn't we run into a 'NestedClassFieldRef' error?
      // Also, if they were two different instantiations of the same
      // composite then this check would be insufficient.
      const bool allowNonLocal = false;
      if (auto receiverInfo = rv.closestMethodReceiverInfo(allowNonLocal)) {
        if (auto t = std::get<1>(*receiverInfo).type()) {
          if (auto ct = t->toCompositeType()) {
            return ct->id() != targetParentSymbolId;
          }
        }
      } else if (!rv.scopeResolveOnly) {
        CHPL_UNIMPL("detecting if field use in nested method is outer "
                    "variable without 'methodReceiverType()'");
        return false;
      }
    }
  }

  return false;
}

const ReceiverScopeHelper* Resolver::getMethodReceiverScopeHelper() {
  auto fn = symbol->toFunction();
  auto ad = symbol->toAggregateDecl();
  if (!fn && !ad && parentResolver) {
    return parentResolver->getMethodReceiverScopeHelper();
  }

  if (!allowReceiverScopes) {
    // can't use receiver scopes yet
    // (e.g. we are computing the type of 'this' & otherwise that would recurse)
    return nullptr;
  }

  // check to make sure the receiver scope helper is only computed once
  if (receiverScopesComputed) return receiverScopeHelper;
  receiverScopesComputed = true;
  receiverScopeHelper = nullptr;

  if (scopeResolveOnly) {
    receiverScopeHelper = &receiverScopeSimpleHelper;
    return receiverScopeHelper;
  }

  const bool allowNonLocal = true;
  if (auto idAndTypePtr = closestMethodReceiverInfo(allowNonLocal)) {
    auto helper = ReceiverScopeTypedHelper(std::get<0>(*idAndTypePtr),
                                           std::get<1>(*idAndTypePtr));
    receiverScopeTypedHelper = std::move(helper);
    receiverScopeHelper = &receiverScopeTypedHelper;
  } else if (ad) {
    // If we're in an aggregate decl, set up to resolve implicit 'this' in,
    // e.g., forwarding declarations.
    //
    // TODO: do something smarter when not scopeResolveOnly.
    receiverScopeHelper = &receiverScopeSimpleHelper;
  }

  return receiverScopeHelper;
}

const MethodLookupHelper* Resolver::getFieldDeclarationLookupHelper() {
  if (!allowReceiverScopes) {
    // can't use receiver scopes yet
    // (e.g. we are computing the parent class types)
    return nullptr;
  }

  // check to make sure the method lookup helper is only computed once
  if (methodHelperComputed) return methodLookupHelper;
  methodHelperComputed = true;
  methodLookupHelper = nullptr;

  if (symbol->isTypeDecl()) {
    // the case of being in a method will be handled
    // by getMethodReceiverScopeHelper. So this path
    // only handles when we are resolving a composite.

    if (scopeResolveOnly) {
      auto helper = ReceiverScopeSimpleHelper();
      ID typeId = symbol->id();
      methodLookupHelper = helper.methodLookupForTypeId(context, typeId);
      return methodLookupHelper;
    }

    if (inCompositeType) {
      // create a helper for the containing class/record type
      // to help with field/method access in field & forwarding declarations
      auto qt = methodReceiverType();
      auto helper = ReceiverScopeTypedHelper();
      methodLookupHelper = helper.methodLookupForType(context, qt);
    }
  }

  return methodLookupHelper;
}

ID Resolver::scopeResolveCompositeIdFromMethodReceiver() {
  // Use scope-resolver logic to compute the receiver scopes.
  // If receiver type is specified by a simple identifier, determine it.
  // For more complicated receiver types we cannot yet gather any information.

  ID methodId;
  const Formal* thisFormal = nullptr;
  if (typedSignature && typedSignature->untyped()->isMethod()) {
    thisFormal = typedSignature->untyped()->formalDecl(0)->toFormal();
    methodId = typedSignature->id();
  }

  if (thisFormal == nullptr) {
    // if there is no typed signature, fall back to computing receiver type
    // from an identifier
    if (auto func = this->symbol->toFunction()) {
      thisFormal = func->thisFormal();
      methodId = this->symbol->id();
    }
  }

  if (thisFormal) {
    auto type = thisFormal->typeExpression();
    if (type == nullptr) {
      // `this` formals of primary methods have no type expression. They
      // are, however, in primary methods, so the method's parent is the
      // aggregate type whose scope should be used.
      return parsing::idToParentId(context, methodId);
    } else if (auto ident = type->toIdentifier()) {

      // Otherwise, the type expr is scope resolved to the aggregate type.
      return byPostorder.byAst(ident).toId();
    }
  } else if (auto agg = symbol->toAggregateDecl()) {
    // Lacking any more specific information, use the containing aggregate ID
    // if available.
    //
    // TODO: when enabled for 'scopeResolveOnly' we must start issuing
    // errors for cyclic class hierarchies and an inability to find
    // parent class expressions.
    if (!scopeResolveOnly) {
      return agg->id();
    }
  }

  return {};
}

std::optional<std::tuple<ID, QualifiedType>>
Resolver::closestMethodReceiverInfo(bool allowNonLocal) {
  auto fn = symbol ? symbol->toFunction() : nullptr;
  auto sig = typedSignature;

  // If there is a signature, always prefer using it first.
  if (sig && sig->isMethod()) {
    return {{ sig->id(), sig->formalType(0) }};

  // Else, try to use the result from 'byPostorder'.
  } else if (fn && fn->isMethod()) {
    auto& qt = byPostorder.byAst(fn->thisFormal()).type();
    return {{ fn->id(), qt }};

  // Else, compute something from the 'inCompositeType'.
  } else if (inCompositeType != nullptr) {

    // TODO: do we want this to be more focused? It might compute
    // the receiver type in too many cases.
    const Type* t = inCompositeType;
    if (auto bct = t->toBasicClassType()) {
      auto b = ClassTypeDecorator::BORROWED_NONNIL;
      t = ClassType::get(context, bct, /* manager */ nullptr,
                         ClassTypeDecorator(b));
    }
    QualifiedType qt(QualifiedType::VAR, t);
    return {{ inCompositeType->id(), std::move(qt) }};

  // Finally, try to scope-resolve a containing aggregate.
  } else if (ID id = scopeResolveCompositeIdFromMethodReceiver()) {
    return {{ std::move(id), QualifiedType() }};
  }

  // If all the above failed, look for a non-local receiver if able.
  if (symbol && allowNonLocal) {

    // This predicate returns the first method stack frame that lexically
    // contains 'symbol'. Stack frames are not always lexically enclosing.
    auto pred = [&](const ResolutionContext::Frame& f) {
      auto sig = f.signature();
      return (sig && sig->isMethod() && sig->id().contains(symbol->id()));
    };

    // Look for a non-local method receiver we can use.
    if (auto f = rc->findFrameMatching(pred)) {
      return {{ f->signature()->id(), f->signature()->formalType(0) }};
    }
  }

  return {};
}

QualifiedType Resolver::methodReceiverType() {
  const bool allowNonLocal = true;
  if (auto parentIdAndType = closestMethodReceiverInfo(allowNonLocal)) {
    return std::get<1>(*parentIdAndType);
  }
  return {};
}

bool Resolver::isPotentialSuper(const Identifier* ident, QualifiedType* outType) {
  if (ident->name() == USTR("super")) {
    // TODO: Consider non-local receivers here?
    const bool allowNonLocal = false;
    if (auto tup = closestMethodReceiverInfo(allowNonLocal)) {
      *outType = std::get<1>(*tup);
      return true;
    }
  }
  return false;
}

bool Resolver::shouldUseUnknownTypeForGeneric(const ID& id) {
  if (signatureOnly && substitutions) {
    // We're computing a final instantiated signature. We'll be incrementally
    // traversing formals and accumulating type information. As a result,
    // we should use (partial, generic) types and not unknowns.
    return false;
  }

  // make sure the set of IDs for fields and formals is computed
  if (!fieldOrFormalsComputed) {
    auto visitor = GatherFieldsOrFormals();
    symbol->traverse(visitor);
    fieldOrFormals.swap(visitor.fieldOrFormals);

    // also compute instantiatedFieldOrFormals
    if (typedSignature != nullptr) {
      auto untyped = typedSignature->untyped();
      int nFormals = untyped->numFormals();
      for (int i = 0; i < nFormals; i++) {
        if (typedSignature->formalIsInstantiated(i)) {
          CHPL_ASSERT(!untyped->formalDecl(i)->id().isEmpty());
          instantiatedFieldOrFormals.insert(untyped->formalDecl(i)->id());
        }
      }
    }

    fieldOrFormalsComputed = true;
  }

  bool isFieldOrFormal = fieldOrFormals.count(id) > 0;
  bool isSubstituted = false;
  bool isFormalInstantiated = false;

  if (substitutions != nullptr) {
    auto search = substitutions->find(id);
    if (search != substitutions->end()) {
      isSubstituted = true;
    }
  }

  // check also instantiated formals from typedSignature
  if (isFieldOrFormal) {
    isFormalInstantiated = instantiatedFieldOrFormals.count(id) > 0;
  }

  return isFieldOrFormal && !isSubstituted && !isFormalInstantiated;
}

// is it a call to int / uint / etc?
static bool isCallToIntEtc(const AstNode* formalTypeExpr) {
  if (auto call = formalTypeExpr->toFnCall()) {
    if (auto calledAst = call->calledExpression()) {
      if (auto calledIdent = calledAst->toIdentifier()) {
        UniqueString n = calledIdent->name();
        if (n == USTR("int") || n == USTR("uint") || n == USTR("bool") ||
            n == USTR("real") || n == USTR("imag") || n == USTR("complex")) {
          return true;
        }
      }
    }
  }

  return false;
}

static bool isCallToPtr(const AstNode* formalTypeExpr) {
  if (auto call = formalTypeExpr->toFnCall()) {
    if (auto calledAst = call->calledExpression()) {
      if (auto calledIdent = calledAst->toIdentifier()) {
        UniqueString n = calledIdent->name();
        if (n == USTR("c_ptr") || n == USTR("c_ptrConst") ||
            n == USTR("_ddata")) {
          return true;
        }
      }
    }
  }

  return false;
}

// helper to gather bad actuals and report NoMatchingCandidates error
static void
handleRejectedCandidates(Resolver::CallResultWrapper& result,
                         std::vector<ApplicabilityResult>& rejected,
                         const std::vector<const uast::AstNode*>& actualAsts) {
  // By performing some processing in the resolver, we can issue a nicer error
  // explaining why each candidate was rejected.
  std::vector<const uast::VarLikeDecl*> actualDecls;
  actualDecls.resize(rejected.size());
  std::reverse(rejected.begin(), rejected.end());
  // check each rejected candidate for uninitialized actuals
  for (size_t i = 0; i < rejected.size(); i++) {
    auto &candidate = rejected[i];
    if (/* skip computing the formal-actual map because it will go poorly
           with an unknown formal. */
        !candidate.failedDueToWrongActual()) {
      continue;
    }
    auto fn = candidate.initialForErr();
    // TODO: this can assert if the fn was resolved but had erroneous type formals
    resolution::FormalActualMap fa(fn, *result.ci);
    auto& badPass = fa.byFormalIdx(candidate.formalIdx());
    const uast::AstNode *actualExpr = nullptr;
    const uast::VarLikeDecl *actualDecl = nullptr;
    size_t actualIdx = badPass.actualIdx();
    CHPL_ASSERT(0 <= actualIdx && actualIdx < actualAsts.size());
    actualExpr = actualAsts[badPass.actualIdx()];

    // look for a definition point of the actual for error reporting of
    // uninitialized vars typically in the case of bad split-initialization
    if (actualExpr && actualExpr->isIdentifier()) {
      auto &resolvedExpr = result.parent->byPostorder.byAst(actualExpr);
      if (auto id = resolvedExpr.toId()) {
        auto var = parsing::idToAst(result.parent->context, id);
        // should put a nullptr if not a VarLikeDecl
        actualDecl = var->toVarLikeDecl();
      }
    }
    actualDecls[i] = actualDecl;
  }
  CHPL_ASSERT(rejected.size() == actualDecls.size());
  result.reportError(result, rejected, actualDecls);
}

static void varArgTypeQueryError(Context* context,
                                 const AstNode* node,
                                 ResolvedExpression& result) {
  context->error(node, "Cannot query type of variable arguments formal when types are not homogeneous");
  auto errType = QualifiedType(QualifiedType::TYPE,
                               ErroneousType::get(context));
  result.setType(errType);
}

static std::vector<const TypeQuery*>
collectTypeQueriesIn(const AstNode* ast, bool recurse=true) {
  std::vector<const TypeQuery*> ret;

  auto func = [&](const AstNode* ast, auto& self) -> void {
    for (auto child : ast->children()) {
      if (auto tq = child->toTypeQuery()) ret.push_back(tq);
      if (recurse) self(child, self);
    }
  };

  func(ast, func);

  return ret;
}

// helper for resolveTypeQueriesFromFormalType
void Resolver::resolveTypeQueries(const AstNode* formalTypeExpr,
                                  const QualifiedType& actualType,
                                  bool isNonStarVarArg,
                                  bool isTopLevel) {

  auto actualTypePtr = actualType.type();
  // Give up if the type is nullptr or UnknownType or AnyType
  if (actualTypePtr == nullptr ||
      actualTypePtr->isUnknownType() ||
      actualTypePtr->isAnyType())
    return;

  CHPL_ASSERT(formalTypeExpr != nullptr);

  // Give up if typeExpr is an Identifier
  if (formalTypeExpr->isIdentifier())
    return;

  if (formalTypeExpr->isTypeQuery()) {
    ResolvedExpression& result = byPostorder.byAst(formalTypeExpr);

    if (isNonStarVarArg) {
      varArgTypeQueryError(context, formalTypeExpr, result);
    } else {
      // Set the type that we know (since it was passed in)
      if (isTopLevel) {
        // Queries at the top level only capture type. e.g., param x: ?t
        result.setType(QualifiedType(QualifiedType::TYPE, actualTypePtr));
      } else {
        result.setType(actualType);
      }
    }
  }

  // Make recursive calls as needed to handle any TypeQuery nodes
  // nested within typeExpr.
  if (auto call = formalTypeExpr->toFnCall()) {
    if (isCallToIntEtc(formalTypeExpr)) {
      // If it is e.g. int(TypeQuery), resolve the type query to the width
      // Set the type that we know (since it was passed in)
      if (call->numActuals() == 1) {
        if (auto tq = call->actual(0)->toTypeQuery()) {
          if (auto pt = actualTypePtr->toPrimitiveType()) {
            ResolvedExpression& resolvedWidth = byPostorder.byAst(tq);
            if (isNonStarVarArg) {
              varArgTypeQueryError(context, call->actual(0), resolvedWidth);
            } else {
              auto qt = QualifiedType::makeParamInt(context, pt->bitwidth());
              resolvedWidth.setType(qt);
            }
          }
        }
      }
    } else if (isCallToPtr(formalTypeExpr)) {
      // If it is e.g. c_ptr(TypeQuery), resolve the type query to the eltType
      // Set the type that we know (since it was passed in)
      if (call->numActuals() == 1) {
        if (auto tq = call->actual(0)->toTypeQuery()) {
          if (auto pt = actualTypePtr->toPtrType()) {
            ResolvedExpression& resolvedElt = byPostorder.byAst(tq);
            if (isNonStarVarArg) {
              varArgTypeQueryError(context, call->actual(0), resolvedElt);
            } else {
              resolvedElt.setType(QualifiedType(QualifiedType::TYPE,
                                                pt->eltType()));
            }
          }
        }
      }
    } else if (parsing::isCallToClassManager(call) &&
               call->numActuals() == 1 &&
               actualTypePtr->isClassType()) {
      // Strip the owned/shared/etc. for both the formal and the type
      // This works since the recursive case expects the actual type to
      // be a composite type, which manageable types are.

      auto classArg = call->actual(0);
      auto actualMt = actualTypePtr->toClassType()->manageableType();
      resolveTypeQueries(classArg, QualifiedType(actualType.kind(), actualMt),
                         isNonStarVarArg,
                         /* isTopLevel */ false);
    } else {
      auto typeQueries = collectTypeQueriesIn(call);

      // There are no type queries in the call, so there is nothing to do.
      if (typeQueries.empty()) return;

      // If we are a resolving a signature, then we have resolved the types
      // of the call's sub-expressions, so we can accurately judge whether
      // or not the call is to a type constructor.
      QualifiedType baseExprType;
      bool isCertainlyCallToTypeConstructor = false;
      if (this->signatureOnly) {
        if (auto expr = call->calledExpression()) {
          CHPL_ASSERT(byPostorder.hasId(expr->id()));

          baseExprType = byPostorder.byAst(expr).type();

          if (baseExprType.kind() == QualifiedType::TYPE &&
              (baseExprType.type() &&
               baseExprType.type()->getCompositeType())) {
            isCertainlyCallToTypeConstructor = true;
          }
        }
      }

      // If we know that the call is not a type constructor, then emit an
      // error about illegal type queries, and then give up. We know from
      // above that there's at least one '?w' type query in the call.
      //
      // TODO: Can emit more elaborate errors here.
      if (this->signatureOnly && !isCertainlyCallToTypeConstructor) {
        CHPL_ASSERT(!typeQueries.empty());
        context->error(typeQueries[0], "One or more type queries appeared "
                                       "outside of a type constructor call - "
                                       "here is the first one");
        return;
      }

      // From this point on, we are only working with the type of the entire
      // call. To explain why, consider the following code:
      //
      //          proc foo(x: helper(bool)) {}
      //
      // If 'signatureOnly == true', then we already would have returned if
      // 'helper' is not a type constructor. However, if '!signatureOnly',
      // then we are preparing to resolve a function's body and only have
      // access to the entire type of 'x'. So the only thing we can use to
      // judge if 'helper(bool)' is a type constructor is whether or not
      // 'helper(bool)' returns a 'CompositeType'.
      //
      // We already know from above that the call has to contain type queries,
      // and we can rely on previous resolution when 'signatureOnly == true'
      // to prevent bogus type queries like 'helper(?w)'.
      //
      // So just assume that 'helper(bool)' is a type constructor and fail
      // if 'typeConstructorInitial' returns 'nullptr'.

      // Try to convert the type of the call to a composite type.
      auto actualCt = actualTypePtr->getCompositeType();

      // If we're not working with a composite type, give up. There's some
      // sort of structural issue that we should have detected earlier.
      if (actualCt == nullptr) {
        CHPL_ASSERT(!this->signatureOnly);
        return;
      }

      // Make sure that 'actualCt' is instantiated. Only do this when we're
      // first evaluating the signature and not when we're "reloading" the
      // types of type queries.
      if (this->signatureOnly) {
        if (!actualCt->instantiatedFromCompositeType()) {
          context->error(formalTypeExpr, "Instantiated type expected");
          return;
        }
      }

      // TODO: need to implement type queries for domain type expressions
      if (actualCt->isDomainType()) return;

      auto baseCt = actualCt->instantiatedFromCompositeType();
      auto sig = typeConstructorInitial(context, baseCt);
      CHPL_ASSERT(sig);

      // Generate a simple CallInfo for the call
      auto callInfo = CallInfo::createUnknown(call);
      // generate a FormalActualMap
      auto faMap = FormalActualMap(sig, callInfo);

      // Now, consider the formals
      int nActuals = call->numActuals();
      for (int i = 0; i < nActuals; i++) {
        // ignore actuals like ?
        // since these aren't type queries & don't match a formal
        if (isQuestionMark(call->actual(i)))
          continue;

        const FormalActual* fa = faMap.byActualIdx(i);
        CHPL_ASSERT(fa != nullptr && fa->formal() != nullptr);

        // get the substitution for that field from the CompositeType
        // and recurse with the result to set types for nested TypeQuery nodes
        auto formal = fa->formal()->toNamedDecl();
        const SubstitutionsMap& subs = actualCt->substitutions();
        auto ad = parsing::idToAst(context, actualCt->id())->toAggregateDecl();
        auto field = findFieldByName(context, ad, actualCt, formal->name());
        auto search = subs.find(field->id());
        if (search != subs.end()) {
          QualifiedType fieldType = search->second;
          auto actual = call->actual(i);
          resolveTypeQueries(actual, fieldType,
                             isNonStarVarArg,
                             /* isTopLevel */ false);
        }
      }
    }
  }
}

void Resolver::resolveVarArgSizeQuery(const uast::VarArgFormal* varArgFormal,
                                      int numVarArgs) {
  if (auto countQuery = varArgFormal->count()) {
    if (countQuery->isTypeQuery()) {
      ResolvedExpression& result = byPostorder.byAst(countQuery);
      result.setType(QualifiedType::makeParamInt(context, numVarArgs));
    }
  }
}

void Resolver::resolveTypeQueriesFromFormalType(const VarLikeDecl* formal,
                                                QualifiedType formalType) {
  if (auto varargs = formal->toVarArgFormal()) {
    const TupleType* tuple = formalType.type()->toTupleType();

    // args...?n
    resolveVarArgSizeQuery(varargs, tuple->numElements());

    if (auto typeExpr = formal->typeExpression()) {
      QualifiedType useType = tuple->elementType(0);
      resolveTypeQueries(typeExpr, useType, !tuple->isStarTuple());
    }
  } else if (auto typeExpr = formal->typeExpression()) {
    resolveTypeQueries(typeExpr, formalType);
  }
}

bool Resolver::checkForKindError(const AstNode* typeForErr,
                                 const AstNode* initForErr,
                                 QualifiedType::Kind declKind,
                                 QualifiedType declaredType,
                                 QualifiedType initExprType) {
  // check that the resolution of the type expression is a type
  if (declaredType.hasTypePtr() &&
      declaredType.kind() != QualifiedType::UNKNOWN &&
      declaredType.kind() != QualifiedType::TYPE_QUERY) {
    if (declaredType.kind() != QualifiedType::TYPE) {
      CHPL_REPORT(context, ValueUsedAsType, typeForErr, declaredType);
      return true;
    }
  }

  // check that the init expression has compatible kind
  if (initExprType.hasTypePtr() &&
      !initExprType.type()->isUnknownType() &&
      initExprType.kind() != QualifiedType::UNKNOWN) {
    if (declKind == QualifiedType::TYPE &&
        initExprType.kind() != QualifiedType::TYPE) {
      CHPL_REPORT(context, IncompatibleKinds, declKind, initForErr, initExprType);
      return true;
    } else if (declKind != QualifiedType::TYPE &&
               initExprType.kind() == QualifiedType::TYPE) {
      CHPL_REPORT(context, IncompatibleKinds, declKind, initForErr, initExprType);
      return true;
    } else if (declKind == QualifiedType::PARAM &&
               initExprType.kind() != QualifiedType::PARAM) {
      CHPL_REPORT(context, IncompatibleKinds, declKind, initForErr, initExprType);
      return true;
    }
  }

  return false; // no error
}


static const CompositeType*
getTypeWithCustomInfer(Context* context, const Type* type) {
  if (auto rec = type->getCompositeType()) {
    if (rec->id().isEmpty() == false) {
      // TODO 2023-05: This might return null for a type like 'string' which
      // we force to exist whether the module is present or not. Is this
      // necessary in the long-term?
      if (const auto node = parsing::idToAst(context, rec->id())) {
        if (const auto attr = node->attributeGroup()) {
          if (attr->hasPragma(PRAGMA_INFER_CUSTOM_TYPE)) {
            return rec;
          }
        }
      }
    }
  }

  return nullptr;
}

const Type*
Resolver::computeCustomInferType(const AstNode* decl,
                                 const CompositeType* ct) {
  QualifiedType ret;
  auto name = UniqueString::get(context, "chpl__inferCopyType");
  QualifiedType calledType = QualifiedType(QualifiedType::CONST_REF, ct);
  auto receiver = CallInfoActual(calledType, USTR("this"));
  std::vector<CallInfoActual> actuals = {std::move(receiver)};

  auto ci = CallInfo(/* name */ name,
                     /* calledType */ calledType,
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     std::move(actuals));
  auto inScopes = CallScopeInfo::forNormalCall(scopeStack.back(), poiScope);
  auto rr = resolveGeneratedCall(nullptr, &ci, &inScopes);
  if (rr.result.mostSpecific().only()) {
    ret = rr.result.exprType();
    rr.noteResult(&byPostorder.byAst(decl),
                   { { AssociatedAction::INFER_TYPE, decl->id() } });
  } else {
    context->error(ct->id(), "'chpl__inferCopyType' is unimplemented");
  }

  return ret.type();
}

const Type* Resolver::computeChplCopyInit(const uast::AstNode* decl,
                                          const QualifiedType::Kind declKind,
                                          const types::QualifiedType& initExprT) {

  std::vector<CallInfoActual> actuals;
  actuals.emplace_back(initExprT, UniqueString());
  auto definedConst =
    QualifiedType::makeParamBool(context, isConstQualifier(declKind));
  actuals.emplace_back(std::move(definedConst), UniqueString());
  auto ci = CallInfo (/* name */ UniqueString::get(context, "chpl__initCopy"),
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ false,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      actuals);

  const Scope* scope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
  auto c = resolveGeneratedCall(decl, &ci, &inScopes);
  c.noteResultWithoutError(&byPostorder.byAst(decl),
                            {{ AssociatedAction::CUSTOM_COPY_INIT, decl->id() }});

  if (!c.result.exprType().isUnknownOrErroneous()) {
    return c.result.exprType().type();
  }

  return nullptr;
}

static const Type* tryFindTypeViaCopyFn(Resolver& resolver,
                                        const AstNode* declForErr,
                                        const AstNode* typeForErr,
                                        const AstNode* initForErr,
                                        const QualifiedType& declaredType,
                                        const QualifiedType& initExprType) {
  const bool tryResolveCopyInit = (declaredType.type()->isRecordType() ||
                                   declaredType.type()->isArrayType() ||
                                   declaredType.type()->isDomainType() ||
                                   declaredType.type()->isUnionType());

  if (tryResolveCopyInit) {
    // Note: This code assumes that this init= will be added as an
    // associated action by ``CallInitDeinit::resolveCopyInit``
    std::vector<const AstNode*> ignoredAsts;
    auto [ci, inScopes] = setupCallForCopyOrMove(resolver, declForErr, initForErr,
                                                 declaredType, initExprType,
                                                 /* forMoveInit */ false,
                                                 ignoredAsts);
    auto c = resolver.resolveGeneratedCall(declForErr, &ci, &inScopes);

    if (c.result.mostSpecific().only()) {
      // For init=, use the receiver type of the function as the new type.
      if (ci.name() == USTR("init=")) {
        return c.result.mostSpecific().only().fn()->formalType(0).type();

      // Otherwise, we resolved some other copy function, use its return type
      } else if (!c.result.exprType().isUnknownOrErroneous()) {
        return c.result.exprType().type();
      }
    }
  }

  // No cigar, issue an error and return ErroneousType
  CHPL_REPORT(resolver.context, IncompatibleTypeAndInit, declForErr, typeForErr,
              initForErr, declaredType.type(), initExprType.type());
  return ErroneousType::get(resolver.context);
}

QualifiedType Resolver::getTypeForDecl(const AstNode* declForErr,
                                       const AstNode* typeForErr,
                                       const AstNode* initForErr,
                                       QualifiedType::Kind declKind,
                                       QualifiedType declaredType,
                                       QualifiedType initExprType) {

  const Type* typePtr = nullptr;
  const Param* paramPtr = nullptr;

  if (typeForErr == nullptr)
    typeForErr = declForErr;
  if (initForErr == nullptr)
    initForErr = declForErr;

  bool inferParam = (declKind == QualifiedType::PARAM &&
                     initExprType.kind() == QualifiedType::PARAM);

  // check that the resolution of the type expression is a type
  if (checkForKindError(typeForErr, initForErr, declKind,
                        declaredType, initExprType)) {
    // error already issued in checkForKindError
    typePtr = ErroneousType::get(context);
  } else if (!declaredType.hasTypePtr() && !initExprType.hasTypePtr()) {
    // Can't establish the type for now
    typePtr = UnknownType::get(context);
  } else if (declaredType.hasTypePtr() && !initExprType.hasTypePtr()) {
    // declared type but no init, so use declared type
    typePtr = declaredType.type();
  } else if (!declaredType.hasTypePtr() && initExprType.hasTypePtr()) {
    // Iterators are 'materialized' into arrays
    if (initExprType.type()->isIteratorType()) {
      typePtr = computeChplCopyInit(declForErr, declKind, initExprType);
    // Check if this type requires custom type inference
    } else if (auto rec = getTypeWithCustomInfer(context, initExprType.type())) {
      typePtr = computeCustomInferType(declForErr, rec);
    } else {
      // init but no declared type, so use init type
      typePtr = initExprType.type();
      if (inferParam) {
        paramPtr = initExprType.param();
      }
    }
  } else {
    // otherwise both declaredType and initExprType are provided.
    // check that they are compatible
    auto fullDeclType = QualifiedType(declKind, declaredType.type());
    auto got = canPass(context, initExprType, fullDeclType);
    if (!got.passes()) {
      if (initExprType.type()->isUnknownType()) {
        // var x: declType = <unknown or erroneous>
        //
        // if declType is concrete use it, else use unknown.
        //
        // TODO: (Daniel) the code as I found it will not hit this case as
        //       as often as it should, defaulting to the type expression
        //       via the checks above instead. We need to be more precise
        //       about "initialization expression is present but has unknown
        //       type" vs "no initialization expression is present".
        //       See also the HACK in Resolver::enter(IndexableLoop)

        if (getTypeGenericity(context, declaredType.type()) == Type::CONCRETE) {
          typePtr = declaredType.type();
        } else {
          typePtr = UnknownType::get(context);
        }

      } else if (declaredType.type()->isExternType()) {
        auto varDT = QualifiedType(QualifiedType::VAR, declaredType.type());
        // We allow for default-init-then-assign for extern types
        std::vector<CallInfoActual> actuals;
        actuals.push_back(CallInfoActual(varDT, UniqueString()));
        actuals.push_back(CallInfoActual(initExprType, UniqueString()));
        auto ci = CallInfo (/* name */ USTR("="),
                            /* calledType */ QualifiedType(),
                            /* isMethodCall */ false,
                            /* hasQuestionArg */ false,
                            /* isParenless */ false,
                            actuals);

        // TODO: store an associated action?
        const Scope* scope = scopeStack.back();
        auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
        auto c = resolution::resolveGeneratedCall(rc, declForErr, ci, inScopes);
        if (!c.mostSpecific().isEmpty()) {
          typePtr = declaredType.type();
        } else {
          CHPL_REPORT(context, IncompatibleTypeAndInit, declForErr, typeForErr,
                      initForErr, declaredType.type(), initExprType.type());
          typePtr = ErroneousType::get(context);
        }
      } else {
        typePtr = tryFindTypeViaCopyFn(*this, declForErr, typeForErr, initForErr,
                                       declaredType, initExprType);
      }
    } else if (!got.instantiates() || declaredType.type()->isUnknownType()) {
      // use the declared type since no conversion/promotion was needed.
      // alternatively, if declared type is present but unknown, we don't
      // know what was intended in the declaratiom, so we leave it as unknown.
      typePtr = declaredType.type();
    } else {
      // instantiation is needed
      if (!got.converts() && !got.promotes()) {
        // use the init expr type since no conversion/promotion was needed
        typePtr = initExprType.type();
        if (inferParam) {
          paramPtr = initExprType.param();
        }
      } else {
        // get instantiation type
        auto t = getInstantiationType(context, initExprType, fullDeclType);
        typePtr = t.type();
        if (inferParam) {
          paramPtr = t.param();
        }
      }
    }
  }

  CHPL_ASSERT(typePtr != nullptr); // should always be set above.
  return QualifiedType(declKind, typePtr, paramPtr);
}

static bool isValidVarArgCount(QualifiedType paramSize) {
  // Allow unknown param sizes in case they depend on preceding formals' info
  if (paramSize.isUnknownOrErroneous()) {
    return true;
  } else if (paramSize.isParam() == false) {
    return false;
  } else {
    const Param* p = paramSize.param();
    if (p == nullptr) {
      // param n : int, args...n
      return true;
    } else if (p->isIntParam()) {
      int64_t val = p->toIntParam()->value();
      return val > 0;
    } else if (p->isUintParam()) {
      uint64_t val = p->toUintParam()->value();
      return val > 0;
    } else {
      return false;
    }
  }

  return true;
}


//
// This function is called in the case that there is no substitution. When
// resolveNamedDecl processes such a case, it will compute the kind/type as if
// for a normal formal. This function will attempt to use that kind/type as
// the star-type for a VarArgTuple.
//
static const Type* computeVarArgTuple(Resolver& resolver,
                                      const VarArgFormal* varArgs,
                                      QualifiedType::Kind qtKind,
                                      const Type* typePtr) {
  Context* context = resolver.context;
  auto& byPostorder = resolver.byPostorder;
  auto tuple = typePtr->toTupleType();
  bool isVarArgTuple = (tuple != nullptr && tuple->isVarArgTuple());
  if (!isVarArgTuple) {
    QualifiedType paramSize;
    bool invalid = false;
    if (auto count = varArgs->count()) {
      if (count->isTypeQuery() == false &&
          isQuestionMark(count) == false) {
        ResolvedExpression& countVal = byPostorder.byAst(count);
        paramSize = countVal.type();
        invalid = !isValidVarArgCount(paramSize);
      }
    }

    if (invalid) {
      typePtr = UnknownType::get(context);
    } else {
      auto newKind = resolveIntent(QualifiedType(qtKind, typePtr),
                                   /* isThis */ false, /* isInit */ false);
      QualifiedType elt = QualifiedType(newKind, typePtr);
      typePtr = TupleType::getStarTuple(context, paramSize, elt);
    }
  }

  return typePtr;
}


static bool adjustTupleTypeIntentForDecl(Context* context,
                                            const NamedDecl* decl,
                                            QualifiedType::Kind declaredKind,
                                            QualifiedType::Kind& qtKind,
                                            const Type*& typePtr) {
  // adjust tuple declarations for value / referential tuples
  if (typePtr != nullptr && decl->isVarArgFormal() == false) {
    if (auto tupleType = typePtr->toTupleType()) {
      if (declaredKind == QualifiedType::DEFAULT_INTENT) {
        typePtr = tupleType->toReferentialTuple(context);
        qtKind = QualifiedType::CONST_REF;
        return true;
      } else if (declaredKind == QualifiedType::CONST_INTENT) {
        typePtr = tupleType->toReferentialTuple(context, /* makeConst */ true);
        qtKind = QualifiedType::CONST_REF;
        return true;
      } else if (qtKind == QualifiedType::CONST_IN ||
                 qtKind == QualifiedType::CONST_REF) {
        typePtr = tupleType->toValueTuple(context, /* makeConst */ true);
        return true;
      } else if (qtKind == QualifiedType::VAR ||
                 qtKind == QualifiedType::CONST_VAR ||
                 qtKind == QualifiedType::REF ||
                 qtKind == QualifiedType::IN ||
                 qtKind == QualifiedType::OUT ||
                 qtKind == QualifiedType::INOUT ||
                 qtKind == QualifiedType::TYPE) {
        typePtr = tupleType->toValueTuple(context);
        return true;
      }
    }
  }
  return false;
}

/* If the type is generic with defaults, computes the defaults of a type.
   Returns the original type if instantiating with defaults isn't necessary. */
static QualifiedType computeTypeDefaults(Resolver& resolver,
                                         const QualifiedType& type) {
  if (auto t = type.type()) {
    if (auto ct = t->getCompositeType()) {
      // test if that type is generic
      auto g = getTypeGenericity(resolver.context, ct);
      if (g == Type::GENERIC_WITH_DEFAULTS) {
        // fill in the defaults
        return typeWithDefaults(resolver.rc, type);
      }
    }
  }
  return type;
}

static const Type* getAnyType(Resolver& resolver, const ID& anchor) {
  // If we use placeholders, we don't create 'AnyTypes' anywhere,
  // and instead invent new placeholder types.
  return resolver.usePlaceholders
         ? PlaceholderType::get(resolver.context, anchor)->to<Type>()
         : AnyType::get(resolver.context)->to<Type>();
}

// useType will be used to set the type if it is not nullptr
void Resolver::resolveNamedDecl(const NamedDecl* decl, const Type* useType) {
  if (scopeResolveOnly)
    return;

  // Figure out the Kind of the declaration
  auto qtKind = qualifiedTypeKindForDecl(context, decl);

  // Figure out the Type of the declaration
  // Nested Identifiers and Expressions should already be resolved
  const Type* typePtr = nullptr;

  // Figure out the param value, if any
  const Param* paramPtr = nullptr;

  bool isField = false;
  bool isFormal = false;
  bool isFormalThis = false;
  bool isFieldOrFormal = false;
  bool isVarArgs = decl->isVarArgFormal();

  if (auto var = decl->toVarLikeDecl()) {
    // Figure out variable type based upon:
    //  * the type in the variable declaration
    //  * the initialization expression in the variable declaration
    //  * the initialization expression from split-init

    auto typeExpr = var->typeExpression();
    auto initExpr = var->initExpression();

    if (auto var = decl->toVariable())
      if (var->isField())
        isField = true;

    isFormal = decl->isFormal() || isVarArgs;
    isFormalThis = isFormal && decl->name() == USTR("this");
    isFieldOrFormal = isField || isFormal;

    bool foundSubstitution = false;
    bool foundSubstitutionDefaultHint = false;

    QualifiedType typeExprT;
    QualifiedType initExprT;

    if (isFieldOrFormal) {
      // use substitutions computed for fields and formals
      if (substitutions != nullptr) {
        auto search = substitutions->find(decl->id());
        if (search != substitutions->end()) {
          const QualifiedType& t = search->second;
          if (t.kind() == QualifiedType::PARAM) {
            typeExprT = t; // keep kind, typePtr, paramPtr
          } else {
            // set kind to TYPE and forget about paramPtr
            typeExprT = QualifiedType(QualifiedType::TYPE, t.type());
          }
          if (t.type() == nullptr && t.kind() == QualifiedType::UNKNOWN)
            foundSubstitutionDefaultHint = true;
          else
            foundSubstitution = true;
        }
      }
    }

    // use the type from the type expression; if this is a `this` formal
    // for a primary method, there's no type expression, but we should
    // act as if there was one.
    if (!foundSubstitution || ignoreSubstitutionFor == decl) {
      bool usedTypeExpressionOrSimilar = false;

      if (typeExpr) {
        // get the type we should have already computed postorder
        ResolvedExpression& r = byPostorder.byAst(typeExpr);
        typeExprT = r.type();
        usedTypeExpressionOrSimilar = true;
        // otherwise, typeExprT can be empty/null
      } else if (isFormalThis) {
        // We're a primary method `this` formal (which do not have type
        // expressions). This means we don't have to go through searching
        // scopes for the ID the formal refers to: it's the ID of the
        // enclosing record or class.
        auto functionId = parsing::idToParentId(context, decl->id());
        auto aggregateId = parsing::idToParentId(context, functionId);
        auto parentType = typeForId(aggregateId);
        typeExprT = parentType;
        usedTypeExpressionOrSimilar = true;
      }


      // As a workaround for getTypeForDecl (see body of that function),
      // if there's a type expression, make sure type ptr is set even if
      // to UnknownType.
      if (usedTypeExpressionOrSimilar && typeExprT.type() == nullptr) {
        typeExprT = QualifiedType(typeExprT.kind(), UnknownType::get(context), typeExprT.param());
      }

      // for 'this' formals of class type, adjust them to be borrowed, so
      // e.g. proc C.foo() { } has 'this' of type 'borrowed C'.
      // This should not apply to parenthesized expressions.
      //
      // An exception to this is when we are looking at a type method or
      // a parenless, type- or param returning method. In that case, the
      // decorator is 'anymanaged any-nilable', not borrowed nonnil.
      bool identOrNoTypeExpr = !typeExpr || typeExpr->isIdentifier();
      bool isClassType = typeExprT.type() && typeExprT.type()->isClassType();
      if (isFormalThis && isClassType && identOrNoTypeExpr) {
        auto dec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
        bool useFullyGenericDecorator = false;

        if (auto varLikeDecl = decl->toVarLikeDecl()) {
          useFullyGenericDecorator |=
            varLikeDecl->storageKind() == QualifiedType::TYPE;
        }

        if (symbol && symbol->isFunction()) {
          auto fn = symbol->toFunction();
          useFullyGenericDecorator |=
            (fn->returnIntent() == Function::TYPE ||
             fn->returnIntent() == Function::PARAM) &&
            fn->isParenless();
        }

        if (useFullyGenericDecorator) {
          dec = ClassTypeDecorator(ClassTypeDecorator::GENERIC);
        }

        auto ct = typeExprT.type()->toClassType();
        typeExprT = QualifiedType(typeExprT.kind(),
                                  ct->withDecorator(context, dec),
                                  typeExprT.param());
      }
    }

    if (initExpr && !foundSubstitution) {
      // compute the type based upon the init expression
      ResolvedExpression& r = byPostorder.byAst(initExpr);
      initExprT = r.type();
    }

    if (typeExpr == nullptr && initExpr == nullptr &&
        var->linkage() == uast::Decl::EXTERN &&
        var->storageKind() == QualifiedType::TYPE) {
      // creating an opaque external type.

      CHPL_ASSERT(var->isVariable());
      auto linkageNameNode = var->linkageName();
      CHPL_ASSERT(linkageNameNode == nullptr || linkageNameNode->isStringLiteral());

      UniqueString linkageName = var->name();
      if (linkageNameNode) {
        linkageName = linkageNameNode->toStringLiteral()->value();
      }
      qtKind = QualifiedType::TYPE;
      typePtr = ExternType::get(context, linkageName, var->id());
      paramPtr = nullptr;
    } else if (!typeExprT.hasTypePtr() && useType != nullptr) {
      // use type from argument to resolveNamedDecl
      typeExprT = QualifiedType(QualifiedType::TYPE, useType);
      typePtr = typeExprT.type();
    } else if (foundSubstitution && ignoreSubstitutionFor != decl) {
      // if we are working with a substitution, just use that
      // without doing lots of kinds checking
      typePtr = typeExprT.type();
      if (qtKind == QualifiedType::PARAM)
        paramPtr = typeExprT.param();
    } else {
      if (isFieldOrFormal &&
          typeExpr == nullptr && !isFormalThis &&
          initExpr == nullptr) {
        // Lack of initializer for a field/formal means the Any type.
        //
        // However, a `this` formal lacks a type expression if it belongs to a
        // primary method. This does not, however, mean that its type should be
        // AnyType; it is not adjusted here.

        typeExprT = QualifiedType(QualifiedType::TYPE, getAnyType(*this, decl->id()));
      } else if (isFieldOrFormal) {
        // figure out if we should potentially infer the type from the init expr
        // (we do so if it's not a field or a formal)
        bool isTypeOrParam = qtKind == QualifiedType::TYPE ||
                             qtKind == QualifiedType::PARAM;
        // infer the type of the variable from its initialization expr?
        bool inferFromInit = foundSubstitutionDefaultHint ||
                             defaultsPolicy == DefaultsPolicy::USE_DEFAULTS;
        // in addition, always infer from init for a concrete type.
        // the non-concrete cases are like this, e.g.:
        //    type t = int;
        //    var x:GenericRecord = f()
        if (inferFromInit == false && !isTypeOrParam)  {
          // check also for a generic type as the type expression
          auto g = getTypeGenericity(context, typeExprT);

          // "generic with defaults" at this point means "generic" because
          // getTypeGenericity considers the type expression's substitutions,
          // and existing logic would have inserted the defaults if possible.
          // Thus, this expression is explicitly generic.
          if (g != Type::GENERIC && g != Type::GENERIC_WITH_DEFAULTS) {
            inferFromInit = true;
          }
        }

        if (!inferFromInit) {
          // if we aren't inferring from the init expr, clear initExprT
          // so it is not used below.
          initExprT = QualifiedType();

          if (isTypeOrParam && isFieldOrFormal && typeExprT == QualifiedType()) {
            // a type or param field with initExpr is still generic, if
            // it doesn't already have a type-expr, e.g.
            // record R { type t = int; }
            // if that behavior is requested with defaultsPolicy == IGNORE_DEFAULTS
            typeExprT = QualifiedType(QualifiedType::TYPE,
                                      AnyType::get(context));
          }
        }
      }

      // for an initializer expression that is the default value of a formal,
      // check if we have the formal type already and then use that to
      // compute the qtKind now rather than trying to getTypeForDecl first,
      // which will fail in cases where an implicit conversion is needed
      if (!isVarArgs && typeExprT.hasTypePtr() &&
          (isFormal || (signatureOnly && isField))) {
        // update qtKind with the result of resolving the intent
        if (!typeExprT.type()->isTupleType()) {
          // skip for tuple types, which are handled along with ref/value below
          computeFormalIntent(decl, qtKind, typeExprT.type(), typeExprT.param());
        }
      }

      // The type expression is a type-tuple (e.g., (type int, type bool)),
      // but the actual value may be value-tuple (e.g., (int, bool)), or a ref
      // tuple, etc. Need to adjust the intents so that getTypeForDecl (which
      // runs canPass) doesn't balk at the mismatch.
      auto adjustedQtKind = qtKind;
      auto adjustedTypePtr = typeExprT.type();
      if (adjustTupleTypeIntentForDecl(context, decl, qtKind,
                                       adjustedQtKind, adjustedTypePtr)) {
        typeExprT = QualifiedType(typeExprT.kind(), adjustedTypePtr, typeExprT.param());
      }

      //
      // Check that the initExpr type is compatible with declared type
      // Check kinds are OK
      // Handle any implicit conversions / instantiations
      auto qt = getTypeForDecl(decl, typeExpr, initExpr,
                               qtKind, typeExprT, initExprT);
      typePtr = qt.type();
      paramPtr = qt.param();
    }
  }

  if (typePtr == nullptr) {
    if (qtKind == QualifiedType::FUNCTION ||
        qtKind == QualifiedType::MODULE) {
      // OK, type can be null for now
    } else {
      // typePtr should not be null; it should use UnknownType instead.
      // it can be UnknownType for a variable that is split inited.
      typePtr = UnknownType::get(context);
    }
  }

  // param value should not be set if the QualifiedType is not param
  CHPL_ASSERT(qtKind == QualifiedType::PARAM || paramPtr == nullptr);

  auto declaredKind = qtKind;

  if (isVarArgs) {
    typePtr = computeVarArgTuple(*this, decl->toVarArgFormal(),
                                 qtKind, typePtr);
  }

  adjustTupleTypeIntentForDecl(context, decl, declaredKind, qtKind, typePtr);

  ResolvedExpression& result = byPostorder.byAst(decl);
  result.setType(QualifiedType(qtKind, typePtr, paramPtr));
}

void Resolver::computeFormalIntent(const uast::NamedDecl *decl,
                                   types::QualifiedType::Kind &qtKind,
                                   const types::Type *typePtr,
                                   const types::Param *paramPtr) {
  // compute the intent for formals (including type constructor formals)
  bool isThis = decl->name() == USTR("this");
  bool isInit = false;
  if (symbol) {
    if (auto named = symbol->toNamedDecl()) {
      isInit = named->name() == USTR("init") ||
               named->name() == USTR("init=");
    }
  }
  auto formalQt = QualifiedType(qtKind, typePtr, paramPtr);
  // update qtKind with the result of resolving the intent
  qtKind = resolveIntent(formalQt, isThis, isInit);
}

// Peformance: this re-computes the vector each time it is called.
// We could make it a query, or try writing an iterator that handles this.
// In the meantime, though, I'll keep it as is.
static std::vector<CompilerDiagnostic>
gatherUserDiagnostics(ResolutionContext* rc,
                      const CallResolutionResult& c) {
  std::vector<CompilerDiagnostic> into;
  for (auto& msc : c.mostSpecific()) {
    if (!msc) continue;

    // compiler-generated fns don't always have ASTs, so we can't resolve them.
    if (msc.fn()->isCompilerGenerated()) continue;

    // shouldn't happen, but it currently does in some cases.
    if (msc.fn()->needsInstantiation()) continue;

    // HACK: suppress errors from resolving the function here. this is a hack to
    //       avoid breaking existing tests which didn't expect errors emitted
    //       during eager resolution.
    auto resultAndErrors = rc->context()->runAndTrackErrors([rc, &msc, &c](Context* context) {
      return resolveFunction(rc, msc.fn(), c.poiInfo().poiScope(), /* skipIfRunning */ true);
    });
    auto resolvedFn = resultAndErrors.result();
    if (!resolvedFn) continue;

    into.insert(into.end(), resolvedFn->diagnostics().begin(),
                resolvedFn->diagnostics().end());
  }
  return into;
}

void Resolver::emitUserDiagnostic(const CompilerDiagnostic& diagnostic,
                                  const uast::AstNode* astForErr) {
  if (diagnostic.isError()) {
    CHPL_REPORT(context, UserDiagnosticEmitError, diagnostic.message(), astForErr->id());
    noteErrorMessage(context, diagnostic.message());
  } else if (diagnostic.isWarning()) {
    CHPL_REPORT(context, UserDiagnosticEmitWarning, diagnostic.message(), astForErr->id());
    noteWarningMessage(context, diagnostic.message());
  }
}

void Resolver::noteEncounteredUserDiagnostic(CompilerDiagnostic diagnostic,
                                             const uast::AstNode* astForErr) {
  if (diagnostic.isError()) {
    CHPL_REPORT(context, UserDiagnosticEncounterError, diagnostic.message(), astForErr->id());
  } else if (diagnostic.isWarning()) {
    CHPL_REPORT(context, UserDiagnosticEncounterWarning, diagnostic.message(), astForErr->id());
  }
  userDiagnostics.push_back(std::move(diagnostic));
}

void Resolver::issueErrorForFailedModuleDot(const Dot* dot,
                                            ID moduleId,
                                            LookupConfig config) {
  LookupConfig configWithPrivate = config & ~LOOKUP_SKIP_PRIVATE_VIS;
  bool thereButPrivate = false;
  // No need to do the search if we already did search for private symbols
  if (configWithPrivate != config) {
    auto modScope = scopeForModule(context, moduleId);
    auto ids = lookupNameInScope(context, modScope,
                                 /* methodLookupHelper */ nullptr,
                                 /* receiverScopeHelper */ nullptr,
                                 dot->field(), configWithPrivate);
    for (auto& id : ids) {
      // Only report "bla is private" if it's originally declared in the
      // given module (i.e., don't do so if the found ID is imported from
      // elsewhere)
      auto modId = parsing::idToParentModule(context, id);
      if (modId != moduleId) continue;

      thereButPrivate = true;
    }
  }

  // figure out what name was used for the module in the Dot expression
  auto modName = moduleId.symbolName(context);
  auto dotModName = modName;
  ID renameClauseId;
  if (auto dotLeftPart = dot->receiver()) {
    if (auto leftIdent = dotLeftPart->toIdentifier()) {
      dotModName = leftIdent->name();
    }
  }
  if (modName != dotModName) {
    // get a trace for where the module was renamed so that
    // the error can show line numbers
    CHPL_ASSERT(scopeStack.size() > 0);
    const Scope* scope = scopeStack.back();
    std::vector<ResultVisibilityTrace> trace;
    lookupNameInScopeTracing(context, scope,
                             /* methodLookupHelper */ nullptr,
                             /* receiverScopeHelper */ nullptr,
                             dotModName,
                             LOOKUP_DECLS | LOOKUP_IMPORT_AND_USE |
                             LOOKUP_PARENTS | LOOKUP_INNERMOST |
                             LOOKUP_EXTERN_BLOCKS,
                             trace);
    // find the last rename in the trace
    for (const auto& t : trace) {
      for (const auto& elt : t.visibleThrough) {
        if (elt.fromUseImport && elt.renameFrom != dotModName) {
          renameClauseId = elt.visibilityClauseId;
        }
      }
    }
  }

  CHPL_REPORT(context, NotInModule, dot, moduleId, modName, renameClauseId, thereButPrivate);

}

void Resolver::CallResultWrapper::issueBasicError() {
  bool foundUserDiagnostics = false;
  for (auto& diagnostic : gatherUserDiagnostics(parent->rc, result)) {
    if (diagnostic.depth() - 1 == 0) {
      parent->emitUserDiagnostic(diagnostic, astForContext);
      foundUserDiagnostics = true;
    }
  }

  if (foundUserDiagnostics) {
    return;
  }

  if (result.mostSpecific().isEmpty()) {
    // if the call resolution result is empty, we need to issue an error
    if (result.mostSpecific().isAmbiguous()) {
      // ambiguity between candidates
      parent->context->error(astForContext, "Cannot resolve call to '%s': ambiguity",
                     callName ? callName : ci->name().c_str());
    } else {
      std::vector<const uast::VarLikeDecl*> uninitializedActuals;
      // could not find a most specific candidate
      std::vector<ApplicabilityResult> rejected;
      CHPL_ASSERT(rejected.size() == uninitializedActuals.size());
      CHPL_REPORT(parent->context, NoMatchingCandidates, astForContext, *ci, rejected, uninitializedActuals);
    }
  } else {
    parent->context->error(astForContext, "Cannot establish type for call expression");

    // expecting call site to check for hasTypePtr.
    CHPL_ASSERT(!result.exprType().hasTypePtr());
  }
}

bool Resolver::CallResultWrapper::noteResultWithoutError(
    Resolver& resolver,
    ResolvedExpression* r,
    const uast::AstNode* astForContext,
    const CallResolutionResult& result,
    optional<ActionAndId> actionAndId) {
  bool needsErrors = false;

  for (auto& diagnostic : gatherUserDiagnostics(resolver.rc, result)) {
    // The diagnostic's depth means it's aimed for further up the call stack.
    // Note it in our own diagnostic list.
    if (diagnostic.depth() - 1 > 0) {
      resolver.userDiagnostics.emplace_back(diagnostic.message(),
                                            diagnostic.kind(),
                                            diagnostic.depth() - 1);
    } else if (diagnostic.depth() - 1 == 0) {
      // we're asked not to emit errors, and only return if errors are
      // needed.
      needsErrors = true;
    }
  }

  if (!result.exprType().hasTypePtr()) {
    if (!actionAndId && r) {
      // Only set the type to erroneous if we're handling an actual user call,
      // and not an associated action.
      r->setType(QualifiedType(r->type().kind(), ErroneousType::get(resolver.context)));
      r->setMostSpecific(result.mostSpecific());
    }

    // If the call was specially handled, assume special-case logic has already
    // issued its own error, so we shouldn't emit a general error.
    return !result.speciallyHandled() || needsErrors;
  } else {
    if (actionAndId) {
      // save candidates as associated functions
      for (auto& sig : result.mostSpecific()) {
        if (sig && r) {
          r->addAssociatedAction(std::get<0>(*actionAndId), sig.fn(),
                                 std::get<1>(*actionAndId));
        }
      }
    } else if (r) {
      r->setPoiScope(result.poiInfo().poiScope());
      r->setType(result.exprType());
      resolver.validateAndSetMostSpecific(*r, astForContext, result.mostSpecific());
    }
    // gather the poi scopes used when resolving the call
    resolver.poiInfo.accumulate(result.poiInfo());
  }
  return needsErrors;

}


bool Resolver::CallResultWrapper::noteResultWithoutError(ResolvedExpression* r,
                                                          optional<ActionAndId> actionAndId) {
  return noteResultWithoutError(*parent, r, astForContext, result, std::move(actionAndId));
}

void Resolver::CallResultWrapper::noteResult(ResolvedExpression* r,
                                              optional<ActionAndId> actionAndId) {
  if (noteResultWithoutError(r, std::move(actionAndId))) {
    issueBasicError();
  }
}

bool Resolver::CallResultWrapper::rerunCallAndPrintCandidates() {
  if (!result.mostSpecific().isEmpty() || result.mostSpecific().isAmbiguous()) return false;

  // The call isn't ambiguous; it might be that we rejected all the candidates
  // that we encountered. Re-run resolution, providing a 'rejected' vector
  // this time to preserve the list of rejected candidates.
  std::vector<ApplicabilityResult> rejected;
  if (wasGeneratedCall) {
    std::ignore = resolution::resolveGeneratedCall(parent->rc, astForContext, *ci, *inScopes, &rejected);
  } else {
    CHPL_ASSERT(astForContext->isCall());
    std::ignore = resolution::resolveCallInMethod(parent->rc, astForContext->toCall(), *ci, *inScopes,
                                                  receiverType,
                                                  &rejected);
  }

  if (!rejected.empty()) {
    // There were candidates but we threw them out. Report on those.
    if (actualAsts) {
      handleRejectedCandidates(*this, rejected, *actualAsts);
    } else {
      std::vector<const uast::AstNode*> actualAsts(ci->numActuals(), nullptr);
      handleRejectedCandidates(*this, rejected, actualAsts);
    }
    return true;
  }

  return false;
}

void Resolver::CallResultWrapper::noteResultPrintCandidates(ResolvedExpression* r,
                                                             optional<ActionAndId> actionAndId) {
  CHPL_ASSERT(!wasGeneratedCall || receiverType.isUnknown());
  if (noteResultWithoutError(r, std::move(actionAndId))) {
    if (rerunCallAndPrintCandidates()) {
      return;
    }

    // Fall through to the more general error handling.
    issueBasicError();
  }
}

void Resolver::adjustTypesForSplitInit(ID id,
                                       const QualifiedType& rhsType,
                                       const AstNode* lhsExprAst,
                                       const AstNode* astForError) {
  if (id.isEmpty()) {
    return;
  }

  // what variable does the LHS refer to? is it within the purview of this
  // Resolver?
  bool useLocalResult = (id.symbolPath() == symbol->id().symbolPath() &&
                         !id.isSymbolDefiningScope());

  // if the the lhs variable has not been initialized and
  // the type of LHS is generic or unknown, update it based on the RHS type.

  if (!useLocalResult) {
    return;
  }

  ResolvedExpression& lhs = byPostorder.byId(id);
  QualifiedType lhsType = lhs.type();

  if (!lhsType.needsSplitInitTypeInfo(context)) return;

  const Param* p = rhsType.param();
  auto useKind = lhsType.kind();
  if (p && symbol->isModule()) {
    // This is a white lie since if we are in a module-level statement, the
    // kind we see here will always be UNKNOWN, whether this is a PARAM or not.
    // This is because module statements are resolved separately from each
    // other.
    // Set it to PARAM here so we are allowed to store a param value for
    // it, else we get a complaint about storing a param value on a non-param
    // kind. The param kind will only get preserved later (at module-level
    // resolution) if it actually is a PARAM decl.
    useKind = QualifiedType::PARAM;
  }
  if (useKind != QualifiedType::PARAM) p = nullptr;
  const auto useType = QualifiedType(useKind, rhsType.type(), p);

  lhs.setType(useType);
  if (lhsExprAst != nullptr) {
    ResolvedExpression& lhsExpr = byPostorder.byAst(lhsExprAst);
    lhsExpr.setType(useType);
  }
}

// handle adjusting a variable's type to support split init
void Resolver::adjustTypesOnAssign(const AstNode* lhsAst,
                                   const AstNode* rhsAst) {

  ResolvedExpression& lhsExpr = byPostorder.byAst(lhsAst);

  ID id = lhsExpr.toId();
  if (id.isEmpty()) {
    return;
  }

  QualifiedType rhsType = byPostorder.byAst(rhsAst).type();

  adjustTypesForSplitInit(id, rhsType, lhsAst, rhsAst);
}

// Update a variable's type if it is generic/unknown
// and the variable is initialized by an 'out' formal
void
Resolver::adjustTypesForOutFormals(const CallInfo& ci,
                                   const std::vector<const AstNode*>& asts,
                                   const MostSpecificCandidates& fns) {

  std::vector<QualifiedType> actualFormalTypes;
  std::vector<Qualifier> actualIntents;
  computeActualFormalIntents(context, fns, ci, asts,
                             actualIntents, actualFormalTypes);

  int actualIdx = 0;
  for (const auto& actual : ci.actuals()) {
    (void) actual; // avoid compilation error about unused variable

    // find an actual referring to an ID that is passed to an 'out' formal
    ID id;
    const AstNode* actualAst = asts[actualIdx];
    if (actualAst != nullptr && byPostorder.hasAst(actualAst)) {
      id = byPostorder.byAst(actualAst).toId();
    }
    if (actualIntents[actualIdx] == Qualifier::OUT && !id.isEmpty()) {
      QualifiedType formalType = actualFormalTypes[actualIdx];
      adjustTypesForSplitInit(id, formalType, actualAst, actualAst);
    }
    actualIdx++;
  }
}

void Resolver::resolveTupleUnpackAssign(ResolvedExpression& r,
                                        const uast::AstNode* astForErr,
                                        const Tuple* lhsTuple,
                                        QualifiedType lhsType,
                                        QualifiedType rhsType) {
  // Check that lhsType = rhsType can work

  if (!lhsType.hasTypePtr()) {
    context->error(lhsTuple, "Unknown lhs tuple type in split tuple assign");
    return;
  }
  if (!rhsType.hasTypePtr()) {
    context->error(lhsTuple, "Unknown rhs tuple type in split tuple assign");
    return;
  }

  // First, check that lhsType and rhsType are tuples
  const TupleType* lhsT = lhsType.type()->toTupleType();
  const TupleType* rhsT = rhsType.type()->toTupleType();

  if (lhsT == nullptr) {
    context->error(lhsTuple, "lhs type is not tuple in split tuple assign");
    return;
  }
  if (rhsT == nullptr) {
    context->error(lhsTuple, "rhs type is not tuple in split tuple assign");
    return;
  }

  // Then, check that they have the same size
  if (lhsTuple->numActuals() != rhsT->numElements()) {
    context->error(lhsTuple, "tuple size mismatch in split tuple assign");
    return;
  }
  if (lhsT->numElements() != rhsT->numElements()) {
    context->error(lhsTuple, "tuple size mismatch in split tuple assign");
    return;
  }

  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);

  // Finally, try to resolve = between the elements
  int i = 0;
  for (auto actual : lhsTuple->actuals()) {
    QualifiedType lhsEltType = lhsT->elementType(i);
    QualifiedType rhsEltType = rhsT->elementType(i);
    if (auto innerTuple = actual->toTuple()) {
      resolveTupleUnpackAssign(r, astForErr, innerTuple, lhsEltType, rhsEltType);
    } else {
      std::vector<CallInfoActual> actuals;
      actuals.push_back(CallInfoActual(lhsEltType, UniqueString()));
      actuals.push_back(CallInfoActual(rhsEltType, UniqueString()));
      auto ci = CallInfo (/* name */ USTR("="),
                          /* calledType */ QualifiedType(),
                          /* isMethodCall */ false,
                          /* hasQuestionArg */ false,
                          /* isParenless */ false,
                          actuals);

      auto c = resolveGeneratedCall(actual, &ci, &inScopes);
      c.noteResult(&r, { { AssociatedAction::ASSIGN, lhsTuple->id() } });
    }
    i++;
  }
}

void Resolver::resolveTupleUnpackDecl(const TupleDecl* lhsTuple,
                                      QualifiedType rhsType) {
  if (!rhsType.hasTypePtr()) {
    CHPL_REPORT(context, TupleDeclUnknownType, lhsTuple);
    return;
  }

  const TupleType* rhsT = rhsType.type() ? rhsType.type()->toTupleType() : nullptr;
  std::vector<QualifiedType> eltTypes;

  if (rhsT == nullptr) {
    if (rhsType.isUnknown() ||
        rhsType.isErroneousType() ||
        (rhsType.hasTypePtr() && rhsType.type()->isAnyType())) {
      // Fill element types with the unknown/generic QualifiedType, so that we
      // at least get the 'kind' right
      eltTypes = std::vector<QualifiedType>(lhsTuple->numDecls(), rhsType);
    } else {
      CHPL_REPORT(context, TupleDeclNotTuple, lhsTuple, rhsType.type());
      return;
    }
  } else if (lhsTuple->numDecls() != rhsT->numElements()) {
    // Then, check that they have the same size
    CHPL_REPORT(context, TupleDeclMismatchedElems, lhsTuple, rhsT);
    return;
  } else {
    for (int i = 0; i < rhsT->numElements(); i++) {
      eltTypes.push_back(rhsT->elementType(i));
    }
  }

  // Finally, try to resolve the types of the elements
  int i = 0;
  for (auto actual : lhsTuple->decls()) {
    QualifiedType rhsEltType = eltTypes[i];
    if (auto innerTuple = actual->toTupleDecl()) {
      resolveTupleUnpackDecl(innerTuple, rhsEltType);
    } else if (auto namedDecl = actual->toNamedDecl()) {
      resolveNamedDecl(namedDecl, rhsEltType.type());
    } else {
      CHPL_ASSERT(false && "case not handled");
    }
    i++;
  }
}

void Resolver::resolveTupleDecl(const TupleDecl* td,
                                const Type* useType) {
  if (scopeResolveOnly) {
    return;
  }

  QualifiedType::Kind declKind = (Qualifier) td->intentOrKind();
  QualifiedType useT;

  // Non-default intents are currently not allowed for tuple-grouped formals.
  //
  // Note: This doesn't apply to nested TupleDecls like ``(a, b)`` in the
  // formal ``((a, b), c)``. Such nested formals should be handled by
  // ``resolveTupleUnpackDecl``.
  if (td->isTupleDeclFormal() && declKind != QualifiedType::DEFAULT_INTENT) {
    useT = QualifiedType(declKind, ErroneousType::get(context));
    ResolvedExpression& result = byPostorder.byAst(td);
    result.setType(useT);
    return;
  }

  // Figure out the type to use for this tuple
  if (useType != nullptr) {
    useT = QualifiedType(declKind, useType);
  } else if (substitutions != nullptr &&
             substitutions->count(td->id()) != 0) {
    auto sub = substitutions->find(td->id())->second;
    auto subTup = sub.hasTypePtr() ? sub.type()->toTupleType() : nullptr;

    if (subTup == nullptr) {
      // Rely on hasTypePtr check below to recognize error
      useT = QualifiedType();
    } else {
      useT = sub;
    }
  } else {
    QualifiedType typeExprT;
    QualifiedType initExprT;

    auto typeExpr = td->typeExpression();
    auto initExpr = td->initExpression();

    if (typeExpr == nullptr && initExpr == nullptr) {
      // Note: we seem to rely on tuple components being 'var', and relying on
      // the tuple's kind instead. Without this, the current instantiation
      // logic won't allow, for example, passing (1, 2, 3) to (?, ?, ?).
      std::vector<QualifiedType> eltTypes;
      for (auto decl : td->decls()) {
        eltTypes.push_back(QualifiedType(QualifiedType::VAR,
                                         getAnyType(*this, decl->id())));
      }
      auto tup = TupleType::getQualifiedTuple(context, eltTypes);
      useT = QualifiedType(declKind, tup);
    } else {
      if (typeExpr != nullptr) {
        ResolvedExpression& result = byPostorder.byAst(typeExpr);
        typeExprT = result.type();
      }
      if (initExpr != nullptr) {
        ResolvedExpression& result = byPostorder.byAst(initExpr);
        initExprT = result.type();
      }

      useT = getTypeForDecl(td, typeExpr, initExpr,
                            declKind, typeExprT, initExprT);

    }
  }

  if (!useT.hasTypePtr()) {
    context->error(td, "Cannot establish type for tuple decl");
    useT = QualifiedType(declKind, ErroneousType::get(context));
  } else if (useT.type()->isTupleType()) {
    useT = QualifiedType(useT.kind(),
                         useT.type()->toTupleType()->toReferentialTuple(context));
  }

  // save the type in byPostorder
  ResolvedExpression& result = byPostorder.byAst(td);
  result.setType(useT);
  // resolve the types of the tuple elements
  resolveTupleUnpackDecl(td, useT);
}

bool Resolver::resolveSpecialNewCall(const Call* call) {
  if (!call->calledExpression() ||
      !call->calledExpression()->isNew()) {
    return false;
  }

  auto newExpr = call->calledExpression()->toNew();
  auto& re = byPostorder.byAst(call);
  auto& reNewExpr = byPostorder.byAst(newExpr);
  auto qtNewExpr = reNewExpr.type();


  // exit immediately if the 'new' failed to resolve
  if (qtNewExpr.isUnknown() || qtNewExpr.isErroneousType()) {
    return true;
  }

  const Type* initReceiverType = qtNewExpr.type();

  if (auto clsType = qtNewExpr.type()->toClassType()) {
    // Remove nilability from e.g., 'new C?()' for the init call (or else it
    // will not resolve because the receiver formal is 'nonnil borrowed').

    // always set the receiver to be borrowed non-nil b/c we don't want to
    // call initializers for '_owned' when the receiver is 'owned(MyClass)'
    auto newDecor = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
    initReceiverType = clsType->withDecorator(context, newDecor);
  } else if (auto recordType = qtNewExpr.type()->toRecordType()) {
    // Rewrite 'new dmap' to 'new _distribution'
    if (recordType->id().symbolPath() == "ChapelArray.dmap") {
      initReceiverType = CompositeType::getDistributionType(context);
    }
  }
  CHPL_ASSERT(initReceiverType);

  /*
  auto cls = qtNewExpr.type()->toClassType();
  CHPL_ASSERT(cls);
  auto calledIntent = cls->manager() ? QualifiedType::REF
                                     : QualifiedType::CONST_IN;
  */
  // TODO: Unclear to me whether we should use this type or the above.
  auto calledType = QualifiedType(qtNewExpr.kind(), initReceiverType);
  bool isMethodCall = true;
  const AstNode* questionArg = nullptr;
  std::vector<CallInfoActual> actuals;
  std::vector<const uast::AstNode*> actualAsts;

  // Prepare receiver.
  auto receiverInfo = CallInfoActual(calledType, USTR("this"));
  actuals.push_back(std::move(receiverInfo));
  actualAsts.push_back(newExpr->typeExpression());

  // if the type has existing substitutions, we feed them as named actuals
  // to the constructor. However, the constructor can do something entirely
  // different, and override the subs we passed in. This is an error, which
  // we should check for if we added named actuals.
  addExistingSubstitutionsAsActuals(context, qtNewExpr.type(), actuals, actualAsts);

  // Remaining actuals.
  prepareCallInfoActuals(call, actuals, questionArg, &actualAsts);
  CHPL_ASSERT(!questionArg);

  // The 'new' will produce an 'init' call as a side effect.
  auto ci = CallInfo(USTR("init"), calledType, isMethodCall,
                     /* hasQuestionArg */ questionArg != nullptr,
                     /* isParenless */ false,
                     std::move(actuals));
  CHPL_ASSERT(actualAsts.size() == (size_t)ci.numActuals());
  auto inScope = scopeStack.back();
  auto inPoiScope = poiScope;
  auto inScopes = CallScopeInfo::forNormalCall(inScope, inPoiScope);

  // note: the resolution machinery will get compiler generated candidates
  auto c = resolveGeneratedCall(call, &ci, &inScopes);
  optional<ActionAndId> action({ AssociatedAction::NEW_INIT, call->id() });
  c.noteResultPrintCandidates(&re, std::move(action));


  // there should be one or zero applicable candidates
  CHPL_ASSERT(c.result.mostSpecific().numBest() <= 1);
  if (auto initMsc = c.result.mostSpecific().only()) {
    auto initTfs = initMsc.fn();

    // Set the final output type based on the result of the 'new' call.
    auto qtInitReceiver = initTfs->formalType(0);
    auto type = qtInitReceiver.type();

    // Preserve original management if receiver is a class.
    if (auto cls = type->toClassType()) {
      auto newCls = qtNewExpr.type()->toClassType();
      CHPL_ASSERT(newCls);
      CHPL_ASSERT(!cls->manager() && cls->decorator().isNonNilable());
      CHPL_ASSERT(cls->decorator().isBorrowed());
      CHPL_ASSERT(cls->basicClassType());
      type = ClassType::get(context, cls->basicClassType(),
                            newCls->manager(),
                            newCls->decorator());
    }

    auto qt = QualifiedType(QualifiedType::VAR, type);
    re.setType(qt);
  }

  return true;
}

bool Resolver::resolveSpecialOpCall(const Call* call) {
  if (!call->isOpCall()) return false;

  auto op = call->toOpCall();

  if (op->op() == USTR("=")) {
    if (op->numActuals() == 2) {
      if (auto lhsTuple = op->actual(0)->toTuple()) {

        ResolvedExpression& r = byPostorder.byAst(op);
        QualifiedType lhsType = byPostorder.byAst(op->actual(0)).type();
        QualifiedType rhsType = byPostorder.byAst(op->actual(1)).type();

        resolveTupleUnpackAssign(r, call,
                                 lhsTuple, lhsType, rhsType);
        return true;
      }

      // Update a generic/unknown type when split-init is used.
      adjustTypesOnAssign(op->actual(0), op->actual(1));
    }
  } else if (op->op() == USTR("...")) {
    // just leave it unknown -- tuple expansion only makes sense
    // in the argument list for another call.
    return true;
  }

  return false;
}

bool Resolver::resolveSpecialPrimitiveCall(const Call* call) {
  auto primCall = call->toPrimCall();
  if (!primCall) return false;

  if (primCall->prim() == PRIM_RESOLVES) {
    QualifiedType result;

    if (primCall->numActuals() != 1) {
      result = typeErr(primCall, "invalid call to \"resolves\" primitive");
    } else {
      auto resultAndErrors = context->runAndTrackErrors([&](Context* context) {
        primCall->actual(0)->traverse(*this);
        return byPostorder.byAst(primCall->actual(0)).type();
      });

      bool resultBool = true;
      if (!resultAndErrors.ranWithoutErrors()) {
        // Errors were emitted, so even if we have a type, it doesn't
        // "resolve".
        resultBool = false;
      } else if (resultAndErrors.result().isErroneousType() ||
                 resultAndErrors.result().isUnknown()) {
        // We got an erroneous or unknown type, so it didn't resolve.
        resultBool = false;
      }

      result = QualifiedType::makeParamBool(context, resultBool);
    }

    byPostorder.byAst(primCall).setType(result);
    return true;
  } else if (primCall->prim() == PRIM_ERROR ||
             primCall->prim() == PRIM_WARNING) {
    // No matter what, this primitive is void-returning.
    byPostorder.byAst(primCall).setType({ QualifiedType::VAR, VoidType::get(context) });

    auto kind = primCall->prim() == PRIM_ERROR ? CompilerDiagnostic::ERROR
                                               : CompilerDiagnostic::WARNING;

    const TupleType* messageComponents = nullptr;
    const IntParam* depthParam = nullptr;
    if (typedSignature) {
      for (int i = 0; i < typedSignature->numFormals(); i++) {
        auto& qt = typedSignature->formalType(i);
        if (qt.isUnknownOrErroneous()) continue;

        if (typedSignature->untyped()->formalName(i) == "msg") {
          messageComponents = qt.type()->toTupleType();
        } else if (typedSignature->untyped()->formalName(i) == "errorDepth") {
          depthParam = qt.param() ? qt.param()->toIntParam() : nullptr;
        }
      }
    }

    if (!messageComponents) {
      context->error(primCall, "invalid use of compiler diagnostic primitive");
      return true;
    }

    std::string message;
    for (int i = 0; i < messageComponents->numElements(); i++) {
      auto qt = messageComponents->elementType(i);
      if (!qt.param()) continue;
      message += qt.param()->toStringParam()->value().c_str();
    }

    // In Dyno, depth counts call sites, since we don't have an accessible stack.
    // by default, we anchor the error to the function that invoked compilerError(),
    // which means two call sites to skip: one of the __primitive("error"), and
    // one for compilerError() itself. The next call site will be the call
    // to the function that invoked compilerError().
    int64_t depth = 2;
    if (depthParam) {
      depth = depthParam->value() + 1;
    }

    auto diagnostic =
      CompilerDiagnostic(UniqueString::get(context, message.c_str()), kind, depth);
    if (depth == 0) {
      emitUserDiagnostic(diagnostic, primCall);
    } else {
      // We are not the target recipient of the error; functions further up
      // the call stack ought to issue this error.
      noteEncounteredUserDiagnostic(diagnostic, primCall);
    }

    return true;
  }

  return false;
}

static SkipCallResolutionReason
shouldSkipCallResolution(Resolver* rv, const uast::AstNode* callLike,
                         std::vector<const uast::AstNode*> actualAsts,
                         const CallInfo& ci) {
  Context* context = rv->context;
  SkipCallResolutionReason skip = NONE;
  auto& byPostorder = rv->byPostorder;

  if (callLike->isTuple()) return skip;

  int actualIdx = 0;
  for (const auto& actual : ci.actuals()) {
    ID toId; // does the actual refer directly to a particular variable?
    const AstNode* actualAst = actualAsts[actualIdx];
    if (actualAst != nullptr && byPostorder.hasAst(actualAst)) {
      toId = byPostorder.byAst(actualAst).toId();
    }
    QualifiedType qt = actual.type();
    const Type* t = qt.type();

    auto formalAst = toId.isEmpty() ? nullptr : parsing::idToAst(context, toId);
    bool isNonOutFormal = formalAst != nullptr &&
                          formalAst->isFormal() &&
                          formalAst->toFormal()->intent() != Formal::Intent::OUT;

    if (t != nullptr && t->isErroneousType()) {
      // always skip if there is an ErroneousType
      skip = ERRONEOUS_ACT;
    } else if (!toId.isEmpty() && !isNonOutFormal &&
               qt.isUnknown() &&
               qt.kind() != QualifiedType::PARAM &&
               qt.kind() != QualifiedType::TYPE &&
               qt.isRef() == false) {
      // don't skip because it could be initialized with 'out' intent,
      // but not for non-out formals because they can't be split-initialized.
    } else if (actualAst->isTypeQuery() && ci.calledType().isType()) {
      // don't skip for type queries in type constructors
    } else {
      if (qt.isParam() && qt.param() == nullptr) {
        skip = UNKNOWN_PARAM;
      } else if (qt.isUnknown()) {
        skip = UNKNOWN_ACT;
      } else if (t != nullptr && qt.kind() != QualifiedType::INIT_RECEIVER) {
        // For initializer calls, allow generic formals using the above
        // condition; this way, 'this.init(..)' while 'this' is generic
        // should be fine.

        auto g = getTypeGenericity(context, t);
        bool isBuiltinGeneric = (g == Type::GENERIC &&
                                 (t->isAnyType() || t->isBuiltinType()));
        if (qt.isType() && isBuiltinGeneric && rv->substitutions == nullptr) {
          skip = GENERIC_TYPE;
        } else if (!qt.isType() && g != Type::CONCRETE) {
          skip = GENERIC_VALUE;
        }
      }
    }

    // Don't skip for type constructors, except due to unknown params.
    if (skip != UNKNOWN_PARAM && skip != UNKNOWN_ACT && ci.calledType().isType()) {
      skip = NONE;
    }

    // Do not skip primitive calls that accept a generic type, since they
    // may be valid.
    if (skip == GENERIC_TYPE && callLike->toPrimCall()) {
      skip = NONE;
    }

    if (skip) {
      break;
    }
    actualIdx++;
  }

  // Don't try to resolve calls to '=' until later
  if (ci.isOpCall() && ci.name() == USTR("=")) {
    skip = OTHER_REASON;
  }

  return skip;
}

template <typename ErrorType>
static void
resolveSpecialKeywordCallAsNormalCall(Resolver& rv,
                                      const FnCall* outerCall,
                                      const FnCall* innerCall,
                                      UniqueString name,
                                      ResolvedExpression& noteInto) {
  auto runResult = rv.context->runAndTrackErrors([&](Context* ctx) {
    std::vector<const AstNode*> actualAsts;
    auto ci = CallInfo::create(rv.context, innerCall, rv.byPostorder,
                               /* raiseErrors */ true,
                               /* actualAsts */ &actualAsts,
                               /* moduleScopeId */ nullptr,
                               /* rename */ name);

    auto skip = shouldSkipCallResolution(&rv, innerCall, actualAsts, ci);
    if (skip != NONE) {
      return CallResolutionResult::getEmpty();
    }

    auto scope = rv.scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(scope, rv.poiScope);
    auto c = rv.resolveGeneratedCall(innerCall, &ci, &inScopes);
    c.noteResult(&noteInto);
    return c.result;
  });

  if (!runResult.ranWithoutErrors()) {
    auto firstActual = QualifiedType();
    if (innerCall->numActuals() > 0) {
      firstActual = rv.byPostorder.byAst(innerCall->actual(0)).type();
    }
    rv.context->report(ErrorType::get(std::make_tuple(outerCall, firstActual)));
  }
}

bool Resolver::resolveSpecialKeywordCall(const Call* call) {
  if (!call->isFnCall()) return false;

  auto fnCall = call->toFnCall();
  if (!fnCall->calledExpression()->isIdentifier()) return false;

  auto& r = byPostorder.byAst(call);

  auto fnName = fnCall->calledExpression()->toIdentifier()->name();
  if (fnName == "index") {
    resolveSpecialKeywordCallAsNormalCall<ErrorInvalidIndexCall>(
        *this, fnCall, fnCall, UniqueString::get(context, "chpl__buildIndexType"), r);
    return true;
  } else if (fnName == "subdomain") {
    // check if we're inside a 'sparse subdomain' call, in which case we should
    // do nothing.
    CHPL_ASSERT(callNodeStack.size() >= 2);
    CHPL_ASSERT(callNodeStack.back() == fnCall);
    auto parentCall = callNodeStack[callNodeStack.size() - 2];
    if (parentCall->numActuals() == 1 && parentCall->actual(0) == fnCall) {
      if (auto parentFnCall = parentCall->toFnCall()) {
        if (parentFnCall->calledExpression()->isIdentifier() &&
            parentFnCall->calledExpression()->toIdentifier()->name() == "sparse") {
          return true;
        }
      }
    }

    resolveSpecialKeywordCallAsNormalCall<ErrorInvalidSubdomainCall>(
        *this, fnCall, fnCall, UniqueString::get(context, "chpl__buildSubDomainType"), r);
    return true;
  } else if (fnName == "sparse") {
    CHPL_ASSERT(fnCall->numActuals() == 1);
    CHPL_ASSERT(fnCall->actual(0)->isFnCall());
    resolveSpecialKeywordCallAsNormalCall<ErrorInvalidSparseSubdomainCall>(
        *this, fnCall, fnCall->actual(0)->toFnCall(),
        UniqueString::get(context, "chpl__buildSparseDomainRuntimeTypeForParentDomain"), r);
    return true;
  } else if (fnName == "domain") {
    auto& rCalledExp = byPostorder.byAst(fnCall->calledExpression());
    CHPL_ASSERT(rCalledExp.type().hasTypePtr());
    // Try resolving 'domain(?)' as a special case.
    if (call->numActuals() == 1 && call->actual(0)->isIdentifier() &&
        call->actual(0)->toIdentifier()->name() == "?") {
      // 'domain(?)' is equivalent to just 'domain', the generic domain
      // type.
      // Copy the result of resolving 'domain' as the called identifier.
      r.setType(rCalledExp.type());
    } else {
      // Get type by resolving the type of corresponding domain builder call
      const AstNode* questionArg = nullptr;
      std::vector<CallInfoActual> actuals;
      // Set up distribution arg
      auto defaultDistArg = CallInfoActual(
          DomainType::getDefaultDistType(context), UniqueString());
      actuals.push_back(std::move(defaultDistArg));
      // Remaining given args from domain() call as written
      prepareCallInfoActuals(call, actuals, questionArg, /*actualAsts*/ nullptr);
      CHPL_ASSERT(!questionArg);

      auto ci =
          CallInfo(UniqueString::get(context, "chpl__buildDomainRuntimeType"),
                   /* calledType */ QualifiedType(),
                   /* isMethodCall */ false,
                   /* hasQuestionArg */ false,
                   /* isParenless */ false,
                   actuals);

      auto scope = scopeStack.back();
      auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
      auto runResult = context->runAndTrackErrors([&](Context* ctx) {
        return resolveGeneratedCall(call, &ci, &inScopes);
      });
      auto& crr = runResult.result().result;

      // Note: this issues errors from compilerError in the body of the
      // domain builder.
      optional<ActionAndId> action({ AssociatedAction::RUNTIME_TYPE, fnCall->id() });
      bool needsErrors =
        runResult.result().noteResultWithoutError(&r, std::move(action));

      const Type* domainTy;
      if (runResult.ranWithoutErrors() &&
          !crr.exprType().isUnknownOrErroneous()) {
        domainTy = crr.exprType().type();
      } else if (crr.mostSpecific().numBest() >= 1) {
        // Errors were issued, but we found a candidate. This means the errors
        // came from resolving the body of the domain builder, which
        // are probably more specific than "InvalidDomainCall". Do not issue
        // more errors.
        CHPL_ASSERT(needsErrors);
        runResult.result().issueBasicError();
        domainTy = ErroneousType::get(context);
      } else {
        std::vector<QualifiedType> actualTypesForErr;
        for (auto it = actuals.begin() + 1; it != actuals.end(); ++it) {
          actualTypesForErr.push_back(it->type());
        }
        domainTy = CHPL_TYPE_ERROR(context, InvalidDomainCall, fnCall,
                                   actualTypesForErr).type();
      }
      r.setType(QualifiedType(QualifiedType::TYPE, domainTy));
    }
    return true;
  }

  return false;
}

bool Resolver::resolveSpecialCall(const Call* call) {
  return resolveSpecialOpCall(call) ||
         resolveSpecialPrimitiveCall(call) ||
         resolveSpecialNewCall(call) ||
         resolveSpecialKeywordCall(call);
}

static QualifiedType
lookupFieldType(Resolver& rv, const CompositeType* ct, const ID& idField) {
  if (!ct || !idField) return {};

  auto newDefaultsPolicy = rv.defaultsPolicy;
  if (newDefaultsPolicy == DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS &&
      ct == rv.inCompositeType) {
    // The USE_DEFAULTS_OTHER_FIELDS policy is supposed to make
    // the Resolver act as if it was running with IGNORE_DEFAULTS
    // at first, but then switch to USE_DEFAULTS for all other fields
    // of the type being resolved. This branch implements the switch:
    // if we're moving on to resolving another field, and if this
    // field is from the current type, we resolve that field with
    // USE_DEFAULTS.
    newDefaultsPolicy = DefaultsPolicy::USE_DEFAULTS;
  }
  // if it is recursive within the current class/record, we can
  // call resolveField.
  auto& rf = resolveFieldDecl(rv.rc, ct, idField, newDefaultsPolicy);

  // find the field that matches
  for (int i = 0; i < rf.numFields(); i++) {
    if (rf.fieldDeclId(i) == idField) return rf.fieldType(i);
  }

  return {};
}

QualifiedType Resolver::typeForId(const ID& id) {
  if (scopeResolveOnly) {
    auto kind = qualifiedTypeKindForId(context, id);
    const Type* type = nullptr;
    return QualifiedType(kind, type);
  }

  // Intercept the standard library `c_ptr`, `c_ptrConst`, and `_ddata`, and
  // turn them into the appropriate builtin type.
  if (id == CPtrType::getId(context)) {
    return QualifiedType(QualifiedType::TYPE, CPtrType::get(context));
  } else if (id == CPtrType::getConstId(context)) {
    return QualifiedType(QualifiedType::TYPE, CPtrType::getConst(context));
  } else if (id == HeapBufferType::getId(context)) {
    return QualifiedType(QualifiedType::TYPE, HeapBufferType::get(context));
  }

  // if the id is contained within this symbol,
  // get the type information from the resolution result.
  //
  // when resolving a module statement, the resolution result only
  // contains things within that statement.
  if (parentResolver != nullptr) {
    const Scope* idScope = scopeForId(context, id);
    bool local = false;
    for (auto sc : scopeStack) {
      if (sc == idScope) {
        local = true;
      }
    }

    if (!local) {
      return parentResolver->typeForId(id);
    }
  }

  bool useLocalResult = allowLocalSearch &&
                        (id.symbolPath() == symbol->id().symbolPath() &&
                        !id.isSymbolDefiningScope());
  if (useLocalResult && curStmt != nullptr) {
    if (curStmt->id().contains(id)) {
      // OK, proceed using local result
    } else {
      useLocalResult = false;
    }
  }

  if (useLocalResult) {
    QualifiedType ret = byPostorder.byId(id).type();
    auto g = Type::MAYBE_GENERIC;
    if (ret.hasTypePtr()) {
      g = getTypeGenericity(context, ret.type());
    }

    if (g != Type::CONCRETE && shouldUseUnknownTypeForGeneric(id)) {
      // if id refers to a field or formal that needs to be instantiated,
      // replace the type with UnknownType since we can't compute
      // the type of anything using this type (since it will change
      // on instantiation).
      auto unknownType = UnknownType::get(context);
      ret = QualifiedType(ret.kind(), unknownType);
    }

    return ret;
  }

  // Otherwise, use a query to try to look it up.

  // Look up class/record types (used for primary method receivers)
  auto tag = parsing::idToTag(context, id);
  if (asttags::isAggregateDecl(tag)) {
    const Type* t = initialTypeForTypeDecl(context, id);
    return QualifiedType(QualifiedType::TYPE, t);
  } else if (asttags::isModule(tag)) {
    return QualifiedType(QualifiedType::MODULE, nullptr);
  } else if (asttags::isEnum(tag)) {
    const Type* t = initialTypeForTypeDecl(context, id);
    return QualifiedType(QualifiedType::TYPE, t);
  } else if (asttags::isInterface(tag)) {
    const Type* t = initialTypeForInterface(context, id);
    return QualifiedType(QualifiedType::TYPE, t);
  }

  if (asttags::isFunction(tag)) {
    // TODO: use real function types
    auto kind = qualifiedTypeKindForId(context, id);
    const Type* type = nullptr;
    return QualifiedType(kind, type);
  }

  if (id.isFabricatedId()) {
    switch (id.fabricatedIdKind()) {
      case ID::ExternBlockElement: {
        // TODO: resolve types for extern block
        // (will need the Identifier name for that)
        auto unknownType = UnknownType::get(context);
        return QualifiedType(QualifiedType::UNKNOWN, unknownType);
      }
      case ID::Generated: break;
    }
  }

  // Figure out what ID is contained within so we can use the
  // appropriate query.
  ID parentId = id.parentSymbolId(context);
  auto parentTag = parentId ? parsing::idToTag(context, parentId)
                            : asttags::AST_TAG_UNKNOWN;

  if (asttags::isModule(parentTag)) {
    // If the id is contained within a module, use typeForModuleLevelSymbol.
    bool isCurrentModule =
        parsing::idToAst(context, parentId)->toModule() == symbol->toModule();
    return typeForModuleLevelSymbol(context, id, isCurrentModule);
  }

  if (asttags::isEnum(parentTag) && asttags::isEnumElement(tag)) {
    return typeForScopeResolvedEnumElement(parentId, id, /* ambiguous */ false);
  }

  // If the id is contained within a class/record/union that we are resolving,
  // get the resolved field.
  const CompositeType* ct = nullptr;
  if (parentId == symbol->id() && inCompositeType) {
    ct = inCompositeType;
  } else {
    // TODO: this needs to consider the possibility
    // that we are working with a nested method
    if (auto rt = methodReceiverType().type()) {
      // get the new class type if the receiver is a class
      if (auto ct = rt->toClassType()) {
        if (auto mr = ct->managerRecordType(context)) {
          rt = mr;
        }
      }

      if (auto tup = rt->toTupleType()) {
        auto field = parsing::idToAst(context, id)->toNamedDecl();
        if (field && field->name() == USTR("size")) {
          // Tuples don't store a 'size' in their substitutions map, so
          // manually take care of things here.
          return QualifiedType::makeParamInt(context, tup->numElements());
        }
      }

      if (auto comprt = rt->getCompositeType()) {
        if (comprt->id() == parentId) {
          ct = comprt; // handle record, class with field
        } else if (auto bct = comprt->toBasicClassType()) {
          // if it's a class, check for parent classes to decide
          // which type corresponds to the uAST ID parentId
          while (bct != nullptr) {
            if (bct->id() == parentId) {
              // found the matching type
              ct = bct;
              break;
            }
            // otherwise, try the parent class type
            bct = bct->parentClassType();
          }
        }
      }
    }
  }

  if (ct) return lookupFieldType(*this, ct, id);

  // Otherwise it is a case not handled yet
  // TODO: handle outer function variables
  CHPL_UNIMPL("not yet handled");
  auto unknownType = UnknownType::get(context);
  return QualifiedType(QualifiedType::UNKNOWN, unknownType);
}

void Resolver::enterScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    scopeStack.push_back(scopeForId(context, ast->id()));
  }
  if (auto d = ast->toDecl()) {
    declStack.push_back(d);
  }
  tagTracker[ast->tag()] += 1;
  if (ast->isLoop()) {
    tagTracker[AstTag::START_Loop] += 1;
  }
}
void Resolver::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    CHPL_ASSERT(!scopeStack.empty());
    scopeStack.pop_back();
  }
  if (ast->isDecl()) {
    CHPL_ASSERT(!declStack.empty());
    declStack.pop_back();
  }
  tagTracker[ast->tag()] -= 1;
  if (ast->isLoop()) {
    tagTracker[AstTag::START_Loop] -= 1;
  }
}

bool Resolver::isInsideTag(uast::asttags::AstTag tag) const {
  return tagTracker[tag] > 0;
}

bool Resolver::enter(const uast::Conditional* cond) {
  auto& r = byPostorder.byAst(cond);

  // Try short-circuiting. Visit the condition to see if it is a param
  cond->condition()->traverse(*this);

  // Make sure the 'if-var' is a class type, if it exists.
  if (auto var = cond->condition()->toVariable()) {
    auto& reVar = byPostorder.byAst(var);
    if (!reVar.type().isUnknown()) {
      auto t = reVar.type().type();

      if (auto ct = t->toClassType()) {
        if (auto basicClass = ct->basicClassType()) {
          // Resolve variable type as the non-nil variant of the init type.
          // Keep management if it was borrowed or unmanaged, otherwise borrow.
          auto dec = ct->decorator();
          dec = (dec.isUnmanaged() ? dec : dec.toBorrowed()).addNonNil();
          auto newClassType = ClassType::get(context, basicClass,
                                             /* manager */ nullptr, dec);
          reVar.setType(QualifiedType(reVar.type().kind(), newClassType));
        }
      } else {
        CHPL_REPORT(context, IfVarNonClassType, cond, reVar.type());
      }
    }
  }

  auto& condType = byPostorder.byAst(cond->condition()).type();
  if (!scopeResolveOnly) {
    // Only do short-circuiting for regular resolution, not scope-resolution

    if (condType.isParamTrue()) {
      // condition is param true, might as well only resolve `then` branch
      cond->thenBlock()->traverse(*this);
      if (cond->isExpressionLevel()) {
        auto& thenType = byPostorder.byAst(cond->thenStmt(0)).type();
        r.setType(thenType);
      }
      // No need to visit children again, or visit `else` branch.
      return false;
    } else if (condType.isParamFalse()) {
      auto elseBlock = cond->elseBlock();
      if (elseBlock == nullptr) {
        // no else branch. leave the type unknown.
        return false;
      }
      elseBlock->traverse(*this);
      if (cond->isExpressionLevel()) {
        auto& elseType = byPostorder.byAst(elseBlock->stmt(0)).type();
        r.setType(elseType);
      }
      // No need to visit children again, especially `then` branch.
      return false;
    }
  }

  // We might as well visit the rest of the children here,
  // since returning `true` at this point would cause a second visit
  // to `cond->condition()`.
  auto thenBlock = cond->thenBlock();
  auto elseBlock = cond->elseBlock();

  owned<InitResolver> thenInit;
  owned<InitResolver> elseInit;

  if (initResolver) {
    thenInit = initResolver->fork();
  }
  initResolver.swap(thenInit);
  thenBlock->traverse(*this);
  initResolver.swap(thenInit);

  if (elseBlock) {
    if (initResolver) elseInit = initResolver->fork();

    initResolver.swap(elseInit);
    elseBlock->traverse(*this);
    initResolver.swap(elseInit);
  }

  if (initResolver) {
    initResolver->merge(thenInit, elseInit);
  }

  if (cond->isExpressionLevel() && !scopeResolveOnly) {
    std::vector<QualifiedType> returnTypes;
    returnTypes.push_back(byPostorder.byAst(thenBlock->stmt(0)).type());
    if (elseBlock != nullptr) {
      returnTypes.push_back(byPostorder.byAst(elseBlock->stmt(0)).type());
    }
    // with useRequiredKind = false, the QualifiedType::Kind argument
    // is ignored. Just pick a dummy value.
    auto ifType = commonType(context, returnTypes);
    if (!ifType && !condType.isUnknown()) {
      // do not error if the condition type is unknown
      r.setType(CHPL_TYPE_ERROR(context, IncompatibleIfBranches, cond,
                                returnTypes[0], returnTypes[1]));
    } else if (ifType) {
      r.setType(*ifType);
    }
  }
  return false;
}
void Resolver::exit(const uast::Conditional* cond) {
}

bool Resolver::enter(const uast::Select* sel) {
  sel->expr()->traverse(*this);

  auto exprType = byPostorder.byAst(sel->expr()).type();

  enterScope(sel);

  const Scope* scope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
  bool foundParamTrue = false;
  int otherwise = -1;

  for (int i = 0; i < sel->numWhenStmts(); i++) {
    auto when = sel->whenStmt(i);
    bool allParamFalse = true;
    bool anyParamTrue = false;

    if (when->isOtherwise()) {
      CHPL_ASSERT(otherwise == -1);
      otherwise = i;
      continue;
    }

    // Resolve all the when-cases
    for (auto caseExpr : when->caseExprs()) {
      caseExpr->traverse(*this);

      if (!scopeResolveOnly) {
        auto caseResult = byPostorder.byAst(caseExpr);
        auto caseType = caseResult.type();
        std::vector<CallInfoActual> actuals;
        actuals.push_back(CallInfoActual(exprType, UniqueString()));
        actuals.push_back(CallInfoActual(caseType, UniqueString()));
        auto ci = CallInfo (/* name */ USTR("=="),
                            /* calledType */ QualifiedType(),
                            /* isMethodCall */ false,
                            /* hasQuestionArg */ false,
                            /* isParenless */ false,
                            actuals);
        auto c = resolveGeneratedCall(caseExpr, &ci, &inScopes);
        c.noteResult(&caseResult, { { AssociatedAction::COMPARE, caseExpr->id() } });

        auto type = c.result.exprType();
        anyParamTrue = anyParamTrue || type.isParamTrue();
        allParamFalse = allParamFalse && type.isParamFalse();
        byPostorder.byAst(caseExpr).setType(type);
      }
    }

    if (!scopeResolveOnly && allParamFalse) {
      // case will never be true, so do not resolve the statement
      continue;
    } else if (when->body()->numChildren() > 0) {
      // Otherwise, resolve the body.
      when->body()->traverse(*this);
    }

    // Current behavior is to ignore when-stmts following a param-true
    // condition.
    //
    // TODO: Should we keep resolving when-cases and warn for param-true
    // cases further down the line?
    if (anyParamTrue) {
      foundParamTrue = true;
      break;
    }
  }

  // If one of the when-stmts is statically true, we should not resolve the
  // 'otherwise' statement, should one exist.
  if (foundParamTrue == false && otherwise != -1) {
    auto other = sel->whenStmt(otherwise);
    if (other->body()->numChildren() > 0) {
      other->body()->traverse(*this);
    }
  }

  exitScope(sel);

  return false;
}

void Resolver::exit(const uast::Select* sel) {
}

bool Resolver::enter(const Literal* literal) {
  ResolvedExpression& result = byPostorder.byAst(literal);
  result.setType(typeForLiteral(context, literal));
  return false;
}
void Resolver::exit(const Literal* literal) {
}

// This class can count Identifiers with a particular name
struct CountIdentifiersWithName {
  UniqueString name;
  int count = 0;

  bool enter(const Identifier* d) {
    if (d->name() == name) count++;
    return false;
  }
  void exit(const Identifier* d) { }

  // traverse everything
  bool enter(const AstNode* ast) { return true; }
  void exit(const AstNode* ast) { }
};

// TODO: this is not quite accurate, since there might be another
// Identifier that does resolve but has the same name (e.g. with shadowing).
// One alternative would be to keep a map of Identifiers that did not
// resolve and then emit errors about them when concluding the resolution
// of a function (but.. would it be a problem if errors related to
// an identifier having an unknown type appear before we say the identifier
// was not found)?
bool Resolver::identHasMoreMentions(const Identifier* ident) {
  if (symbol) {
    CountIdentifiersWithName visitor;
    visitor.name = ident->name();
    symbol->traverse(visitor);
    if (visitor.count > 1) {
      return true;
    }
  }

  return false;
}

void Resolver::issueAmbiguityErrorIfNeeded(const Identifier* ident,
                                           const Scope* scope,
                                           LookupConfig prevConfig) {

  auto pair = namesWithErrorsEmitted.insert(ident->name());
  if (pair.second) {
    // insertion took place so emit the error
    bool printFirstMention = identHasMoreMentions(ident);

    auto fHelper = getFieldDeclarationLookupHelper();
    auto helper = getMethodReceiverScopeHelper();
    std::vector<ResultVisibilityTrace> traceResult;
    auto vec = lookupNameInScopeTracing(
                        context, scope,
                        /* methodLookupHelper */ fHelper,
                        /* receiverScopeHelper */ helper,
                        ident->name(), prevConfig,
                        traceResult);

    // emit an ambiguity error if this is not resolving a called ident
    CHPL_REPORT(context, AmbiguousIdentifier,
                ident, printFirstMention, vec, traceResult);
  }
}

static LookupConfig identifierLookupConfig(bool resolvingCalledIdent) {
  LookupConfig config = IDENTIFIER_LOOKUP_CONFIG;
  if (resolvingCalledIdent) {
    config |= LOOKUP_STOP_NON_FN;
  } else {
    config |= LOOKUP_INNERMOST;
  }
  return config;
}

MatchingIdsWithName Resolver::lookupIdentifier(
      const Identifier* ident,
      bool resolvingCalledIdent,
      ParenlessOverloadInfo& outParenlessOverloadInfo) {
  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  outParenlessOverloadInfo = ParenlessOverloadInfo();

  if (ident->name() == USTR("super")) {
    // Super is a keyword, and should't be looked up in scopes. Return
    // an empty ID to indicate that this identifier points to something,
    // but that something has a special meaning.
    return MatchingIdsWithName::createWithIdAndFlags(
                                      IdAndFlags::createForBuiltinVar());
  }

  LookupConfig config = identifierLookupConfig(resolvingCalledIdent);

  auto fHelper = getFieldDeclarationLookupHelper();
  auto helper = getMethodReceiverScopeHelper();

  auto m = lookupNameInScopeWithWarnings(
                     context, scope,
                     /* methodLookupHelper */ fHelper,
                     /* receiverScopeHelper */ helper,
                     ident->name(), config, ident->id());

  if (!m.isEmpty()) {
    // We might be ambiguous, but due to having found multiple parenless procs.
    // It's not certain that this is an error; in particular, some parenless
    // procs can be ruled out if their 'where' clauses are false. If even
    // one identifier is not a parenless proc, there's an ambiguity.
    //
    // outParenlessOverloadInfo will be false if we found non-parenless-proc
    // IDs. In that case we may emit an ambiguity error later, after filtering
    // out incorrect receivers.
    outParenlessOverloadInfo =
        ParenlessOverloadInfo::fromMatchingIds(context, m);
  }

  return m;
}



static bool
checkForErrorModuleAsVariable(Context* context, const AstNode* node,
                              const ID& target) {
  auto targetTag = parsing::idToTag(context, target);

  // It shouldn't refer to a module unless the node is an identifier in one of
  // the places where module references are allowed (e.g. imports).
  if (asttags::isModule(targetTag)) {
    if (auto nodeParentId = parsing::idToParentId(context, node->id())) {
      auto nodeParentTag = parsing::idToTag(context, nodeParentId);
      if (asttags::isUse(nodeParentTag) ||
          asttags::isImport(nodeParentTag) ||
          asttags::isAs(nodeParentTag) ||
          asttags::isVisibilityClause(nodeParentTag) ||
          asttags::isDot(nodeParentTag)) {
        // OK
      } else {
        auto targetAst = parsing::idToAst(context, target);
        auto mod = targetAst->toModule();
        auto nodeParentAst = parsing::idToAst(context, nodeParentId);
        CHPL_REPORT(context, ModuleAsVariable, node, nodeParentAst, mod);
        return true;
      }
    }
  }
  return false;
}

static bool
checkForErrorNestedClassFieldRef(Context* context, const AstNode* node,
                                 const ID& target) {
  auto targetTag = parsing::idToTag(context, target);

  // If we're in a nested class, it shouldn't refer to an outer class' field.
  auto targetParentId = !Builder::astTagIndicatesNewIdScope(targetTag)
          ? target.parentSymbolId(context)
          : target;
  auto targetParentTag = parsing::idToTag(context, targetParentId);

  if (asttags::isAggregateDecl(targetParentTag) &&
      targetParentId.contains(node->id()) &&
      /* It's okay to refer to the record itself */
      targetParentId != target) {

    // Referring to a field of a class that's surrounding the current node.
    // Loop upwards looking for a composite type.
    auto searchId = node->id().parentSymbolId(context);
    while (searchId) {
      if (searchId == targetParentId) {
        // We found the aggregate type in which the to-ID is declared,
        // so there's no nested class issues.
        break;
      } else if (asttags::isTypeDecl(parsing::idToTag(context, searchId))) {
        auto targetParentAst = parsing::idToAst(context, targetParentId);
        auto searchAst = parsing::idToAst(context, searchId);
        auto searchAD = searchAst->toTypeDecl();
        // It's an error!
        CHPL_REPORT(context, NestedClassFieldRef,
                    targetParentAst->toTypeDecl(),
                    searchAD, node, target);
        return true;
      }

      // Move on to the surrounding ID.
      searchId = searchId.parentSymbolId(context);
    }
  }
  return false;
}

static bool
checkForErrorSelfDefinition(Context* context, const AstNode* node,
                            const ID& target) {
  auto targetAst = parsing::idToAst(context, target);
  if (node->isIdentifier() && target.contains(node->id())) {
    if (targetAst && targetAst->isVarLikeDecl() ) {
      auto namedDeclMaybeVar = targetAst->toVarLikeDecl();
      auto identNode = node->toIdentifier();
      if (namedDeclMaybeVar->name() == identNode->name()) {
        // This is a self-reference, so it's an error.
        CHPL_REPORT(context, SelfDefinition, namedDeclMaybeVar, identNode);
        return true;
      }
    }
  }
  return false;
}

static bool
checkForErrorUseBeforeDefine(Context* context, const AstNode* node,
                             const ID& target) {
    // treat self-definition as a special case of use-before-define
    if (checkForErrorSelfDefinition(context, node, target)) {
      return true;
    }
    if (node->tag() == AstTag::Identifier) {
      if (node->id().symbolPath() == target.symbolPath()) {
        if (target.postOrderId() > node->id().postOrderId()) {
          // resolved to an identifier defined later
          auto nd = parsing::idToAst(context, target)->toNamedDecl();
          CHPL_ASSERT(nd && "identifier target was not a NamedDecl");
          CHPL_REPORT(context, UseOfLaterVariable, node, target, nd->name());
          return true;
        }
    }
  }
  return false;
}

static const bool&
checkForIdentifierTargetErrorsQuery(Context* context, ID nodeId, ID targetId) {
  QUERY_BEGIN(checkForIdentifierTargetErrorsQuery, context, nodeId, targetId);
  bool ret = false;

  auto nodeAst = parsing::idToAst(context, nodeId);

  // Use bitwise-OR here to avoid short-circuiting.
  ret |= checkForErrorModuleAsVariable(context, nodeAst, targetId);
  ret |= checkForErrorNestedClassFieldRef(context, nodeAst, targetId);
  ret |= checkForErrorUseBeforeDefine(context, nodeAst, targetId);

  return QUERY_END(ret);
}

void Resolver::validateAndSetToId(ResolvedExpression& r,
                                  const AstNode* node,
                                  const ID& toId) {
  r.setToId(toId);
  if (!toId || toId.isFabricatedId()) return;
  auto error = checkForIdentifierTargetErrorsQuery(context, node->id(), toId);
  // If there was an error, clear the target to prevent false lookups.
  if (error) r.setToId(ID());
}

void Resolver::setToBuiltin(ResolvedExpression& r, UniqueString name) {
  QualifiedType type = typeForBuiltin(context, name);
  r.setIsBuiltin(true);

  // Some builtin types have a more useful ID than an empty ID
  ID builtinId = ID();
  if (type.hasTypePtr()) {
    if (auto cl = type.type()->toClassType()) {
      if (auto basic = cl->basicClassType()) {
        builtinId = basic->id();
      }
    } else if (auto ct = type.type()->toCompositeType()) {
      builtinId = ct->id();
    }
  }
  r.setToId(builtinId); // note: circumvents validateAndSetToId since it should
                        //       not be triggered by builtns.
  r.setType(type);
}

void Resolver::validateAndSetMostSpecific(ResolvedExpression& r,
                                          const uast::AstNode* expr,
                                          const MostSpecificCandidates& mostSpecific) {
  if (auto only = mostSpecific.only()) {
    // A single candidate was selected, either immediately or after return
    // intent overloading. Now, if calling this candidate requires a subtype
    // coercion for a 'const ref' formal, we need to emit an error: this
    // is allowed by the spec, but can't be compiled in the C backend due
    // to C's aliasing rules.

    if (only.hasConstRefCoercion()) {
      r.setType(CHPL_TYPE_ERROR(context, ConstRefCoercion, expr, only));
    }
  }

  r.setMostSpecific(mostSpecific);
}

static void maybeEmitWarningsForId(Resolver* rv, QualifiedType qt,
                                   const AstNode* astMention,
                                   ID idTarget) {
  if (astMention == nullptr || idTarget.isEmpty()) return;

  // Emit deprecation or unstable warnings if needed. Note that components
  // of use/import statements are handled entirely by the scope resolver
  // and will never be reached here. Overloaded functions are skipped via
  // the previous branch, and warnings will not be emitted until the parent
  // call is resolved. For consistency, have all calls emit errors at the
  // same place. Emit errors for other identifiers here.
  //
  // TODO: If we adjust production to emit errors for unambiguous calls
  // (and to skip doing so under '--dyno'), we can remove this branch.
  // TODO: We can skip all parenless functions using the below check, but
  // I'm not sure we can write something similar for functions, since it's
  // possible for function names to appear in other places besides calls.

  if (qt.kind() == QualifiedType::PARENLESS_FUNCTION) return;

  bool emitUnstableAndDeprecationWarnings = true;
  if (rv->nearestCalledExpression() == astMention) {
    // Note: the above conditional assumes that the astMention is
    // currently the thing being traversed (which is what makes
    // nearestCalledExpression() sufficient).
    //
    // For functions, do not warn, since call resolution will take
    // care of that.  However, if we're referring to other symbol
    // kinds, we know right now that a deprecation warning should be
    // emitted.
    emitUnstableAndDeprecationWarnings =
      !asttags::isFunction(parsing::idToTag(rv->context, idTarget));
  }

  if (emitUnstableAndDeprecationWarnings) {
    ID idMention = astMention->id();
    Context* context = rv->context;
    parsing::reportDeprecationWarningForId(context, idMention, idTarget);
    parsing::reportUnstableWarningForId(context, idMention, idTarget);
  }
}

QualifiedType Resolver::getSuperType(Context* context,
                                     const QualifiedType& sub,
                                     const Identifier* identForError) {
  // Early return: if we don't know the child type, we can't figure out the
  // parent type either.
  if (sub.isUnknownKindOrType()) {
    return QualifiedType();
  }

  if (auto classType = sub.type()->toClassType()) {
    if (auto basicClass = classType->basicClassType()) {
      auto basicParentClass = basicClass->parentClassType();
      auto newClassType = ClassType::get(context,
          basicParentClass,
          /* no manager for borrowed class */ nullptr,
          classType->decorator().toBorrowed());
      return QualifiedType(sub.kind(), newClassType);
    }
  }
  CHPL_REPORT(context, InvalidSuper, identForError, sub);
  return QualifiedType();
}

void Resolver::tryResolveParenlessCall(const ParenlessOverloadInfo& info,
                                       const Identifier* ident) {

  ResolvedExpression& r = byPostorder.byAst(ident);
  // resolve a parenless call
  std::vector<CallInfoActual> actuals;
  auto ci = CallInfo (/* name */ ident->name(),
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ false,
                      /* hasQuestionArg */ false,
                      /* isParenless */ true,
                      actuals);
  CHPL_ASSERT(!scopeStack.empty());
  auto inScope = scopeStack.back();

  // Note: this is only ever used from resolveIdentifier, so no qualifiers
  // are needed; resolve as an unqualified call.
  auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);

  // If some IDs were methods and some weren't, we have to resolve two
  // calls: one with the (implicit) receiver, and one without.
  if (info.hasMethodCandidates() && info.hasNonMethodCandidates()) {
    auto cMethod = resolveGeneratedCallInMethod(ident, &ci,
                                                &inScopes,
                                                methodReceiverType());
    auto cNonMethod = resolveGeneratedCall(ident, &ci, &inScopes);

    if (!cMethod.result.mostSpecific().isEmpty() &&
        !cNonMethod.result.mostSpecific().foundCandidates()) {
      // Only found a valid method call.
      cMethod.noteResult(&r);
    } else if (!cNonMethod.result.mostSpecific().isEmpty() &&
               !cMethod.result.mostSpecific().foundCandidates()) {
      // Only found a valid non-method call.
      cNonMethod.noteResult(&r);
    } else if (cMethod.result.mostSpecific().isEmpty() && cNonMethod.result.mostSpecific().isEmpty()) {
      // Found neither; lots of candidates, but none worked! Use handleResolvedCall
      // on cMethod to issue an error and record the result.
      cMethod.noteResult(&r);
    } else {
      // Found both, it's an ambiguity after all. Issue the ambiguity error
      // late, for which we need to recover some context.

      bool resolvingCalledIdent = nearestCalledExpression() == ident;
      LookupConfig config = identifierLookupConfig(resolvingCalledIdent);

      issueAmbiguityErrorIfNeeded(ident, inScope, config);
      auto& rr = byPostorder.byAst(ident);
      rr.setType(QualifiedType(QualifiedType::UNKNOWN,
                               ErroneousType::get(context)));

    }
  } else if (info.hasMethodCandidates()) {
    auto c = resolveGeneratedCallInMethod(ident, &ci, &inScopes,
                                          methodReceiverType());
    // save the most specific candidates in the resolution result
    c.noteResult(&r);
  } else {
    CHPL_ASSERT(info.hasNonMethodCandidates());

    // as above, but don't consider method scopes
    auto c = resolveGeneratedCall(ident, &ci, &inScopes);
    // save the most specific candidates in the resolution result
    c.noteResult(&r);
  }
}

bool Resolver::lookupOuterVariable(QualifiedType& out,
                                   const Identifier* ident,
                                   const ID& target) {
  if (!isOuterVariable(*this, ident, target)) return false;
  auto& mention = ident->id();

  QualifiedType type;
  bool isFieldAccess = parsing::idIsField(context, target);

  // Use the cached result if it exists.
  if (auto p = outerVariables.targetAndTypeOrNull(mention)) {
    type = p->second;

  } else if (isFieldAccess) {
    // If the target ID is a field, we have to walk up parent frames
    // until we find a method with a matching receiver, and then use it
    // to look up the field's type.
    for (auto f = rc->lastFrame(); f; f = f->parent(rc)) {
      auto sig = f->signature();
      if (!sig || !sig->isMethod()) continue;

      auto t = sig->formalType(0).type();
      auto ct = t ? t->getCompositeType() : nullptr;

      // TODO: What if an ID check alone is not sufficient? If I cannot
      // lookup using field ID alone, then I cannot cache with field ID.
      if (ct && ct->id() == target.parentSymbolId(context)) {
        type = lookupFieldType(*this, ct, target);
        outerVariables.add(mention, target, type);
        break;
      }
    }

  // Otherwise, it's a variable, so walk up parent frames and look up
  // the variable's type using the resolution results.
  } else if (parsing::idToParentFunctionId(context, target) ||
             parsing::idToParentInterfaceId(context, target)) {
    if (auto f = rc->findFrameWithId(target)) {
      type = f->typeForContainedId(rc, target);
      outerVariables.add(mention, target, type);
    }
  }

  // Write out the type we found (or an unset type, potentially).
  out = type;

  return true;
}


ResolvedExpression Resolver::resolveNameInModule(const UniqueString name) {
  ResolvedExpression result;

  CHPL_ASSERT(symbol->isModule());
  enterScope(symbol);

  auto parenlessInfo = ParenlessOverloadInfo();

  LookupConfig config = identifierLookupConfig(/* resolvingCalledIdent */ false);
  auto fHelper = getFieldDeclarationLookupHelper();
  auto helper = getMethodReceiverScopeHelper();
  auto ids = lookupNameInScope(context, scopeStack.back(),
                               /* methodLookupHelper */ fHelper,
                               /* receiverScopeHelper */ helper,
                               name, config);

  if (!ids.isEmpty()) {
    // see comment on lookupIdentifier.
    parenlessInfo = ParenlessOverloadInfo::fromMatchingIds(context, ids);
  }

  if (parenlessInfo.areCandidatesOnlyParenlessProcs() &&
      parenlessInfo.hasNonMethodCandidates()) {
    // See also resolveIdentifier. Ambiguous, but we might be able to disambiguate
    // using 'where' clauses.
    auto inScope = scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
    std::vector<CallInfoActual> actuals;
    auto ci = CallInfo (/* name */ name,
                        /* calledType */ QualifiedType(),
                        /* isMethodCall */ false,
                        /* hasQuestionArg */ false,
                        /* isParenless */ true,
                        actuals);

    // as above, but don't consider method scopes
    auto c = resolveGeneratedCall(symbol, &ci, &inScopes);
    c.noteResultWithoutError(&result);
    return result;
  }

  if (ids.numIds() > 1) return result;

  // One ID, not a parenless fn. Try to resolve its type "normally".
  auto id = ids.firstId();

  if (id.isEmpty()) {
    setToBuiltin(result, name);
    return result;
  }

  auto type = typeForId(id);

  // Note: not computing defaults for generic-with-defaults since use/import
  // statements don't support `bla(?)`.
  result.setType(type);
  return result;
}

static QualifiedType computeDefaultsIfNecessary(Resolver& rv,
                                                const QualifiedType& type,
                                                const ID& id,
                                                const Identifier* ident) {
  // now, for a type that is generic with defaults,
  // compute the default version when needed. e.g.
  //   record R { type t = int; }
  //   var x: R; // should refer to R(int)
  bool computeDefaults = true;
  bool resolvingCalledIdent = rv.nearestCalledExpression() == ident;

  // For calls like
  //
  //   type myType = anotherType(int)
  //
  // Use the generic version of anotherType to feed as receiver.
  if (resolvingCalledIdent) {
    computeDefaults = false;
  }

  // If we're referring to variable-ish thing, don't instantiate
  // generics. This way, `type t = someGeneric(?); t` doesn't instantiate.
  // Peformance: finding the AST is pretty expensive. Can we fold
  // the knowledge into IdAndFlags?
  if (id && asttags::isVarLikeDecl(parsing::idToTag(rv.context, id))) {
    computeDefaults = false;
  }

  // Other special exceptions like 'r' in:
  //
  //  proc r.init() { ... }
  //
  if (!rv.genericReceiverOverrideStack.empty()) {
    auto& topEntry = rv.genericReceiverOverrideStack.back();
    if ((topEntry.first.isEmpty() || topEntry.first == ident->name()) &&
        topEntry.second == parsing::parentAst(rv.context, ident)) {
      computeDefaults = false;
    }
  }

  if (computeDefaults) {
    return computeTypeDefaults(rv, type);
  }

  return type;
}

void Resolver::resolveIdentifier(const Identifier* ident) {
  ResolvedExpression& result = byPostorder.byAst(ident);

  // for 'proc f(arg:?)' need to set 'arg' to have type AnyType
  CHPL_ASSERT(declStack.size() > 0);
  const Decl* inDecl = declStack.back();
  if (inDecl->isVarLikeDecl() && ident->name() == USTR("?")) {
    result.setType(QualifiedType(QualifiedType::TYPE, getAnyType(*this, ident->id())));
    return;
  }

  // lookupIdentifier reports any errors that are needed
  bool resolvingCalledIdent = nearestCalledExpression() == ident;
  auto parenlessInfo = ParenlessOverloadInfo();
  auto ids = lookupIdentifier(ident, resolvingCalledIdent, parenlessInfo);

  // If we requested IDs including potential overloads, and found
  // both a variable and a function at the same point, we're not sure how to
  // resolve the call (via regular call resolution or 'proc this' resolution).
  // Issue an error.
  if (ids.encounteredFnNonFnConflict()) {
    result.setType(typeErr(ident, "ambiguity between function and non-function at the same scope level"));
    return;
  }

  // If we looked up a called identifier and got back several variables,
  // give up, since the lookup process only does that if they're defined
  // in the same place.
  if (resolvingCalledIdent && ids.numIds() > 1) {
    bool onlyVars = true;
    for (int i = 0; i < ids.numIds(); i++) {
      if (ids.idAndFlags(i).isFunctionLike() ){
        onlyVars = false;
        break;
      }
    }

    if (onlyVars) {
      result.setType(typeErr(ident, "ambiguous callable value in called expression"));
      return;
    }
  }

  // TODO: these errors should be enabled for scope resolution
  // but for now, they are off, as a temporary measure to enable
  // the production compiler handle these cases. To enable this,
  // we will have to adjust the dyno scope resolver to handle 'domain',
  // and probably a few other features.
  bool emitLookupErrors = !resolvingCalledIdent && !scopeResolveOnly;

  if (parenlessInfo.hasMethodCandidates() &&
      getTypeGenericity(context, methodReceiverType()) != Type::CONCRETE) {
    // Can't establish type yet, defer until receiver is instantiated.
    result.setType(QualifiedType());
  } else if (parenlessInfo.areCandidatesOnlyParenlessProcs() &&
             !scopeResolveOnly) {
    // Ambiguous, but a special case: there are many parenless functions.
    // This might be fine, if their 'where' clauses leave only one.
    //
    // Call resolution will issue an error if the overload selection fails.
    tryResolveParenlessCall(parenlessInfo, ident);
  } else if (ids.numIds() == 0) {
    if (emitLookupErrors) {
      auto pair = namesWithErrorsEmitted.insert(ident->name());
      if (pair.second) {
        // insertion took place so emit the error
        bool mentionedMoreThanOnce = identHasMoreMentions(ident);
        CHPL_REPORT(context, UnknownIdentifier, ident, mentionedMoreThanOnce);
      }
    }

    result.setType(QualifiedType());
  } else if (ids.numIds() > 1) {
    // Ambiguous. Check if we can break ambiguity by performing function resolution with
    // an implicit 'this' receiver, to filter based on receiver type.
    ID receiverId;
    QualifiedType receiverType;

    bool allowNonLocal = true;
    auto receiverInfo = closestMethodReceiverInfo(allowNonLocal);
    if (receiverInfo) receiverType = std::get<1>(*receiverInfo);

    if (receiverInfo && receiverType.type()) {
      std::vector<CallInfoActual> actuals;
      actuals.push_back(CallInfoActual(receiverType, USTR("this")));
      auto ci = CallInfo(/* name */ ident->name(),
                         /* calledType */ QualifiedType(),
                         /* isMethodCall */ true,
                         /* hasQuestionArg */ false,
                         /* isParenless */ true, actuals);
      auto inScope = scopeStack.back();
      auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
      auto c = resolveGeneratedCall(ident, &ci, &inScopes);
      MatchingIdsWithName redeclarations;
      inScope->lookupInScope(ident->name(), redeclarations, IdAndFlags::Flags(),
                             IdAndFlags::FlagSet());
      if (c.result.mostSpecific().numBest() == 1) {
        // A local variable would be ambiguous with a paren-less method, so
        // let's check for redeclarations within the current method.
        if (!redeclarations.isEmpty()) {
          auto only = c.result.mostSpecific().only();
          bool otherThanParenless = false;
          for (auto& elt : redeclarations) {
            if (only.fn()->id() != elt) {
              otherThanParenless = true;
              break;
            }
          }

          if (otherThanParenless) {
            LookupConfig config = identifierLookupConfig(resolvingCalledIdent);
            issueAmbiguityErrorIfNeeded(ident, inScope, config);
          }
        } else {
          // Save result if successful
          if (c.noteResultWithoutError(&result) &&
              emitLookupErrors) {
            c.issueBasicError();
          }
        }
      }
    } else {
      // Can't establish the type. If this is in a function
      // call, we'll establish it later anyway.
      result.setType(QualifiedType());
    }
  } else {
    // just a single match
    const ID& id = ids.firstId();
    QualifiedType type;

    // empty IDs from the scope resolution process are builtins or super
    if (id.isEmpty() && isPotentialSuper(ident, &type)) {
      result.setType(getSuperType(context, type, ident));
      return;
    } else if (id.isEmpty()) {
      setToBuiltin(result, ident->name());
      result.setType(computeDefaultsIfNecessary(*this, result.type(), id, ident));
      return;
    }

    validateAndSetToId(result, ident, id);

    // Before computing the type, handle field use inside an initializer.
    //
    // NB: this is important for parent fields, which may result in an
    //   implicit 'super.init()' call, which may instantiate a generic field.
    if (initResolver) {
      std::ignore = initResolver->handleUseOfField(ident);
    }

    // Attempt to lookup an outer variable.
    if (!lookupOuterVariable(type, ident, id)) {

      // Otherwise, use the type established at declaration/initialization
      type = typeForId(id);
    }

    maybeEmitWarningsForId(this, type, ident, id);

    if (type.kind() == QualifiedType::TYPE) {
      type = computeDefaultsIfNecessary(*this, type, id, ident);
    // Do not resolve function calls under 'scopeResolveOnly'
    } else if (type.kind() == QualifiedType::PARENLESS_FUNCTION) {
      CHPL_ASSERT(scopeResolveOnly && "resolution of parenless functions should've happened above");

      // Possibly a "compatibility hack" with production: we haven't checked
      // whether the call is valid, but the production scope resolver doesn't
      // care and assumes `ident` points to this parenless function. Setting
      // the toId also helps determine if this is a method call and should
      // have `this` inserted, as well as whether or not to turn this
      // into a parenless call.

      // Fall through
    } else if (scopeResolveOnly &&
               type.kind() == QualifiedType::FUNCTION) {
      // Possibly a "compatibility hack" with production: we haven't checked
      // whether the call is valid, but the production scope resolver doesn't
      // care and assumes `ident` points to this function. Setting
      // the toId also helps determine if this is a method call

      // Fall through
    }

    result.setType(type);
    // if there are multiple ids we should have gotten
    // a multiple definition error at the declarations.
  }
}

bool Resolver::enter(const Identifier* ident) {
  resolveIdentifier(ident);
  return false;
}

void Resolver::exit(const Identifier* ident) {
}

bool Resolver::enter(const uast::Init* init) {
  return true;
}

void Resolver::exit(const uast::Init* init) {
  if (initResolver) {
    std::ignore = initResolver->handleInitStatement(init);
  }
}

bool Resolver::enter(const TypeQuery* tq) {
  if (usePlaceholders) {
    // If we're resolving an interface, create a placeholder for the type
    // query. This way, we get a concrete type for `foo(?x)`, which is
    // desireable when validating user-provided functions against the
    // interface signature.
    ResolvedExpression& result = byPostorder.byAst(tq);
    result.setType(QualifiedType(QualifiedType::TYPE,
                                 PlaceholderType::get(context, tq->id())));
    return false;
  }

  if (skipTypeQueries) {
    return false;
  }

  // Consider 'proc f(arg:?t)'
  //   * if there is no substitution for 'arg', 't' should be AnyType
  //   * if there is a substitution for 'arg', 't' should be computed from it

  // Find the parent Formal and check for a substitution for that Formal
  const VarLikeDecl* formal = nullptr;
  bool foundFormalSubstitution = false;
  QualifiedType foundFormalType;
  for (auto it = declStack.rbegin(); it != declStack.rend(); ++it) {
    const Decl* d = *it;
    if (auto fml = d->toFormal()) {
      formal = fml;
      break;
    } else if (auto varargs = d->toVarArgFormal()) {
      formal = varargs;
    }
  }
  if (formal != nullptr) {
    if (substitutions != nullptr) {
      auto search = substitutions->find(formal->id());
      if (search != substitutions->end()) {
        foundFormalSubstitution = true;
        foundFormalType = search->second;
      }
    }
  }

  ResolvedExpression& result = byPostorder.byAst(tq);

  if (!foundFormalSubstitution) {
    // No substitution (i.e. initial signature) so use AnyType
    if (!callNodeStack.empty() && isCallToIntEtc(callNodeStack.back())) {
      auto defaultInt = IntType::get(context, 0);
      result.setType(QualifiedType(QualifiedType::PARAM, defaultInt));
    } else {
      // the type query could refer to a param or to a type,
      // so use the TYPE_QUERY kind
      result.setType(QualifiedType(QualifiedType::TYPE_QUERY,
                                   AnyType::get(context)));
    }
  } else {
    // Found a substitution after instantiating, so gather the components
    // of the type. We do this in a way that handles all TypeQuery
    // nodes within the Formal uAST node.
    resolveTypeQueriesFromFormalType(formal, foundFormalType);
  }

  return false;
}
void Resolver::exit(const TypeQuery* tq) {
}

// Treat receiver types specially in terms of generic resolution. That is,
// when resolving the following initializer when r is generic with defaults,
//
//   proc r.init() {}
//
// Make sure r's defaults aren't used so that the most general receiver is
// constructed. On the other hand, defaults _should_ be used for more
// complicated expressions:
//
//   proc (someTypeFn(r)).init() {}
//
static bool shouldUseGenericTypeForTypeExpr(const NamedDecl* decl) {
 return decl->isFormal() && decl->name() == USTR("this");
}

bool Resolver::enter(const NamedDecl* decl) {

  if (decl->id().isSymbolDefiningScope()) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.
    return false;
  }

  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  // Silence redefinition warnings for enum elements because we have
  // a special error (DuplicateEnumElement) for those.
  if (!decl->isEnumElement()) {
    emitMultipleDefinedSymbolErrors(context, scope);
  }

  enterScope(decl);

  if (shouldUseGenericTypeForTypeExpr(decl)) {
    // Empty string indicates that all identifiers should be treated as
    // non-defaulted. Using 'decl' means that only the top-level identifiers
    // will be resolved this way.
    genericReceiverOverrideStack.emplace_back(UniqueString(), decl);
  }

  // This logic exists to prioritize the field's type expression when
  // resolving a field's type. If the type expression is concrete, then we
  // do not need to resolve the init-expression. This is beneficial in cases
  // where the init-expression would otherwise result in recursive resolution.
  //
  // TODO: Initializers will need to check the compatibility of the type
  // expression and initialization expression.
  if (!scopeResolveOnly && fieldTypesOnly && decl == curStmt) {
    auto field = decl->toVarLikeDecl();
    if (field->typeExpression() != nullptr &&
        field->initExpression() != nullptr &&
        field->storageKind() != Qualifier::TYPE &&
        field->storageKind() != Qualifier::PARAM) {
      field->typeExpression()->traverse(*this);
      auto res = byPostorder.byAst(field->typeExpression());
      auto g = getTypeGenericity(context, res.type().type());
      if (g != Type::CONCRETE) {
        field->initExpression()->traverse(*this);
      }
      return false;
    } else {
      return true;
    }
  } else {
    return true;
  }
}

void Resolver::exit(const NamedDecl* decl) {
  if (shouldUseGenericTypeForTypeExpr(decl)) {
    genericReceiverOverrideStack.pop_back();
  }

  // Stop resolving if the declaration introduces a new path component.
  auto idChild = decl->id();
  if (idChild.isSymbolDefiningScope()) {
    return;
  }

  resolveNamedDecl(decl, /* useType */ nullptr);

  if (decl->isFormal() && decl->name() == USTR("this")) {
    allowReceiverScopes = true;
    // when needed, the receiver scopes can be computed, now that
    // 'this' has been resolved.
  }

  exitScope(decl);
}

bool Resolver::enter(const uast::Manage* manage) {
  for (auto managerExpr : manage->managers()) {
    const As* as = managerExpr->toAs();
    const Variable* asVar = nullptr;
    if (as) {
      managerExpr = as->symbol();
      asVar = as->rename()->toVariable();
      CHPL_ASSERT(asVar);
    }
    managerExpr->traverse(*this);

    if (scopeResolveOnly) continue;

    auto& rr = byPostorder.byAst(managerExpr);
    if (rr.type().isUnknownOrErroneous()) continue;

    // Ok, we've resolved the manager expression fine. First, resolve
    // the stdlib function `chpl__verifyTypeContext`, which ensures
    // that the context manager supports the `contextManager` interface.
    auto ci = CallInfo(UniqueString::get(context, "chpl__verifyTypeContext"),
                       /* calledType */ QualifiedType(),
                       /* isMethodCall */ false,
                       /* hasQuestionArg */ false,
                       /* isParenless */ false,
                       /* actuals */ {CallInfoActual(rr.type(), UniqueString())});
    auto inScopes = CallScopeInfo::forNormalCall(scopeStack.back(), poiScope);
    auto c = resolveGeneratedCall(manage, &ci, &inScopes);
    c.noteResult(&byPostorder.byAst(manage));
    CHPL_ASSERT(c.result.mostSpecific().only());

    // Now, we actually want the witness for the interface if one exists.
    // Use the POI scope from the call so that POI is the same as if we were
    // resolving from inside the body of chpl__verifyTypeContext. This
    // should improve re-use of interface search.
    auto inScopesForInterface =
      CallScopeInfo::forNormalCall(scopeStack.back(), c.result.poiInfo().poiScope());
    auto contextManagerInterface = InterfaceType::getContextManagerType(context);
    bool ignoredFoundExisting;
    auto witness =
      findOrImplementInterface(rc, contextManagerInterface, rr.type().type(),
                               inScopesForInterface, c.result.mostSpecific().only().fn()->id(),
                               ignoredFoundExisting);

    if (!witness) {
      auto errType = typeErr(managerExpr, "'manage' statements are only for types implementing 'contextManager'");
      if (asVar) {
        byPostorder.byAst(asVar).setType(errType);
      }
      continue;
    }

    Access accessContext = Access::VALUE;
    if (asVar) {
      // We're storing the result of entering the context manager. Determine
      // the type it yields and use it to resolve that expression.
      auto contextReturnTypeStr = UniqueString::get(context, "contextReturnType");
      auto& contextReturnTypeId =
        contextManagerInterface->idForAssociatedType(context, contextReturnTypeStr);
      auto& contextReturnType = witness->associatedTypes().at(contextReturnTypeId);


      // This is effectively a variable declaration with an initialization
      // expression being 'enterContext()'. The same semantics
      // are expected. As a result, we call resolveNamedDecl to handle this.
      //
      // Performance: We are taking a very simple path through resolveNamedDecl
      // here (most conditions it checks for are 'false': finding substitutions,
      // detecting 'this' formals, handling 'extern' variables). However,
      // it seems good to share the general logic.
      //
      // It's possible for the enterContext() method to e.g., provide a value
      // when the declaration is a ref. We don't check for that here specifically,
      // since the behavior ought to be the same as trying to assign a value to
      // a ref variable. However, 'resolveNamedDecl' does not handle that case
      // at the time of writing, and therefore we effectively don't handle it
      // here either.
      resolveNamedDecl(asVar, contextReturnType);

      auto kind = byPostorder.byAst(asVar).type().kind();
      if (kind == QualifiedType::INDEX) {
        // Intent is explicitly left unknown. When inferring (see
        // preFold.cpp, FLAG_MANAGER_RESOURCE_INFER_STORAGE), the old logic
        // was to determine the const-ness from use. This used to match
        // formals (where the intent was ref-if-modified). This behavior
        // has been deprecated in favor of explicitly requesting a reference.
        // To match, here, request a 'const ref' by default. The user will
        // have to write 'ref' to get that overload.
        accessContext = Access::CONST_REF;
      } else {
        accessContext = accessForQualifier(byPostorder.byAst(asVar).type().kind());
      }
    }

    // Since we're in a manage statement, we will call 'enter' and 'exit',
    // so note those as associated actions. For 'enterContext', there may
    // be several overloads, so pick the best one.
    const TypedFnSignature* enterSig = nullptr;
    const TypedFnSignature* exitSig = nullptr;
    for (auto& [id, fn] : witness->requiredFns()) {
      if (fn->untyped()->name() == USTR("enterContext")) {

        // If several overloads were stored, use return intent overload
        // resolution now to pick the best one. Importantly, this is done
        // separately from return intent resolution in maybe-const.cpp
        // because we need to know the right overload _now_ because these
        // are generated calls created when satisfying an interface,
        // and thus not tracked via a MostSpecificCandidates saved in
        // a ResolvedExpression.
        auto overloadsIt = witness->returnIntentOverloads().find(id);
        if (overloadsIt != witness->returnIntentOverloads().end()) {
          bool ambiguity;
          auto bestCandidate =
            determineBestReturnIntentOverload(overloadsIt->second,
                                              accessContext,
                                              ambiguity);
          CHPL_ASSERT(!ambiguity);
          CHPL_ASSERT(bestCandidate);
          enterSig = bestCandidate->fn();
        } else {
          enterSig = fn;
        }
      } else if (fn->untyped()->name() == USTR("exitContext")) {
        exitSig = fn;
      } else {
        CHPL_ASSERT(false && "unexpected function in contextManager interface");
      }
    }
    CHPL_ASSERT(enterSig && exitSig);
    rr.addAssociatedAction(AssociatedAction::ENTER_CONTEXT, enterSig, manage->id());
    rr.addAssociatedAction(AssociatedAction::EXIT_CONTEXT, exitSig, manage->id());
  }

  enterScope(manage);
  for (auto stmt : manage->stmts()) {
    stmt->traverse(*this);
  }

  return false;
}

void Resolver::exit(const uast::Manage* manage) {
  exitScope(manage);
}

static void getVarLikeOrTupleTypeInit(const AstNode* ast,
                                      const AstNode*& typeExpr,
                                      const AstNode*& initExpr) {
  typeExpr = nullptr;
  initExpr = nullptr;
  if (auto v = ast->toVarLikeDecl()) {
    if (auto t = v->typeExpression()) {
      typeExpr = t;
    }
    if (auto e = v->initExpression()) {
      initExpr = e;
    }
  } else if (auto td = ast->toTupleDecl()) {
    if (auto t = td->typeExpression()) {
      typeExpr = t;
    }
    if (auto e = td->initExpression()) {
      initExpr = e;
    }
  }
}

bool Resolver::enter(const MultiDecl* decl) {
  enterScope(decl);

  // Traversal is done in exit
  return false;
}

void Resolver::exit(const MultiDecl* decl) {
  // Move through decls in order, propagating type backwards each time one with
  // a type or init is encountered.
  // This effectively splits the decls into groups that share a type/init.
  auto it = decl->decls().begin();
  auto groupBegin = it;
  const AstNode* curTypeExpr = nullptr;
  const AstNode* curInitExpr = nullptr;
  while (it != decl->decls().end()) {
    const Decl* individualDecl = *it;

    // Resolve type and init expressions if present.
    getVarLikeOrTupleTypeInit(individualDecl, curTypeExpr, curInitExpr);
    if (curTypeExpr != nullptr) {
      curTypeExpr->traverse(*this);
    }
    if (curInitExpr != nullptr) {
      curInitExpr->traverse(*this);
    }

    if (!scopeResolveOnly && (curTypeExpr || curInitExpr)) {
      // Decl with type/init encountered, resolve and propagate the type info
      // backwards through its group.
      auto groupEnd = std::next(it);
      auto groupIt = groupEnd;
      const Type* lastType = nullptr;
      while (groupIt != groupBegin) {
        const Decl* d = *(--groupIt);

        // Skip extracting type/init if we're on the last decl in the group
        // (first inner loop iteration), as we've already just done so in the
        // current iteration of the outer loop.
        // Take null lastType to mean we're on the first inner iteration.
        if (lastType) getVarLikeOrTupleTypeInit(d, curTypeExpr, curInitExpr);

        // if it has neither init nor type, use the type from the
        // variable to the right.
        // e.g., in
        //    var a, b: int
        // a is of type int
        const Type* t = nullptr;
        if (curTypeExpr == nullptr && curInitExpr == nullptr) {
          if (lastType == nullptr) {
            // this could be split init
            t = UnknownType::get(context);
          } else {
            t = lastType;
          }
        }

        // for the functions called in these conditionals:
        //  * if t is nullptr, just resolve it like usual
        //  * update the type of d in byPostorder
        if (auto v = d->toVarLikeDecl()) {
          resolveNamedDecl(v, t);
        } else if (auto td = d->toTupleDecl()) {
          resolveTupleDecl(td, t);
        }

        // update lastType
        ResolvedExpression& result = byPostorder.byAst(d);
        lastType = result.type().type();
      }

      // Advance to beginning of next group
      groupBegin = groupEnd;
    }

    ++it;
  }

  exitScope(decl);
}

bool Resolver::enter(const TupleDecl* decl) {
  enterScope(decl);

  // TODO: Can we just do this every time we 'enterScope'?
  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  emitMultipleDefinedSymbolErrors(context, scope);

  // Establish the type of the type expr / init expr within
  if (auto t = decl->typeExpression()) {
    t->traverse(*this);
  }
  if (auto e = decl->initExpression()) {
    e->traverse(*this);
  }
  return false;
}

void Resolver::exit(const TupleDecl* decl) {
  resolveTupleDecl(decl, /* useType */ nullptr);
  exitScope(decl);
}

bool Resolver::enter(const Range* range) {
  return true;
}
void Resolver::exit(const Range* range) {
  if (scopeResolveOnly) {
    return;
  }

  // Encode the type of the range as two bits: bounded below, bounded above.
  int boundType = (range->lowerBound() != nullptr) << 1 |
                  (range->upperBound() != nullptr);

  // Decide which Chapel function to call for this.
  static const char* functions[] = {
    "chpl_build_unbounded_range",
    "chpl_build_high_bounded_range",
    "chpl_build_low_bounded_range",
    "chpl_build_bounded_range"
  };
  const char* function = functions[boundType];

  std::vector<CallInfoActual> actuals;
  std::vector<const AstNode*> actualAsts;
  if (range->lowerBound()) {
    actuals.emplace_back(/* type */ byPostorder.byAst(range->lowerBound()).type(),
                         /* byName */ UniqueString());
    actualAsts.push_back(range->lowerBound());
  }
  if (range->upperBound()) {
    actuals.emplace_back(/* type */ byPostorder.byAst(range->upperBound()).type(),
                         /* byName */ UniqueString());
    actualAsts.push_back(range->upperBound());
  }


  auto ci = CallInfo(/* name */ UniqueString::get(context, function),
                     /* calledType */ QualifiedType(),
                     /* isMethodCall */ false,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false, actuals);

  // Skip calls when the bounds are unknown to avoid putting function resolution
  // into an awkward position.
  auto skip = shouldSkipCallResolution(this, range, actualAsts, ci);
  if (!skip) {
    auto scope = scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
    auto c = resolveGeneratedCall(range, &ci, &inScopes);
    c.noteResult(&byPostorder.byAst(range));
  } else {
    auto& r = byPostorder.byAst(range);
    r.setType(QualifiedType());
  }
}

bool Resolver::enter(const uast::Array* decl) {
  return true;
}
void Resolver::exit(const uast::Array* decl) {
  ResolvedExpression& r = byPostorder.byAst(decl);

  if (!decl->isMultiDim()) {
    std::vector<CallInfoActual> actuals;
    for (auto expr : decl->exprs()) {
      auto exprType = byPostorder.byAst(expr).type();
      // Short circuit if any elements have unknown type, since we won't be
      // able to resolve the array builder proc.
      if (exprType.isUnknown()) {
        r.setType(QualifiedType());
        return;
      }
      actuals.emplace_back(exprType, UniqueString());
    }

    static auto arrayBuilderProc =
        UniqueString::get(context, "chpl__buildArrayExpr");
    auto ci = CallInfo(/* name */ arrayBuilderProc,
                       /* calledType */ QualifiedType(),
                       /* isMethodCall */ false,
                       /* hasQuestionArg */ false,
                       /* isParenless */ false, actuals);
    auto scope = scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
    auto c = resolveGeneratedCall(decl, &ci, &inScopes);

    c.noteResult(&r);
  } else {
    CHPL_UNIMPL("multidimensional array literals");
  }
}

bool Resolver::enter(const uast::Domain* decl) {
  return true;
}

void Resolver::exit(const uast::Domain* decl) {
  if (scopeResolveOnly) {
    return;
  }

  const DomainType* genericDomainType = DomainType::getGenericDomainType(context);

  if (decl->numExprs() == 0) {
    // Generic domain, as in a generic-domain array
    auto& re = byPostorder.byAst(decl);
    auto dt = QualifiedType(QualifiedType::CONST_VAR, genericDomainType);
    re.setType(dt);
  } else {
    // Call appropriate domain builder proc. Use ensureDomainExpr when the
    // domain is declared without curly braces (within an array type).
    const char* domainBuilderProc = decl->usedCurlyBraces()
                                        ? "chpl__buildDomainExpr"
                                        : "chpl__ensureDomainExpr";

    // Add key or range actuals
    std::vector<CallInfoActual> actuals;
    bool freshDomainQuery = false;
    for (auto expr : decl->exprs()) {
      auto exprType = byPostorder.byAst(expr).type();

      // If it's a type query, we may be looking at [?D] (where a Domain node
      // is implicitly created in the array expression AST). In that case,
      // we want the fully generic domain type.
      if (expr->isTypeQuery() && exprType.type() && exprType.type()->isAnyType()) {
        freshDomainQuery = true;
        break;
      }

      actuals.emplace_back(exprType, UniqueString());
    }

    if (freshDomainQuery) {
      if (decl->numExprs() > 1 || decl->usedCurlyBraces()) {
        // We can only query the whole domain using a type query, so reject
        // the domain expression.
        context->error(decl, "cannot query part of a domain");
      }

      auto& re = byPostorder.byAst(decl);
      auto dt = QualifiedType(QualifiedType::CONST_VAR, genericDomainType);
      re.setType(dt);

      // No need to perform the call to chpl__buildDomainExpr etc.
      return;
    }

    // Add definedConst actual if appropriate
    if (decl->usedCurlyBraces()) {
      actuals.emplace_back(QualifiedType::makeParamBool(context, true)),
          UniqueString();
    }

    auto ci = CallInfo(/* name */ UniqueString::get(context, domainBuilderProc),
                       /* calledType */ QualifiedType(),
                       /* isMethodCall */ false,
                       /* hasQuestionArg */ false,
                       /* isParenless */ false,
                       actuals);
    auto scope = scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
    auto c = resolveGeneratedCall(decl, &ci, &inScopes);

    ResolvedExpression& r = byPostorder.byAst(decl);
    c.noteResult(&r);
  }
}

types::QualifiedType Resolver::typeForBooleanOp(const uast::OpCall* op) {
  if (op->numActuals() != 2) {
    return typeErr(op, "invalid op call");
  }

  bool isAnd = op->op() == USTR("&&");
  // visit the LHS
  op->actual(0)->traverse(*this);
  // look at the LHS type. Is it param?
  const QualifiedType& lhs = byPostorder.byAst(op->actual(0)).type();
  // can we short circuit? e.g., is this false && x, or true || y?
  bool shortCircuit = isAnd ? lhs.isParamFalse() : lhs.isParamTrue();
  if (shortCircuit) {
    // short circuit, no need to evaluate RHS.
    return lhs;
  }

  // go ahead and evaluate the RHS
  op->actual(1)->traverse(*this);
  // look at the RHS type.
  const QualifiedType& rhs = byPostorder.byAst(op->actual(1)).type();

  // are we looking at true && true or false || false?
  bool bothIdentity = isAnd
    ? (lhs.isParamTrue() && rhs.isParamTrue())
    : (lhs.isParamFalse() && rhs.isParamFalse());
  if (bothIdentity) {
    // true && true == true, false || false == false.
    // return lhs type.
    return lhs;
  } else if (lhs.isUnknown() || rhs.isUnknown()) {
    // if one is unknown, return unknown
    return QualifiedType(QualifiedType::CONST_VAR,
                         UnknownType::get(context));
  } else {
    CHPL_ASSERT(rhs.type()->isBoolType() && lhs.type()->isBoolType());
    if (rhs.isParam() && lhs.isParam()) {
      // preserve param-ness
      // this case is only hit when the result is false (for &&)
      // or when the result is true (for ||), so return !isAnd.
      return QualifiedType::makeParamBool(context, !isAnd);
    } else {
      // otherwise just return a Bool value
      return QualifiedType(QualifiedType::CONST_VAR,
                             BoolType::get(context));
    }
  }
}

bool Resolver::enter(const Call* call) {
  // At this time, we don't allow method calls in inheritance expressions,
  // so we assume that there can't be overloading etc.
  if (call != curInheritanceExpr) {
    callNodeStack.push_back(call);
  }
  auto op = call->toOpCall();

  if (op && initResolver) {
    initResolver->doDetectPossibleAssignmentToField(op);
  }

  // Scope resolve without short-circuiting if we're only scope resolving.
  if (scopeResolveOnly) {
    return true;
  }

  // handle && and || to not bother to evaluate the RHS
  // if the LHS is param and false/true, respectively.
  if (op && (op->op() == USTR("&&") || op->op() == USTR("||"))) {
    QualifiedType result = typeForBooleanOp(op);
    // Update the type of the && call
    byPostorder.byAst(op).setType(result);
    // Don't visit the children since we already did
    return false;
  }

  // Do not descend into children for 'prim resolves' since it accepts a single
  // _expression_ (not a value!) and checks if it resolves.
  if (auto primCall = call->toPrimCall()) {
    if (primCall->prim() == PRIM_RESOLVES) {
      return false;
    }
  }

  return true;
}

void Resolver::prepareCallInfoActuals(const Call* call,
                                      std::vector<CallInfoActual>& actuals,
                                      const AstNode*& questionArg,
                                      std::vector<const uast::AstNode*>* actualAsts) {
  CallInfo::prepareActuals(context, call, byPostorder,
                           /* raiseErrors */ true,
                           actuals, questionArg,
                           actualAsts);
}

static const Type* getGenericType(Context* context, const Type* recv) {
  const Type* gen = nullptr;
  if (auto cur = recv->toRecordType()) {
    gen = cur->instantiatedFromCompositeType();
    if (gen == nullptr) gen = cur;
  } else if (auto bct = recv->toBasicClassType()) {
    if (auto pct = bct->parentClassType()) {
      if (pct->instantiatedFromCompositeType()) {
        auto pt = getGenericType(context, pct)->toBasicClassType();
        bct = BasicClassType::get(context, bct->id(), bct->name(), pt, bct->instantiatedFrom(), bct->substitutions());
      }
    }

    gen = bct->instantiatedFromCompositeType();
    if (gen == nullptr) gen = bct;
  } else if (auto cur = recv->toClassType()) {
    auto m = getGenericType(context, cur->manageableType());
    gen = ClassType::get(context, m->toManageableType(),
                         cur->manager(), cur->decorator());
  } else if (recv->isDomainType()) {
    gen = DomainType::getGenericDomainType(context);
  } else if (recv->isArrayType()) {
    gen = ArrayType::getGenericArrayType(context);
  }
  return gen;
}

static const bool& warnForMissingIterKindEnum(Context* context,
                                              const AstNode* astForErr) {
  QUERY_BEGIN(warnForMissingIterKindEnum, context, astForErr);
  context->warning(astForErr, "resolving parallel iterators is not supported "
                              "without module code");
  return QUERY_END(true);
}

static const QualifiedType&
getIterKindConstantOrWarn(Context* context,
                          const AstNode* astForErr,
                          Function::IteratorKind iterKind) {
  QUERY_BEGIN(getIterKindConstantOrWarn, context, astForErr, iterKind);

  auto iterKindActual = getIterKindConstantOrUnknown(context, iterKind);
  bool needSerial = iterKind == Function::SERIAL;

  // Exit early if we need a parallel iterator and don't have the enum.
  if (!needSerial && iterKindActual.isUnknown()) {
    warnForMissingIterKindEnum(context, astForErr);
  }

  return QUERY_END(iterKindActual);
}

static optional<CallResolutionResult>
rerunCallInfoWithIteratorTag(ResolutionContext* rc,
                             const uast::Call* call,
                             ResolvedExpression& r,
                             const CallInfo& ci,
                             const CallScopeInfo& inScopes,
                             QualifiedType receiverType,
                             uast::Function::IteratorKind iterKind) {
  auto iterKindActual = getIterKindConstantOrWarn(rc->context(), call, iterKind);
  if (iterKindActual.isUnknown()) return empty;

  std::vector<CallInfoActual> actuals;
  for (const auto& actual : ci.actuals())
    actuals.push_back(actual);
  actuals.emplace_back(iterKindActual, USTR("tag"));

  auto newCi = CallInfo(ci.name(), ci.calledType(), ci.isMethodCall(),
      ci.hasQuestionArg(), ci.isParenless(), actuals);

  auto newC = resolveCallInMethod(rc, call, newCi, inScopes, receiverType,
      /* rejected */ nullptr);

  // Also note the call as an associated action
  if (!newC.mostSpecific().isEmpty()) {
    for (auto sig : newC.mostSpecific()) {
      if (!sig) continue;
      r.addAssociatedAction(AssociatedAction::ITERATE, sig.fn(), call->id());
    }

    return newC;
  }

  return empty;
}

// Invokes resolveCallInMethod, and if that fails due to iterator candidates
// (e.g., we called foo() but only foo(standalone) is in scope), re-attempts
// the resolution with the other candidates.
//
// Note that the order here should match resolveIterDetailsInPriorityOrder.
static Resolver::CallResultWrapper
resolveCallInMethodReattemptIfNeeded(Resolver& r,
                                     const uast::Call* call,
                                     ResolvedExpression& rr,
                                     const CallInfo& ci,
                                     const CallScopeInfo& inScopes,
                                     QualifiedType receiverType,
                                     std::vector<const AstNode*>& actuals) {
  auto c = r.resolveCallInMethod(call, &ci, &inScopes,
                                 receiverType, actuals);

  // Other overloads are present and may be usable to fill in for 'foo()'.
  if (c.result.mostSpecific().isEmpty() && c.result.rejectedPossibleIteratorCandidates()) {
    if (auto standalone =
          rerunCallInfoWithIteratorTag(r.rc, call, rr, ci, inScopes, receiverType,
                                       Function::STANDALONE)) {
      c.result = std::move(*standalone);
    } else if (auto parallel =
                rerunCallInfoWithIteratorTag(r.rc, call, rr, ci, inScopes, receiverType,
                                             Function::LEADER)) {
      c.result = std::move(*parallel);
    }
  }
  return c;
}

static void noteWarningForIteratorDefinedElsewhere(Resolver& rv,
                                                   const uast::Call* callForErr,
                                                   const Scope* expectedScope,
                                                   const Scope* callScope,
                                                   const FnIteratorType* fnIterType,
                                                   Function::IteratorKind iterKind,
                                                   std::vector<const TypedFnSignature*>& warningCauses) {
  auto otherIter = findTaggedIteratorForType(rv.rc, fnIterType, iterKind, callScope);
  if (otherIter) {
    auto otherScope = scopeForId(rv.context, otherIter.fn()->id())->parentScope();
    if (expectedScope != otherScope) {
      warningCauses.push_back(otherIter.fn());
    }
  }
}

static void
issueIteratorDiagnosticsIfNeeded(Resolver& rv,
                                 const uast::Call* call,
                                 const CallScopeInfo& inScopes,
                                 const CallResolutionResult& c) {
  const FnIteratorType* iterType = nullptr;
  if (!c.exprType().isUnknownOrErroneous() &&
      (iterType = c.exprType().type()->toFnIteratorType())) {
    const Scope* myScope =
      scopeForId(rv.context, iterType->iteratorFn()->id())->parentScope();
    std::vector<const TypedFnSignature*> ignoredItersInCallScope;

    std::vector<std::tuple<Function::IteratorKind, QualifiedType, const TypedFnSignature*>> yieldTypes;
    static const Function::IteratorKind iterKinds[] =
      { Function::SERIAL, Function::STANDALONE, Function::FOLLOWER };

    for (auto iterKind : iterKinds) {
      auto qt = taggedYieldTypeForType(rv.rc, iterType, iterKind);
      if (!qt.isUnknownOrErroneous()) {
        yieldTypes.emplace_back(iterKind, qt, nullptr);
      }

      // Also try finding tagged iterator in the current scope, to warn about
      // invalid overloading (you can't change iterator groups in other scopes).
      noteWarningForIteratorDefinedElsewhere(rv, call, myScope,
                                             inScopes.callScope(),
                                             iterType, iterKind,
                                             ignoredItersInCallScope);
    }

    // We don't check the leader for yield types, but we do warn about
    // leaders defined out-of-scope.
    noteWarningForIteratorDefinedElsewhere(rv, call, myScope,
                                           inScopes.callScope(),
                                           iterType, Function::LEADER,
                                           ignoredItersInCallScope);

    bool incompatibleYieldTypes = false;
    if (yieldTypes.size() > 1) {
      for (size_t i = 1; i < yieldTypes.size(); i++) {
        if (std::get<1>(yieldTypes[i]) != std::get<1>(yieldTypes[0])) {
          incompatibleYieldTypes = true;
          break;
        }
      }
    }

    if (incompatibleYieldTypes) {
      for (auto& foundIter : yieldTypes) {
        std::get<2>(foundIter) =
          findTaggedIteratorForType(rv.rc, iterType, std::get<0>(foundIter)).fn();
      }
      CHPL_REPORT(rv.context, IncompatibleYieldTypes, call, std::move(yieldTypes));
    }

    if (!ignoredItersInCallScope.empty()) {
      CHPL_REPORT(rv.context, IteratorsInOtherScopes, call, iterType->iteratorFn(),
                  std::move(ignoredItersInCallScope));
    }
  }
}

void Resolver::handleCallExpr(const uast::Call* call) {
  if (scopeResolveOnly) {
    return;
  }

  if (initResolver && initResolver->handleResolvingCall(call))
    return;

  auto op = call->toOpCall();
  if (op && (op->op() == USTR("&&") || op->op() == USTR("||"))) {
    if (!scopeResolveOnly) {
      // these are handled in 'enter' to do param folding
      return;
    }
  }

  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  // try to resolve it as a special call (e.g. Tuple assignment)
  if (resolveSpecialCall(call)) {
    return;
  }

  std::vector<const uast::AstNode*> actualAsts;
  ID moduleScopeId;
  auto ci = CallInfo::create(context, call, byPostorder,
                             /* raiseErrors */ true,
                             &actualAsts,
                             &moduleScopeId);
  auto inScopes =
    moduleScopeId.isEmpty() ?
    CallScopeInfo::forNormalCall(scope, poiScope) :
    CallScopeInfo::forQualifiedCall(context, moduleScopeId, scope, poiScope);

  auto skip = shouldSkipCallResolution(this, call, actualAsts, ci);
  if (!skip) {
    ResolvedExpression& r = byPostorder.byAst(call);
    QualifiedType receiverType = methodReceiverType();

    // If the user has mistakenly instantiated a field of the type before
    // calling ``init``, then the receiver type will either be fully or
    // partially instantiated. This will cause a failure to resolve the
    // ``init`` call, and result in a confusing and unhelpful error message.
    // To resolve this problem, manually compute the fully-generic type that
    // is being initialized and reset the receiver.
    //
    // Note: erroneous field accesses will be handled by invoking
    // ``InitResolver::handleResolvedCall`` below.
    //
    // TODO: Move this logic into InitResolver.cpp - but where?
    if (initResolver && ci.name() == USTR("init")) {
      auto gt = ci.isMethodCall() ? ci.calledType() : receiverType;
      auto gen = getGenericType(context, gt.type());
      receiverType = QualifiedType(QualifiedType::INIT_RECEIVER, gen);

      if (ci.isMethodCall()) {
        ci = CallInfo::createWithReceiver(ci, receiverType);
      }
    }

    auto c = resolveCallInMethodReattemptIfNeeded(*this, call, r, ci, inScopes,
                                                  receiverType, actualAsts);

    // save the most specific candidates in the resolution result for the id
    c.noteResultPrintCandidates(&r);

    // handle type inference for variables split-inited by 'out' formals
    adjustTypesForOutFormals(ci, actualAsts, c.result.mostSpecific());

    // issue errors for iterator groups where e.g. serial/standalone types mismatch
    issueIteratorDiagnosticsIfNeeded(*this, call, inScopes, c.result);

    if (initResolver) {
      initResolver->handleResolvedCall(call, &c.result);
    }
  } else {
    // We're skipping the call, but explicitly store the 'unknown type'
    // in the map.
    ResolvedExpression& r = byPostorder.byAst(call);
    r.setType(QualifiedType());

    if (initResolver) {
      initResolver->handleResolvedCall(call, /* call resolution result */ nullptr);
    }
  }
}

Resolver::CallResultWrapper
Resolver::resolveGeneratedCall(const uast::AstNode* astForContext,
                               const CallInfo* ci,
                               const CallScopeInfo* inScopes,
                               const char* callName) {
  auto result = resolution::resolveGeneratedCall(rc, astForContext, *ci, *inScopes);
  return {
    /* parent */ this,
    /* result */ std::move(result),
    /* astForContext */ astForContext,
    /* ci */ ci,
    /* inScopes */ inScopes,
    /* receiverType */ types::QualifiedType(),
    /* wasGeneratedCall */ true,
    /* actualAsts */ nullptr,
    /* callName */ callName
  };
}

Resolver::CallResultWrapper
Resolver::resolveGeneratedCallInMethod(const AstNode* astContext,
                                       const CallInfo* ci,
                                       const CallScopeInfo* inScopes,
                                       QualifiedType implicitReceiver) {
  auto result = resolution::resolveGeneratedCallInMethod(rc, astContext, *ci, *inScopes, implicitReceiver);
  return {
    /* parent */ this,
    /* result */ std::move(result),
    /* astForContext */ astContext,
    /* ci */ ci,
    /* inScopes */ inScopes,
    /* receiverType */ std::move(implicitReceiver),
    /* wasGeneratedCall */ true,
    /* actualAsts */ nullptr,
    /* callName */ nullptr
  };
}

Resolver::CallResultWrapper
Resolver::resolveCallInMethod(const uast::Call* call,
                              const CallInfo* ci,
                              const CallScopeInfo* inScopes,
                              types::QualifiedType implicitReceiver,
                              std::vector<const uast::AstNode*>& actualAsts) {
  auto result = resolution::resolveCallInMethod(rc, call, *ci, *inScopes, implicitReceiver);
  return {
    /* parent */ this,
    /* result */ std::move(result),
    /* astForContext */ call,
    /* ci */ ci,
    /* inScopes */ inScopes,
    /* receiverType */ std::move(implicitReceiver),
    /* wasGeneratedCall */ false,
    /* actualAsts */ &actualAsts,
    /* callName */ nullptr
  };
}

void Resolver::exit(const Call* call) {
  handleCallExpr(call);

  // Always remove the call from the stack if we pushed it there,
  // to make sure it's properly set.
  if (call != curInheritanceExpr) {
    callNodeStack.pop_back();
  }
}

bool Resolver::enter(const Dot* dot) {
  // Need to handle fields before the compiler-generated accessor, in case
  // an implicit super.init() instantiates a generic field.
  if (initResolver) {
    initResolver->handleUseOfField(dot);
  }

  return true;
}

ID Resolver::scopeResolveEnumElement(const Enum* enumAst,
                                     UniqueString elementName,
                                     const AstNode* nodeForErr,
                                     bool& outAmbiguous) {
  outAmbiguous = false;
  LookupConfig config = LOOKUP_DECLS | LOOKUP_INNERMOST;
  auto enumScope = scopeForId(context, enumAst->id());
  auto ids = lookupNameInScope(context, enumScope,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               elementName, config);
  if (ids.numIds() == 0) {
    // Do not report the error here, because it might not be an error.
    // In particular, we could be in a parenless method call. Callers
    // will decide whether or not to emit the error.
  } else if (ids.numIds() > 1) {
    // multiple candidates. report an error, but the expression most likely has a type given by the enum.
    std::vector<ID> redefinedIds(ids.numIds());
    std::copy(ids.begin(), ids.end(), redefinedIds.begin());
    CHPL_REPORT(context, MultipleEnumElems, nodeForErr, elementName, enumAst,
                std::move(redefinedIds));
    outAmbiguous = true;
  } else {
    return ids.firstId();
  }

  return ID();
}

QualifiedType
Resolver::typeForScopeResolvedEnumElement(const EnumType* enumType,
                                          const ID& refersToId,
                                          bool ambiguous) {
  if (!refersToId.isEmpty()) {
    // Found a single enum element, the type can be a param.
    auto newParam = Param::getEnumParam(context, refersToId);
    return QualifiedType(QualifiedType::PARAM, enumType, newParam);
  } else if (ambiguous) {
    // multiple candidates. but the expression most likely has a type given by
    // the enum.
    return QualifiedType(QualifiedType::CONST_VAR, enumType);
  } else {
    return QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
  }
}

QualifiedType
Resolver::typeForScopeResolvedEnumElement(const ID& enumTypeId,
                                          const ID& refersToId,
                                          bool ambiguous) {
  auto type = initialTypeForTypeDecl(context, enumTypeId);
  CHPL_ASSERT(type && type->isEnumType());
  return typeForScopeResolvedEnumElement(type->toEnumType(), refersToId,
                                         ambiguous);
}

QualifiedType Resolver::typeForEnumElement(const EnumType* enumType,
                                           UniqueString elementName,
                                           const AstNode* nodeForErr) {
  auto enumAst = parsing::idToAst(context, enumType->id())->toEnum();
  CHPL_ASSERT(enumAst != nullptr);
  bool ambiguous;
  auto refersToId = scopeResolveEnumElement(enumAst, elementName,
                                            nodeForErr, ambiguous);
  auto qt = typeForScopeResolvedEnumElement(enumType, refersToId, ambiguous);
  if (refersToId.isEmpty() && !ambiguous) {
    // scopeResolveEnumElement doesn't report a "not found" error because
    // not being able to find an enum element isn't always an error. Here,
    // though, we are specifically interested in an element, so report
    // the error.
    CHPL_REPORT(context, UnknownEnumElem, nodeForErr, elementName, enumAst);
  }
  return qt;
}

void Resolver::exit(const Dot* dot) {
  ResolvedExpression& receiver = byPostorder.byAst(dot->receiver());
  ResolvedExpression& r = byPostorder.byAst(dot);

  bool deferToFunctionResolution = false;
  bool resolvingCalledDot = nearestCalledExpression() == dot;
  if (resolvingCalledDot && !scopeResolveOnly) {
    // When resolving `a.b()`, we likely want to perform call resolution,
    // as b(this=a). This happens when the parent function call expression is
    // processed. So, we can skip the work here. However, before we do skip,
    // handle other cases, such as `M.b()` where `M.b` is a value being
    // called using `proc this`.
    deferToFunctionResolution = true;

    // Try to resolve a it as a field/parenless proc so we can resolve 'this' on
    // it later if needed.
    if (!receiver.type().isUnknown() && receiver.type().type() &&
        receiver.type().type()->getCompositeType() &&
        dot->field() != "init") {
      std::vector<CallInfoActual> actuals;
      actuals.push_back(CallInfoActual(receiver.type(), USTR("this")));
      auto ci = CallInfo(/* name */ dot->field(),
                         /* calledType */ QualifiedType(),
                         /* isMethodCall */ true,
                         /* hasQuestionArg */ false,
                         /* isParenless */ true, actuals);
      auto inScope = scopeStack.back();
      auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
      auto c = resolveGeneratedCall(dot, &ci, &inScopes);
      if (!c.result.mostSpecific().isEmpty()) {
        // save the most specific candidates in the resolution result for the id
        c.noteResult(&r);
      }
      return;
    }
  }

  if (dot->field() == USTR("type")) {
    const Type* receiverType = nullptr;
    if (receiver.type().type() != nullptr) {
      receiverType = receiver.type().type();
    } else {
      receiverType = UnknownType::get(context);
    }

    if (!receiver.type().isType()) {
      r.setType(QualifiedType(QualifiedType::TYPE, receiverType));
    } else {
      r.setType(CHPL_TYPE_ERROR(context, DotTypeOnType, dot, receiverType,
                                receiver.toId()));
    }
    return;
  }

  if (dot->field() == USTR("locale")) {
    r.setType(QualifiedType(QualifiedType::CONST_VAR,
                            CompositeType::getLocaleType(context)));
    return;
  }

  // Handle .domain on an array (which doesn't exist in module code) as a call
  // to _dom.
  if (receiver.type().type() && receiver.type().type()->isArrayType() &&
      dot->field() == USTR("domain")) {
    std::vector<CallInfoActual> actuals;
    actuals.emplace_back(receiver.type(), USTR("this"));
    auto name = UniqueString::get(context, "_dom");
    auto ci = CallInfo(/* name */ name,
                       /* calledType */ receiver.type(),
                       /* isMethodCall */ true,
                       /* hasQuestionArg */ false,
                       /* isParenless */ true, actuals);
    auto inScope = scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
    auto rr = resolveGeneratedCall(dot, &ci, &inScopes, name.c_str());

    auto baseDomainType = rr.result.exprType().type()->toDomainType();
    r.setType(QualifiedType(QualifiedType::CONST_VAR, baseDomainType));
    return;
  }

  if (receiver.type().kind() == QualifiedType::MODULE &&
      !receiver.toId().isEmpty()) {
    ID moduleId = receiver.toId();

    // resolve e.g. M.x where M is a module
    LookupConfig config = LOOKUP_DECLS |
                          LOOKUP_IMPORT_AND_USE |
                          LOOKUP_EXTERN_BLOCKS;

    if (!moduleId.contains(dot->id())) {
      config |= LOOKUP_SKIP_PRIVATE_VIS;
    }

    auto modScope = scopeForModule(context, moduleId);
    auto ids = lookupNameInScope(context, modScope,
                                 /* methodLookupHelper */ nullptr,
                                 /* receiverScopeHelper */ nullptr,
                                 dot->field(), config);
    if (ids.numIds() == 0) {
      // emit a "can't find that thing" error
      issueErrorForFailedModuleDot(dot, moduleId, config);
      r.setType(QualifiedType());
    } else if (ids.numIds() > 1) {
      // can't establish the type. If this is in a function
      // call, we'll establish it later anyway.
    } else {
      // ids.numIds == 1
      const ID& id = ids.firstId();

      // TODO: handle extern blocks correctly.
      // This conditional is a workaround to leave extern block resolution
      // to the production scope resolver.
      if (!isExternBlock(parsing::idToTag(context, id))) {
        QualifiedType type;
        if (id.isEmpty()) {
          // empty IDs from the scope resolution process are builtins
          CHPL_ASSERT(false && "Not handled yet!");
        } else {
          // use the type established at declaration/initialization
          type = typeForId(id);
        }
        maybeEmitWarningsForId(this, type, dot, id);
        validateAndSetToId(r, dot, id);
        r.setType(type);
      }
    }
    return;
  }

  if (receiver.type().kind() == QualifiedType::TYPE &&
      asttags::isEnum(parsing::idToTag(context, receiver.toId()))) {
    // resolve E.x where E is an enum.
    auto enumAst = parsing::idToAst(context, receiver.toId())->toEnum();
    CHPL_ASSERT(enumAst != nullptr);

    bool ambiguous;
    auto elemId = scopeResolveEnumElement(enumAst, dot->field(), dot, ambiguous);
    validateAndSetToId(r, dot, elemId);

    if (!scopeResolveOnly &&
        receiver.type().type() != nullptr &&
        receiver.type().type()->isEnumType()) {
      const EnumType* enumType = receiver.type().type()->toEnumType();
      CHPL_ASSERT(enumType != nullptr);
      auto qt = typeForScopeResolvedEnumElement(enumType, elemId, ambiguous);
      r.setType(qt);
      maybeEmitWarningsForId(this, qt, dot, elemId);
    }

    if (!elemId.isEmpty() || ambiguous) {
      // Found something in the enum, no need to check for parenless methods.
      return;
    } else {
      // Continue on to parenless methods, maybe it was a parenless call.
    }
  }

  // Handle null, unknown, or erroneous receiver type
  if (receiver.type().type() == nullptr ||
      receiver.type().type()->isUnknownType()) {
    r.setType(QualifiedType(QualifiedType::UNKNOWN, UnknownType::get(context)));
    return;
  }
  if (receiver.type().type()->isErroneousType()) {
    r.setType(QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context)));
    return;
  }

  // Handle generic receiver type later in function resolution,
  // once we have an instantiation.
  if (getTypeGenericity(context, receiver.type()) != Type::CONCRETE) {
    deferToFunctionResolution = true;
  }

  if (scopeResolveOnly || deferToFunctionResolution)
    return;

  // TODO: Unique error for user-defined field accessor

  // resolve a.x where a is a record/class and x is a field or parenless method
  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(receiver.type(), USTR("this")));
  auto ci = CallInfo (/* name */ dot->field(),
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ true,
                      /* hasQuestionArg */ false,
                      /* isParenless */ true,
                      actuals);
  auto inScope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
  auto c = resolveGeneratedCall(dot, &ci, &inScopes);
  // save the most specific candidates in the resolution result for the id
  c.noteResult(&r);
}

bool Resolver::enter(const New* node) {
  if (auto ident = node->typeExpression()->toIdentifier()) {
    genericReceiverOverrideStack.emplace_back(ident->name(), node);
  }

  return true;
}

// TODO: How do we wire this up with 'getManagedClassType'? Is it possible?
// TODO: How to handle nilability, e.g. new owned C'?'
static const ClassType*
getDecoratedClassForNew(Context* context, const New* node,
                        const ClassType* classType) {
  auto basic = classType->basicClassType();

  if (!basic) {
    context->error(node, "attempt to 'new' the generic 'class' type");
    return nullptr;
  }

  auto decorator = classType->decorator();
  const Type* manager = nullptr;

  switch (node->management()) {
    case New::DEFAULT_MANAGEMENT:
      // Management might've been provided for us already; otherwise
      // fall back to the default: 'owned'.
      if (!classType->decorator().isUnknownManagement())
        break;

      // Fall through to 'owned' management.

    case New::OWNED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::MANAGED);
      manager = AnyOwnedType::get(context);
      break;
    case New::SHARED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::MANAGED);
      manager = AnySharedType::get(context);
      break;
    case New::BORROWED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::BORROWED);
      break;
    case New::UNMANAGED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::UNMANAGED);
      break;
  }

  // Combine the decorators to preserve e.g. nilability.
  auto combine = decorator.combine(classType->decorator());

  // TODO: How to get outer '?' from type expression?
  if (combine.isUnknownNilability()) {
    combine = combine.addNonNil();
  }

  auto ret = ClassType::get(context, basic, manager, combine);
  return ret;
}

static void resolveNewForClass(Resolver& rv, const New* node,
                               const ClassType* classType) {
  ResolvedExpression& re = rv.byPostorder.byAst(node);
  auto cls = getDecoratedClassForNew(rv.context, node, classType);
  auto qt = QualifiedType(QualifiedType::INIT_RECEIVER, cls);
  re.setType(qt);
}

static void resolveNewForRecordLike(Resolver& rv, const New* node,
                                const CompositeType* recordLikeType) {
  CHPL_ASSERT(recordLikeType->isRecordType() ||
              recordLikeType->isDomainType() ||
              recordLikeType->isArrayType() ||
              recordLikeType->isUnionType());

  ResolvedExpression& re = rv.byPostorder.byAst(node);

  if (node->management() != New::DEFAULT_MANAGEMENT) {
    CHPL_REPORT(rv.context, MemManagementNonClass, node, recordLikeType);
  } else {
    auto qt = QualifiedType(QualifiedType::INIT_RECEIVER, recordLikeType);
    re.setType(qt);
  }
}

void Resolver::exit(const New* node) {
  if (node->typeExpression()->isIdentifier()) {
    genericReceiverOverrideStack.pop_back();
  }

  if (scopeResolveOnly)
    return;

  // Fetch the pieces of the type expression.
  const AstNode* typeExpr = node->typeExpression();
  ResolvedExpression& reTypeExpr = byPostorder.byAst(typeExpr);
  auto& qtTypeExpr = reTypeExpr.type();

  // Propagate up type expression before doing further work.
  ResolvedExpression& re = byPostorder.byAst(node);
  re.setType(qtTypeExpr);

  // Check and exit on obvious error cases.
  if (qtTypeExpr.isUnknown()) {
    context->error(node, "Attempt to 'new' a function or undefined symbol");
    return;
  } else if (qtTypeExpr.kind() != QualifiedType::TYPE) {
    context->error(node, "'new' must be followed by a type expression");
    return;
  }

  auto type = qtTypeExpr.type();
  if (type->isBasicClassType()) {
    CHPL_ASSERT(false && "Expected fully decorated class type");
  } else if (auto classType = type->toClassType()) {
    resolveNewForClass(*this, node, classType);
  } else if (type->isRecordType() || type->isDomainType() ||
             type->isUnionType() || type->isArrayType()) {
    resolveNewForRecordLike(*this, node, type->toCompositeType());
  } else {
    if (node->management() != New::DEFAULT_MANAGEMENT) {
      CHPL_REPORT(context, MemManagementNonClass, node, qtTypeExpr.type());
    }

    CHPL_REPORT(context, InvalidNewTarget, node, qtTypeExpr);
  }
}

class IterandComponent {
 private:
  QualifiedType iterandQt_;
  const AstNode* iterandAstCtx_;
  const AstNode* astForErr_;

  const IteratorType* iteratingOver_;
  const TypedFnSignature* invokedIterator_;
  bool isExplicitlyTaggedIteratorCall_;

  IterandComponent(QualifiedType iterandQt,
                   const AstNode* iterandAstCtx,
                   const AstNode* astForErr,
                   const TypedFnSignature* invokedIterator,
                   bool isExplicitlyTaggedIteratorCall)
    : iterandQt_(std::move(iterandQt)), iterandAstCtx_(iterandAstCtx),
      astForErr_(astForErr),
      invokedIterator_(invokedIterator),
      isExplicitlyTaggedIteratorCall_(isExplicitlyTaggedIteratorCall) {
    if (!this->iterandQt_.isUnknownOrErroneous()) {
      this->iteratingOver_ = this->iterandQt_.type()->toIteratorType();
    } else {
      // The thing-to-be-iterated is not an iterator, but it might be iterable
      // using its 'these()' method. Don't resolve it now, since we haven't
      // decided on which overloads we need; resolveIterTypeWithTag will do that
      // on finding that toIterate is null.

      this->iteratingOver_ = nullptr;
    }
  }

 public:
  static bool unpackIterand(Resolver& rv,
                            std::vector<IterandComponent>& out,
                            const AstNode* astForErr,
                            const AstNode* iterand) {
    const OpCall* opCall = nullptr;
    bool isUnpack = (opCall = iterand->toOpCall()) && opCall->op() == USTR("...");

    // This is an expression in the form (...someTuple). Each element of the
    // tuple should b ecome its own iterand component. In this case,
    // we don't have an exact iterator fn (and thus, we're not a tagged call),
    // since there are several elements to iterate over.
    if (isUnpack) {
      CHPL_ASSERT(opCall->numActuals() == 1 && "expected exactly one actual for '...'");

      auto& qt = rv.byPostorder.byAst(opCall->actual(0)).type();

      if (qt.isUnknownOrErroneous()) return false;
      auto tt = qt.type()->toTupleType();
      if (!tt) {
        rv.context->error(astForErr, "unexpected type in tuple unpacking operator");
        return false;
      }

      for (int i = 0; i < tt->numElements(); i++) {
        out.push_back({tt->elementType(i), iterand, astForErr, nullptr, false});
      }

    // This is a "normal" expression, so it should create a single iterand.
    // In this case, the iterand may be the result of direct call to an iterator,
    // and, as a special case of that, a call to an iterator with a tag.
    } else {
      auto& rr = rv.byPostorder.byAst(iterand);
      auto& qt = rr.type();
      auto fn = rr.mostSpecific().only() ? rr.mostSpecific().only().fn() : nullptr;

      bool isExplicitlyTaggedIteratorCall = false;
      if (fn && fn->isParallelIterator(rv.context)) {
        // Determine if this was an explicitly tagged call.
        //
        // We could've ended up resolving a leader automatically from a serial
        // call (if the serial overload doesn't exist). To check that this was
        // an explicit tag, we need to not have any ITERATE associated actions.
        auto count = std::count_if(rr.associatedActions().begin(),
                                   rr.associatedActions().end(),
                                   [](const AssociatedAction& aa) {
                                     return aa.action() == AssociatedAction::ITERATE;
                                   });
        isExplicitlyTaggedIteratorCall = count == 0;
      }
      out.push_back({qt, iterand, astForErr, fn, isExplicitlyTaggedIteratorCall});
    }

    return true;
  }

  static bool
  unpackNonZipperedIterand(Resolver& rv,
                           std::vector<IterandComponent>& out,
                           const AstNode* astForErr,
                           const AstNode* iterand) {
    bool result = unpackIterand(rv, out, astForErr, iterand);
    if (!result) return result;

    CHPL_ASSERT(out.size() >= 1);
    if (out.size() > 1) {
      rv.context->error(astForErr, "cannot perform tuple unpacking in non-zippered loop iterand");
      return false;
    }

    return true;
  }

  const QualifiedType& iterandQt() const { return iterandQt_; }

  const AstNode* iterandAstCtx() const { return iterandAstCtx_; }

  const AstNode* astForErr() const { return astForErr_; }

  const IteratorType* iteratingOver() const { return iteratingOver_; }

  const TypedFnSignature* invokedIterator() const { return invokedIterator_; }

  bool isExplicitlyTaggedIteratorCall() const {
    return isExplicitlyTaggedIteratorCall_;
  }
};

using IteratorFailures = std::vector<std::tuple<Function::IteratorKind, TheseResolutionResult>>;

// If index is nonzero, notes the given failure are a cause for a zippered
// failure. Otherwise, notes it as a top-level failure.
static void
noteTheseResolutionFailure(Resolver& rv,
                           IteratorFailures& failures,
                           Function::IteratorKind kind,
                           int index,
                           const QualifiedType& receiver,
                           TheseResolutionResult&& result) {
  if (rv.scopeResolveOnly) {
    return;
  }

  if (index != -1) {
    auto ownedResult = std::make_unique<TheseResolutionResult>(std::move(result));
    auto zipperedTheseResult =
      TheseResolutionResult::failure(std::move(ownedResult), index, receiver);
    failures.push_back({ kind, std::move(zipperedTheseResult) });
  } else {
    failures.push_back({ kind, std::move(result) });
  }
}

static bool
resolveIterDetailsForZipperedArgs(Resolver& rv,
                                  IterDetails& outIterDetails,
                                  const std::vector<IterandComponent>& ics,
                                  Function::IteratorKind iterKind,
                                  const QualifiedType& leaderYieldType,
                                  IteratorFailures* storeFailures) {
  bool succeededAll = true;
  std::vector<QualifiedType> idxTypes;
  auto kind = QualifiedType::CONST_VAR;
  int index = 0;
  for (auto& ic : ics) {
    auto& pieces = outIterDetails.piecesForIterKind(iterKind);
    auto idxType = resolveIterTypeWithTag(rv, pieces,
                                          ic, iterKind,
                                          leaderYieldType);
    if (idxType.isUnknownOrErroneous()) {
      // Note the first failed resolution
      if (succeededAll && storeFailures) {
        noteTheseResolutionFailure(rv, *storeFailures, iterKind, index,
                                   ic.iterandQt(),
                                   std::move(pieces.resolutionResult));
      }
      succeededAll = false;
    } else if (!idxType.isConst()) {
      kind = QualifiedType::VAR;
    }

    idxTypes.push_back(std::move(idxType));
    index++;
  }

  if (succeededAll) {
    CHPL_ASSERT(idxTypes.size() == ics.size());
    if (ics.size() > 1) {
      auto tupleType =
        TupleType::getQualifiedTuple(rv.context, std::move(idxTypes));
      outIterDetails.idxType = QualifiedType(kind, tupleType);
    } else {
      outIterDetails.idxType = idxTypes[0];
    }
  }

  return succeededAll;
}

// This helper resolves by priority order as described in 'IterDetails'.
static IterDetails
resolveIterDetailsInPriorityOrder(Resolver& rv,
                                  const std::vector<IterandComponent>& ics,
                                  int mask,
                                  IteratorFailures* storeFailures = nullptr) {
  CHPL_ASSERT(ics.size() > 0);

  IterDetails ret;

  if (mask & IterDetails::STANDALONE) {
    CHPL_ASSERT(ics.size() == 1);
    auto& ic = ics[0];
    ret.idxType = resolveIterTypeWithTag(rv, ret.standalone,
                                         ic, Function::STANDALONE, {});
    if (!ret.idxType.isUnknownOrErroneous()) {
      ret.succeededAt = IterDetails::STANDALONE;
      return ret;
    }
  }

  if (mask & IterDetails::LEADER_FOLLOWER) {
    auto& ic = ics[0];
    ret.leaderYieldType = resolveIterTypeWithTag(rv, ret.leader,
                                                 ic, Function::LEADER,
                                                 {});
  }

  if (mask & IterDetails::LEADER_FOLLOWER &&
      !ret.leaderYieldType.isUnknownOrErroneous()) {
    if (resolveIterDetailsForZipperedArgs(rv, ret, ics, Function::FOLLOWER,
                                          ret.leaderYieldType, storeFailures)) {
      ret.succeededAt = IterDetails::LEADER_FOLLOWER;
      return ret;
    }
  }

  if (mask & IterDetails::SERIAL) {
    if (resolveIterDetailsForZipperedArgs(rv, ret, ics, Function::SERIAL,
                                          QualifiedType(), storeFailures)) {
      ret.succeededAt = IterDetails::SERIAL;
      return ret;
    }
  }

  return ret;
}

static QualifiedType
issueErrorForFailedIterDetails(Context* context,
                               IterDetails& details,
                               const uast::AstNode* astForErr,
                               const uast::AstNode* iterand,
                               QualifiedType iterandType) {
  std::vector<std::tuple<Function::IteratorKind, TheseResolutionResult>> failures;
  auto tryAdd = [&](Function::IteratorKind kind, TheseResolutionResult& tr) {
    if (tr.reason() != TheseResolutionResult::THESE_SUCCESS) {
      failures.push_back({ kind, std::move(tr) });
    }
  };

  tryAdd(Function::STANDALONE, details.standalone.resolutionResult);
  tryAdd(Function::LEADER, details.leader.resolutionResult);
  tryAdd(Function::FOLLOWER, details.follower.resolutionResult);
  tryAdd(Function::SERIAL, details.serial.resolutionResult);

  return CHPL_TYPE_ERROR(context, NonIterable, astForErr, iterand,
                         iterandType, std::move(failures));
}

static IterDetails resolveNonZipExpression(Resolver& rv,
                                           const AstNode* astForErr,
                                           const AstNode* iterand,
                                           const QualifiedType& leaderYieldType,
                                           int mask) {
  if (rv.scopeResolveOnly) {
    return {};
  }

  auto iterandRe = rv.byPostorder.byAst(iterand);

  if (iterandRe.type().isUnknownOrErroneous()) {
    // The iterand is unknown, no work to do.
    return {};
  }

  // Resolve iterators, stopping immediately when we get a valid yield type.
  // We are outside of a zippering contex, so call with only a single IterandComponent.
  std::vector<IterandComponent> ics;
  if (!IterandComponent::unpackNonZipperedIterand(rv, ics, astForErr, iterand)) {
    // Couldn't understand the expression. an error was already issued,
    // no more work to do.
    return {};
  }
  auto ret = resolveIterDetailsInPriorityOrder(rv, ics, mask);

  // Only issue a "not iterable" error if the iterand has a type. If it was
  // not typed then earlier resolution of the iterand will have spit out an
  // appropriate error for us already.
  if (ret.succeededAt == IterDetails::NONE && !iterandRe.type().isUnknownOrErroneous()) {
    auto& iterandRE = rv.byPostorder.byAst(iterand);
    if (!iterandRE.type().isUnknownOrErroneous()) {
      ret.idxType = issueErrorForFailedIterDetails(rv.context, ret, astForErr, iterand, iterandRE.type());
    }
  }

  return ret;
}

static TheseResolutionResult resolveTheseMethod(Resolver& rv,
                                                const AstNode* iterandAstCtx,
                                                const QualifiedType& iterandType,
                                                Function::IteratorKind iterKind,
                                                const QualifiedType& followThisType) {
  auto& iterandRe = rv.byPostorder.byAst(iterandAstCtx);
  auto inScope = rv.scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(inScope, rv.poiScope);

  auto tr = resolveTheseCall(rv.rc, iterandAstCtx, iterandType, iterKind, followThisType, inScopes);
  if (auto cr = tr.callResult()) {
    Resolver::CallResultWrapper::noteResultWithoutError(
        rv, &iterandRe, iterandAstCtx, *cr,
        { { AssociatedAction::ITERATE, iterandAstCtx->id() } });
  }

  return tr;
}

static QualifiedType
resolveIterTypeWithTag(Resolver& rv,
                       IterDetails::Pieces& outIterPieces,
                       const IterandComponent& ic,
                       Function::IteratorKind iterKind,
                       const QualifiedType& followThisFormal) {
  Context* context = rv.context;
  QualifiedType unknown(QualifiedType::UNKNOWN, UnknownType::get(context));
  QualifiedType error(QualifiedType::UNKNOWN, ErroneousType::get(context));

  auto iterKindActual = getIterKindConstantOrWarn(context, ic.astForErr(), iterKind);
  bool needSerial = iterKind == Function::SERIAL;
  bool needStandalone = iterKind == Function::STANDALONE;
  bool needLeader = iterKind == Function::LEADER;
  bool needFollower = iterKind == Function::FOLLOWER;

  // Exit early if we need a parallel iterator and don't have the enum.
  if (!needSerial && iterKindActual.isUnknown()) {
    return error;
  }

  // Inspect the resolution result to determine what should be done next.
  auto& iterandType = ic.iterandQt();
  CHPL_ASSERT(!iterandType.isUnknownOrErroneous());

  auto fn = ic.invokedIterator();

  // For iterator forwarding, we can write serial 'for' loops over tagged iterator calls
  bool treatAsSerial = fn &&
    (fn->isSerialIterator(context) || ic.isExplicitlyTaggedIteratorCall());
  // Call to a serial iterator overload, and we are looking for a serial iterator.
  bool wasMatchingIterResolved = fn &&
    ((needSerial && treatAsSerial) ||
     (needStandalone && fn->isParallelStandaloneIterator(context)) ||
     (needLeader && fn->isParallelLeaderIterator(context)) ||
     (needFollower && fn->isParallelFollowerIterator(context)));
  // Loop expressions (which we just resolved) and we are looking for a serial iterator.
  wasMatchingIterResolved |=
    (iterandType.type() && iterandType.type()->isLoopExprIteratorType() && needSerial);

  // The iterand was a call to a serial iterator, and we need a serial iterator.
  if (wasMatchingIterResolved) {
    CHPL_ASSERT(iterandType.type()->isIteratorType() &&
                iterandType.type() == ic.iteratingOver() &&
                "an iterator was resolved, expecting an iterator type");
    // We no longer have a call resolution result that produced the MSC,
    // so just create a mock one here.
    outIterPieces = { ic.iteratingOver(), TheseResolutionResult::success(iterandType) };
    return yieldTypeForIterator(rv.rc, iterandType.type()->toIteratorType());
  }

  // The iterand is either not an iterator (but could have a 'these' method)
  // or an iterator. The latter have compiler-generated 'these' methods
  // which implement the dispatch logic like rewriting an iterator from `iter foo()`
  // to `iter foo(tag)`. So just resolve the 'these' method.
  auto iteratingOver = ic.iteratingOver();
  auto tr = resolveTheseMethod(rv, ic.iterandAstCtx(), iterandType, iterKind, followThisFormal);
  auto qt = tr.exprType();
  if (!qt.isUnknownOrErroneous() && qt.type()->isIteratorType()) {
    // These produced a valid iterator. We already configured the call
    // with the desired tag, so that's sufficient.

    iteratingOver = qt.type()->toIteratorType();
  }
  auto yieldType = tr.yieldedType();
  outIterPieces = { iteratingOver, std::move(tr) };
  return yieldType;
}

struct ParamRangeInfo {
  int64_t current;
  int64_t end;
  const Type* yieldType = nullptr;

  // Optionally specified properties
  int64_t step = 1;

  static optional<ParamRangeInfo> fromBound(Context* context, ResolutionResultByPostorderID& rr, const AstNode* node) {
    ParamRangeInfo scratch;
    int numElts = -1;

    // Compositions of ranges can get very complicated:
    //
    //   (0..#10 by 2) is a 5-element range, 0, 2, 4, 6, 8
    //   (0.. by 2 #10) is a 10-element range, 2, 4, ...,
    //
    // For now, only allow #10 in the last position, to mimic something like 0<10,
    // and only allow one 'by', since multi-by combinations require smarts.
    // This is an improvement over production anyway, where # is not supported
    // in params at all.
    bool seenPound = false;
    bool seenBy = false;
    while (auto op = node->toOpCall()) {
      if (seenPound) {
        context->error(op, "unexpected composition of operators in 'param' loop");
        return {};
      }

      if (op->op() == USTR("by")) {
        if (seenBy) {
          context->error(op, "multiple 'by' operators unsupported in 'param' loop");
          return {};
        }
        seenBy = true;

        node = op->actual(0);
        auto& byRe = rr.byAst(op->actual(1));
        auto byParam = byRe.type().param();
        if (!byParam || !byParam->isIntParam()) {
          context->error(op, "expected an integer 'param' for 'by'");
          return {};
        }

        scratch.step *= byParam->toIntParam()->value();
        if (scratch.step == 0) {
          context->error(op, "step size for 'by' must be non-zero");
          return {};
        }
      } else if (op->op() == USTR("#")) {
        seenPound = true;

        node = op->actual(0);
        auto& byRe = rr.byAst(op->actual(1));
        auto byParam = byRe.type().param();
        if (!byParam || !byParam->isIntParam()) {
          context->error(op, "expected an integer 'param' for '#'");
          return {};
        }

        numElts = byParam->toIntParam()->value();
        if (numElts < 0) {
          context->error(op, "number of elements for '#' must be non-negative");
          return {};
        }
      }
    }

    auto rng = node->toRange();
    if (!rng) {
      context->error(node, "'param' loops can only iterate over range literals");
      return {};
    }

    // TODO: Simplify once we no longer use nullptr for param()
    auto findBoundParam = [&scratch, &rr](const AstNode* bound) -> const IntParam* {
      if (!bound) return nullptr;
      ResolvedExpression& boundRE = rr.byAst(bound);
      if (!scratch.yieldType) scratch.yieldType = boundRE.type().type();
      auto param = boundRE.type().param();
      return param ? param->toIntParam() : nullptr;
    };
    auto low = findBoundParam(rng->lowerBound());
    auto hi = findBoundParam(rng->upperBound());

    // TODO: various overflow issue here; if hiVal is INT_MIN, subtracting would
    //       overflow, etc.

    bool validBounds = false;
    if (low && hi) {
      validBounds = true;
      scratch.current = low->value();
      scratch.end = hi->value() - (rng->opKind() == Range::OPEN_HIGH ? 1 : 0);
    } else if (low && numElts >= 0) {
      validBounds = true;
      scratch.current = low->value();
      scratch.end = scratch.current + numElts - 1;
    } else if (hi && numElts >=0) {
      validBounds = true;
      scratch.end = hi->value() - (rng->opKind() == Range::OPEN_HIGH ? 1 : 0);
      scratch.current = scratch.end - numElts + 1;
    }
    if (!validBounds) {
      context->error(rng, "param loops may only iterate over bounded integer range literals");
      return {};
    }

    if (scratch.step < 0) {
      std::swap(scratch.current, scratch.end);
    }

    return scratch;
  }

  bool done() const {
    if (step > 0) return (current > end);
    return (current < end);
  }

  QualifiedType advance(Context* context) {
    CHPL_ASSERT(!done());
    int64_t save = current;
    current += step;

    return QualifiedType(QualifiedType::PARAM,
                         yieldType,
                         IntParam::get(context, save));
  }
};

struct TupleInfo {
  const TupleType* tupleType;
  int idx = 0;

  bool done() const {
    return idx >= tupleType->numElements();
  }

  QualifiedType advance(Context* context) {
    CHPL_ASSERT(!done());
    return tupleType->elementType(idx++);
  }
};

template <typename BoundInfo>
static bool resolveParamForLoop(Resolver& rv, const For* forLoop, BoundInfo&& boundInfo) {
  Context* context = rv.context;
  std::vector<ResolutionResultByPostorderID> loopResults;
  while (!boundInfo->done()) {
    ResolutionResultByPostorderID bodyResults;
    auto cur = Resolver::paramLoopResolver(rv, forLoop, bodyResults);

    cur.enterScope(forLoop);

    ResolvedExpression& idx = cur.byPostorder.byAst(forLoop->index());
    idx.setType(boundInfo->advance(context));
    forLoop->body()->traverse(cur);

    cur.exitScope(forLoop);

    loopResults.push_back(std::move(cur.byPostorder));
  }

  auto paramLoop = new ResolvedParamLoop(forLoop);
  paramLoop->setLoopBodies(loopResults);
  auto& resolvedLoopExpr = rv.byPostorder.byAst(forLoop);
  resolvedLoopExpr.setParamLoop(paramLoop);

  return false;
}

static bool resolveParamForLoop(Resolver& rv, const For* forLoop) {
  const AstNode* iterand = forLoop->iterand();
  Context* context = rv.context;

  iterand->traverse(rv);

  if (rv.scopeResolveOnly) {
    rv.enterScope(forLoop);
    return true;
  }

  auto iterandInfo = ParamRangeInfo::fromBound(context, rv.byPostorder, iterand);
  if (!iterandInfo) return false;

  return resolveParamForLoop(rv, forLoop, std::move(iterandInfo));
}

static bool resolveHeterogenousTupleForLoop(Resolver& rv, const For* forLoop, const TupleType* tupleType) {
  auto tupleInfo = TupleInfo { tupleType };
  return resolveParamForLoop(rv, forLoop, &tupleInfo);
}

static QualifiedType
resolveZipExpression(Resolver& rv, const IndexableLoop* loop, const Zip* zip, std::vector<IterandComponent>& outIcs) {
  // Failures to find various iteration strategies (serial, follower) go here.
  IteratorFailures failures;

  Context* context = rv.context;
  bool loopRequiresParallel = loop->isForall();
  bool loopPrefersParallel = loopRequiresParallel || loop->isBracketLoop();
  QualifiedType ret;

  // Compute iterator components to resolve as part of zippering. This
  // handles unpacking any iterands in the form (...bla) into a flattened
  // repersentation.
  for (auto actual : zip->actuals()) {
    if (!IterandComponent::unpackIterand(rv, outIcs, actual, actual)) {
      // Couldn't make sense of the iterand, we can't go on. An error
      // was already emitted, but contruct an ErroneousType to return.
      return QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
    }
  }
  bool singletonZip = outIcs.size() == 1;

  // Compute the mask for this zip expression
  if (outIcs.size() > 0) {
    auto leaderQt = outIcs[0].iterandQt();

    if (leaderQt.isUnknownOrErroneous()) {
      return QualifiedType();
    }

    const auto skippingAllIterands = -1;
    int m = IterDetails::NONE;
    if (singletonZip) {
      m |= IterDetails::STANDALONE;
    }
    if (loopPrefersParallel) {
      m |= IterDetails::LEADER_FOLLOWER;
    } else {
      // Note that we will not attempt a leader/follower iterator.
      noteTheseResolutionFailure(rv, failures, Function::LEADER,
                                 skippingAllIterands,
                                 leaderQt,
                                 TheseResolutionResult());
    }

    if (!loopRequiresParallel) {
      m |= IterDetails::SERIAL;
    } else {
      // Note that we will not attempt a serial iterator for any iterand.
      noteTheseResolutionFailure(rv, failures, Function::SERIAL, skippingAllIterands,
                                 leaderQt, TheseResolutionResult());
    }

    CHPL_ASSERT(m != IterDetails::NONE);

    for (size_t i = 1; i < outIcs.size(); i++) {
      if (outIcs[i].iterandQt().isUnknownOrErroneous()) {
        return QualifiedType();
      }
    }

    auto result = resolveIterDetailsInPriorityOrder(rv, outIcs, m, &failures);
    if (result.succeededAt != IterDetails::NONE) {
      ret = result.idxType;
    }
  }


  if (!rv.scopeResolveOnly && ret.isUnknownOrErroneous()) {
    // Emit a NonIterable error.
    ret = CHPL_TYPE_ERROR(context, NonIterable, loop, zip,
                          QualifiedType(), std::move(failures));
  }

  auto& reZip = rv.byPostorder.byAst(zip);
  reZip.setType(ret);

  return ret;
}

static bool isShapedLikeArray(const IndexableLoop* loop) {
  // 'forall' expressions are not arrays, only [] ... expressions could be.
  if (!loop->isBracketLoop() || !loop->isExpressionLevel()) return false;

  // If there's an 'in' or 'with' clause, it's not an array
  if (loop->index() != nullptr || loop->withClause() != nullptr) return false;

  // If there's a 'zip', it's not an array
  if (loop->iterand()->isZip()) return false;

    // If there's more than one statement, it's not an array
  if (loop->numStmts() > 1) return false;

  return true;
}

static bool handleArrayTypeExpr(Resolver& rv,
                                const IndexableLoop* loop) {
  auto& re = rv.byPostorder.byAst(loop);
  const auto genericArrayType = QualifiedType(
      QualifiedType::TYPE, ArrayType::getGenericArrayType(rv.context));

  // Get element type from loop body
  QualifiedType eltType;
  if (loop->numStmts() == 1) {
    eltType = rv.byPostorder.byAst(loop->stmt(0)).type();
  } else if (loop->numStmts() == 0) {
    // Interpret no body as generic element type
    eltType = QualifiedType(QualifiedType::TYPE, getAnyType(rv, loop->id()));
  }

  // The body wasn't a type, so this isn't an array type expression.
  // Make an exception for unknown or erroneous bodies, since the user may
  // have been trying to define a type but made a mistake (or we may be
  // in a partially-instantiated situation and the type is not yet known).
  if (!eltType.isUnknownOrErroneous() &&
      !eltType.isType() &&
      !eltType.isTypeQuery()) {
    return false;
  }

  // Determine the domain type
  QualifiedType domainType;
  const auto genericDomainType = QualifiedType(
      QualifiedType::TYPE, DomainType::getGenericDomainType(rv.context));
  auto iterandExpr = loop->iterand();
  auto iterandType = rv.byPostorder.byAst(iterandExpr).type();
  if (iterandExpr->isDomain() && iterandExpr->toDomain()->numExprs() == 0) {
    domainType = genericDomainType;
  } else if (iterandType.isUnknown() || iterandType.isTypeQuery()) {
    domainType = iterandType;
  } else if (iterandType.type()->isDomainType()) {
    domainType = iterandType;
  } else if (iterandType.type()->isRecordType() &&
             iterandType.type()->toRecordType()->id() ==
                 CompositeType::getRangeType(rv.context)->id()) {
    // Convert range into domain.
    // Note this is only hit for a plain old range, not a comma-separated list
    // of them which is already recognized as a domain.
    // TODO: This code is largely copied from Domain decl resolution, can
    // likely be refactored.

    std::vector<CallInfoActual> actuals;
    actuals.emplace_back(iterandType, UniqueString());
    auto ci = CallInfo(
        /* name */ UniqueString::get(rv.context, "chpl__ensureDomainExpr"),
        /* calledType */ QualifiedType(),
        /* isMethodCall */ false,
        /* hasQuestionArg */ false,
        /* isParenless */ false,
        std::move(actuals));
    auto scope = rv.scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(scope, rv.poiScope);
    auto c = resolveGeneratedCall(rv.rc, iterandExpr, ci, inScopes);
    if (!c.exprType().isUnknownOrErroneous()) {
      domainType = c.exprType();
    }
  }
  if (domainType.isUnknown()) {
    domainType = genericDomainType;
  }


  // Assemble the array type
  auto arrayType = genericArrayType;
  if (domainType.isErroneousType() || eltType.isErroneousType()) {
    // Propagate error from domain or element type
    arrayType =
        QualifiedType(QualifiedType::TYPE, ErroneousType::get(rv.context));
  } else if (domainType.type() == genericDomainType.type()) {
    // Preserve eltType info, if we have it.
    if (!eltType.isUnknown() && !eltType.isTypeQuery()) {
      auto domainTypeAsType =
          QualifiedType(QualifiedType::TYPE, domainType.type());
      arrayType = QualifiedType(
          QualifiedType::TYPE,
          ArrayType::getArrayType(
              rv.context,
              /* instance */
              QualifiedType(QualifiedType::VAR, getAnyType(rv, loop->id())),
              /* domainType */ domainTypeAsType,
              /* eltType */ eltType));
    }
  } else {
    // We have an instantiated domain, so get array type via call to its
    // array builder function.
    const char* arrayBuilderProc = "chpl__buildArrayRuntimeType";
    std::vector<CallInfoActual> actuals;
    actuals.emplace_back(domainType, UniqueString::get(rv.context, "dom"));
    actuals.emplace_back(eltType, UniqueString::get(rv.context, "eltType"));
    auto ci = CallInfo(
        /* name */ UniqueString::get(rv.context, arrayBuilderProc),
        /* calledType */ QualifiedType(),
        /* isMethodCall */ false,
        /* hasQuestionArg */ false,
        /* isParenless */ false,
        actuals);
    auto scope = rv.scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(scope, rv.poiScope);
    auto c = resolveGeneratedCall(rv.rc, iterandExpr, ci, inScopes);
    if (!c.exprType().isUnknownOrErroneous()) {
      arrayType = QualifiedType(QualifiedType::TYPE, c.exprType().type());
    }
  }

  re.setType(arrayType);
  return true;
}

static void noteLoopExprType(Resolver& rv, const IndexableLoop* loop, const std::vector<IterandComponent>& ics) {
  if (!loop->isExpressionLevel()) return;

  CHPL_ASSERT(loop->numStmts() == 1);
  auto bodyType = rv.byPostorder.byAst(loop->stmt(0)).type();

  auto loopType = QualifiedType();
  if (!bodyType.isUnknownOrErroneous()) {
    // Loop expressions keep the types of their iterands (effectively)
    // because they need to preserve them for resolving leaders and followers
    // later.

    QualifiedType iterandType;
    bool isZippered = false;
    if (loop->iterand()->isZip()) {
      isZippered = true;

      std::vector<QualifiedType> iterandTypes;
      for (auto child : ics) iterandTypes.push_back(child.iterandQt());
      iterandType =
        QualifiedType(QualifiedType::TYPE,
                      TupleType::getQualifiedTuple(rv.context,
                                                   std::move(iterandTypes)));
    } else {
      iterandType = rv.byPostorder.byAst(loop->iterand()).type();
    }

    if (!iterandType.isUnknownOrErroneous()) {
      bool supportsParallel = loop->isForall() || loop->isBracketLoop();
      auto loopExprType =
        LoopExprIteratorType::get(rv.context, bodyType, rv.poiScope, isZippered,
                                  supportsParallel, iterandType, loop->id());
      loopType = QualifiedType(QualifiedType::CONST_VAR, loopExprType);
    }
  }

  auto& re = rv.byPostorder.byAst(loop);
  re.setType(loopType);
}

bool Resolver::enter(const IndexableLoop* loop) {
  auto forLoop = loop->toFor();
  bool isParamForLoop = forLoop != nullptr && forLoop->isParam();

  // whether this is a param or regular loop, before entering its body
  // or considering its iterand, resolve expressions in the loop's attribute
  // group.
  if (auto ag = loop->attributeGroup()) {
    ag->traverse(*this);
  }

  if (isParamForLoop) return resolveParamForLoop(*this, loop->toFor());

  auto iterand = loop->iterand();
  iterand->traverse(*this);

  bool shapedLikeArray = false;
  if (!scopeResolveOnly && (shapedLikeArray = isShapedLikeArray(loop))) {
    // Array expressions and bracket loops can look very similar.
    // For array type expressions, we do not need to go through the
    // iterator/'these' logic handled below. Resolve the body so we can check
    // if it's an array.
    enterScope(loop);
    loop->body()->traverse(*this);

    // If it's an array, no need to do any more work.
    if (handleArrayTypeExpr(*this, loop)) {
      return false;

    // Otherwise, we need to continue doing loop resolution, including the
    // iterator/'these' logic. When performing that logic, we don't want
    // the loop body to be our scope (indices are outside the loop), so
    // exit the scope here.
    } else {
      exitScope(loop);
    }
  }

  // iteration over non-homogenous tuples is handled directly by
  // the compiler, very much like a param loop.
  if (forLoop && !scopeResolveOnly && !iterand->isZip()) {
    auto iterandRe = byPostorder.byAst(iterand);
    if (!iterandRe.type().isUnknownOrErroneous()) {
      if (auto tt = iterandRe.type().type()->toTupleType()) {
        if (!tt->isStarTuple()) {
          return resolveHeterogenousTupleForLoop(*this, forLoop, tt);
        }
      }
    }
  }

  // Not an array expression. In this case, depending on the loop type,
  // we need to resolve various iterators.
  QualifiedType idxType;
  // Resolving zip indices will collect iterand components (which don't
  // always map one-to-one to indices if we're using tuple unpacking). This
  // will be used below for noting the loop expression type.
  std::vector<IterandComponent> ics;
  if (iterand->isZip()) {
    idxType = resolveZipExpression(*this, loop, iterand->toZip(), ics);
  } else {
    bool loopRequiresParallel = loop->isForall();
    bool loopPrefersParallel = loopRequiresParallel || loop->isBracketLoop();

    int m = IterDetails::NONE;
    if (loopPrefersParallel) m |= IterDetails::LEADER_FOLLOWER |
                                  IterDetails::STANDALONE;
    if (!loopRequiresParallel) m |= IterDetails::SERIAL;
    CHPL_ASSERT(m != IterDetails::NONE);

    auto dt = resolveNonZipExpression(*this, loop, iterand, {}, m);
    idxType = dt.idxType;
  }

  enterScope(loop);

  if (const Decl* idx = loop->index()) {
    ResolvedExpression& re = byPostorder.byAst(idx);

    if (idx->isTupleDecl() && !scopeResolveOnly) {
      resolveTupleUnpackDecl(idx->toTupleDecl(), idxType);
    } else {
      re.setType(idxType);
    }
  }

  if (auto with = loop->withClause()) {
    with->traverse(*this);
  }

  // If the loop is shaped like an array, we've already resolved the body
  if (!shapedLikeArray) {
    loop->body()->traverse(*this);
  }

  if (!idxType.isUnknownOrErroneous()) {
    noteLoopExprType(*this, loop, ics);
  } else {
    // HACK: need this because declaration handling checks for typePtr()
    //       and causes weirdness with `var r: _iteratorRecord = [...]`.
    //       we should update the type-for-decl computation to handle
    //       missing vs unknown types, but that's somewhat involved.
    //       See the TODO in getTypeForDecl.
    byPostorder.byAst(loop).setType(
        QualifiedType(QualifiedType::UNKNOWN,
                      UnknownType::get(context)));
  }

  return false;
}

void Resolver::exit(const IndexableLoop* loop) {
  // Param loops handle scope differently
  auto forLoop = loop->toFor();
  bool isParamForLoop = forLoop != nullptr && forLoop->isParam();

  if (isParamForLoop == false || scopeResolveOnly) {
    exitScope(loop);
  }
}

bool Resolver::enter(const DoWhile* loop) {
  enterScope(loop);

  // traversing the block directly will push its scope onto the stack, which would
  // duplicate the enter we'll do here. So we just visit the children manually.
  enterScope(loop->body());
  for (auto child : loop->body()->children()) {
    child->traverse(*this);
  }
  // traverse the condition in the same scope as the loop body.
  loop->condition()->traverse(*this);
  exitScope(loop->body());

  return false;
}

void Resolver::exit(const DoWhile* loop) {
  exitScope(loop);
}


// Returns 'true' if a single Id was scope-resolved, in which case the function
// will also return via the ID and QualifiedType formals.
static bool computeTaskIntentInfo(Resolver& resolver, const NamedDecl* intent,
                                  ID& resolvedId, QualifiedType& type) {
  auto& scopeStack = resolver.scopeStack;

  // Look at the scope before the loop-statement
  const Scope* scope = scopeStack[scopeStack.size()-2];
  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS |
                        LOOKUP_INNERMOST;

  auto fHelper = resolver.getFieldDeclarationLookupHelper();
  auto helper = resolver.getMethodReceiverScopeHelper();

  auto ids = lookupNameInScope(resolver.context, scope,
                               /* methodLookupHelper */ fHelper,
                               /* receiverScopeHelper */ helper,
                               intent->name(), config);

  if (ids.numIds() == 1) {
    resolvedId = ids.firstId();
    if (resolver.scopeResolveOnly == false) {
      if (resolvedId.isEmpty()) {
        type = typeForBuiltin(resolver.context, intent->name());
      } else {
        type = resolver.typeForId(resolvedId);
      }
    }
    return true;
  } else {
    return false;
  }
}

bool Resolver::enter(const ReduceIntent* reduce) {
  ID id;
  QualifiedType type;
  ResolvedExpression& result = byPostorder.byAst(reduce);

  if (computeTaskIntentInfo(*this, reduce, id, type)) {
    validateAndSetToId(result, reduce, id);
    // set reduce intent shadow variable to a VAR with type of shadowed variable
    QualifiedType reduceIntentType =
        QualifiedType(QualifiedType::Kind::VAR, type.type());
    result.setType(reduceIntentType);
  } else if (!scopeResolveOnly) {
    context->error(reduce, "Unable to find declaration of \"%s\" for reduction", reduce->name().c_str());
  }

  // TODO: Resolve reduce->op() with shadowed type
  // E.g. "+ reduce x" --> "SumReduceOp(x.type)"

  return false;
}

void Resolver::exit(const ReduceIntent* reduce) {
}

static UniqueString identifierReduceScanOpName(Context* context,
                                               UniqueString name) {
  if (name == USTR("+")) return UniqueString::get(context, "SumReduceScanOp");
  if (name == USTR("*")) return UniqueString::get(context, "ProductReduceScanOp");
  if (name == USTR("&&")) return UniqueString::get(context, "LogicalAndReduceScanOp");
  if (name == USTR("||")) return UniqueString::get(context, "LogicalOrReduceScanOp");
  if (name == USTR("&")) return UniqueString::get(context, "BitwiseAndReduceScanOp");
  if (name == USTR("|")) return UniqueString::get(context, "BitwiseOrReduceScanOp");
  if (name == USTR("^")) return UniqueString::get(context, "BitwiseXorReduceScanOp");

  if (name == USTR("max")) return UniqueString::get(context, "MaxReduceScanOp");
  if (name == USTR("min")) return UniqueString::get(context, "MinReduceScanOp");

  return UniqueString();
}

static const ClassType *
constructReduceScanOpClass(Resolver& resolver,
                           const uast::AstNode* reduceOrScan,
                           UniqueString opName,
                           const QualifiedType& iterType) {
  auto context = resolver.context;
  auto actualType = QualifiedType(Qualifier::TYPE, iterType.type());

  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(actualType, UniqueString()));
  auto ci = CallInfo (/* name */ opName,
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ false,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      actuals);
  const Scope* scope = scopeForId(context, reduceOrScan->id());
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
  auto c = resolver.resolveGeneratedCall(reduceOrScan, &ci, &inScopes);
  auto opType = c.result.exprType();

  // Couldn't resolve the call; is opName a valid reduction?
  if (opType.isUnknown()) {
    CHPL_REPORT(context, ReductionInvalidName, reduceOrScan, opName, iterType);
    return nullptr;
  } else {
    c.noteResult(&resolver.byPostorder.byAst(reduceOrScan),
                  { { AssociatedAction::REDUCE_SCAN, reduceOrScan->id() } });
  }

  // We found some type; is it a subclass of ReduceScanOp?
  auto baseClass = BasicClassType::getReduceScanOpType(context);
  auto actualClass = opType.type()->toClassType();
  bool converts, instantiates;
  if (opType.kind() != QualifiedType::TYPE ||
      !actualClass ||
      !actualClass->basicClassType() ||
      !actualClass->basicClassType()->isSubtypeOf(context, baseClass, converts, instantiates)) {
    CHPL_REPORT(context, ReductionNotReduceScanOp, reduceOrScan, opType);
  }

  return actualClass;
}

static const ClassType* determineReduceScanOp(Resolver& resolver,
                                              const uast::AstNode* reduceOrScan,
                                              const uast::AstNode* op,
                                              const QualifiedType& iterType) {
  if (auto ident = op->toIdentifier()) {
    auto toLookUp = ident->name();
    auto opName = identifierReduceScanOpName(resolver.context, ident->name());
    if (!opName.isEmpty()) {
      // The identifier does not itself name a ReduceScanOp class, but is
      // associated with such a class. Use the associated class' name instead
      // of the identifier itself.
      toLookUp = opName;
    }
    auto scanOp = constructReduceScanOpClass(resolver, reduceOrScan, toLookUp, iterType);
    if (scanOp != nullptr) {
      // Since we found a ReduceScanOp, set the refersToId of the identifier.
      //
      // It's safe to call basicClassType since constructReduceScanOpClass
      // ensures it's a basic class type.
      resolver.validateAndSetToId(resolver.byPostorder.byAst(ident),
                                  ident, scanOp->basicClassType()->id());
    }
    // No further processing is needed; we found the operation.
    return scanOp;
  }

  // In the future, node could be a value or something else, and this function
  // would return the corresponding ClassType.

  return nullptr;
}

static QualifiedType getReduceScanOpResultType(Resolver& resolver,
                                               const uast::AstNode* reduceOrScan,
                                               const ClassType* opClass) {
  auto context = resolver.context;

  auto borrowedDecorator =
    ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto borrowedClass = opClass->withDecorator(context,
                                              std::move(borrowedDecorator));
  auto thisActual = QualifiedType(Qualifier::CONST_IN, borrowedClass);

  std::vector<CallInfoActual> typeActuals;
  typeActuals.push_back(CallInfoActual(thisActual, USTR("this")));
  auto ci = CallInfo (/* name */ USTR("generate"),
                      /* calledType */ thisActual,
                      /* isMethodCall */ true,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      typeActuals);
  const Scope* scope = scopeForId(context, reduceOrScan->id());
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);

  // don't use resolver's wrapper of resolveGeneratedCall, since we don't
  // do anything beyond getting the result type.
  auto c = resolveGeneratedCall(resolver.rc, reduceOrScan, ci, inScopes);
  return c.exprType();
}

static QualifiedType resolveReduceScanOp(Resolver& resolver,
                                         const AstNode* reduceOrScan,
                                         const AstNode* op,
                                         const AstNode* iterand) {
  iterand->traverse(resolver);
  auto dt = resolveNonZipExpression(resolver, reduceOrScan, iterand, {},
                                    IterDetails::SERIAL);
  auto idxType = dt.idxType;
  if (idxType.isUnknown()) return QualifiedType();
  auto opClass = determineReduceScanOp(resolver, reduceOrScan, op, idxType);
  if (opClass == nullptr) return QualifiedType();

  return getReduceScanOpResultType(resolver, reduceOrScan, opClass);
}

bool Resolver::enter(const uast::Reduce* reduce) {
  auto elementType = resolveReduceScanOp(*this, reduce,
                                         reduce->op(), reduce->iterand());
  byPostorder.byAst(reduce).setType(std::move(elementType));
  return false;
}

void Resolver::exit(const uast::Reduce* reduce) {
}

// helper to determine if a TaskVar is a task intent
static bool isTaskIntent(const TaskVar* taskVar) {
  return taskVar->typeExpression() == nullptr &&
         taskVar->initExpression() == nullptr;
}

bool Resolver::enter(const TaskVar* taskVar) {
  if (isTaskIntent(taskVar)) {
    ID id;
    QualifiedType type;
    ResolvedExpression& result = byPostorder.byAst(taskVar);
    if (computeTaskIntentInfo(*this, taskVar, id, type)) {
      QualifiedType taskVarType = QualifiedType(taskVar->storageKind(),
                                                type.type());
      validateAndSetToId(result, taskVar, id);

      // TODO: Handle in-intents where type can change (e.g. array slices)
      result.setType(taskVarType);
    } else if (!scopeResolveOnly) {
      context->error(taskVar, "Unable to find declaration of \"%s\" for task intent", taskVar->name().c_str());
    }
    return false;
  } else {
    enterScope(taskVar);
    return true;
  }
}

void Resolver::exit(const TaskVar* taskVar) {
  if (!isTaskIntent(taskVar)) {
    exitScope(taskVar);
  }
}

bool Resolver::enter(const Return* ret) {
  return true;
}

void Resolver::exit(const Return* ret) {
  if (initResolver) {
    initResolver->checkEarlyReturn(ret);
  }
}

bool Resolver::enter(const Throw* node) {
  return true;
}

void Resolver::exit(const Throw* node) {
  if (initResolver) {
    context->error(node, "initializers are not yet allowed to throw errors");
  }
}

bool Resolver::enter(const Try* node) {
  enterScope(node);
  return true;
}

void Resolver::exit(const Try* node) {
  if (initResolver && node->isTryBang() && node->numHandlers() > 0) {
    context->error(node, "Only catch-less try! statements are allowed in initializers for now");
  }
  // Node inherits the type of its expression if it is not a statement.
  if (node->isExpressionLevel()) {
    auto expr = node->stmt(0);
    CHPL_ASSERT(expr);
    auto& reNode = byPostorder.byAst(node);
    auto& reExpr = byPostorder.byAst(expr);
    reNode.setType(reExpr.type());
  }
  exitScope(node);
}

bool Resolver::enter(const Catch* node) {
  enterScope(node);
  if (scopeResolveOnly) {
    return true;
  }

  if (auto errVar = node->error()) {
    CHPL_ASSERT(errVar->initExpression() == nullptr &&
      "catch variable should not have an init expression");

    const AstNode* typeExpr = errVar->typeExpression();
    if (typeExpr == nullptr) {
      // default to Error placeholder
      const ClassType* errorType = CompositeType::getErrorType(context);
      auto qt = QualifiedType(QualifiedType::VAR, errorType);
      ResolvedExpression& re = byPostorder.byAst(errVar);
      re.setType(qt);
    } else {
      errVar->traverse(*this);
    }

    ResolvedExpression& re = byPostorder.byAst(errVar);

    bool isBasicClass = false;
    if (auto ct = re.type().type()->toClassType()) {
      bool converts = false;
      bool instantiates = false;
      if (auto bct = ct->basicClassType()) {
        isBasicClass = true;
        if (!bct->isSubtypeOf(context, CompositeType::getErrorType(context)->basicClassType(), converts, instantiates)) {
          // get the penultimate type in the chain
          while (!bct->parentClassType()->isObjectType()) {
            bct = bct->parentClassType();
          }
          context->error(errVar, "catch variable '%s' must be a class that inherits from Error, not '%s'", errVar->name().c_str(), bct->name().c_str());
        } else {
          auto dec = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NONNIL);
          auto manager = AnyOwnedType::get(context);
          auto ret = ClassType::get(context, bct, manager, dec);
          auto qt = QualifiedType(re.type().kind(), ret->withDecorator(context, dec));
          re.setType(qt); // replace type
        }
      }
    }

    if (!isBasicClass) {
      context->error(errVar, "catch variable '%s' must be a class that inherits from Error", errVar->name().c_str());
    }
  } // TODO: is there an else case to handle here for catchall without an error variable (e.g. catch {})?

  // do traverse of body
  if (auto body = node->body()) {
    body->traverse(*this);
  }

  return false;
}

void Resolver::exit(const Catch* node) {
  exitScope(node);
}

// Do not visit children. Un-ambiguous symbols will have warnings emitted
// for them in scope resolve.
bool Resolver::enter(const Use* node) {
  const Scope* scope = scopeStack.back();
  CHPL_ASSERT(scope);
  std::ignore = resolveVisibilityStmts(context, scope);
  emitMultipleDefinedSymbolErrors(context, scope);
  return false;
}

void Resolver::exit(const Use* node) {}

// Ditto the above.
bool Resolver::enter(const Import* node) {
  const Scope* scope = scopeStack.back();
  CHPL_ASSERT(scope);
  std::ignore = resolveVisibilityStmts(context, scope);
  emitMultipleDefinedSymbolErrors(context, scope);
  return false;
}

void Resolver::exit(const Import* node) {}

bool Resolver::enter(const uast::VisibilityClause* node) {
  // Specially handled for use/import; the limitations are names (and
  // not identifiers) for the purposes of forwarding.
  node->symbol()->traverse(*this);
  return false;
}

void Resolver::exit(const uast::VisibilityClause* node) {}

bool Resolver::enter(const uast::Zip* zip) {
  return true;
}
void Resolver::exit(const uast::Zip* zip) {}

bool Resolver::enter(const uast::Let* node) {
  enterScope(node);
  return true;
}

void Resolver::exit(const uast::Let* node) {
  // The type of a let-expression is the type of its expression.
  auto& otherR = byPostorder.byAst(node->expression());
  auto& thisR = byPostorder.byAst(node);

  auto& resultQt = otherR.type();
  if (resultQt.type() && resultQt.type()->isVoidType()) {
    context->error(node, "invalid use of 'void' value as expression");
  }
  thisR.setType(resultQt);
  exitScope(node);
}

bool Resolver::enter(const AstNode* ast) {
  enterScope(ast);

  bool skipChildren = signatureOnly && ast == fnBody;
  return !skipChildren;
}
void Resolver::exit(const AstNode* ast) {
  exitScope(ast);
}


} // end namespace resolution
} // end namespace chpl
