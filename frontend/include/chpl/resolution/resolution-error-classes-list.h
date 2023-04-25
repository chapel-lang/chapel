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
// Errors are grouped by which compilation stage they relate to, and sorted
// alphabetically within these groups. Groups themselves are ordered by the
// order their compilation stages occur in.
//
// Also, the macro calls in this file should each be on a single line,
// even though that makes long lines that are normally avoided.

ERROR_CLASS(AmbiguousConfigName, std::string, const uast::Variable*, ID)
ERROR_CLASS(AmbiguousConfigSet, const uast::Variable*, std::string, std::string)
ERROR_CLASS(AmbiguousIdentifier, const uast::Identifier*, bool, std::vector<resolution::BorrowedIdsWithName>, std::vector<resolution::ResultVisibilityTrace>)
ERROR_CLASS(AmbiguousVisibilityIdentifier, UniqueString, ID, std::vector<ID>)
ERROR_CLASS(AsWithUseExcept, const uast::Use*, const uast::As*)
WARNING_CLASS(Deprecation, std::string, const uast::AstNode*, const uast::NamedDecl*)
ERROR_CLASS(DotExprInUseImport, const uast::VisibilityClause*, const uast::VisibilityClause::LimitationKind, const uast::Dot*)
WARNING_CLASS(HiddenFormal, const uast::Formal*, resolution::BorrowedIdsWithName, resolution::ResultVisibilityTrace)
ERROR_CLASS(IfVarNonClassType, const uast::Conditional*, types::QualifiedType)
WARNING_CLASS(ImplicitFileModule, const uast::AstNode*, const uast::Module*, const uast::Module*)
ERROR_CLASS(IncompatibleIfBranches, const uast::Conditional*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(IncompatibleKinds, types::QualifiedType::Kind, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(IncompatibleRangeBounds, const uast::Range*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(IncompatibleTypeAndInit, const uast::AstNode*, const uast::AstNode*, const uast::AstNode*, const types::Type*, const types::Type*)
ERROR_CLASS(InvalidNewTarget, const uast::New*, types::QualifiedType)
ERROR_CLASS(InvalidSuper, const uast::Identifier*, types::QualifiedType)
ERROR_CLASS(MemManagementNonClass, const uast::New*, const types::Type*)
ERROR_CLASS(MissingInclude, const uast::Include*, std::string)
ERROR_CLASS(ModuleAsVariable, const uast::AstNode*, const uast::AstNode*, const uast::Module*)
ERROR_CLASS(MultipleEnumElems, const uast::AstNode*, chpl::UniqueString, const uast::Enum*, std::vector<ID>)
ERROR_CLASS(MultipleQuestionArgs, const uast::FnCall*, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(NestedClassFieldRef, const uast::AggregateDecl*, const uast::AggregateDecl*, const uast::AstNode*, ID)
ERROR_CLASS(NonIterable, const uast::AstNode*, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(NotInModule, const uast::Dot*, ID, UniqueString, ID)
ERROR_CLASS(PrivateToPublicInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(ProcDefExplicitAnonFormal, const uast::Function*, const uast::Formal*)
ERROR_CLASS(ProcTypeUnannotatedFormal, const uast::FunctionSignature*, const uast::AnonFormal*)
ERROR_CLASS(PrototypeInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(Redefinition, ID, UniqueString, std::vector<resolution::BorrowedIdsWithName>, std::vector<resolution::ResultVisibilityTrace>)
ERROR_CLASS(ReductionInvalidName, const uast::AstNode*, UniqueString, types::QualifiedType)
ERROR_CLASS(ReductionNotReduceScanOp, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(SuperFromTopLevelModule, const uast::AstNode*, const uast::Module*, resolution::VisibilityStmtKind)
ERROR_CLASS(TupleDeclMismatchedElems, const uast::TupleDecl*, const types::TupleType*)
ERROR_CLASS(TupleDeclNotTuple, const uast::TupleDecl*, const types::Type*)
ERROR_CLASS(TupleDeclUnknownType, const uast::TupleDecl*)
ERROR_CLASS(TupleExpansionNamedArgs, const uast::OpCall*, const uast::FnCall*)
ERROR_CLASS(TupleExpansionNonTuple, const uast::FnCall*, const uast::OpCall*, types::QualifiedType)
ERROR_CLASS(UnknownEnumElem, const uast::AstNode*, chpl::UniqueString, const uast::Enum*)
ERROR_CLASS(UnknownIdentifier, const uast::Identifier*, bool)
WARNING_CLASS(Unstable, std::string, const uast::AstNode*, const uast::NamedDecl*)
ERROR_CLASS(UseImportMultiplyDefined, chpl::UniqueString, const uast::AstNode*, const uast::AstNode*)
WARNING_CLASS(UseImportMultiplyMentioned, chpl::UniqueString, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(UseImportNotModule, const ID, const resolution::VisibilityStmtKind, std::string)
WARNING_CLASS(UseImportTransitiveRename, chpl::UniqueString, chpl::UniqueString, chpl::UniqueString, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(UseImportUnknownMod, const ID, const resolution::VisibilityStmtKind, std::string, std::string, std::vector<const uast::AstNode*>)
ERROR_CLASS(UseImportUnknownSym, std::string, const uast::AstNode*, const uast::VisibilityClause*, const resolution::Scope*, const resolution::VisibilityStmtKind, bool)
ERROR_CLASS(UseOfLaterVariable, const uast::AstNode*, ID)
ERROR_CLASS(ValueUsedAsType, const uast::AstNode*, types::QualifiedType)
