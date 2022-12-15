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
// error-classes-list.h which invokes this macro for every error type.
#define DIAGNOSTIC_CLASS(NAME__, KIND__, EINFO__...)\
  const owned<Error##NAME__>&\
  Error##NAME__::getError##NAME__(Context* context,\
                                  std::tuple<EINFO__> tuple) {\
    QUERY_BEGIN(getError##NAME__, context, tuple);\
    auto result = owned<Error##NAME__>(new Error##NAME__(tuple));\
    return QUERY_END(result);\
  }\
\
  const Error##NAME__*\
  Error##NAME__::get(Context* context, std::tuple<EINFO__> tuple) {\
    return Error##NAME__::getError##NAME__(context, std::move(tuple)).get();\
  }
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

/**
  Get a non-param version of the given type.

  This function is useful if the param-ness of something is unimportant to the
  error message (e.g., string and int aren't compatible if-expression types,
  whether or not they are params), so we might as well print them as values
  to reduce confusion.
 */
static types::QualifiedType decayToValue(const types::QualifiedType& qt) {
  if (qt.kind() == types::QualifiedType::PARAM) {
    return types::QualifiedType(types::QualifiedType::VAR, qt.type());
  }
  return qt;
}

//
// Below are the implementations of 'write' for each error class, which does
// the specialized work.
//

/* begin parser errors */

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

void ErrorExceptOnlyInvalidExpr::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto limitationKind = std::get<uast::VisibilityClause::LimitationKind>(info);
  wr.heading(kind_, type_, loc, "incorrect expression in '", limitationKind,
             "' list, identifier expected.");
  wr.message("In the '", limitationKind, "' list here:");
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

/* begin Bison-specific parser errors */
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

/* end Bison-specific parser errors */

/* begin lexer-specific parser errors */
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

void ErrorExternUnclosedPair::write(ErrorWriterBase& wr) const {
  auto loc = std::get<const Location>(info);
  auto pairSymbol = std::get<std::string>(info);
  wr.heading(kind_, type_, loc, "missing closing ", pairSymbol,
             " symbol in extern block.");
  wr.message("In this extern block:");
  wr.code(loc);
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

/* end lexer-specific parser errors */
/* end parser errors */

/* begin post-parse-checks errors */
void ErrorCantApplyPrivate::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto appliedToWhat = std::get<std::string>(info);
  wr.heading(kind_, type_, id, "can't apply private to ", appliedToWhat,
             " yet.");
  wr.message("The following declaration has unsupported 'private' modifier:");
  wr.code(id);
}

void ErrorMultipleManagementStrategies::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto outerMgt = std::get<1>(info);
  auto innerMgt = std::get<2>(info);
  wr.heading(kind_, type_, id,
             "type expression uses multiple memory management strategies ('",
             outerMgt, "' and '", innerMgt, "').");
  wr.message("Multiple class kinds used in type expression here:");
  wr.code(id);
  if (outerMgt == innerMgt) {
    wr.message(
        "The same strategy is listed twice; one instance should be removed.");
  } else {
    wr.message("These strategies are incompatible; one should be removed.");
  }
}

void ErrorPostParseErr::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto errorMessage = std::get<std::string>(info);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorPostParseErr message");
  wr.heading(kind_, type_, id, errorMessage);
  wr.code(id);
}

void ErrorPostParseWarn::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto errorMessage = std::get<std::string>(info);
  CHPL_ASSERT(errorMessage.back() == '.' &&
         "expected a period at the end of ErrorPostParseWarn message");
  wr.heading(kind_, type_, id, errorMessage);
  wr.code(id);
}

/* end post-parse-checks errors */

/* begin resolution errors */
void ErrorAmbiguousConfigName::write(ErrorWriterBase& wr) const {
  auto& name = std::get<std::string>(info);
  auto variable = std::get<const uast::Variable*>(info);
  auto otherId = std::get<ID>(info);
  wr.heading(kind_, type_, locationOnly(variable), "ambiguous config name (", name, ").");
  wr.code(variable);
  wr.note(locationOnly(otherId), "also defined here:");
  wr.code(otherId);
  wr.note(locationOnly(otherId), "(disambiguate using -s<modulename>.", name, "...)");
}

void ErrorAmbiguousConfigSet::write(ErrorWriterBase& wr) const {
  auto& name1 = std::get<1>(info);
  auto& name2 = std::get<2>(info);
  auto variable = std::get<const uast::Variable*>(info);
  wr.heading(kind_, type_, locationOnly(variable),
            "config set ambiguously via '-s", name1, "' and '-s", name2, "'");
}

void ErrorAsWithUseExcept::write(ErrorWriterBase& wr) const {
  auto use = std::get<const uast::Use*>(info);
  auto as = std::get<const uast::As*>(info);
  wr.heading(kind_, type_, locationOnly(use),
             "invalid use of 'as' in this 'use' statement.");
  wr.code(use, { as });
  wr.message("The 'except' keyword and renaming cannot be used together.");
}

void ErrorDotExprInUseImport::write(ErrorWriterBase& wr) const {
  auto dot = std::get<const uast::Dot*>(info);
  auto visibilityClause = std::get<const uast::VisibilityClause*>(info);
  auto limitationKind =
      std::get<const uast::VisibilityClause::LimitationKind>(info);
  wr.heading(kind_, type_, locationOnly(dot), "cannot use dot expression '",
             dot, "' in '", limitationKind, "' list.");
  wr.code(visibilityClause, {dot});
  wr.message(
      "Dot expressions are not allowed in the 'except' or 'only' list of a "
      "'use' or 'import'.");
}

void ErrorImplicitFileModule::write(ErrorWriterBase& wr) const {
  auto code = std::get<const uast::AstNode*>(info);
  auto lastModule = std::get<1>(info);
  auto implicitModule = std::get<2>(info);
  wr.heading(kind_, type_, code, "an implicit module named '",
             implicitModule->name(), "' is being introduced to contain "
             "file-scope code.");
  wr.message("The following is the first file-scope statement:");
  wr.code(code);
  wr.message("The implicit module '", implicitModule->name(), "' is being "
             "created because the above code is outside of any module "
             "declarations (e.g., 'module ", lastModule->name(), "').");
  wr.message("Note that all of the file's contents -- including module '",
             lastModule->name() ,"' -- will be placed into the new '",
             implicitModule->name(), "' module.");
}

void ErrorIncompatibleIfBranches::write(ErrorWriterBase& wr) const {
  auto ifExpr = std::get<const uast::Conditional*>(info);
  auto qt1 = std::get<1>(info);
  auto qt2 = std::get<2>(info);

  wr.heading(kind_, type_, ifExpr, "branches of if-expression have incompatible types.");
  wr.message("In the following if-expression:");
  wr.code(ifExpr, { ifExpr->thenBlock(), ifExpr->elseBlock() });
  if (qt1.kind() == types::QualifiedType::TYPE ||
      qt2.kind() == types::QualifiedType::TYPE) {
    // If any of the branches is not a value (i.e. a type, since we pretend
    // params are values for the sake of clarity) then we need to be more
    // clear about when something is a type and when it isn't.
    wr.message("the first branch is ", decayToValue(qt1),
                ", while the second is ", decayToValue(qt2), ".");
  } else {
    // Otherwise, both things are values, so just talk about their types.
    wr.message("the first branch is of type '", qt1.type(), "'"
                ", while the second is of type '", qt2.type(), "'.");
  }
}

void ErrorIncompatibleKinds::write(ErrorWriterBase& wr) const {
  auto initExpr = std::get<const uast::AstNode*>(info);
  auto initType = std::get<types::QualifiedType>(info);
  auto declKind = std::get<types::QualifiedType::Kind>(info);

  bool valueToType = declKind == types::QualifiedType::Kind::TYPE &&
    initType.kind() != types::QualifiedType::Kind::TYPE;
  bool typeToValue = initType.kind() == types::QualifiedType::Kind::TYPE &&
    declKind != types::QualifiedType::Kind::TYPE;
  bool nonParamToParam = declKind == types::QualifiedType::Kind::PARAM &&
    initType.kind() != types::QualifiedType::Kind::PARAM;
  if (valueToType) {
    wr.heading(kind_, type_, initExpr,
               "a type variable cannot be initialized with a regular value.");
  } else if (typeToValue) {
    wr.heading(kind_, type_, initExpr,
               "a regular variable cannot be initialized with a type.");
  } else if (nonParamToParam) {
    wr.heading(kind_, type_, initExpr,
               "a 'param' cannot be initialized with a non-'param' value.");
  }
  wr.message("In the following initialization expression:");
  wr.code(initExpr, { initExpr });
  wr.message("the initialization expression is ", initType, ".");
  if (valueToType) {
    wr.message("If you were trying to extract the type of the expression on "
               "the left of the '=', try using '.type'?");
  } else if (typeToValue) {
    if (initType.type() && initType.type()->isCompositeType()) {
      auto compositeType = initType.type()->toCompositeType();
      auto initIdent = initExpr->toIdentifier();
      if (initIdent && initIdent->name() == compositeType->name()) {
        // There's no aliasing involved, the user is just using the record name.
        wr.message("If you're trying to create a new value of type '",
                   initType.type()->toCompositeType()->name(),
                   "', try writing 'new ", compositeType->name(), "()' instead");
      } else {
        // They are referring to a composite type, but through an alias. Not
        // quite sure how to report this better.
        wr.message("If you're trying to create a new value of type '",
                   initType.type()->toCompositeType()->name(),
                   "', try using the 'new' keyword");
      }
    }
  } else if (nonParamToParam) {
    wr.message("Note that 'param' values must be known at compile-time.");
  }
}

void ErrorIncompatibleRangeBounds::write(ErrorWriterBase& wr) const {
  auto range = std::get<const uast::Range*>(info);
  auto qt1 = std::get<1>(info);
  auto qt2 = std::get<2>(info);

  wr.heading(kind_, type_, range,
            "bounds of range expression have incompatible types.");
  wr.message("In the following range expression:");
  wr.code(range, { range->lowerBound(), range->upperBound() });
  if (qt1.kind() == types::QualifiedType::TYPE ||
      qt2.kind() == types::QualifiedType::TYPE) {
    // As in IncompatibleIfBranches, if one of the things is a type, be more
    // explicit about what is and what isn't a type.
    wr.message("the lower bound is ", decayToValue(qt1),
               ", while the upper bound is ", decayToValue(qt2), ".");
  } else {
    // Both things are values, just refer to their types.
    wr.message("the lower bound is of type '", qt1.type(), "'"
               ", while the upper bound is of type '", qt2.type(), "'.");
  }
}

void ErrorIncompatibleTypeAndInit::write(ErrorWriterBase& wr) const {
  auto decl = std::get<0>(info);
  auto type = std::get<1>(info);
  auto init = std::get<2>(info);
  auto typeExprType = std::get<3>(info);
  auto initExprType = std::get<4>(info);

  if (auto namedDecl = decl->toNamedDecl()) {
    wr.heading(kind_, type_, decl,
               "type mismatch between declared type of '", namedDecl->name(),
               "' and initialization expression.");
  } else {
    wr.heading(kind_, type_, decl,
               "type mismatch between declared type and initialization expression.");
  }
  wr.message("In the following declaration:");
  wr.code(decl, { type, init });
  wr.message("the type specifier has type '", typeExprType, "', while the "
             "initial value has type '", initExprType, "'.");
}

void ErrorInvalidNewTarget::write(ErrorWriterBase& wr) const {
  auto newExpr = std::get<const uast::New*>(info);
  auto type = std::get<types::QualifiedType>(info);

  if (auto primType = type.type()->toPrimitiveType()) {
    wr.heading(kind_, type_, newExpr,
               "invalid use of 'new' on primitive '", primType, "'");
  } else if (auto enumType = type.type()->toEnumType()) {
    wr.heading(kind_, type_, newExpr,
               "invalid use of 'new' on enum type '", enumType->name(), "'");
  } else if (auto tupleType = type.type()->toTupleType()) {
    wr.heading(kind_, type_, newExpr,
               "invalid use of 'new' on tuple type '", tupleType, "'");
  } else {
    wr.heading(kind_, type_, newExpr, "invalid use of 'new' with type '",
               type.type(), "', which is neither a class nor a record.");
  }
  wr.code(newExpr, { newExpr->typeExpression() });
  wr.message("The 'new' expression can only be used with records or classes.");
}

void ErrorMemManagementNonClass::write(ErrorWriterBase& wr) const {
  auto newCall = std::get<const uast::New*>(info);
  auto type = std::get<const types::Type*>(info);
  auto record = type ? type->toRecordType() : nullptr;

  if (record) {
    wr.heading(kind_, type_, newCall,
               "cannot use memory management strategy '",
               uast::New::managementToString(newCall->management()),
               "' with record '",
               record->name(), "'.");
  } else {
    wr.heading(kind_, type_, newCall,
               "cannot use memory management strategy '",
               uast::New::managementToString(newCall->management()),
               "' with non-class type '", type, "'.");
  }
  wr.code(newCall, { newCall->typeExpression() });
  wr.message("Memory management strategies can only be used with classes.");
  if (record) {
    wr.note(record->id(), "'", record->name(), "' declared as record here:");
    wr.code(record->id());
    wr.message(
               "Consider removing the '", uast::New::managementToString(newCall->management()),
               "' keyword to fix this error, or defining '", record->name(),
               "' as a class.");
  }
}

void ErrorMissingInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto& filePath = std::get<std::string>(info);
  wr.heading(kind_, type_, moduleInclude, "cannot find included submodule");
  wr.code(moduleInclude);
  wr.note(moduleInclude, "expected file at path '", filePath, "'");
}

void ErrorMultipleEnumElems::write(ErrorWriterBase& wr) const {
  auto enumType = std::get<const types::EnumType*>(info);
  auto elemName = std::get<UniqueString>(info);
  auto& possibleElems = std::get<std::vector<ID>>(info);

  wr.heading(kind_, type_, enumType->id(), "enum '", enumType->name(),
             "' has multiple elements named '", elemName, "'.");
  wr.code(enumType->id());
  bool printedOne = false;
  for (auto& id : possibleElems) {
    wr.note(id, printedOne ? "another" : "one", " instance occurs here:");
    printedOne = true;
    wr.code<ID, ID>(id, { id });
  }
  wr.message("An enum cannot have repeated elements of the same name.");
}

void ErrorMultipleQuestionArgs::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::FnCall*>(info);
  auto firstQuestion = std::get<1>(info);
  auto secondQuestion = std::get<2>(info);

  wr.heading(kind_, type_, call, "cannot have '?' more than once in a call");
  wr.message("The first ? argument occurs here:");
  wr.code(firstQuestion, { firstQuestion });
  wr.message("The second ? argument occurs here:");
  wr.code(secondQuestion, { secondQuestion });
}

void ErrorNonIterable::write(ErrorWriterBase &wr) const {
  auto loop = std::get<const uast::IndexableLoop*>(info);
  auto iterand = std::get<const uast::AstNode*>(info);
  auto& iterandType = std::get<types::QualifiedType>(info);
  wr.heading(kind_, type_, loop, "cannot iterate over ", decayToValue(iterandType), ".");
  wr.message("In the following loop:");
  wr.code(loop, { iterand });
}

void ErrorPrivateToPublicInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, type_, moduleInclude,
             "cannot make a private module public through "
             "an include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "module declared private here:");
  wr.code(moduleDef);
}

void ErrorProcDefExplicitAnonFormal::write(ErrorWriterBase& wr) const {
  auto fn = std::get<const uast::Function*>(info);
  auto formal = std::get<const uast::Formal*>(info);
  wr.heading(kind_, type_, formal, "formals in a procedure definition must "
                            "be named");
  wr.code(fn, {formal});
}

void ErrorProcTypeUnannotatedFormal::write(ErrorWriterBase& wr) const {
  auto sig = std::get<const uast::FunctionSignature*>(info);
  auto formal = std::get<const uast::AnonFormal*>(info);
  wr.heading(kind_, type_, formal, "unannotated formal is ambiguous in this "
                            "context");
  wr.code(sig, {formal});
  wr.message("The meaning of an unannotated formal (a formal not of the "
             "form '<name>:<type>') in a procedure type is ambiguous. "
             "It is currently undecided whether it represents a formal's "
             "type or name.");
}

void ErrorPrototypeInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, type_, moduleInclude,
             "cannot apply prototype to module in include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "put prototype keyword at module declaration here:");
  wr.code(moduleDef);
}

void ErrorRedefinition::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::NamedDecl*>(info);
  auto& ids = std::get<std::vector<ID>>(info);
  wr.heading(kind_, type_, decl, "'", decl->name(), "' has multiple definitions.");
  wr.message("The first definition is here:");
  wr.code(decl);
  for (const ID& id : ids) {
    if (id != decl->id()) {
      wr.note(id, "redefined here:");
      wr.code<ID, ID>(id);
    }
  }
}

void ErrorSuperFromTopLevelModule::write(ErrorWriterBase& wr) const {
  auto use = std::get<const uast::AstNode*>(info);
  auto mod = std::get<const uast::Module*>(info);
  auto useOrImport = std::get<resolution::VisibilityStmtKind>(info);
  auto useOrImportStr = (useOrImport == resolution::VIS_USE) ? "use"
                                                             : "import";
  wr.heading(kind_, type_, use, "invalid use of 'super' in '",
                                useOrImportStr, "'; '", mod->name().c_str(),
                                "' is a top-level module.");
  wr.code(use, {use});
  wr.note(mod->id(), "module '", mod->name(), "' was declared at the ",
                     "top level here:");
  wr.code(mod);
}

void ErrorTupleDeclMismatchedElems::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  auto type = std::get<const types::TupleType*>(info);
  wr.heading(kind_, type_, decl,
            "tuple size mismatch in split tuple declaration.");
  wr.code(decl);
  wr.message("The left-hand side of the declaration expects a ",
             decl->numDecls(), "-tuple, but the right-hand side is a ",
             type->numElements(), "-tuple, '", type, "'.");
}

void ErrorTupleDeclNotTuple::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  auto type = std::get<const types::Type*>(info);
  wr.heading(kind_, type_, decl,
            "value of non-tuple type '", type, "' cannot be split using a tuple "
            "declaration.");
  wr.message("In the following tuple declaration:");
  wr.code(decl);
  wr.message("the initialization expression has type '", type, "', while it is expected "
             "to be a ", decl->numDecls(), "-tuple.");
}

void ErrorTupleDeclUnknownType::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  wr.heading(kind_, type_, decl,
             "value of unknown type cannot be split using tuple assignment.");
  wr.code(decl);
}

void ErrorTupleExpansionNamedArgs::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info);
  auto tupleOp = std::get<const uast::OpCall*>(info);

  wr.heading(kind_, type_, fnCall, "tuple expansion cannot be used to pass "
             "values to a non-variadic named argument.");
  wr.message("A tuple is being expanded here:");
  wr.code(fnCall, { tupleOp });
}

void ErrorTupleExpansionNonTuple::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::FnCall*>(info);
  auto expansion = std::get<const uast::OpCall*>(info);
  auto& type = std::get<types::QualifiedType>(info);

  wr.heading(kind_, type_, call, "cannot apply tuple expansion to an "
             "expression of non-tuple type");
  wr.message("In the following function call:");
  wr.code(call, { expansion });
  wr.message("the expanded element has non-tuple type '", type.type(), "', "
             "but expansion can only be used on tuples.");
}

void ErrorUnknownEnumElem::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto elemName = std::get<UniqueString>(info);
  auto enumType = std::get<const types::EnumType*>(info);

  wr.heading(kind_, type_, node, "enum '", enumType->name(),
             "' has no element named '", elemName, "'.");
  wr.code(node, { node });
  wr.note(enumType->id(), "'", enumType->name(), "' is declared here.");
  wr.code(enumType->id());
}

void ErrorUnsupportedAsIdent::write(ErrorWriterBase& wr) const {
  auto as = std::get<const uast::As*>(info);
  auto expectedIdentifier = std::get<const uast::AstNode*>(info);
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

void ErrorUseImportNotModule::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto moduleName = std::get<std::string>(info);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info);
  wr.heading(kind_, type_, id, "cannot '", useOrImport, "' '", moduleName,
             "', which is not a module.");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code<ID, ID>(id, { id });
  wr.message("Only modules and enums can be used with '", useOrImport,
             "' statements.");
}

void ErrorUseImportUnknownMod::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto moduleName = std::get<std::string>(info);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info);
  wr.heading(kind_, type_, id, "cannot find module '", moduleName,
             "' for '", useOrImport, "'.");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code<ID, ID>(id, { id });
}

void ErrorUseImportUnknownSym::write(ErrorWriterBase& wr) const {
  auto visibilityClause = std::get<const uast::VisibilityClause*>(info);
  auto symbolName = std::get<std::string>(info);
  auto searchedScope = std::get<const resolution::Scope*>(info);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info);
  wr.heading(kind_, type_, locationOnly(visibilityClause),
             "cannot find symbol '", symbolName, "' for ", useOrImport, ".");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code(visibilityClause, { visibilityClause });
  // get class name of AstNode that generated scope (probably Module or Enum)
  // and lowercase it for readability
  std::string whatIsSearched = tagToString(searchedScope->tag());
  whatIsSearched[0] = std::tolower(whatIsSearched[0]);
  wr.message("Searching in the scope of ", whatIsSearched, " '",
             searchedScope->name(), "':");
  wr.code(searchedScope->id());
}

void ErrorUseOfLaterVariable::write(ErrorWriterBase& wr) const {
  auto stmt = std::get<const uast::AstNode*>(info);
  auto laterId = std::get<ID>(info);
  wr.heading(kind_, type_, stmt,
             "statement references a variable before it is defined.");
  wr.message("In the following statement:");
  wr.code(stmt);
  wr.message("there is a reference to a variable defined later:");
  wr.code(laterId);
  wr.message("Variables cannot be referenced before they are defined.");
}

void ErrorValueUsedAsType::write(ErrorWriterBase& wr) const {
  auto typeExpr = std::get<const uast::AstNode*>(info);
  auto type = std::get<types::QualifiedType>(info);
  wr.heading(kind_, type_, typeExpr,
             "type specifier is ", type, ", but it was expected to be a type.");
  wr.message("In the following type specifier:");
  wr.code(typeExpr, { typeExpr });
  // wr.message("Did you mean to use '.type'?");
}

/* end resolution errors */

} // end namespace 'chpl'
