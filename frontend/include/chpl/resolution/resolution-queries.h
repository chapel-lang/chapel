/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_RESOLUTION_QUERIES_H
#define CHPL_RESOLUTION_RESOLUTION_QUERIES_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-types.h"

namespace chpl {
namespace resolution {


////// resolution basics

/**
  Resolve a module-level statement or variable declaration.
 */
const ResolutionResultByPostorderID& resolveModuleStmt(Context* context, ID id);

/**
  Resolve the contents of a Module
 */
const ResolutionResultByPostorderID& resolveModule(Context* context, ID id);

/**
  Resolve the contents of a Module but don't resolve any paren-ful function
  calls or establish types.
 */
const ResolutionResultByPostorderID& scopeResolveModule(Context* context,
                                                        ID id);

/**
  Compute the type for a NamedDecl with a particular id.
 */
const types::QualifiedType& typeForModuleLevelSymbol(
    Context* context, ID id, bool currentModule = false);

/**
  Compute the type for a Builtin type using just its name
 */
const types::QualifiedType& typeForBuiltin(Context* context, UniqueString name);

/**
  Get the QualifiedType for the well-known types in ChapelSysCTypes
 */
const types::QualifiedType& typeForSysCType(Context* context,
                                            UniqueString name);

/**
  Compute the type for a literal
 */
types::QualifiedType typeForLiteral(Context* context,
                                    const uast::Literal* literal);

/**
  Returns the type that results when instantiating formalType,
  which must be generic, with actualType.
 */
types::QualifiedType getInstantiationType(Context* context,
                                          types::QualifiedType actualType,
                                          types::QualifiedType formalType);

/**
  Returns a map from enum element IDs to their numeric values.
  The caller is responsible for validating that node is an enum ID.
  If an invalid ID is given, an empty map is returned.

  Abstract elements are not stored in the returned map, to distinguish
  from non-abstract elements whose values could not be computed.
 */
const std::map<ID, types::QualifiedType>&
computeNumericValuesOfEnumElements(Context* context, ID node);

const chpl::optional<types::QualifiedType>&
computeUnderlyingTypeOfEnum(Context* context, ID element);

/**
  Returns the numeric value of an enum element.
  The caller is responsible for validating that element is an enum element ID.
  If an invalid ID is given, an empty optional is returned.
 */
const chpl::optional<types::QualifiedType>&
computeNumericValueOfEnumElement(Context* context, ID element);

ID lookupEnumElementByNumericValue(Context* context,
                                   const ID& node,
                                   const types::QualifiedType& value);

/////// function resolution

/**
  Compute a TypedFnSignature from an UntypedFnSignature.
  (An UntypedFnSignature can be computed with UntypedFnSignature::get()).
  The TypedFnSignature will represent generic and potentially unknown
  types if the function is generic.
 */
const TypedFnSignature* const&
typedSignatureInitial(ResolutionContext* rc, const UntypedFnSignature* untyped);

/**
  Compute a initial TypedFnSignature for an ID.
  The TypedFnSignature will represent generic and potentially unknown
  types if the function is generic.
 */
const TypedFnSignature*
typedSignatureInitialForId(ResolutionContext* rc, ID id);

/**
  Returns a Type that represents the initial type provided by a TypeDecl
  (e.g. Class, Record, etc). This type does not store the fields.
  */
const types::Type* initialTypeForTypeDecl(Context* context, ID declId);

/**
  Resolve a single field decl (which could be e.g. a MultiDecl)
  within a CompositeType.

  The result will not have summary information computed.
  fieldsForTypeDecl should be used instead unless there
  is a reason that one-at-a-time resolution is important.

  If syntaxOnly is set, computes basic information (field order, IDs)
  but does not compute types.
 */
const ResolvedFields& resolveFieldDecl(Context* context,
                                       const types::CompositeType* ct,
                                       ID fieldId,
                                       DefaultsPolicy defaultsPolicy,
                                       bool syntaxOnly = false);

/**
  Compute the fields and their types for a CompositeType
  (such as one returned by initialTypeForTypeDecl).

  If useGenericFormalDefaults is true, a generic field like
    record R {
      type t = int;
    }
  will be assumed to have the default value (int in the above case).
  Otherwise, these fields will remain generic.

  Even if useGenericFormalDefaults is set, the default value will be ignored if
  the field already has a substitution in the CompositeType.

  The returned fields do not include any parent class fields.

  If syntaxOnly is set, computes basic information (field order, IDs)
  but does not compute types.
 */
const ResolvedFields& fieldsForTypeDecl(Context* context,
                                        const types::CompositeType* ct,
                                        DefaultsPolicy defaultsPolicy,
                                        bool syntaxOnly = false);

/**
  If 'name' is the name of a field for type 't', returns a non-null pointer;
  Otherwise, returns 'nullptr'.

  The returned pointer will point to the type containing the field.
  For records and unions, that will just be 't'.
  For classes, it won't necessarily be 't', since a field
  might come from a superclass. If the field comes from a superclass,
  this function will return the BasicClass type for the superclass
  that contains the field directly.

*/
const types::CompositeType* isNameOfField(Context* context,
                                          UniqueString name,
                                          const types::Type* t);

/**
  Computes the version of a type assuming that defaults for generics
  are needed. So, for 'record R { type t = int; }', this will return R(int).
 */
const types::QualifiedType typeWithDefaults(Context* context,
                                            types::QualifiedType t);

/**
  Compute whether a type is generic or not.
  Considers the field of a record/class.
  For a UnknownType, returns MAYBE_GENERIC.

  Considers types in the ignore set as concrete.
 */
types::Type::Genericity
getTypeGenericityIgnoring(Context* context,
                          const types::Type* t,
                          std::set<const types::Type*>& ignore);

/**
  Compute whether a QualifiedType is generic or not.
  Considers the field of a record/class.
  For a UnknownType, returns MAYBE_GENERIC.

  Considers types in the ignore set as concrete.
 */
types::Type::Genericity
getTypeGenericityIgnoring(Context* context,
                          types::QualifiedType qt,
                          std::set<const types::Type*>& ignore);
/**
  Compute whether a QualifiedType is generic or not.
  Considers the field of a record/class.
  For a UnknownType, returns MAYBE_GENERIC.
*/
types::Type::Genericity getTypeGenericity(Context* context,
                                          const types::Type* t);

/**
  Compute whether a QualifiedType is generic or not.
  Considers the field of a record/class.
  For a UnknownType, returns MAYBE_GENERIC.
 */
types::Type::Genericity getTypeGenericity(Context* context,
                                          types::QualifiedType qt);


bool isFieldSyntacticallyGeneric(Context* context,
                                 const ID& field,
                                 types::QualifiedType* formalType = nullptr);

/**
  Returns true if the field should be included in the type constructor.
  In that event, also sets formalType to the type the formal should use.

  This is also used to decide if a field needs to be include in a type's
  substitutions.
 */
bool shouldIncludeFieldInTypeConstructor(Context* context,
                                         const ID& fieldId,
                                         const types::QualifiedType& fieldType,
                                         types::QualifiedType* formalType = nullptr);

/**
  Compute an initial TypedFnSignature for a type constructor for a
  particular type. If some fields of `t` are still generic,
  it will be necessary to call instantiateSignature on it.
 */
const TypedFnSignature* typeConstructorInitial(Context* context,
                                               const types::Type* t);

/**
  Instantiate a TypedFnSignature from
   * the result of typedSignatureInitial,
   * a CallInfo describing the types at the call site, and
   * a point-of-instantiation scope representing the POI scope of the call
 */
ApplicabilityResult instantiateSignature(ResolutionContext* rc,
                                         const TypedFnSignature* sig,
                                         const CallInfo& call,
                                         const PoiScope* poiScope);

/**
  Compute a ResolvedFunction given a TypedFnSignature.
  Checks the generic cache for potential for reuse. When reuse occurs,
  the ResolvedFunction might point to a different TypedFnSignature.
 */
const ResolvedFunction* resolveFunction(ResolutionContext* rc,
                                        const TypedFnSignature* sig,
                                        const PoiScope* poiScope);

/**
  Helper to resolve a concrete function using the above queries.
  Will return `nullptr` if the function is generic or has a `where false`.
  */
const ResolvedFunction*
resolveConcreteFunction(Context* context, ID id);

/**
  Compute a ResolvedFunction given a TypedFnSignature, but don't
  do full resolution of types or paren-ful calls in the body.
 */
const ResolvedFunction* scopeResolveFunction(Context* context, ID id);

/*
 * Scope-resolve an AggregateDecl's fields, along with their type expressions
 * and initialization expressions.
 */
const ResolutionResultByPostorderID& scopeResolveAggregate(Context* context,
                                                          ID id);

/*
 * Scope-resolve an EnumDecl's constants
 */
const ResolutionResultByPostorderID& scopeResolveEnum(Context* context,
                                                      ID id);

/**
  Compute the return/yield type for a function.

  TODO: If the function returns a param, the param's value may not
  be available. This is because the function body is not resolved when
  the return type is explicitly declared. We probably still want to compute
  the value in such cases, though.
 */
types::QualifiedType returnType(ResolutionContext* rc,
                                const TypedFnSignature* sig,
                                const PoiScope* poiScope);

/**
  Compute the types for any generic 'out' formal types after instantiation
  of any other generic arguments.

  'out' formals with concrete type will already have their types
  represented in the 'sig' passed here (through typedSignatureInitial and
  potentially instantiateSignature).

  For the generic 'out' formals, their types are inferred from the
  body of the function.

  The returned TypedFnSignature* will have the inferred out formal types.
 */
const TypedFnSignature* inferOutFormals(ResolutionContext* rc,
                                        const TypedFnSignature* sig,
                                        const PoiScope* poiScope);

/**
  Try to compute the TypedFnSignature with REF_MAYBE_CONST formals computed
  as 'ref' or 'const ref'. If the TypedFnSignature is currently being resolved,
  instead of returning a new TypedFnSignature, this function returns
  'nullptr'. In that case, the caller is responsible for attempting this
  again later once the current set of recursive functions is resolved.
 */
const TypedFnSignature* inferRefMaybeConstFormals(ResolutionContext* rc,
                                                  const TypedFnSignature* sig,
                                                  const PoiScope* poiScope);

/////// call resolution

/**
  Compute the (potentially generic) TypedFnSignatures of possibly applicable
  candidate functions from a list of visible functions.
 */
const CandidatesAndForwardingInfo&
filterCandidatesInitial(ResolutionContext* rc,
                        MatchingIdsWithName lst,
                        CallInfo call);

/**
  Further filter the result of filterCandidatesInitial down by doing
  instantiations. After this, all of the resulting TypedFnSignatures
  are actually candidates.

  If instantiation occurs, gets/creates the new POI scope for
  inScope/inPoiScope.

 */
void
filterCandidatesInstantiating(ResolutionContext* rc,
                              const CandidatesAndForwardingInfo& lst,
                              const CallInfo& call,
                              const Scope* inScope,
                              const PoiScope* inPoiScope,
                              CandidatesAndForwardingInfo& result,
                              std::vector<ApplicabilityResult>* rejected = nullptr);

/**
  Given a uast::Call, a CallInfo representing the call, a Scope representing the
  scope of that call, and a PoiScope representing the point-of-instantiation
  scope of that call, find the most specific candidates as well
  as the point-of-instantiation scopes that were used when resolving them.

  'resolveCallInMethod' should be used instead when resolving a non-method call
  within a method.
 */
CallResolutionResult resolveCall(ResolutionContext* rc,
                                 const uast::Call* call,
                                 const CallInfo& ci,
                                 const CallScopeInfo& inScopes,
                                 std::vector<ApplicabilityResult>* rejected=nullptr);

/**
  Similar to resolveCall, but handles the implicit scope provided by a method.

  When a resolving a call within a method, the implicitReceiver should be
  set to the 'this' type of the method.

  If implicitReceiver.type() == nullptr, it will be ignored.
 */
CallResolutionResult resolveCallInMethod(ResolutionContext* rc,
                                         const uast::Call* call,
                                         const CallInfo& ci,
                                         const CallScopeInfo& inScopes,
                                         types::QualifiedType implicitReceiver,
                                         std::vector<ApplicabilityResult>* rejected=nullptr);

/**
  Given a CallInfo representing a call, a Scope representing the
  scope of that call, and a PoiScope representing the point-of-instantiation
  scope of that call, find the most specific candidates as well
  as the point-of-instantiation scopes that were used when resolving them.
 */
CallResolutionResult resolveGeneratedCall(Context* context,
                                          const uast::AstNode* astForErrAndPoi,
                                          const CallInfo& ci,
                                          const CallScopeInfo& inScopes,
                                          std::vector<ApplicabilityResult>* rejected = nullptr);

/**
  Similar to resolveGeneratedCall but handles the implicit scope
  provided by a method.

  When a resolving a call within a method, the implicitReceiver should be
  set to the 'this' type of the method.

  If implicitReceiver.type() == nullptr, it will be ignored.
 */
CallResolutionResult
resolveGeneratedCallInMethod(Context* context,
                             const uast::AstNode* astForErrAndPoi,
                             const CallInfo& ci,
                             const CallScopeInfo& inScopes,
                             types::QualifiedType implicitReceiver);

// tries to resolve an (unambiguous) init=
const TypedFnSignature* tryResolveInitEq(Context* context,
                                         const uast::AstNode* astForScopeOrErr,
                                         const types::Type* lhsType,
                                         const types::Type* rhsType,
                                         const PoiScope* poiScope = nullptr);

// tries to resolve an (unambiguous) assign
const TypedFnSignature* tryResolveAssign(Context* context,
                                         const uast::AstNode* astForScopeOrErr,
                                         const types::Type* lhsType,
                                         const types::Type* rhsType,
                                         const PoiScope* poiScope = nullptr);

// tries to resolve an (unambiguous) deinit
const TypedFnSignature* tryResolveDeinit(Context* context,
                                         const uast::AstNode* astForScopeOrErr,
                                         const types::Type* t,
                                         const PoiScope* poiScope = nullptr);

/**
  Given a type 't', compute whether or not 't' is default initializable.
  If 't' is a generic type, it is considered non-default-initializable.
  Considers the fields and substitutions of composite types.
*/
bool isTypeDefaultInitializable(Context* context, const types::Type* t);

/**
  Determine whether type 't' is copyable/assignable from const or/and from ref.
  When checkCopyable is true, this checks copyability, and for false checks
  assignability.
*/
CopyableAssignableInfo getCopyOrAssignableInfo(Context* context,
                                               const types::Type* t,
                                               bool checkCopyable);

/**
  Determine the types of various compiler-generated globals, which depend
  on the settings the compiler / Dyno was started with.
 */
const std::unordered_map<UniqueString, types::QualifiedType>&
getCompilerGeneratedGlobals(Context* context);

void
reportInvalidMultipleInheritance(Context* context,
                                 const uast::Class* node,
                                 const uast::AstNode* firstParent,
                                 const uast::AstNode* secondParent);

/**
  One of the compiler primitives has the side effect of collecting all
  test functions. This helper retrieves the list of test functions that has
  been collected.
 */
const std::vector<const uast::Function*>& getTestsGatheredViaPrimitive(Context* context);

/**
  Retrieve the 'param' tag for an iterator constant. If the 'iterKind' enum
  is not available, this will return an unknown type.
 */
const types::QualifiedType&
getIterKindConstantOrUnknown(Context* context, uast::Function::IteratorKind kind);

/**
  Returns the field in 'ad' (or its parent) that matches 'name'.
*/
const uast::Decl* findFieldByName(Context* context,
                                  const uast::AggregateDecl* ad,
                                  const types::CompositeType* ct,
                                  UniqueString name);

const MostSpecificCandidate&
findTaggedIteratorForType(ResolutionContext* rc,
                          const types::FnIteratorType* fnIter,
                          uast::Function::IteratorKind iterKind);

const types::QualifiedType&
taggedYieldTypeForType(ResolutionContext* rc,
                       const types::FnIteratorType* fnIter,
                       uast::Function::IteratorKind iterKind);

CallResolutionResult resolveTheseCall(ResolutionContext* rc,
                                      const uast::AstNode* iterand,
                                      const types::QualifiedType& receiverType,
                                      uast::Function::IteratorKind iterKind,
                                      const types::QualifiedType& followThis,
                                      const CallScopeInfo& inScopes);


} // end namespace resolution
} // end namespace chpl
#endif
