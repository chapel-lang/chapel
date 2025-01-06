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
#include "chpl/parsing/parser-error-classes-list.h"
#undef DIAGNOSTIC_CLASS

//
// Below are the implementations of 'write' for each error class, which does
// the specialized work.
//
//

// Bison* errors are reported by the Bison parser to yyerror

void ErrorBisonMemoryExhausted::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  wr.heading(kind_, type_, loc, "memory exhausted while parsing.");
}

void ErrorBisonSyntaxError::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto nearestToken = std::get<std::string>(info_);
  wr.heading(kind_, type_, loc,
             (nearestToken.empty() ? "(no token given)"
                                   : "near '" + nearestToken + "'"),
             ":");
  wr.code(loc);
}

void ErrorBisonUnknownError::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto errorMessage = std::get<1>(info_);
  auto nearestToken = std::get<2>(info_);
  wr.heading(kind_, type_, loc,
             "unknown error from Bison parser: ", errorMessage, ".");
  if (!nearestToken.empty()) {
    wr.note(loc, "Nearest token when error was encountered: '", nearestToken,
            "'");
  }
}

// other parser errors

void ErrorCannotAttachPragmas::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto stmt = std::get<const uast::AstNode*>(info_);
  wr.heading(kind_, type_, loc, "cannot attach 'pragma's to this statement.");
  if (stmt) {
    wr.code(stmt);
  }
  wr.message(
      "Only declarations, such as variable declarations, can have 'pragma's "
      "attached to them.");
}

void ErrorCommentEOF::write(ErrorWriterBase& wr) const {
  auto loc = std::get<0>(info_);
  auto startLoc = std::get<1>(info_);
  auto nestedLoc = std::get<2>(info_);
  wr.heading(kind_, type_, loc, "end-of-file in comment.");
  wr.note(startLoc, "unterminated comment here:");
  wr.code(startLoc);
  if (!nestedLoc.isEmpty()) {
    wr.note(nestedLoc, "nested comment here:");
    wr.code(nestedLoc);
  }
}

void ErrorExceptOnlyInvalidExpr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<0>(info_);
  auto limitLoc = std::get<1>(info_);
  auto limitationKind = std::get<uast::VisibilityClause::LimitationKind>(info_);
  wr.heading(kind_, type_, loc, "incorrect expression in '", limitationKind,
             "' list, identifier expected.");
  wr.message("In the '", limitationKind, "' list here:");
  wr.code<Location>(loc, { limitLoc });
}

void ErrorExternUnclosedPair::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto pairSymbol = std::get<std::string>(info_);
  wr.heading(kind_, type_, loc, "missing closing ", pairSymbol,
             " symbol in extern block.");
  wr.message("In this extern block:");
  wr.code(loc);
}

void ErrorInvalidIndexExpr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  wr.heading(kind_, type_, loc,
             "only identifiers or tuples of identifiers are allowed as index "
             "expressions.");
  wr.message("Invalid index expression used here:");
  wr.code(loc);
}

void ErrorInvalidNewForm::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto newExpr = std::get<const uast::AstNode*>(info_);
  wr.heading(kind_, type_, loc, "Invalid form for 'new' expression.");
  wr.message("'new' expression used here:");
  wr.code(loc, {newExpr});
}

void ErrorInvalidNumericLiteral::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto errMessage = std::get<std::string>(info_);
  CHPL_ASSERT(!errMessage.empty());
  CHPL_ASSERT(errMessage.back() == '.' &&
         "expected a period at the end of InvalidNumericLiteral message");
  errMessage[0] = std::tolower(errMessage[0]);
  wr.heading(kind_, type_, loc, errMessage);
  wr.message("Numeric literal encountered here:");
  wr.code(loc);
}

void ErrorLabelIneligibleStmt::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto maybeStmt = std::get<const uast::AstNode*>(info_);
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
  auto loc = std::get<const Location>(info_);
  wr.heading(
      kind_, type_, loc,
      "external symbol renaming can only be applied to one symbol at a time.");
  wr.message("Multiple renaming used here:");
  wr.code(loc);
}

void ErrorNonAssociativeComparison::write(ErrorWriterBase& wr) const {
  auto root = std::get<const uast::OpCall*>(info_);
  auto& ops = std::get<std::vector<const uast::OpCall*>>(info_);
  auto& operands = std::get<std::vector<const uast::AstNode*>>(info_);

  bool allSimple = std::all_of(operands.begin(), operands.end(), [](auto operand) {
    return operand->isLiteral() || operand->isIdentifier();
  });

  // Bitfield: 001 for < or <=, 010 for > or >=, 100 for == or !=
  // We only want to suggest the "normalized" form if all operators are
  // in the same direction, and if no equality constraints are present.
  //
  // The reason for the latter is, how would you desugar x != y != z?
  //     x != y && y != z
  // or
  //     x != y && y != z && x != z
  // It's best not to guess there.
  int types = 0;
  for (auto op : ops) {
    if (op->op() == "<" || op->op() == "<=") {
      types |= 0b1;
    } else if (op->op() == ">" || op->op() == ">=") {
      types |= 0b10;
    } else if (op->op() == "==" || op->op() == "!=") {
      types |= 0b100;
    }
  }

  wr.heading(kind_, type_, ops.front(),
             "comparison operators are not associative.");
  wr.codeForLocation(ops.front());
  wr.message("Comparisons in the form 'x op y op z' are not supported.");

  if (allSimple && (types == 0b1 || types == 0b10)) {
    std::ostringstream oss;
    // Print out what the user's code would look like elementwise
    for (size_t idx = 0; idx < ops.size(); idx++) {
      if (idx > 0) {
        oss << " && ";
      }

      operands[idx]->stringify(oss, CHPL_SYNTAX);
      oss << " " << ops[idx]->op() << " ";
      operands[idx + 1]->stringify(oss, CHPL_SYNTAX);
    }

    // TODO: this uses explicit indentation using spaces, which is probably bad.
    wr.message("If you wanted to perform elementwise comparison, consider using "
               "the following instead:");
    wr.message("");
    wr.message("    ", oss.str());
    wr.message("");
  } else {
    wr.message("If you wanted to perform elementwise comparison, please use "
               "'&&' to combine operations.");
  }

  const uast::OpCall* firstNonRoot = nullptr;
  for (auto op : ops) {
    if (op != root) {
      firstNonRoot = op;
      break;
    }
  }

  wr.message("If you wanted to use the result of a comparison as an operand in "
             "another comparison, consider using parentheses in a subexpression "
             "to disambiguate:");
  wr.code(root, { firstNonRoot });
}

void ErrorNewWithoutArgs::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto expr = std::get<const uast::AstNode*>(info_);
  wr.heading(kind_, type_, loc,
             "'new' expression is missing its argument list.");
  wr.message("'new' expression used here:");
  wr.code(loc, { expr });
  wr.message("Perhaps you intended to write 'new ", expr, "()' instead?");
}

void ErrorPreIncDecOp::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto isDoublePlus = std::get<bool>(info_);
  if (isDoublePlus) {
    wr.heading(kind_, type_, loc, "'++' is not a pre-increment.");
  } else {
    wr.heading(kind_, type_, loc, "'--' is not a pre-decrement.");
  }
  wr.message("Used here:");
  wr.code(loc);
}

void ErrorStringLiteralEOF::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto startChar = std::get<char>(info_);
  auto startCharCount = std::get<int>(info_);
  auto startDelimiter = std::string((size_t)startCharCount, startChar);
  wr.heading(kind_, type_, loc, "end-of-file in string literal.");
  wr.message("Unterminated string literal here:");
  wr.code(loc);
  wr.message("This string literal began with ", startDelimiter,
             " and must end with the same un-escaped delimiter.");
}

void ErrorUseImportNeedsModule::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto isImport = std::get<bool>(info_);
  std::string useOrImport = isImport ? "import" : "use";
  wr.heading(kind_, type_, loc, "'", useOrImport,
             "' statements must refer to module",
             (isImport ? "" : " or 'enum'"), " symbols.");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.codeForLocation(loc);
}

// catch-alls for simple parsing errors

void ErrorParseErr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto errorMessage = std::get<std::string>(info_);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorParseErr message");
  wr.heading(kind_, type_, loc, errorMessage);
  wr.code(loc);
}

void ErrorParseSyntax::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info_);
  auto errorMessage = std::get<std::string>(info_);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorParseSyntax message");
  wr.heading(kind_, type_, loc, errorMessage);
  wr.code(loc);
}

/* begin post-parse-checks errors */

void ErrorCantApplyPrivate::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto appliedToWhat = std::get<std::string>(info_);
  wr.heading(kind_, type_, node, "can't apply private to ", appliedToWhat,
             " yet.");
  wr.message("The following declaration has unsupported 'private' modifier:");
  wr.code(node);
}

void ErrorWhenAfterOtherwise::write(ErrorWriterBase& wr) const {
  auto node = std::get<0>(info_);
  auto otherwise = std::get<1>(info_);
  auto when = std::get<2>(info_);
  wr.heading(kind_, type_, node, "'otherwise' clause must follow all 'when' clauses.");
  wr.message("In the following 'select' statement:");
  wr.code(node);
  wr.note(otherwise, "the 'otherwise' clause occurs here:");
  wr.code(otherwise);
  wr.note(when, "however, the following 'when' clause is found below it:");
  wr.code(when);
  wr.message("Chapel requires 'otherwise' clauses to occur last within 'select' statements.");
}

void ErrorDisallowedControlFlow::write(ErrorWriterBase& wr) const {
  auto invalidAst = std::get<0>(info_);
  auto blockingAst = std::get<1>(info_);
  auto allowingAst = std::get<2>(info_);

  // Match some special cases:
  auto ret = invalidAst->toReturn();
  const uast::Function *blockingAstFn = blockingAst ? blockingAst->toFunction() : nullptr;
  bool isFuncWithReturn = ret && blockingAstFn;
  bool isIterWithReturn = isFuncWithReturn && blockingAstFn->kind() == uast::Function::ITER;
  if (isIterWithReturn) {
    wr.heading(kind_, type_, ret,
               "'return' statements with values are not allowed in iterators.");
    wr.message("The following 'return' statement has a value:");
    wr.code(ret, { ret->value() });
    wr.note(locationOnly(blockingAstFn), "'", blockingAstFn->name(),
                              "' is declared as an iterator here:");
    wr.codeForLocation(blockingAstFn);
    if (allowingAst != nullptr) {
      auto allowingFn = allowingAst->toFunction();
      CHPL_ASSERT(allowingFn);
      // There _was_ a function that allowed a return, but it must be further
      // out.
      wr.note(locationOnly(allowingFn), "'", blockingAstFn->name(), "' is declared inside '",
              allowingFn->name(), "', but returning from '", allowingFn->name(), "' here is not allowed.");
    } else {
      wr.message("Did you mean to use the 'yield' keyword instead of 'return'?");
    }
    return;
  } else if (isFuncWithReturn) {
    wr.heading(kind_, type_, ret,
               "'return' statements with values are not allowed in ", blockingAstFn->name());
    wr.message("The following 'return' statement has a value:");
    wr.code(ret, { ret->value() });
    wr.codeForLocation(blockingAstFn);
    return;
  }

  std::string astType = "";
  std::string labelName = "";
  if (invalidAst->isReturn()) {
    astType = "return";
  } else if (invalidAst->isYield()) {
    astType = "yield";
  } else if (auto brk = invalidAst->toBreak()) {
    astType = "break";
    if (auto target = brk->target()) {
      labelName = target->name().str();
    }
  } else if (auto cont = invalidAst->toContinue()) {
    astType = "continue";
    if (auto target = cont->target()) {
      labelName = target->name().str();
    }
  } else {
    CHPL_ASSERT(false && "not a control flow element handled by this error");
  }

  std::string blockingName = "";
  std::string blockingNameArticle = "a";
  if (!blockingAst) {
    // Nothing
  } else if (blockingAst->isForall()) {
    blockingName = "'forall' statement";
  } else if (blockingAst->isForeach()) {
    blockingName = "'foreach' statement";
  } else if (blockingAst->isCoforall()) {
    blockingName = "'coforall' statement";
  } else if (blockingAst->isOn()) {
    blockingName = "'on' statement";
    blockingNameArticle = "an";
  } else if (blockingAst->isBegin()) {
    blockingName = "'begin' statement";
  } else if (blockingAst->isSync()) {
    blockingName = "'sync' statement";
  } else if (blockingAst->isCobegin()) {
    blockingName = "'cobegin' statement";
  } else if (auto fn = blockingAst->toFunction()) {
    if (fn->kind() == uast::Function::Kind::ITER) {
      blockingName = "iterator";
      blockingNameArticle = "an";
    } else {
      blockingName = "procedure";
    }
  } else {
    CHPL_ASSERT(false && "not a blocking element handled by this error");
  }


  if (!labelName.empty()) {
    // labeled break or continue did not find the target thing.

    if (!allowingAst) {
      // Simply no such label.
      wr.heading(kind_, type_, invalidAst, "could not find label '", labelName,
          "' for '", astType, "' statement.");
      wr.code(invalidAst, { invalidAst });
    } else {
      // It we found a node to jump to, why are we issuing this error unless
      // something blocked it?
      CHPL_ASSERT(blockingAst != nullptr);

      // There is such a label, but a coforall etc. gets in the way.
      wr.heading(kind_, type_, invalidAst, "'", astType, "' to label '",
                 labelName, "' outside of ", blockingNameArticle, " ",
                 blockingName, " is not allowed.");
      wr.code(invalidAst, { invalidAst });
      wr.note(locationOnly(blockingAst), "cannot '", astType, "' "
              "out of ", blockingNameArticle, " ", blockingName, ":");
      wr.codeForLocation(blockingAst);
    }

  } else if (!blockingAst || blockingAst->isFunction()) {
    // break or continue without a label, or a return or yield, outside any
    // loop or function.
    std::string missingThing = "";
    if (invalidAst->isReturn()) {
      missingThing = "a procedure or an iterator";
    } else if (invalidAst->isYield()) {
      missingThing = "an iterator";
    } else {
      missingThing = "a loop";
    }
    wr.heading(kind_, type_, invalidAst, "'", astType,
               "' is not allowed outside of ", missingThing, ".");
    wr.code(invalidAst, { invalidAst });

    // If something blocked the jump, only print what it was if the jump
    // would've worked otherwise (allowingAst != null)
    if (blockingAst && allowingAst) {
      CHPL_ASSERT(invalidAst->isBreak() || invalidAst->isContinue() ||
                  invalidAst->isYield());
      wr.note(locationOnly(blockingAst), "cannot '", astType, "' "
              "out of ", blockingNameArticle, " ", blockingName, ":");
      wr.codeForLocation(blockingAst);
    }
  } else {
    // any piece of control flow banned within a particular language feature
    wr.heading(kind_, type_, invalidAst, "'", astType, "' is not allowed in ",
               blockingNameArticle, " ", blockingName, ".");
    wr.code(invalidAst, { invalidAst });
    wr.message("The relevant ", blockingName, " is here:");
    wr.codeForLocation(blockingAst);
  }
}

void ErrorInvalidReturns::write(ErrorWriterBase& wr) const {
  const uast::Return* firstReturnSomething = std::get<0>(info_);
  const uast::Return* firstReturnEmpty = std::get<1>(info_);

  // figure out which appears first syntactically
  const uast::Return* first = nullptr;
  const uast::Return* second = nullptr;
  if (firstReturnSomething->id() < firstReturnEmpty->id()) {
    first = firstReturnSomething;
    second = firstReturnEmpty;
  } else {
    first = firstReturnEmpty;
    second = firstReturnSomething;
  }

  wr.heading(kind_, type_, first, "Invalid mix of 'return' statements");
  const char* something = "this statement returns something:";
  const char* nothing = "this statement does not return anything:";
  wr.note(first, first->value()?something:nothing);
  wr.code(first);
  wr.note(second, second->value()?something:nothing);
  wr.code(second);
}

void ErrorIllegalUseImport::write(ErrorWriterBase& wr) const {
  auto clause = std::get<0>(info_);
  auto parent = std::get<1>(info_);
  const char* useOrImport = parent->isImport() ? "import" : "use";
  wr.heading(kind_, type_, clause,
             "Illegal expression in '", useOrImport, "' statement");
  wr.code(clause);
  wr.note(clause, "only identifiers and 'dot' expressions are supported");
}

void ErrorInvalidThrowaway::write(ErrorWriterBase& wr) const {
  auto underscore = std::get<0>(info_);
  auto parent = std::get<1>(info_);

  wr.heading(kind_, type_, underscore,
             "Throwaway variable '_' is not allowed in this context.");
  wr.message("The throwaway variable is used here:");
  wr.code(underscore, { underscore });
  wr.message("Throwaway variables are only allowed on the left-hand side of "
             "tuple unpacking, in function formal names, and when renaming the "
             "target of a 'use' statement.");

  if (parent) {
    if (auto loop = parent->toIndexableLoop()) {
      if (underscore == loop->index()) {
        wr.message("To avoid naming the loop index, omit the name and 'in' keyword.");
      }
    }
  }
}

void ErrorInvalidGpuAttribute::write(ErrorWriterBase& wr) const {
  const uast::AstNode*   node = std::get<const uast::AstNode*>(info_);
  const uast::Attribute* attr = std::get<const uast::Attribute*>(info_);
  ErrorBase::Kind        kind = kind_;
  ErrorType              type = type_;

  const char* loopTypes = nullptr;
  if (node->isFor()) {
    loopTypes = "for";
  } else if (node->isWhile()) {
    loopTypes = "while";
  } else if (node->isDoWhile()) {
    loopTypes = "do-while";
  } else if (node->isCoforall()) {
    loopTypes = "coforall";
  }

  // For now, attribute locations aren't correctly computed, so this is unhelpful.
  // wr.note(attr, "marked for GPU execution here:");
  // wr.codeForLocation(attr);

  const char* whatIsAffected = "statement";
  if (loopTypes) {
    wr.heading(kind, type, node, "loop marked with @", attr->name(),
               ", but '", loopTypes, "' loops don't support GPU execution.");
    whatIsAffected = "loop";
  } else if (node->isFunction()) {
    wr.heading(kind, type, node, "functions do not currently support the @",
               attr->name(), " attribute.");
    whatIsAffected = "function";
  } else if (node->isVariable() && node->toVariable()->isField()) {
    wr.heading(kind, type, node, "fields do not support the @",
               attr->name(), " attribute.");
  } else {
    wr.heading(kind, type, node, "statement does not support the @",
               attr->name(), " attribute.");
  }

  wr.message("The affected ", whatIsAffected, " is here:");
  wr.codeForLocation(node);
}

void ErrorInvalidImplementsIdent::write(ErrorWriterBase& wr) const {
  auto implements = std::get<const uast::Implements*>(info_);
  auto ident = std::get<const uast::Identifier*>(info_);

  auto typeName = ident->name();
  if (typeName == USTR("domain")) {
    wr.heading(kind_, type_, ident, "type '", typeName, "' cannot implement an interface.");
  } else {
    wr.heading(kind_, type_, ident, "invalid identifier '", typeName,
                                     "' on the left of an 'implements' keyword.");
  }
  wr.message("In the following 'implements' statement:");
  wr.code(implements, { ident });
}

void ErrorInvalidParenfulDeprecation::write(ErrorWriterBase& wr) const {
  auto attributeGroup = std::get<const uast::AttributeGroup*>(info_);
  auto appliedTo = std::get<const uast::AstNode*>(info_);

  wr.heading(kind_, type_, attributeGroup, "the '@deprecated' attribute with 'parenful=true' can only be applied to parenless functions.");
  wr.message("It is used to indicate that a parenless function used to be callable with parentheses.");

  if (auto fn = appliedTo->toFunction()) {
    wr.message("The attribute is applied to a non-parenless function here:");
    wr.codeForDef(fn);
  } else {
    wr.message("the attribute is applied to a non-function here:");
    wr.codeForLocation(appliedTo);
  }
}

void ErrorMultipleManagementStrategies::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto outerMgt = std::get<1>(info_);
  auto innerMgt = std::get<2>(info_);
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
  auto node = std::get<const uast::AstNode*>(info_);
  auto errorMessage = std::get<std::string>(info_);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorPostParseErr message");
  wr.heading(kind_, type_, node, errorMessage);
  wr.code(node);
}

void ErrorPostParseWarn::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto errorMessage = std::get<std::string>(info_);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorPostParseWarn message");
  wr.heading(kind_, type_, node, errorMessage);
  wr.code(node);
}

void ErrorUnsupportedAsIdent::write(ErrorWriterBase& wr) const {
  auto as = std::get<const uast::As*>(info_);
  auto expectedIdentifier = std::get<const uast::AstNode*>(info_);
  wr.heading(kind_, type_, locationOnly(as),
             "this form of 'as' is not yet supported.");
  // determine and report which of the original or new name is invalid
  std::string whichName;
  if (expectedIdentifier == as->symbol()) {
    whichName = "original";
  } else if (expectedIdentifier == as->rename()) {
    whichName = "new";
  } else {
    CHPL_ASSERT(false && "should not be reachable");
  }
  wr.message("'as' requires the ", whichName,
             " name to be a simple identifier, but instead got the following:");
  wr.code(expectedIdentifier, { expectedIdentifier });
}

/* end post-parse-checks errors */


// main module / command line invocation errors
void ErrorAmbiguousSourceFile::write(ErrorWriterBase& wr) const {
  auto& loc = std::get<IdOrLocation>(info_);
  auto& found = std::get<1>(info_);
  auto& other = std::get<2>(info_);
  auto& warnUnstable = std::get<3>(info_);
  wr.heading(kind_, type_, loc,
             "ambiguous module source file -- using ",
             found,
             " over ",
             other);
  if (warnUnstable) {
    wr.note(loc, "which module is chosen in this case is unstable");
  }
}

void ErrorAmbiguousMain::write(ErrorWriterBase& wr) const {
  auto& loc = std::get<IdOrLocation>(info_);
  auto& mainFns = std::get<std::vector<const uast::Function*>>(info_);
  auto& modIds = std::get<std::vector<ID>>(info_);
  auto& modNames = std::get<std::vector<UniqueString>>(info_);

  bool allSameModule = true;
  ID firstModuleId;
  if (modIds.size() > 0) firstModuleId = modIds[0];
  for (auto modId : modIds) {
    if (modId != firstModuleId) {
      allSameModule = false;
    }
  }

  if (allSameModule) {
    const char* moduleName = "<unknown>";
    if (modNames.size() > 0)
      moduleName = modNames[0].c_str();

    wr.heading(kind_, type_, loc,
               "multiple 'main' procedures in module '", moduleName, "'");
    wr.message("");
    for (auto fn : mainFns) {
      wr.note(locationOnly(fn), "found 'proc main' here");
      wr.codeForDef(fn);
    }
  } else {
    wr.heading(kind_, type_, loc,
               "unknown main module due to multiple 'main' procedures");
    wr.note(loc, "please use '--main-module' to disambiguate");
    wr.message("");
    size_t i = 0;
    for (auto fn : mainFns) {
      const char* moduleName = "<unknown>";
      if (i < modNames.size())
        moduleName = modNames[i].c_str();
      wr.note(locationOnly(fn),
              "found 'proc main' in module '", moduleName, "'");
      wr.codeForDef(fn);
      i++;
    }
  }
}
void ErrorAmbiguousMainModule::write(ErrorWriterBase& wr) const {
  auto& loc = std::get<IdOrLocation>(info_);
  auto& modules = std::get<std::vector<const uast::Module*>>(info_);
  wr.heading(kind_, type_, loc,
             "unknown main module due to multiple command line modules");
  wr.note(loc, "add a 'proc main' or use '--main-module'");
  wr.message("");
  for (auto mod : modules) {
    wr.note(locationOnly(mod), "'module ", mod->name(), "' could be the main module");
    wr.codeForDef(mod);
  }
}
void ErrorUnknownMainModule::write(ErrorWriterBase& wr) const {
  auto& loc = std::get<IdOrLocation>(info_);
  auto& name = std::get<UniqueString>(info_);
  wr.heading(kind_, type_, loc,
             "cannot find module named '", name, "' for '--main-module'");
}

void ErrorImplicitModuleSameName::write(ErrorWriterBase& wr) const {
  auto mod = std::get<const uast::Module*>(info_);
  wr.heading(kind_, type_, locationOnly(mod),
             "module '", mod->name(), "' "
             "has the same name as the implicit file module");
  wr.note(locationOnly(mod),
          "did you mean to include all statements in the module declaration?");
}


}
