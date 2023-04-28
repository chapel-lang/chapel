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
// error-classes-list.h which invokes this macro for every error type.
#define DIAGNOSTIC_CLASS DIAGNOSTIC_CLASS_IMPL
#include "chpl/resolution/resolution-error-classes-list.h"
#undef DIAGNOSTIC_CLASS

// helper functions that apply to multiple errors

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

static const char* allowedItem(resolution::VisibilityStmtKind kind) {
  return kind == resolution::VIS_USE ? "module or enum" : "module";
}

static const char* allowedItems(resolution::VisibilityStmtKind kind) {
  return kind == resolution::VIS_USE ? "modules or enums" : "modules";
}

// describe where a symbol came from
// (in a way, it prints out a ResultVisibilityTrace)
//
// 'errId' is an ID to anchor output if we have no better location
//
// 'name' is the name of the symbol that was looked up
//
// 'trace' stores the details of how this symbol was found
//
// 'start' indicates where in the trace to start, since sometimes
// the first element might have already been printed.
// 'oneOnly' indicates that only the 1st match should be described
//
// 'intro' will be emitted before the first message for a trace
// (only relevant if start==0). If it is not empty, it should probably
// end with a space.
//
// 'encounteredAutoModule' is set to 'false' at the start of this function
// but will be set to 'true' if the symbol came from an automatically use'd
// module
//
// 'from' is set to 'name' at the start of this function and will be
// set to the source name if the symbol was renamed with 'as'
//
// 'needsIntoText' is set to 'true' at the start of this function and
// will be set to 'false' if 'intro' was emitted
static void describeSymbolTrace(ErrorWriterBase& wr,
                                ID errId,
                                UniqueString name,
                                const resolution::ResultVisibilityTrace& trace,
                                int start,
                                const char* intro,
                                bool& encounteredAutoModule,
                                UniqueString& from,
                                bool& needsIntroText) {
  CHPL_ASSERT(0 <= start);

  from = name;
  encounteredAutoModule = false;
  int n = trace.visibleThrough.size();
  needsIntroText = true;
  for (int i = start; i < n; i++) {
    const auto& elt = trace.visibleThrough[i];
    if (elt.automaticModule) {
      std::string msg;
      if (start==0 && needsIntroText) {
        msg = intro;
      }
      if (from != name) {
        msg += "'" + from.str() + "'";
      } else {
        msg += "it";
      }
      wr.note(errId, msg,
              " was provided by the automatically-included modules.");
      encounteredAutoModule = true;
      needsIntroText = false;
      break;
    } else if (elt.fromUseImport) {
      std::string errbegin;
      std::string nameSuffix;
      if (start==0 && needsIntroText) {
        errbegin = intro;
        errbegin += "through";
      } else {
        errbegin = "and then through";
      }
      if (from != name) {
        nameSuffix += " providing '" + from.str() + "'";
      }

      wr.note(locationOnly(elt.visibilityClauseId), errbegin, " the '", elt.visibilityStmtKind, "' statement", nameSuffix, " here:");
      wr.code<ID,ID>(elt.visibilityClauseId, { elt.visibilityClauseId });
      from = elt.renameFrom;
      needsIntroText = false;
    }
  }
}


//
// Below are the implementations of 'write' for each error class, which does
// the specialized work.
//

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

static
void describeAmbiguousMatch(ErrorWriterBase& wr,
                            const uast::Identifier* ident,
                            const resolution::BorrowedIdsWithName& match,
                            const resolution::ResultVisibilityTrace& trace,
                            bool oneOnly,
                            const char* intro) {
  bool encounteredAutoModule = false;
  UniqueString from = ident->name();
  bool needsIntroText = true;
  describeSymbolTrace(wr, ident->id(), ident->name(), trace, 0, intro,
                      encounteredAutoModule, from, needsIntroText);

  if (!encounteredAutoModule) {
    if (match.numIds() == 1 || oneOnly) {
      ID firstId = match.firstId();
      if (needsIntroText) {
        wr.note(locationOnly(firstId), intro, "found '", from, "' defined here:");
      } else {
        wr.note(locationOnly(firstId), "found '", from, "' defined here:");
      }
      wr.code<ID,ID>(firstId, { firstId });
    } else {
      bool firstHere = true;
      for (const auto& id : match) {
        if (needsIntroText) {
          wr.note(id, intro, "found '", from, "' defined here:");
        } else if (firstHere) {
          wr.note(id, "found '", from, "' defined here:");
        } else {
          wr.note(id, "and found '", from, "' defined here:");
        }
        wr.code<ID,ID>(id, { id });
        needsIntroText = false;
        firstHere = false;
      }
    }
  }
}

void ErrorAmbiguousIdentifier::write(ErrorWriterBase& wr) const {
  auto ident = std::get<const uast::Identifier*>(info);
  auto moreMentions = std::get<bool>(info);
  auto matches = std::get<std::vector<resolution::BorrowedIdsWithName>>(info);
  auto trace = std::get<std::vector<resolution::ResultVisibilityTrace>>(info);

  wr.heading(kind_, type_, ident,
             "'", ident->name(), "' is ambiguous",
             moreMentions?" (first mention this function)":"");

  wr.code(ident, { ident });

  CHPL_ASSERT(matches.size() > 0);
  if (matches[0].numIds() > 1) {
    describeAmbiguousMatch(wr, ident, matches[0], trace[0],
                           /* one only */ false, /* intro */ "");
  } else {
    CHPL_ASSERT(matches.size() > 1);
    describeAmbiguousMatch(wr, ident, matches[0], trace[0],
                           /* one only */ true, /* intro */ "first, ");
    describeAmbiguousMatch(wr, ident, matches[1], trace[1],
                           /* one only */ true, /* intro */ "additionally, ");
  }

  return;
}

void ErrorAmbiguousVisibilityIdentifier::write(ErrorWriterBase& wr) const {
  auto name = std::get<UniqueString>(info);
  auto mentionId = std::get<ID>(info);
  auto potentialTargetIds = std::get<std::vector<ID>>(info);

  wr.heading(kind_, type_, mentionId,
             "'", name, "' is ambiguous");

  wr.code<ID, ID>(mentionId, { mentionId });

  bool printedOne = false;
  for (auto& id : potentialTargetIds) {
    wr.note(id, printedOne ? "or ":"", "it could refer to this '", name, "'");
    printedOne = true;
    wr.code<ID, ID>(id, { id });
  }

  // TODO: call describeSymbolTrace
  return;
}

void ErrorAsWithUseExcept::write(ErrorWriterBase& wr) const {
  auto use = std::get<const uast::Use*>(info);
  auto as = std::get<const uast::As*>(info);
  wr.heading(kind_, type_, locationOnly(use),
             "cannot rename in 'except' list.");
  wr.message("The renaming occurs because of the use of 'as' here:");
  wr.code(use, { as });
}

void ErrorDeprecation::write(ErrorWriterBase& wr) const {
  auto msg = std::get<std::string>(info);
  auto mention = std::get<const uast::AstNode*>(info);
  auto target = std::get<const uast::NamedDecl*>(info);
  CHPL_ASSERT(mention && target);

  wr.headingVerbatim(kind_, type_, mention, msg);
  wr.code(mention, {mention});

  /* TODO: Need to know whether the symbol is user or not:
  wr.note(target, "declared here:");
  wr.code(target);
  */

  std::ignore = target;
  return;
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

void ErrorHiddenFormal::write(ErrorWriterBase& wr) const {
  auto formal = std::get<const uast::Formal*>(info);
  const auto& match = std::get<resolution::BorrowedIdsWithName>(info);
  const auto& trace = std::get<resolution::ResultVisibilityTrace>(info);
  CHPL_ASSERT(formal && !trace.visibleThrough.empty());

  // find the first visibility clause ID
  ID firstVisibilityClauseId;
  resolution::VisibilityStmtKind firstUseOrImport = resolution::VIS_USE;

  int start = 0;

  int i = 0;
  for (const auto& elt : trace.visibleThrough) {
    if (elt.fromUseImport) {
      firstVisibilityClauseId = elt.visibilityClauseId;
      firstUseOrImport = elt.visibilityStmtKind;
      start = i+1; // skip this one in describeSymbolTrace
      break;
    }
    i++;
  }

  wr.heading(kind_, type_, firstVisibilityClauseId,
             "module-level symbol is hiding function argument '",
             formal->name(), "'");

  wr.message("The formal argument:");
  wr.code(formal, { formal });
  wr.message("is shadowed by a symbol provided by the following '",
             firstUseOrImport, "' statement:");
  wr.code<ID, ID>(firstVisibilityClauseId, { firstVisibilityClauseId });

  // print where it came from
  bool encounteredAutoModule = false;
  UniqueString from;
  bool needsIntroText = true;
  describeSymbolTrace(wr, formal->id(), formal->name(), trace, start, "",
                      encounteredAutoModule, from, needsIntroText);

  if (!encounteredAutoModule) {
    ID firstId = match.firstId();
    wr.note(locationOnly(firstId), "found '", from, "' defined here:");
    wr.code<ID,ID>(firstId, { firstId });
  }
}

void ErrorIfVarNonClassType::write(ErrorWriterBase& wr) const {
  auto cond = std::get<const uast::Conditional*>(info);
  auto qtVar = std::get<types::QualifiedType>(info);
  auto var = cond->condition()->toVariable();
  CHPL_ASSERT(var);
  auto ifKindStr = cond->isExpressionLevel() ? "expression" : "statement";
  wr.heading(kind_, type_, var, "a variable declared in the condition of "
                                "an if ", ifKindStr, " must be a class, "
                                "but it has non-class type '",
                                qtVar.type(), "'.");
  wr.code(cond, {var});
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

void ErrorInvalidSuper::write(ErrorWriterBase& wr) const {
  auto superExpr = std::get<const uast::Identifier*>(info);
  auto qt = std::get<types::QualifiedType>(info);

  const types::RecordType* recordType = nullptr;
  if (auto type = qt.type()) {
    recordType = type->toRecordType();
  }

  if (recordType) {
    wr.heading(kind_, type_, superExpr, "invalid use of 'super' in record '", recordType->name(), "'.");
  } else {
    wr.heading(kind_, type_, superExpr, "invalid use of 'super' with ", qt);
  }
  wr.code(superExpr, { superExpr });
  if (recordType) {
    wr.note(superExpr, "inheritance is not currently supported for records.");
    wr.message(
        "Thoughts on what record inheritance should entail can be added to "
        "https://github.com/chapel-lang/chapel/issues/6851.");
    wr.message(recordType->name(), " declared as a record here:");
    wr.codeForLocation(recordType->id());
    wr.message("If you meant to declare '", recordType->name(), "' as a class ",
               "instead, you can do that by writing 'class ", recordType->name(),
               "' instead of 'record ", recordType->name(), "'.");
  }
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
    wr.codeForLocation(record->id());
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

void ErrorModuleAsVariable::write(ErrorWriterBase& wr) const {
  auto node = std::get<0>(info);
  auto parent = std::get<1>(info);
  auto mod = std::get<const uast::Module*>(info);
  const char* reason = "cannot be mentioned like variables";

  if (parent) {
    if (auto call = parent->toCall()) {
      if (call->calledExpression() == node) {
        reason = "cannot be called like procedures";
      }
    }
  }
  wr.heading(kind_, type_, node, "modules (like '", mod->name(), "' here) ",
             reason, ".");
  wr.code(parent, { node });
}

void ErrorMultipleEnumElems::write(ErrorWriterBase& wr) const {
  auto enumAst = std::get<const uast::Enum*>(info);
  auto elemName = std::get<UniqueString>(info);
  auto& possibleElems = std::get<std::vector<ID>>(info);

  wr.heading(kind_, type_, enumAst->id(), "enum '", enumAst->name(),
             "' has multiple elements named '", elemName, "'.");
  wr.code(enumAst->id());
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

void ErrorNestedClassFieldRef::write(ErrorWriterBase& wr) const {
  auto outerDecl = std::get<0>(info);
  auto innerDecl = std::get<1>(info);
  auto reference = std::get<2>(info);
  auto id = std::get<3>(info);

  const char* outerName = outerDecl->isClass() ? "class" : "record";
  const char* innerName = innerDecl->isClass() ? "class" : "record";
  // Shouldn't even be possible to trigger this with unions.
  CHPL_ASSERT(!outerDecl->isUnion());
  CHPL_ASSERT(!innerDecl->isUnion());

  if (auto ident = reference->toIdentifier()) {
    wr.heading(kind_, type_, reference, "illegal use of identifier '",
               ident->name(), "' from enclosing type.");
  } else {
    wr.heading(kind_, type_, reference,
               "illegal use of identifier from enclosing type.");
  }
  wr.code(reference, { reference });
  wr.note(innerDecl, "the identifier is used within ", innerName, " '",
          innerDecl->name(), "', declared here:");
  wr.codeForLocation(innerDecl);
  wr.note(outerDecl, "however, the identifier refers to a field of an enclosing ",
          outerName, " '", outerDecl->name(), "', declared here:");
  wr.codeForLocation(outerDecl);
  wr.note(id, "field declared here:");
  wr.codeForDef(id);
}

void ErrorNonIterable::write(ErrorWriterBase &wr) const {
  auto loop = std::get<0>(info);
  auto iterand = std::get<1>(info);
  auto& iterandType = std::get<types::QualifiedType>(info);
  wr.heading(kind_, type_, loop, "cannot iterate over ", decayToValue(iterandType), ".");
  wr.message("Used as an iterand in a loop here:");
  wr.code(iterand, { iterand });
}

void ErrorNotInModule::write(ErrorWriterBase& wr) const {
  const uast::Dot* dot = std::get<0>(info);
  //ID moduleId = std::get<1>(info);
  UniqueString moduleName = std::get<2>(info);
  ID renameClauseId = std::get<3>(info);

  wr.heading(kind_, type_, dot,
             "cannot find '", dot->field(), "' in module '", moduleName, "'");

  wr.code(dot, { dot });

  UniqueString dotModName = moduleName;
  if (auto dotLeftPart = dot->receiver()) {
    if (auto leftIdent = dotLeftPart->toIdentifier()) {
      dotModName = leftIdent->name();
    }
  }

  if (dotModName != moduleName) {
    if (renameClauseId.isEmpty()) {
      wr.note(dot, "module '", moduleName, "' was renamed to"
              " '", dotModName, "' in this scope");
    } else {
      wr.note(locationOnly(renameClauseId),
              "module '", moduleName, "' was renamed to"
              " '", dotModName, "' here");
      wr.code<ID>(renameClauseId, { renameClauseId });
    }
  }

  //wr.note(moduleId, "module '", moduleName, "' declared here");

  return;
}

void ErrorPrivateToPublicInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, type_, moduleInclude,
             "cannot make a private module public through "
             "an include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "module declared private here:");
  wr.codeForLocation(moduleDef);
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
  wr.codeForLocation(moduleDef);
}

// find the first ID not from use/import, returns true and sets result
// if one is found.
static bool firstIdFromDecls(
    const std::vector<resolution::BorrowedIdsWithName>& matches,
    const std::vector<resolution::ResultVisibilityTrace>& trace,
    ID& result) {
  int n = matches.size();
  for (int i = 0; i < n; i++) {
    const auto& t = trace[i];
    if (t.visibleThrough.size() == 0) {
      // TODO: find the first non-function ID?
      // To do that, would use flags in BorrowedIdsWithName
      // to filter Ids.
      result = matches[i].firstId();
      return true;
    }
  }
  return false;
}

void ErrorRedefinition::write(ErrorWriterBase& wr) const {
  auto scopeId = std::get<ID>(info);
  auto name = std::get<UniqueString>(info);
  auto& matches = std::get<std::vector<resolution::BorrowedIdsWithName>>(info);
  auto& trace = std::get<std::vector<resolution::ResultVisibilityTrace>>(info);

  // compute the anchor ID for the error message
  ID id;
  bool anchorToDef = firstIdFromDecls(matches, trace, id);
  if (!anchorToDef) {
    // Use the scope's ID
    id = scopeId;
  }

  if (anchorToDef) {
    wr.heading(kind_, type_, id, "'", name, "' has multiple definitions.");
  } else {
    wr.heading(kind_, type_, id, "'", name, "' has multiple definitions in this scope.");
  }

  bool firstGroup = true;
  int n = matches.size();
  for (int i = 0; i < n; i++) {
    bool encounteredAutoModule = false;
    UniqueString from;
    bool needsIntroText = true;
    std::string intro;
    if (firstGroup) {
      intro = "it was first defined ";
      firstGroup = false;
    } else {
      intro = "redefined ";
    }
    describeSymbolTrace(wr, scopeId, name, trace[i], 0, intro.c_str(),
                        encounteredAutoModule, from, needsIntroText);
    bool printedUseTrace = !needsIntroText;

    // print out the other IDs
    bool firstDef = true;
    for (const auto& matchId : matches[i]) {
      if (needsIntroText) {
        if (anchorToDef) {
          wr.message("It was first defined here:");
        } else {
          wr.note(locationOnly(matches[i].firstId()), intro + "here:");
        }
        needsIntroText = false;
      } else {
        if (printedUseTrace) {
          if (firstDef) {
            wr.note(locationOnly(matchId), "leading to the definition here:");
            firstDef = false;
          } else {
            wr.note(locationOnly(matchId), "and to the definition here:");
          }
        } else {
          wr.note(locationOnly(matchId), "redefined here:");
        }
      }

      wr.codeForDef(matchId);
    }
  }
}

static const uast::AstNode* getReduceOrScanOp(const uast::AstNode* reduceOrScan) {
  if (auto reduce = reduceOrScan->toReduce()) {
    return reduce->op();
  } else if (auto scan = reduceOrScan->toScan()) {
    return scan->op();
  }
  return nullptr;
}

void ErrorReductionInvalidName::write(ErrorWriterBase& wr) const {
  auto reduceOrScan = std::get<const uast::AstNode*>(info);
  auto name = std::get<UniqueString>(info);
  auto& iterType = std::get<types::QualifiedType>(info);

  auto op = getReduceOrScanOp(reduceOrScan);
  wr.heading(kind_, type_, op,
            "identifier '", name, "' does not represent "
            "a valid reduction operation.");
  wr.message("In the following 'reduce' expression:");
  wr.code(reduceOrScan, { op });
  wr.message("Identifiers on the left of the 'reduce' expression are applied "
             "to the type of the iterator's elements ('", iterType.type(), "' "
             "in this case)");
  wr.message("Is '", name, "(", iterType.type(), ")' a valid reduction operation?");
}

void ErrorReductionNotReduceScanOp::write(ErrorWriterBase& wr) const {
  auto reduceOrScan = std::get<const uast::AstNode*>(info);
  auto actualType = std::get<types::QualifiedType>(info);
  const types::BasicClassType* actualClassType = nullptr;

  auto op = getReduceOrScanOp(reduceOrScan);
  wr.heading(kind_, type_, op, "invalid operation in 'reduce' expression.");
  wr.code(reduceOrScan, { op });

  // Don't print the details of managed / unmanaged / etc.
  if (auto classType = actualType.type()->toClassType()) {
    actualClassType = classType->basicClassType();
    while (auto instFrom = actualClassType->instantiatedFrom()) {
      actualClassType = instFrom;
    }
    actualType = types::QualifiedType(actualType.kind(), actualClassType);
  }
  wr.message("The operation must be a type extending 'ReduceScanOp', but "
             "it is ", actualType);
  if (actualClassType) {
    wr.message("Did you mean for class '", actualClassType,
        "' to extend 'ReduceScanOp'?");
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
  wr.codeForLocation(mod);
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
  auto enumAst = std::get<const uast::Enum*>(info);

  wr.heading(kind_, type_, node, "enum '", enumAst->name(),
             "' has no element named '", elemName, "'.");
  wr.code(node, { node });
  wr.note(enumAst->id(), "'", enumAst->name(), "' is declared here.");
  wr.codeForLocation(enumAst->id());
}

void ErrorUnknownIdentifier::write(ErrorWriterBase& wr) const {
  auto ident = std::get<const uast::Identifier*>(info);
  auto mentionedMoreThanOnce = std::get<bool>(info);

  wr.heading(kind_, type_, ident,
             "'", ident->name(), "' cannot be found",
             mentionedMoreThanOnce?" (first mention this function)":"");

  wr.code(ident, { ident });

  return;
}

void ErrorUnstable::write(ErrorWriterBase& wr) const {
  auto msg = std::get<std::string>(info);
  auto mention = std::get<const uast::AstNode*>(info);
  auto target = std::get<const uast::NamedDecl*>(info);
  CHPL_ASSERT(mention && target);

  wr.headingVerbatim(kind_, type_, mention, msg);
  wr.code(mention, {mention});

  /* TODO: Need to know whether the symbol is user or not:
  wr.note(target, "declared here:");
  wr.code(target);
  */

  std::ignore = target;
  return;
}

void ErrorUseImportMultiplyDefined::write(ErrorWriterBase& wr) const {
  auto symbolName = std::get<UniqueString>(info);
  auto firstOccurrence = std::get<1>(info);
  auto secondOccurrence = std::get<2>(info);

  wr.heading(kind_, type_, secondOccurrence, "'",
             symbolName, "' is multiply defined.");
  wr.message("'", symbolName, "' was first defined here:");
  wr.codeForDef(firstOccurrence);
  wr.message("Redefined here:");
  wr.codeForDef(secondOccurrence);
}

void ErrorUseImportMultiplyMentioned::write(ErrorWriterBase& wr) const {
  auto symbolName = std::get<UniqueString>(info);
  auto firstOccurrence = std::get<1>(info);
  auto secondOccurrence = std::get<2>(info);

  wr.heading(kind_, type_, secondOccurrence, "'",
             symbolName, "' is repeated.");
  wr.message("'", symbolName, "' was first mentioned here:");
  wr.codeForDef(firstOccurrence);
  wr.message("Mentioned again here:");
  wr.codeForDef(secondOccurrence);
}

void ErrorUseImportNotModule::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto moduleName = std::get<std::string>(info);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info);

  wr.heading(kind_, type_, id, "cannot '", useOrImport, "' symbol '", moduleName,
             "', which is not a ", allowedItem(useOrImport), ".");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code<ID>(id, { id });
  wr.message("Only ", allowedItems(useOrImport), " can be used with '",
             useOrImport, "' statements.");
}

void ErrorUseImportTransitiveRename::write(ErrorWriterBase& wr) const {
  auto from = std::get<0>(info);
  auto middle = std::get<1>(info);
  auto to = std::get<2>(info);

  auto firstRename = std::get<3>(info);
  auto secondRename = std::get<4>(info);

  wr.heading(kind_, type_, secondRename, "'",
             middle, "' is repeated.");
  wr.message("First, '", from, "' is renamed to '", middle, "'.");
  wr.code(firstRename, {firstRename});
  wr.message("Then, '", middle, "' is renamed to '", to, "'.");
  wr.code(secondRename, {secondRename});
  wr.note(firstRename, "did you mean to rename '", from, "' to '", to, "'?");
  wr.message("You can do so by a single rename, '", from, " as ", to, "'.");
}

void ErrorUseImportUnknownMod::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info);
  auto moduleName = std::get<2>(info);
  auto previousPartName = std::get<3>(info);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info);
  auto& improperMatches = std::get<std::vector<const uast::AstNode*>>(info);

  if (previousPartName.empty()) {
    wr.heading(kind_, type_, id, "cannot find ", allowedItem(useOrImport),
               " named '", moduleName, "'.");
  } else {
    wr.heading(kind_, type_, id, "cannot find ", allowedItem(useOrImport),
               " named '", moduleName, "' in module '", previousPartName, "'.");
  }
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code<ID>(id, { id });
  if (!improperMatches.empty()) {
    wr.message("The following declarations are not covered by the '", useOrImport,
               "' statement but seem similar to what you meant.");
    // Do we need to tell the user how to use super.M or this.M?
    bool explainRelativeImport = false;

    for (auto ast : improperMatches) {
      // Separate out the various suggestions.
      wr.message("");

      // Use locationOnly to avoid printing 'in module M' for every suggestion.

      auto improperId = ast->id();
      auto tag = ast->tag();
      if (tag == uast::asttags::AstTag::Module) {
        wr.note(locationOnly(improperId),
                "a module named '", moduleName, "' is defined here:");
        wr.codeForDef(improperId);
        wr.note(locationOnly(improperId),
                "however, a full path or an explicit relative ", useOrImport,
                " is required for modules that are not at the root level.");
        explainRelativeImport = true;
      } else {
        wr.note(locationOnly(improperId), "a declaration of '", moduleName,
                "' is here:");
        wr.codeForDef(improperId);
        wr.note(locationOnly(improperId), "however, '", useOrImport,
                "' statements can only be used with ",
                allowedItems(useOrImport), " (and this '", moduleName,
                "' is not a ", allowedItem(useOrImport), ").");
      }

      // Comment out if we want to show all improper matches, not just one.
      break;
    }

    if (explainRelativeImport) {
      wr.message("");
      wr.note(id, "For non-root-level modules, please specify the full path "
                  "to the module or use a relative import e.g. 'import this.M' "
                  "or 'import super.M'");
    }
  }
}

void ErrorUseImportUnknownSym::write(ErrorWriterBase& wr) const {
  auto visibilityClause = std::get<const uast::VisibilityClause*>(info);
  auto symbolName = std::get<std::string>(info);
  auto searchedScope = std::get<const resolution::Scope*>(info);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info);
  auto isRename = std::get<bool>(info);

  auto limitationKind = visibilityClause->limitationKind();
  if (isRename) {
    wr.heading(kind_, type_, visibilityClause,
               "cannot rename symbol '", symbolName, "' as it is not defined in '",
               searchedScope->name(),"'.");
  } else if (limitationKind == uast::VisibilityClause::ONLY ||
      limitationKind == uast::VisibilityClause::EXCEPT) {
    wr.heading(kind_, type_, visibilityClause,
               "cannot use '", limitationKind, "' clause with symbol '",
               symbolName, "' as it is not defined in '",
               searchedScope->name(),"'.");
  } else {
    wr.heading(kind_, type_, visibilityClause,
               "cannot '", useOrImport, "' symbol '", symbolName, "' "
               "as it is not defined in '", searchedScope->name(), "'.");
  }
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code(visibilityClause, { visibilityClause });
  // get class name of AstNode that generated scope (probably Module or Enum)
  // and lowercase it for readability
  std::string whatIsSearched = tagToString(searchedScope->tag());
  whatIsSearched[0] = std::tolower(whatIsSearched[0]);
  wr.message("Searching in the scope of ", whatIsSearched, " '",
             searchedScope->name(), "':");
  wr.codeForLocation(searchedScope->id());
}

void ErrorUseOfLaterVariable::write(ErrorWriterBase& wr) const {
  auto stmt = std::get<const uast::AstNode*>(info);
  auto laterId = std::get<ID>(info);
  wr.heading(kind_, type_, stmt,
             "statement references a variable before it is defined.");
  wr.message("In the following statement:");
  wr.code(stmt);
  wr.message("there is a reference to a variable defined later:");
  wr.codeForDef(laterId);
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
