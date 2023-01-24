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
 *
 * limitations under the License.
 */
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/scope-types.h"
#include "chpl/framework/query-impl.h"
#include "chpl/uast/VisibilityClause.h"
#include "chpl/uast/AstTag.h"
#include "chpl/types/all-types.h"
#include <sstream>
#include <cstring>

namespace chpl {

// Generate query function implementations, like ErrorMessage::get for every
// error type. We do this by defining the DIAGNOSTIC_CLASS macro, and including
// error-classes-list.h which invokes this macro (and thus
// DIAGNOSTIC_CLASS_IMPL) for every error type.
#define DIAGNOSTIC_CLASS DIAGNOSTIC_CLASS_IMPL
#include "chpl/framework/parser-error-classes-list.h"
#undef DIAGNOSTIC_CLASS

//
// Below are the implementations of 'write' for each error class, which does
// the specialized work.
//
//

void ErrorBisonMemoryExhausted::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  wr.heading(kind_, type_, loc, "memory exhausted while parsing.");
}

void ErrorBisonSyntaxError::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto nearestToken = std::get<std::string>(info);
  wr.heading(kind_, type_, loc,
             (nearestToken.empty() ? "(no token given)"
                                   : "near '" + nearestToken + "'"),
             ":");
  wr.code(loc);
}

void ErrorBisonUnknownError::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto errorMessage = std::get<1>(info);
  auto nearestToken = std::get<2>(info);
  wr.heading(kind_, type_, loc,
             "unknown error from Bison parser: ", errorMessage, ".");
  if (!nearestToken.empty()) {
    wr.note(loc, "Nearest token when error was encountered: '", nearestToken,
            "'");
  }
}

void ErrorCannotAttachPragmas::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto stmt = std::get<const uast::AstNode*>(info);
  wr.heading(kind_, type_, loc, "cannot attach 'pragma's to this statement.");
  if (stmt) {
    wr.code(stmt);
  }
  wr.message(
      "Only declarations, such as variable declarations, can have 'pragma's "
      "attached to them.");
}

void ErrorCommentEOF::write(ErrorWriterBase& wr) const {
  auto loc = std::get<0>(info);
  auto startLoc = std::get<1>(info);
  auto nestedLoc = std::get<2>(info);
  wr.heading(kind_, type_, loc, "end-of-file in comment.");
  wr.note(startLoc, "unterminated comment here:");
  wr.code(startLoc);
  if (!nestedLoc.isEmpty()) {
    wr.note(nestedLoc, "nested comment here:");
    wr.code(nestedLoc);
  }
}

void ErrorExceptOnlyInvalidExpr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto limitationKind = std::get<uast::VisibilityClause::LimitationKind>(info);
  wr.heading(kind_, type_, loc, "incorrect expression in '", limitationKind,
             "' list, identifier expected.");
  wr.message("In the '", limitationKind, "' list here:");
  wr.code(loc);
}

void ErrorExternUnclosedPair::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto pairSymbol = std::get<std::string>(info);
  wr.heading(kind_, type_, loc, "missing closing ", pairSymbol,
             " symbol in extern block.");
  wr.message("In this extern block:");
  wr.code(loc);
}

void ErrorInvalidIndexExpr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  wr.heading(kind_, type_, loc,
             "only identifiers or tuples of identifiers are allowed as index "
             "expressions.");
  wr.message("Invalid index expression used here:");
  wr.code(loc);
}

void ErrorInvalidNewForm::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto newExpr = std::get<const uast::AstNode*>(info);
  wr.heading(kind_, type_, loc, "Invalid form for 'new' expression.");
  wr.message("'new' expression used here:");
  wr.code(loc, {newExpr});
}

void ErrorInvalidNumericLiteral::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto errMessage = std::get<std::string>(info);
  CHPL_ASSERT(!errMessage.empty());
  CHPL_ASSERT(errMessage.back() == '.' &&
         "expected a period at the end of InvalidNumericLiteral message");
  errMessage[0] = std::tolower(errMessage[0]);
  wr.heading(kind_, type_, loc, errMessage);
  wr.message("Numeric literal encountered here:");
  wr.code(loc);
}

void ErrorLabelIneligibleStmt::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto maybeStmt = std::get<const uast::AstNode*>(info);
  if (maybeStmt && maybeStmt->tag() == uast::AstTag::EmptyStmt) {
    // this is the case where there is a semicolon following the label name
    wr.heading(kind_, type_, loc,
               "labels should not be terminated with semicolons.");
  } else {
    wr.heading(kind_, type_, loc, "cannot label this kind of statement.");
  }
  wr.message("Label on ineligible statement here:");
  wr.code(loc);
  wr.message("Only 'for', 'while', and 'do-while' statements can have labels.");
}

void ErrorMultipleExternalRenaming::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  wr.heading(
      kind_, type_, loc,
      "external symbol renaming can only be applied to one symbol at a time.");
  wr.message("Multiple renaming used here:");
  wr.code(loc);
}

void ErrorNewWithoutArgs::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto expr = std::get<const uast::AstNode*>(info);
  wr.heading(kind_, type_, loc,
             "'new' expression is missing its argument list.");
  wr.message("'new' expression used here:");
  wr.code(loc, { expr });
  wr.message("Perhaps you intended to write 'new ", expr, "()' instead?");
}

void ErrorParseErr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto errorMessage = std::get<std::string>(info);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorParseErr message");
  wr.heading(kind_, type_, loc, errorMessage);
  wr.code(loc);
}

void ErrorParseSyntax::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto errorMessage = std::get<std::string>(info);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorParseSyntax message");
  wr.heading(kind_, type_, loc, errorMessage);
  wr.code(loc);
}

void ErrorPreIncDecOp::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto isDoublePlus = std::get<bool>(info);
  if (isDoublePlus) {
    wr.heading(kind_, type_, loc, "'++' is not a pre-increment.");
  } else {
    wr.heading(kind_, type_, loc, "'--' is not a pre-decrement.");
  }
  wr.message("Used here:");
  wr.code(loc);
}

void ErrorRecordInheritanceNotSupported::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto recordName = std::get<std::string>(info);
  wr.heading(kind_, type_, loc,
             "inheritance is not currently supported for records.");
  wr.note(loc, recordName, " declared as a record here:");
  wr.code(loc);
  wr.message(
      "Thoughts on what record inheritance should entail can be added to "
      "https://github.com/chapel-lang/chapel/issues/6851.");
}

void ErrorStringLiteralEOF::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto startChar = std::get<char>(info);
  auto startCharCount = std::get<int>(info);
  auto startDelimiter = std::string((size_t)startCharCount, startChar);
  wr.heading(kind_, type_, loc, "end-of-file in string literal.");
  wr.message("Unterminated string literal here:");
  wr.code(loc);
  wr.message("This string literal began with ", startDelimiter,
             " and must end with the same un-escaped delimiter.");
}

void ErrorUseImportNeedsModule::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto isImport = std::get<bool>(info);
  std::string useOrImport = isImport ? "import" : "use";
  wr.heading(kind_, type_, loc, "'", useOrImport,
             "' statements must refer to module",
             (isImport ? "" : " or 'enum'"), " symbols.");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code(loc);
}

/* begin post-parse-checks errors */

void ErrorCantApplyPrivate::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto appliedToWhat = std::get<std::string>(info);
  wr.heading(kind_, type_, node, "can't apply private to ", appliedToWhat,
             " yet.");
  wr.message("The following declaration has unsupported 'private' modifier:");
  wr.code(node);
}

void ErrorMultipleManagementStrategies::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto outerMgt = std::get<1>(info);
  auto innerMgt = std::get<2>(info);
  wr.heading(kind_, type_, node,
             "type expression uses multiple memory management strategies ('",
             outerMgt, "' and '", innerMgt, "').");
  wr.message("Multiple class kinds used in type expression here:");
  wr.code(node);
  if (outerMgt == innerMgt) {
    wr.message(
        "The same strategy is listed twice; one instance should be removed.");
  } else {
    wr.message("These strategies are incompatible; one should be removed.");
  }
}

void ErrorPostParseErr::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto errorMessage = std::get<std::string>(info);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorPostParseErr message");
  wr.heading(kind_, type_, node, errorMessage);
  wr.code(node);
}

void ErrorPostParseWarn::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto errorMessage = std::get<std::string>(info);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorPostParseWarn message");
  wr.heading(kind_, type_, node, errorMessage);
  wr.code(node);
}

/* end post-parse-checks errors */

}
