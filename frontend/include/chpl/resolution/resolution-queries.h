/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
const types::QualifiedType& typeForModuleLevelSymbol(Context* context, ID id);

/**
  Compute the type for a Builtin type using just its name
 */
const types::QualifiedType& typeForBuiltin(Context* context, UniqueString name);

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


/////// function resolution

/**
  Compute a TypedFnSignature from an UntypedFnSignature.
  (An UntypedFnSignature can be computed with UntypedFnSignature::get()).
  The TypedFnSignature will represent generic and potentially unknown
  types if the function is generic.
 */
const TypedFnSignature*
typedSignatureInitial(Context* context,
                      const UntypedFnSignature* untypedSig);

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
 */
const ResolvedFields& resolveFieldDecl(Context* context,
                                       const types::CompositeType* ct,
                                       ID fieldId,
                                       DefaultsPolicy defaultsPolicy);

/**
  Compute the types of the fields for a CompositeType
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
 */
const ResolvedFields& fieldsForTypeDecl(Context* context,
                                        const types::CompositeType* ct,
                                        DefaultsPolicy defaultsPolicy);

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

  Returns nullptr if the instantiation failed.
 */
const TypedFnSignature* instantiateSignature(Context* context,
                                             const TypedFnSignature* sig,
                                             const CallInfo& call,
                                             const PoiScope* poiScope);

/**
  Compute a ResolvedFunction given a TypedFnSignature.
  Checks the generic cache for potential for reuse. When reuse occurs,
  the ResolvedFunction might point to a different TypedFnSignature.
 */
const ResolvedFunction* resolveFunction(Context* context,
                                        const TypedFnSignature* sig,
                                        const PoiScope* poiScope);

/**
  Compute a ResolvedFunction given a TypedFnSignature for an initializer.
  The difference between this and 'resolveFunction' is that it is
  possible for the type of the receiver to still be generic (as the
  initializer body must be resolved before the concrete type is known).
*/
const ResolvedFunction* resolveInitializer(Context* context,
                                           const TypedFnSignature* sig,
                                           const PoiScope* poiScope);

/**
  Helper to resolve a concrete function using the above queries.
  Will return `nullptr` if the function is generic or has a `where false`.
  */
const ResolvedFunction* resolveConcreteFunction(Context* context, ID id);

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

/**
  Returns the ResolvedFunction called by a particular
  ResolvedExpression, if there was exactly one candidate.
  Otherwise, it returns nullptr.

  This function does not handle return intent overloading.
 */
const ResolvedFunction* resolveOnlyCandidate(Context* context,
                                             const ResolvedExpression& r);
/**
  Compute the return/yield type for a function.

  TODO: If the function returns a param, the param's value may not
  be available. This is because the function body is not resolved when
  the return type is explicitly declared. We probably still want to compute
  the value in such cases, though.
 */
const types::QualifiedType& returnType(Context* context,
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
const TypedFnSignature* inferOutFormals(Context* context,
                                        const TypedFnSignature* sig,
                                        const PoiScope* poiScope);

/**
  Try to compute the TypedFnSignature with REF_MAYBE_CONST formals computed
  as 'ref' or 'const ref'. If the TypedFnSignature is currently being resolved,
  instead of returning a new TypedFnSignature, this function returns
  'nullptr'. In that case, the caller is responsible for attempting this
  again later once the current set of recursive functions is resolved.
 */
const TypedFnSignature* inferRefMaybeConstFormals(Context* context,
                                                  const TypedFnSignature* sig,
                                                  const PoiScope* poiScope);

/////// call resolution

/**
  Compute the (potentially generic) TypedFnSignatures of possibly applicable
  candidate functions from a list of visible functions.
 */
const std::vector<const TypedFnSignature*>&
filterCandidatesInitial(Context* context,
                        std::vector<BorrowedIdsWithName> lst,
                        CallInfo call);

/**
  Further filter the result of filterCandidatesInitial down by doing
  instantiations. After this, all of the resulting TypedFnSignatures
  are actually candidates.

  If instantiation occurs, gets/creates the new POI scope for
  inScope/inPoiScope.

 */
void
filterCandidatesInstantiating(Context* context,
                              const std::vector<const TypedFnSignature*>& lst,
                              const CallInfo& call,
                              const Scope* inScope,
                              const PoiScope* inPoiScope,
                              std::vector<const TypedFnSignature*>& result);

/**
  Given a uast::Call, a CallInfo representing the call, a Scope representing the
  scope of that call, and a PoiScope representing the point-of-instantiation
  scope of that call, find the most specific candidates as well
  as the point-of-instantiation scopes that were used when resolving them.
 */
CallResolutionResult resolveCall(Context* context,
                                 const uast::Call* call,
                                 const CallInfo& ci,
                                 const Scope* inScope,
                                 const PoiScope* inPoiScope);

/**
  Given a CallInfo representing a call, a Scope representing the
  scope of that call, and a PoiScope representing the point-of-instantiation
  scope of that call, find the most specific candidates as well
  as the point-of-instantiation scopes that were used when resolving them.
 */
CallResolutionResult resolveGeneratedCall(Context* context,
                                          const uast::AstNode* astForErr,
                                          const CallInfo& ci,
                                          const Scope* inScope,
                                          const PoiScope* inPoiScope);


} // end namespace resolution
} // end namespace chpl
#endif
