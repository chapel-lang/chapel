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

// We enumerate parser errors as macros in this file. By doing so,
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

// Bison* errors are reported by the Bison parser to yyerror
PARSER_ERROR_CLASS(BisonMemoryExhausted)
PARSER_SYNTAX_CLASS(BisonSyntaxError, std::string)
PARSER_ERROR_CLASS(BisonUnknownError, std::string, std::string)

// other parser errors
PARSER_SYNTAX_CLASS(CannotAttachPragmas, const uast::AstNode*)
PARSER_SYNTAX_CLASS(CommentEOF, Location, Location)
PARSER_SYNTAX_CLASS(ExceptOnlyInvalidExpr, uast::VisibilityClause::LimitationKind)
PARSER_SYNTAX_CLASS(ExternUnclosedPair, std::string)
PARSER_SYNTAX_CLASS(InvalidIndexExpr)
PARSER_SYNTAX_CLASS(InvalidNewForm, const uast::AstNode*)
PARSER_SYNTAX_CLASS(InvalidNumericLiteral, std::string)
PARSER_SYNTAX_CLASS(LabelIneligibleStmt, const uast::AstNode*)
PARSER_ERROR_CLASS(MultipleExternalRenaming)
PARSER_SYNTAX_CLASS(NewWithoutArgs, const uast::AstNode*)
PARSER_WARNING_CLASS(PreIncDecOp, bool)
PARSER_WARNING_CLASS(SingleStmtReturnDeprecated, const uast::AstNode*)
PARSER_ERROR_CLASS(RecordInheritanceNotSupported, std::string)
PARSER_SYNTAX_CLASS(StringLiteralEOF, char, int)
PARSER_SYNTAX_CLASS(UseImportNeedsModule, bool)

// ParseErr and ParseSyntax are catch-alls for simple parsing errors that do not
// have a specialized error class
PARSER_ERROR_CLASS(ParseErr, std::string)
PARSER_SYNTAX_CLASS(ParseSyntax, std::string)

/* begin post-parse-checks errors */
POSTPARSE_ERROR_CLASS(CantApplyPrivate, std::string)
ERROR_CLASS(DisallowedControlFlow, const uast::AstNode*, const uast::AstNode*, const uast::AstNode*)
ERROR_CLASS(IllegalUseImport, const uast::AstNode*, const uast::AstNode*)
POSTPARSE_ERROR_CLASS(MultipleManagementStrategies, const uast::New::Management, const uast::New::Management)
WARNING_CLASS(ParenlessAttributeArgDeprecated, const uast::Attribute*)
POSTPARSE_ERROR_CLASS(PostParseErr, std::string)
POSTPARSE_WARNING_CLASS(PostParseWarn, std::string)
ERROR_CLASS(UnsupportedAsIdent, const uast::As*, const uast::AstNode*)
/* end post-parse-checks errors */
