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
                                       bool useGenericFormalDefaults);

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
                                        bool useGenericFormalDefaults);

/**
  Return true if 'name' is the name of a field for type 't'
*/
bool isNameOfField(Context* context, UniqueString name, const types::Type* t);

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
  Resolves a concrete function using the above queries.
  */
const ResolvedFunction* resolveConcreteFunction(Context* context, ID id);

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
 */
const types::QualifiedType& returnType(Context* context,
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
