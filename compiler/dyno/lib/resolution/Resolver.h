/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef RESOLVER_H
#define RESOLVER_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {


struct Resolver {
  // inputs to the resolution process
  Context* context = nullptr;
  const uast::AstNode* symbol = nullptr;
  const uast::AstNode* curStmt = nullptr;
  const types::CompositeType* inCompositeType = nullptr;
  const SubstitutionsMap* substitutions = nullptr;
  bool useGenericFormalDefaults = false;
  const TypedFnSignature* typedSignature = nullptr;
  const PoiScope* poiScope = nullptr;

  // internal variables
  std::vector<const uast::Decl*> declStack;
  std::vector<const Scope*> scopeStack;
  bool signatureOnly = false;
  const uast::Block* fnBody = nullptr;
  bool fieldOrFormalsComputed = false;
  std::set<ID> fieldOrFormals;
  std::set<ID> instantiatedFieldOrFormals;
  const uast::Call* inLeafCall = nullptr;

  // results of the resolution process

  // the resolution results for the contained AstNodes
  ResolutionResultByPostorderID& byPostorder;
  // the set of POI scopes from which POI functions were used --
  // these are gathered here during resolution in order to
  // allow accurate caching and reuse of instantiations
  PoiInfo poiInfo;

  static PoiInfo makePoiInfo(const PoiScope* poiScope) {
    if (poiScope == nullptr)
      return PoiInfo();

    return PoiInfo(poiScope);
  }

 private:
  Resolver(Context* context,
           const uast::AstNode* symbol,
           ResolutionResultByPostorderID& byPostorder,
           const PoiScope* poiScope)
    : context(context), symbol(symbol),
      poiScope(poiScope),
      byPostorder(byPostorder), poiInfo(makePoiInfo(poiScope)) {

    enterScope(symbol);
  }
 public:

  // set up Resolver to resolve a Module
  static Resolver
  moduleStmtResolver(Context* context, const uast::Module* mod,
                     const uast::AstNode* modStmt,
                     ResolutionResultByPostorderID& byPostorder);

  // set up Resolver to resolve a potentially generic Function signature
  static Resolver
  initialSignatureResolver(Context* context, const uast::Function* fn,
                           ResolutionResultByPostorderID& byPostorder);

  // set up Resolver to resolve an instantiation of a Function signature
  static Resolver
  instantiatedSignatureResolver(Context* context,
                                const uast::Function* fn,
                                const SubstitutionsMap& substitutions,
                                const PoiScope* poiScope,
                                ResolutionResultByPostorderID& byPostorder);

  // set up Resolver to resolve a Function body/return type after
  // instantiation (if any instantiation was needed)
  static Resolver
  functionResolver(Context* context,
                   const uast::Function* fn,
                   const PoiScope* poiScope,
                   const TypedFnSignature* typedFnSignature,
                   ResolutionResultByPostorderID& byPostorder);

  // set up Resolver to initially resolve field declaration types
  static Resolver
  initialFieldStmtResolver(Context* context,
                           const uast::AggregateDecl* decl,
                           const uast::AstNode* fieldStmt,
                           const types::CompositeType* compositeType,
                           ResolutionResultByPostorderID& byPostorder,
                           bool useGenericFormalDefaults);

  // set up Resolver to resolve instantiated field declaration types
  static Resolver
  instantiatedFieldStmtResolver(Context* context,
                                const uast::AggregateDecl* decl,
                                const uast::AstNode* fieldStmt,
                                const types::CompositeType* compositeType,
                                const PoiScope* poiScope,
                                ResolutionResultByPostorderID& byPostorder,
                                bool useGenericFormalDefaults);

  // set up Resolver to resolve instantiated field declaration types
  // without knowing the CompositeType
  static Resolver
  instantiatedSignatureFieldsResolver(Context* context,
                                      const uast::AggregateDecl* decl,
                                      const SubstitutionsMap& substitutions,
                                      const PoiScope* poiScope,
                                      ResolutionResultByPostorderID& byId);

  // set up Resolver to resolve a parent class type expression
  static Resolver
  parentClassResolver(Context* context,
                      const uast::AggregateDecl* decl,
                      const SubstitutionsMap& substitutions,
                      const PoiScope* poiScope,
                      ResolutionResultByPostorderID& byPostorder);

  /* Returns ErroneousType and emits the error message msg
     relevant to location for 'ast'.
   */
  types::QualifiedType typeErr(const uast::AstNode* ast, const char* msg);

  /* When resolving a generic record or a generic function,
     there might be generic types that we don't know yet.
     E.g.

       proc f(type t, arg: t)

     before instantiating, we should conclude that:
       * t has type AnyType
       * arg has type UnknownType (and in particular, not AnyType)

     But, if we have a substitution for `t`, we should use that.
   */
  bool shouldUseUnknownTypeForGeneric(const ID& id);

  // helper for resolveTypeQueriesFromFormalType
  void resolveTypeQueries(const uast::AstNode* formalTypeExpr,
                          const types::Type* actualType);

  /* When resolving a function with a TypeQuery, we need to
     resolve the type that is queried, since it can be used
     on its own later.

     E.g.

       proc a(arg: ?t) { }
       proc b(arg: GenericRecord(?u)) { }

     This function resolves the types of all TypeQuery nodes
     contained in the passed Formal (by updating 'byPostorder').
   */
  void resolveTypeQueriesFromFormalType(const uast::Formal* formal,
                                        types::QualifiedType formalType);

  // helper for getTypeForDecl -- checks the Kinds are compatible
  // if so, returns false.
  // if not, issues error(s) and returns true.
  bool checkForKindError(const uast::AstNode* typeForErr,
                         const uast::AstNode* initForErr,
                         types::QualifiedType::Kind declKind,
                         types::QualifiedType declaredType,
                         types::QualifiedType initExprType);

  // Helper to figure out what type to use for a declaration
  // that can have both a declared type and an init expression.
  // If both are provided, checks that they are compatible.
  // Returns the type to use for the declaration.
  types::QualifiedType getTypeForDecl(const uast::AstNode* declForErr,
                                      const uast::AstNode* typeForErr,
                                      const uast::AstNode* initForErr,
                                      types::QualifiedType::Kind declKind,
                                      types::QualifiedType declaredType,
                                      types::QualifiedType initExprType);

  // helper to resolve a NamedDecl
  // useType will be used to set the type if it is not nullptr
  void resolveNamedDecl(const uast::NamedDecl* decl,
                        const types::Type* useType);

  // issue ambiguity / no matching candidates / etc error
  void issueErrorForFailedCallResolution(const uast::AstNode* astForErr,
                                         const CallInfo& ci,
                                         const CallResolutionResult& c);

  // handle the result of one of the functions to resolve a call. Handles:
  //  * r.setMostSpecific
  //  * r.setPoiScope
  //  * r.setType
  //  * issueErrorForFailedCallResolution if there was an error
  //  * poiInfo.accumulate
  void handleResolvedCall(ResolvedExpression& r,
                          const uast::AstNode* astForErr,
                          const CallInfo& ci,
                          const CallResolutionResult& c);
  // like handleResolvedCall saves the call in associatedFns.
  void handleResolvedAssociatedCall(ResolvedExpression& r,
                                    const uast::AstNode* astForErr,
                                    const CallInfo& ci,
                                    const CallResolutionResult& c);

  // e.g. (a, b) = mytuple
  // checks that tuple size matches and that the elements are assignable
  // saves any '=' called into r.associatedFns
  void resolveTupleUnpackAssign(ResolvedExpression& r,
                                const uast::AstNode* astForErr,
                                const uast::Tuple* lhsTuple,
                                types::QualifiedType lhsType,
                                types::QualifiedType rhsType);

  // helper for resolveTupleDecl
  // e.g. var (a, b) = mytuple
  // checks that tuple size matches and establishes types for a and b
  void resolveTupleUnpackDecl(const uast::TupleDecl* lhsTuple,
                              types::QualifiedType rhsType);

  // e.g. var (a, b) = mytuple
  void resolveTupleDecl(const uast::TupleDecl* td,
                        const types::Type* useType);

  // e.g. new shared C(a, 0)
  // also resolves initializer call as a side effect
  bool resolveSpecialNewCall(const uast::Call* call);

  // resolve a special op call such as tuple unpack assign
  bool resolveSpecialOpCall(const uast::Call* call);

  // helper to resolve a special call
  // returns 'true' if the call was a special call handled here, false
  // if it is a regular call.
  bool resolveSpecialCall(const uast::Call* call);

  /* What is the type for the symbol with a particular ID?
     localGenericUnknown, if true, indicates that a use of a
     field/formal with generic type (that is not substituted)
     should be resolved to unknown. That is important
     for initial resolution of such functions/types.
   */
  types::QualifiedType typeForId(const ID& id, bool localGenericToUnknown);

  // prepare the CallInfoActuals by inspecting the actuals of a call
  // includes special handling for operators and tuple literals
  void prepareCallInfoActuals(const uast::Call* call,
                              std::vector<CallInfoActual>& actuals,
                              bool& hasQuestionArg);

  // prepare a CallInfo by inspecting the called expression and actuals
  CallInfo prepareCallInfoNormalCall(const uast::Call* call);

  // resolve 'new R' for a given record type 'R'
  void resolveNewForRecord(const uast::New* node,
                           const types::RecordType* recordType);

  // resolve 'new C' for a given class type 'C', including management
  void resolveNewForClass(const uast::New* node,
                          const types::ClassType* classType);

  /* Resolver keeps a stack of scopes and a stack of decls.
     enterScope and exitScope update those stacks. */
  void enterScope(const uast::AstNode* ast);
  void exitScope(const uast::AstNode* ast);

  // the visitor methods
  bool enter(const uast::Literal* literal);
  void exit(const uast::Literal* literal);

  bool enter(const uast::Identifier* ident);
  void exit(const uast::Identifier* ident);

  bool enter(const uast::TypeQuery* tq);
  void exit(const uast::TypeQuery* tq);

  bool enter(const uast::NamedDecl* decl);
  void exit(const uast::NamedDecl* decl);

  bool enter(const uast::MultiDecl* decl);
  void exit(const uast::MultiDecl* decl);

  bool enter(const uast::TupleDecl* decl);
  void exit(const uast::TupleDecl* decl);

  // Note: Call cases here include Tuple
  bool enter(const uast::Call* call);
  void exit(const uast::Call* call);

  bool enter(const uast::Dot* dot);
  void exit(const uast::Dot* dot);

  bool enter(const uast::New* node);
  void exit(const uast::New* node);

  bool enter(const uast::For* loop);
  void exit(const uast::For* loop);

  // if none of the above is called, fall back on this one
  bool enter(const uast::AstNode* ast);
  void exit(const uast::AstNode* ast);
};


} // end namespace resolution
} // end namespace chpl

#endif
