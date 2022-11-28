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
ERROR_CLASS(InvalidNewTarget, const uast::New*, types::QualifiedType)
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
// catch-all for simple parsing errors that do not have a specialized error
// class
PARSER_ERROR_CLASS(ParseErr, std::string)
PARSER_SYNTAX_CLASS(ParseSyntax, std::string)
PARSER_SYNTAX_CLASS(CannotAttachPragmas, const uast::AstNode*)
PARSER_SYNTAX_CLASS(InvalidIndexExpr)
PARSER_ERROR_CLASS(RecordInheritanceNotSupported, std::string)
PARSER_SYNTAX_CLASS(InvalidNumericLiteral, std::string)
PARSER_ERROR_CLASS(MultipleExternalRenaming)
PARSER_WARNING_CLASS(PreIncDecOp, bool)
PARSER_SYNTAX_CLASS(InvalidNewForm, const uast::AstNode*)
PARSER_SYNTAX_CLASS(NewWithoutArgs, const uast::AstNode*)
PARSER_SYNTAX_CLASS(UseImportNeedsModule, bool)
PARSER_SYNTAX_CLASS(ExceptOnlyInvalidExpr,
                    uast::VisibilityClause::LimitationKind)
PARSER_SYNTAX_CLASS(LabelIneligibleStmt, const uast::AstNode*)
// Bison* errors are reported by the Bison parser to yyerror
PARSER_ERROR_CLASS(BisonMemoryExhausted)
PARSER_ERROR_CLASS(BisonUnknownError, std::string, std::string)
PARSER_SYNTAX_CLASS(BisonSyntaxError, std::string)

/* lexer-specific parser errors */
PARSER_SYNTAX_CLASS(StringLiteralEOF, char, int)
PARSER_SYNTAX_CLASS(ExternUnclosedPair, std::string)
PARSER_SYNTAX_CLASS(CommentEOF, Location, Location)
