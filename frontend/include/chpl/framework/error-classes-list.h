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

// We enumerate all the errors in Dyno as macros in this file. By doing so,
// and by re-defining the DIAGNOSTIC_CLASS macro (which ERROR_CLASS and
// WARNING_CLASS are shorthands for), we can generate code for every error
// type, multiple times. With judicious definitions of DIAGNOSTIC_CLASS, we
// can generate an enum of all error types, class declarations for every
// error, as well as implementation of query / caching functions.
//
// Calls to the ERROR_CLASS/WARNING_CLASS/etc. macros should list the new
// error's name first, followed a list of types describing error details.

ERROR_CLASS(IncompatibleIfBranches,
    const uast::Conditional*,
    types::QualifiedType,
    types::QualifiedType)
ERROR_CLASS(TupleExpansionNamedArgs, const uast::OpCall*, const uast::FnCall*)
ERROR_CLASS(MemManagementNonClass, const uast::New*, const types::Type*)
ERROR_CLASS(PrivateToPublicInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(PrototypeInclude, const uast::Include*, const uast::Module*)
ERROR_CLASS(MissingInclude, const uast::Include*, std::string)
ERROR_CLASS(UseImportUnknownSym, const uast::VisibilityClause*,
            const resolution::Scope*, const resolution::VisibilityStmtKind,
            std::string)
ERROR_CLASS(UseImportUnknownMod, const ID, const resolution::VisibilityStmtKind,
            std::string)
ERROR_CLASS(UseImportNotModule, const ID, const resolution::VisibilityStmtKind,
            std::string)
ERROR_CLASS(AsWithUseExcept, const uast::Use*, const uast::As*)
ERROR_CLASS(DotExprInUseImport, const uast::VisibilityClause*,
            const uast::VisibilityClause::LimitationKind, const uast::Dot*)
ERROR_CLASS(UnsupportedAsIdent, const uast::As*, const uast::AstNode*)
ERROR_CLASS(Redefinition, const uast::NamedDecl*, std::vector<ID>)
ERROR_CLASS(AmbiguousConfigName, std::string, const uast::Variable*, ID)
ERROR_CLASS(AmbiguousConfigSet,
    const uast::Variable*,
    std::string,
    std::string)
ERROR_CLASS(ValueUsedAsType, const uast::AstNode*, types::QualifiedType)
ERROR_CLASS(IncompatibleKinds,
    types::QualifiedType::Kind,
    const uast::AstNode*,
    types::QualifiedType)
ERROR_CLASS(IncompatibleTypeAndInit,
    const uast::AstNode*,
    const uast::AstNode*,
    const uast::AstNode*,
    const types::Type*,
    const types::Type*)
ERROR_CLASS(TupleDeclUnknownType, const uast::TupleDecl*)
ERROR_CLASS(TupleDeclNotTuple, const uast::TupleDecl*, const types::Type*)
ERROR_CLASS(TupleDeclMismatchedElems, const uast::TupleDecl*, const types::TupleType*)
ERROR_CLASS(UseOfLaterVariable, const uast::AstNode*, ID)
ERROR_CLASS(IncompatibleRangeBounds, const uast::Range*, types::QualifiedType, types::QualifiedType)
ERROR_CLASS(UnknownEnumElem, const uast::AstNode*, chpl::UniqueString, const types::EnumType*)
ERROR_CLASS(MultipleEnumElems, const uast::AstNode*, chpl::UniqueString, const types::EnumType*, std::vector<ID>)
ERROR_CLASS(InvalidNew, const uast::New*, types::QualifiedType)
ERROR_CLASS(MultipleQuestionArgs, const uast::FnCall*, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(TupleExpansionNonTuple, const uast::FnCall*, const uast::OpCall*, types::QualifiedType)
ERROR_CLASS(NonIterable, const uast::IndexableLoop*, const uast::AstNode*, types::QualifiedType)
WARNING_CLASS(ImplicitFileModule,
    const uast::AstNode*,
    const uast::Module*,
    const uast::Module*)
ERROR_CLASS(ProcTypeUnannotatedFormal,
    const uast::FunctionSignature*,
    const uast::AnonFormal*)
ERROR_CLASS(ProcDefExplicitAnonFormal,
    const uast::Function*,
    const uast::Formal*)
ERROR_CLASS(SuperFromTopLevelModule,
    const uast::AstNode*,
    const uast::Module*,
    resolution::VisibilityStmtKind)

/* parser errors */
ERROR_CLASS(UnknownPragma, const Location, std::string)
ERROR_CLASS(TypeCannotImplementInterface, const Location, std::string)
ERROR_CLASS(PragmasBeforeDeprecation, const Location)
ERROR_CLASS(CannotAttachPragmas, const Location)
ERROR_CLASS(PrimCallNamedArgs, const Location)
ERROR_CLASS(PrimCallNoStrLiteral, const Location)
ERROR_CLASS(UnknownPrimitive, const Location)
ERROR_CLASS(SecondaryTypeMethodNoType, const Location)
ERROR_CLASS(ThisIntentNotMethod, const Location)
ERROR_CLASS(InvalidIndexExpr, const Location)
ERROR_CLASS(ClassExportExtern, const Location)
ERROR_CLASS(UnionExport, const Location)
ERROR_CLASS(RecordInheritanceNotSupported, const Location)
ERROR_CLASS(UnionInheritanceNotAllowed, const Location)
ERROR_CLASS(MultipleInheritance, const Location)
ERROR_CLASS(InheritInvalidExpr, const Location)
ERROR_CLASS(InvalidNumericLiteral, const Location, std::string, std::string)
ERROR_CLASS(MultipleExternalRenaming, const Location)
ERROR_CLASS(ReduceIntentNoIdent, const Location)
WARNING_CLASS(PreIncDecOp, const Location, std::string, std::string)
ERROR_CLASS(NewWithoutArgs, const Location)
ERROR_CLASS(DotAfterNew, const Location)
ERROR_CLASS(InvalidNewForm, const Location)
ERROR_CLASS(InvalidIterandExpr, const Location)
ERROR_CLASS(UseImportNeedsModule, const Location, const bool)
ERROR_CLASS(ExceptOnlyInvalidExpr, const Location,
            const uast::VisibilityClause::LimitationKind)
// this error is very similar to InvalidAsIdent, but takes an explicit Location
// because it is raised during parsing when AstNodes do not have attached
// locations
ERROR_CLASS(ImportInvalidAs, const Location)
ERROR_CLASS(ImportInvalidExpr, const Location)
ERROR_CLASS(DeprecateForwardingStmt, const Location)
ERROR_CLASS(SelectMultipleOtherwise, const Location)
ERROR_CLASS(LabelIneligibleStmt, const Location)
ERROR_CLASS(VarargNotGrouped, const Location)
ERROR_CLASS(ArrayReturnInvalidDom, const Location)
ERROR_CLASS(TaskVarNameNotIdent, const Location)

/* lexer errors */
ERROR_CLASS(StringLiteralEOL, const Location)
ERROR_CLASS(NonHexChar, const Location)
ERROR_CLASS(HexOverflow, const Location, const bool)
ERROR_CLASS(UnknownHexError, const Location)
ERROR_CLASS(UniversalCharUnsupported, const Location)
ERROR_CLASS(OctalEscapeUnsupported, const Location)
ERROR_CLASS(UnexpectedStrEscape, const Location, const char)
ERROR_CLASS(StringLiteralEOF, const Location)
ERROR_CLASS(ExternUnclosedPair, const Location, std::string)
ERROR_CLASS(ExternCommentNoNewline, const Location)
ERROR_CLASS(CommentEOF, const Location, const Location, const Location)
