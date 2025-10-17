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

// We enumerate resolution errors as macros in this file. By doing so,
// and by re-defining the DIAGNOSTIC_CLASS macro (which ERROR_CLASS and
// WARNING_CLASS are shorthands for), we can generate code for every error
// type, multiple times. With judicious definitions of DIAGNOSTIC_CLASS, we
// can generate an enum of all error types, class declarations for every
// error, as well as implementation of query / caching functions.
//
// Calls to the ERROR_CLASS/WARNING_CLASS/etc. macros should list the new
// error's name first, followed a list of types describing error details.
//
// Errors/warnings/etc should be ordered alphabetically by name.
//
// Also, the macro calls in this file should each be on a single line,
// even though that makes long lines that are normally avoided.

ERROR_CLASS(AmbiguousCall, const uast::Call*, ID, ID)
ERROR_CLASS(AmbiguousConfigName, std::string, const uast::Variable*, ID)
ERROR_CLASS(AmbiguousConfigSet, const uast::Variable*, std::string, std::string)
ERROR_CLASS(AmbiguousIdentifier, const uast::Identifier*, bool, resolution::MatchingIdsWithName, std::vector<resolution::ResultVisibilityTrace>)
ERROR_CLASS(AmbiguousVisibilityIdentifier, UniqueString, ID, resolution::MatchingIdsWithName)
ERROR_CLASS(AsWithUseExcept, const uast::Use*, const uast::As*)
ERROR_CLASS(AssignFieldBeforeInit, const uast::FnCall*, std::vector<std::pair<const uast::VarLikeDecl*, ID>>)
ERROR_CLASS(ConstRefCoercion, const uast::AstNode*, resolution::MostSpecificCandidate)
WARNING_CLASS(Deprecation, std::string, const uast::AstNode*, const uast::NamedDecl*)
ERROR_CLASS(DotExprInUseImport, const uast::VisibilityClause*, const uast::VisibilityClause::LimitationKind, const uast::Dot*)
ERROR_CLASS(DotTypeOnType, const uast::Dot*, const types::Type*, ID)
ERROR_CLASS(EnumAbstract, const uast::AstNode*, std::string, const types::EnumType*, const types::Type*)
ERROR_CLASS(EnumInitializerNotParam, const uast::EnumElement*, types::QualifiedType)
ERROR_CLASS(EnumInitializerNotInteger, const uast::EnumElement*, types::QualifiedType)
ERROR_CLASS(EnumValueAbstract, const uast::AstNode*, const types::EnumType*, const uast::EnumElement*)
ERROR_CLASS(ExternCCompilation, ID, std::vector<std::pair<Location, std::string>>)
WARNING_CLASS(GenericFieldWithoutMark, const uast::Decl*, types::QualifiedType)
ERROR_CLASS(IfVarNonClassType, const uast::Conditional*, types::QualifiedType)
WARNING_CLASS(ImplicitFileModule, const uast::AstNode*, const uast::Module*, const uast::Module*)
ERROR_CLASS(IncompatibleIfBranches, const uast::Conditional*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(IncompatibleKinds, types::QualifiedType::Kind, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(IncompatibleRangeBounds, const uast::Range*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(IncompatibleTypeAndInit, const uast::AstNode*, const uast::AstNode*, const uast::AstNode*, const types::Type*, const types::Type*)
ERROR_CLASS(IncompatibleYieldTypes, const uast::AstNode*, std::vector<std::tuple<uast::Function::IteratorKind, types::QualifiedType, const resolution::TypedFnSignature*>>)
ERROR_CLASS(InterfaceAmbiguousFn, const types::InterfaceType*, ID, const uast::Function*, std::vector<const resolution::TypedFnSignature*>)
ERROR_CLASS(InterfaceInvalidIntent, const types::InterfaceType*, ID, const resolution::TypedFnSignature*, const resolution::TypedFnSignature*)
ERROR_CLASS(InterfaceMissingAssociatedType, const types::InterfaceType*, ID, const uast::Variable*, resolution::CallInfo, std::vector<resolution::ApplicabilityResult>)
ERROR_CLASS(InterfaceMissingFn, const types::InterfaceType*, ID, const resolution::TypedFnSignature*, resolution::CallInfo, std::vector<resolution::ApplicabilityResult>)
ERROR_CLASS(InterfaceMultipleImplements, const uast::AggregateDecl*, const types::InterfaceType*, ID, ID)
ERROR_CLASS(InterfaceNaryInInherits, const uast::AggregateDecl*, const types::InterfaceType*, ID)
ERROR_CLASS(InterfaceReorderedFnFormals, const types::InterfaceType*, ID, const resolution::TypedFnSignature*, const resolution::TypedFnSignature*)
ERROR_CLASS(InvalidClassCast, const uast::PrimCall*, types::QualifiedType)
ERROR_CLASS(InvalidContinueBreakTarget, const uast::AstNode*, ID, types::QualifiedType)
ERROR_CLASS(InvalidDomainCall, const uast::FnCall*, std::vector<types::QualifiedType>)
ERROR_CLASS(InvalidImplementsActual, const uast::Implements*, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(InvalidImplementsArity, const uast::Implements*, const types::InterfaceType*, std::vector<types::QualifiedType>)
ERROR_CLASS(InvalidImplementsInterface, const uast::Implements*, types::QualifiedType)
ERROR_CLASS(InvalidIndexCall, const uast::FnCall*, types::QualifiedType)
ERROR_CLASS(InvalidSubdomainCall, const uast::FnCall*, types::QualifiedType)
ERROR_CLASS(InvalidSparseSubdomainCall, const uast::FnCall*, types::QualifiedType)
ERROR_CLASS(InvalidNewTarget, const uast::New*, types::QualifiedType)
ERROR_CLASS(InvalidParamCast, const uast::AstNode*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(InvalidSuper, const uast::Identifier*, types::QualifiedType)
ERROR_CLASS(IteratorsInOtherScopes, const uast::AstNode*, const resolution::TypedFnSignature*, std::vector<const resolution::TypedFnSignature*>)
ERROR_CLASS(LoopLabelOutsideBreakOrContinue, const uast::AstNode*, ID)
ERROR_CLASS(MemManagementNonClass, const uast::New*, const types::Type*)
ERROR_CLASS(MissingInclude, const uast::Include*, std::string)
ERROR_CLASS(MissingFormalInstantiation, const uast::AstNode*, std::vector<std::tuple<const uast::Decl*, types::QualifiedType>>)
ERROR_CLASS(MismatchedInitializerResult, const uast::AstNode*, const types::CompositeType*, const types::CompositeType*, std::vector<std::tuple<ID, chpl::UniqueString, types::QualifiedType, types::QualifiedType>>)
ERROR_CLASS(ModuleAsVariable, const uast::AstNode*, const uast::AstNode*, const uast::Module*)
ERROR_CLASS(MultipleEnumElems, const uast::AstNode*, chpl::UniqueString, const uast::Enum*, std::vector<ID>)
ERROR_CLASS(MultipleInheritance, const uast::Class*, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(MultipleQuestionArgs, const uast::FnCall*, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(NestedClassFieldRef, const uast::TypeDecl*, const uast::TypeDecl*, const uast::AstNode*, ID)
ERROR_CLASS(NoMatchingCandidates, const uast::AstNode*, resolution::CallInfo, std::vector<resolution::ApplicabilityResult>, std::vector<const uast::VarLikeDecl*>)
ERROR_CLASS(NoMatchingSuper, const uast::AstNode*, resolution::CallInfo, std::vector<resolution::ApplicabilityResult>, std::vector<const uast::VarLikeDecl*>, std::vector<std::pair<ID, ID>>)
ERROR_CLASS(NonClassInheritance, const uast::AggregateDecl*, const uast::AstNode*, const types::Type*)
ERROR_CLASS(NonIterable, const uast::AstNode*, const uast::AstNode*, types::QualifiedType, std::vector<std::tuple<uast::Function::IteratorKind, chpl::resolution::TheseResolutionResult>>)
ERROR_CLASS(NoMatchingEnumValue, const uast::AstNode*, const types::EnumType*, types::QualifiedType)
ERROR_CLASS(NotInModule, const uast::Dot*, ID, UniqueString, ID, bool)
ERROR_CLASS(NoTypeForEnumElem, const uast::Enum*, const uast::EnumElement*, types::QualifiedType, const uast::EnumElement*, types::QualifiedType)
ERROR_CLASS(PhaseTwoInitMarker, const uast::AstNode*, std::vector<ID>)
WARNING_CLASS(PotentiallySurprisingShadowing, ID, UniqueString, resolution::MatchingIdsWithName, std::vector<resolution::ResultVisibilityTrace>, resolution::MatchingIdsWithName, std::vector<resolution::ResultVisibilityTrace>)
ERROR_CLASS(PrivateToPublicInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(ProcDefExplicitAnonFormal, const uast::Function*, const uast::Formal*)
ERROR_CLASS(ProcTypeUnannotatedFormal, const uast::FunctionSignature*, const uast::AnonFormal*)
ERROR_CLASS(PrototypeInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(Recursion, UniqueString)
ERROR_CLASS(RecursionFieldDecl, const uast::AstNode*, const uast::AggregateDecl*, const types::CompositeType*, std::vector<TraceElement>)
ERROR_CLASS(RecursionModuleStmt, const uast::AstNode*, const uast::Module*, std::vector<TraceElement>)
ERROR_CLASS(Redefinition, ID, UniqueString, resolution::MatchingIdsWithName, std::vector<resolution::ResultVisibilityTrace>)
ERROR_CLASS(ReductionAssignNonIdentifier, const uast::OpCall*)
ERROR_CLASS(ReductionAssignNotReduceIntent, const uast::OpCall*, const uast::AstNode*)
ERROR_CLASS(ReductionAssignInvalidRhs, const uast::OpCall*, const uast::ReduceIntent*, const uast::AstNode*, resolution::CallInfo)
ERROR_CLASS(ReductionIntentChangesType, const uast::ReduceIntent*, const uast::AstNode*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(ReductionInvalidName, const uast::AstNode*, UniqueString, types::QualifiedType)
ERROR_CLASS(ReductionNotReduceScanOp, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(SelfDefinition, const uast::NamedDecl*, const uast::Identifier*)
ERROR_CLASS(SplitInitMismatchedConditionalTypes, const uast::Variable*, const uast::AstNode*, const types::QualifiedType, const types::QualifiedType, const int, const int)
ERROR_CLASS(SuperFromTopLevelModule, const uast::AstNode*, const uast::Module*, resolution::VisibilityStmtKind)
ERROR_CLASS(SyntacticGenericityMismatch, const uast::Decl*, const types::Type::Genericity, const types::Type::Genericity, types::QualifiedType)
WARNING_CLASS(FieldWithGenericManagement, const uast::Decl*)
WARNING_CLASS(TertiaryUseImportUnstable, UniqueString, const uast::AstNode*, const uast::VisibilityClause*, const resolution::Scope*, resolution::VisibilityStmtKind)
ERROR_CLASS(TupleDeclMismatchedElems, const uast::TupleDecl*, const types::TupleType*)
ERROR_CLASS(TupleDeclNotTuple, const uast::TupleDecl*, const types::Type*)
ERROR_CLASS(TupleDeclUnknownType, const uast::TupleDecl*)
ERROR_CLASS(TupleExpansionNamedArgs, const uast::OpCall*, const uast::FnCall*)
ERROR_CLASS(TupleExpansionNonTuple, const uast::FnCall*, const uast::OpCall*, types::QualifiedType)
ERROR_CLASS(TupleIndexOOB, const uast::Call*, const types::TupleType*, const int)
ERROR_CLASS(UnknownEnumElem, const uast::AstNode*, chpl::UniqueString, const uast::Enum*)
ERROR_CLASS(UnknownIdentifier, const uast::Identifier*, bool)
WARNING_CLASS(Unstable, std::string, const uast::AstNode*, const uast::NamedDecl*)
ERROR_CLASS(UseImportMultiplyDefined, chpl::UniqueString, const uast::AstNode*, const uast::AstNode*)
WARNING_CLASS(UseImportMultiplyMentioned, chpl::UniqueString, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(UseImportNotModule, const ID, const resolution::VisibilityStmtKind, std::string)
WARNING_CLASS(UseImportTransitiveRename, chpl::UniqueString, chpl::UniqueString, chpl::UniqueString, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(UseImportUnknownMod, const ID, const resolution::VisibilityStmtKind, std::string, std::string, std::vector<const uast::AstNode*>)
ERROR_CLASS(UseImportUnknownSym, std::string, const uast::AstNode*, const uast::VisibilityClause*, const resolution::Scope*, const resolution::VisibilityStmtKind, bool)
ERROR_CLASS(UseOfLaterVariable, const uast::AstNode*, ID, chpl::UniqueString)
ERROR_CLASS(UserDiagnosticEncounterError, UniqueString, ID)
ERROR_CLASS(UserDiagnosticEmitError, UniqueString, ID)
WARNING_CLASS(UserDiagnosticEncounterWarning, UniqueString, ID)
WARNING_CLASS(UserDiagnosticEmitWarning, UniqueString, ID)
ERROR_CLASS(ValueUsedAsType, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(VariableWithoutInitOrType, const uast::AstNode*, ID, chpl::UniqueString)
