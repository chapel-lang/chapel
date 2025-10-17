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
#include <cstdlib>
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

static const char* nilabilityStr(const types::ClassTypeDecorator& dec) {
  return dec.isNilable() ? "nilable" : "non-nilable";
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
      wr.note(errId, msg, "in the automatically-included modules.");
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
        errbegin = "and then ";
        errbegin += "through";
      }
      if (from != name) {
        nameSuffix += " providing '" + from.str() + "'";
      }

      std::string of;
      if (!elt.usedImportedThingName.isEmpty()) {
        of += " of '";
        of += elt.usedImportedThingName.str();
        of += "'";
      } else {
        of = " statement";
      }

      wr.note(locationOnly(elt.visibilityClauseId), errbegin,
              " the '", elt.visibilityStmtKind, "'", of,
              nameSuffix, " here:");
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
void ErrorAmbiguousCall::write(ErrorWriterBase&wr) const {
  auto& callName = std::get<0>(info_);
  auto& option1 = std::get<1>(info_);
  auto& option2 = std::get<2>(info_);
  wr.heading(kind_, type_, locationOnly(callName), "ambiguous call to ", callName);
  wr.note(locationOnly(option1),"found this method:");
  wr.code(option1);
  wr.note(locationOnly(option2), "and this function:");
  wr.code(option2);
}

void ErrorAmbiguousConfigName::write(ErrorWriterBase& wr) const {
  auto& name = std::get<std::string>(info_);
  auto variable = std::get<const uast::Variable*>(info_);
  auto otherId = std::get<ID>(info_);
  wr.heading(kind_, type_, locationOnly(variable), "ambiguous config name (", name, ").");
  wr.code(variable);
  wr.note(locationOnly(otherId), "also defined here:");
  wr.code(otherId);
  wr.note(locationOnly(otherId), "(disambiguate using -s<modulename>.", name, "...)");
}

void ErrorAmbiguousConfigSet::write(ErrorWriterBase& wr) const {
  auto& name1 = std::get<1>(info_);
  auto& name2 = std::get<2>(info_);
  auto variable = std::get<const uast::Variable*>(info_);
  wr.heading(kind_, type_, locationOnly(variable),
            "config set ambiguously via '-s", name1, "' and '-s", name2, "'");
}

static
void describeAmbiguousMatch(ErrorWriterBase& wr,
                            const uast::Identifier* ident,
                            const resolution::MatchingIdsWithName& match,
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
  auto ident = std::get<const uast::Identifier*>(info_);
  auto moreMentions = std::get<bool>(info_);
  auto matches = std::get<resolution::MatchingIdsWithName>(info_);
  auto trace = std::get<std::vector<resolution::ResultVisibilityTrace>>(info_);

  wr.heading(kind_, type_, ident,
             "'", ident->name(), "' is ambiguous",
             moreMentions?" (first mention this function)":"");

  wr.code(ident, { ident });

  CHPL_ASSERT(matches.numIds() > 0);
  describeAmbiguousMatch(wr, ident, matches, trace[0],
                         /* one only */ false, /* intro */ "");

  return;
}

void ErrorAmbiguousVisibilityIdentifier::write(ErrorWriterBase& wr) const {
  auto name = std::get<UniqueString>(info_);
  auto mentionId = std::get<ID>(info_);
  auto potentialTargetIds = std::get<resolution::MatchingIdsWithName>(info_);

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
  auto use = std::get<const uast::Use*>(info_);
  auto as = std::get<const uast::As*>(info_);
  wr.heading(kind_, type_, locationOnly(use),
             "cannot rename in 'except' list.");
  wr.message("The renaming occurs because of the use of 'as' here:");
  wr.code(use, { as });
}

void ErrorAssignFieldBeforeInit::write(ErrorWriterBase& wr) const {
  auto initCall = std::get<const uast::FnCall*>(info_);
  auto& initializations = std::get<1>(info_);

  wr.heading(kind_, type_, initCall,
             "field initialization not allowed before 'super.init()' or 'this.init()'");

  for (auto& initPair : initializations) {
    auto decl = initPair.first;
    auto id = initPair.second;
    wr.note(id, "field '", decl->name(), "' is initialized before the 'init' call here:");
    wr.code<ID, ID>(id, { id });
  }
}

void ErrorConstRefCoercion::write(ErrorWriterBase& wr) const {
  auto ast = std::get<const uast::AstNode*>(info_);
  auto& c = std::get<resolution::MostSpecificCandidate>(info_);

  auto formalName = c.fn()->formalName(c.constRefCoercionFormal());

  wr.heading(kind_, type_, ast, "function call requires coercion of actual ",
             (c.constRefCoercionActual() + 1) ," for 'const ref' formal '",
             formalName, "'.");
  if (auto call = ast->toCall()) {
    wr.code(call, { call->actual(c.constRefCoercionActual()) });
  } else {
    wr.code(ast);
  }
  wr.message("Formals with the 'const ref' intent do not currently support coercions.");

  auto fmlDecl = c.fn()->untyped()->formalDecl(c.constRefCoercionFormal());
  if (fmlDecl) {
    wr.message("The formal was declared 'const ref' here:");
    wr.code(fmlDecl, { fmlDecl });
  }
}

void ErrorDeprecation::write(ErrorWriterBase& wr) const {
  auto msg = std::get<std::string>(info_);
  auto mention = std::get<const uast::AstNode*>(info_);
  auto target = std::get<const uast::NamedDecl*>(info_);
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
  auto dot = std::get<const uast::Dot*>(info_);
  auto visibilityClause = std::get<const uast::VisibilityClause*>(info_);
  auto limitationKind =
      std::get<const uast::VisibilityClause::LimitationKind>(info_);
  wr.heading(kind_, type_, locationOnly(dot), "cannot use dot expression '",
             dot, "' in '", limitationKind, "' list.");
  wr.code(visibilityClause, {dot});
  wr.message(
      "Dot expressions are not allowed in the 'except' or 'only' list of a "
      "'use' or 'import'.");
}

void ErrorDotTypeOnType::write(ErrorWriterBase& wr) const {
  auto dot = std::get<const uast::Dot*>(info_);
  auto dottedType = std::get<const types::Type*>(info_);
  auto typeDeclId = std::get<ID>(info_);
  const bool haveType = dottedType && !dottedType->isErroneousType();
  if (haveType) {
    wr.heading(kind_, type_, dot, "can't apply '.type' to a type ('",
               dottedType, "').");
  } else {
    wr.heading(kind_, type_, dot, "can't apply '.type' to a type.");
  }
  wr.code(dot, {dot});
  if (haveType) {
    wr.message(
        "The '.type' accessor can only be applied to values, but the receiver "
        "of the above expression is the type '",
        dottedType, "'.");
  } else {
    wr.message(
        "The '.type' accessor can only be applied to values, but the receiver "
        "of the above expression is a type.");
  }
  if (typeDeclId) {
    wr.message("The receiver is declared as a 'type' variable here:");
    wr.code(typeDeclId);
  }
}


void ErrorEnumAbstract::write(ErrorWriterBase& wr) const {
  auto location = std::get<const uast::AstNode*>(info_);
  auto& direction = std::get<std::string>(info_);
  auto enumType = std::get<const types::EnumType*>(info_);
  auto otherType = std::get<const types::Type*>(info_);

  if (direction == "from" && otherType) {
    wr.heading(kind_, type_, location,
               "cannot cast from abstract enumeration type '", enumType->name(),"' to '", otherType, "'.");
  } else if (direction == "to" && otherType) {
    wr.heading(kind_, type_, location,
               "cannot cast from '", otherType, "' to abstract enumeration type '", enumType->name(),"'.");
  } else {
    wr.heading(kind_, type_, location, "enumeration type '", enumType->name(),
               "' is abstract and cannot be converted to a numeric value.");
  }
  wr.code(location);
  wr.message("The enumeration type '", enumType->name(), "' is declared here:");
  wr.codeForLocation(enumType->id());
  wr.message("The type is abstract because none of its constants have been assigned numeric values.");
}

void ErrorEnumInitializerNotParam::write(ErrorWriterBase& wr) const {
  auto enumElement = std::get<const uast::EnumElement*>(info_);
  auto qt = std::get<types::QualifiedType>(info_);

  wr.heading(kind_, type_, enumElement,
             "only 'param' values can be associated with enumeration constants");
  wr.codeForLocation(enumElement);
  wr.message("The initialization expression of '", enumElement->name(), "' is ", qt);
}

void ErrorEnumInitializerNotInteger::write(ErrorWriterBase& wr) const {
  auto enumElement = std::get<const uast::EnumElement*>(info_);
  auto qt = std::get<types::QualifiedType>(info_);

  wr.heading(kind_, type_, enumElement,
             "only integer (signed or unsigned) values can be associated with enumeration constants");
  wr.codeForLocation(enumElement);
  wr.message("The initialization expression of '", enumElement->name(), "' produces ", qt.param(), ", ", qt);
}

void ErrorEnumValueAbstract::write(ErrorWriterBase& wr) const {
  auto location = std::get<const uast::AstNode*>(info_);
  auto enumType = std::get<const types::EnumType*>(info_);
  auto enumElement = std::get<const uast::EnumElement*>(info_);

  wr.heading(kind_, type_, location,
             "cannot get numeric value from abstract enumeration constant '",
             enumElement->name(), "'");
  wr.code(location);
  wr.message("The constant '", enumElement->name(),
             "' is declared here:");
  wr.codeForLocation(enumElement);
  wr.message("The constant is abstract because neither it nor any of the "
             "preceding constants in '", enumType->name(),
             "' have been assigned numeric values.");
}

void ErrorExternCCompilation::write(ErrorWriterBase& wr) const {
  auto externBlockId = std::get<ID>(info_);
  auto errors = std::get<std::vector<std::pair<Location, std::string>>>(info_);
  wr.heading(kind_, type_, externBlockId,
             "running clang on extern block failed",
             (errors.size() > 0 ? " -- clang errors follow" : ""));
  for (const auto& error : errors) {
    wr.note(error.first, error.second);
  }
}

void ErrorGenericFieldWithoutMark::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::Decl*>(info_);
  auto actualQt = std::get<types::QualifiedType>(info_);

  CHPL_ASSERT(decl->isVarLikeDecl());
  auto var = decl->toVarLikeDecl();

  wr.heading(kind_, type_, decl, "field '", var->name(), "' has generic type '",
             actualQt.type(), "' but is not marked with '?'.");
  wr.code(decl, { decl });
  wr.note(decl, "generic fields must be marked with '?'.");
}

void ErrorIfVarNonClassType::write(ErrorWriterBase& wr) const {
  auto cond = std::get<const uast::Conditional*>(info_);
  auto qtVar = std::get<types::QualifiedType>(info_);
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
  auto code = std::get<const uast::AstNode*>(info_);
  auto lastModule = std::get<1>(info_);
  auto implicitModule = std::get<2>(info_);
  wr.heading(kind_, type_, locationOnly(code), "an implicit module named '",
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
  auto ifExpr = std::get<const uast::Conditional*>(info_);
  auto qt1 = std::get<1>(info_);
  auto qt2 = std::get<2>(info_);

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
  auto initExpr = std::get<const uast::AstNode*>(info_);
  auto initType = std::get<types::QualifiedType>(info_);
  auto declKind = std::get<types::QualifiedType::Kind>(info_);

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
  auto range = std::get<const uast::Range*>(info_);
  auto qt1 = std::get<1>(info_);
  auto qt2 = std::get<2>(info_);

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
  auto decl = std::get<0>(info_);
  auto type = std::get<1>(info_);
  auto init = std::get<2>(info_);
  auto typeExprType = std::get<3>(info_);
  auto initExprType = std::get<4>(info_);

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

void ErrorIncompatibleYieldTypes::write(ErrorWriterBase& wr) const {
  auto call = std::get<0>(info_);
  auto& yieldTypes = std::get<1>(info_);

  wr.heading(kind_, type_, call, "incompatible yield types in iterator group.");
  wr.message("Encountered while resolving the following call:");
  wr.codeForLocation(call);
  wr.message("All versions of the iterator except the leader should yield the same type.");

  for (auto& data : yieldTypes) {
    wr.message("");
    auto iterKindStr = uast::Function::iteratorKindToString(std::get<0>(data));
    auto qt = std::get<1>(data);
    auto fn = std::get<2>(data);

    wr.note(fn->id(), "the ", iterKindStr, " version of the iterator '", fn->untyped()->name() ,"' yields values of type '", qt.type(), "'.");
    wr.codeForLocation(fn->id());
  }
}

static std::string buildTupleDeclName(const uast::TupleDecl* tup) {
  std::string ret = "(";
  int count = 0;
  for (auto decl : tup->decls()) {
    if (count != 0) {
      ret += ",";
    }
    count += 1;

    if (decl->isTupleDecl()) {
      ret += buildTupleDeclName(decl->toTupleDecl());
    } else {
      ret += decl->toFormal()->name().str();
    }
  }

  if (count == 1) {
    ret += ",";
  }

  ret += ")";

  return ret;
}

template <typename GetActual>
static void printRejectedCandidates(ErrorWriterBase& wr,
                                    const ID& anchorId,
                                    const resolution::CallInfo& ci,
                                    const std::vector<resolution::ApplicabilityResult>& rejected,
                                    const char* passedThingArticle,
                                    const char* passedThing,
                                    const char* expectedThingArticle,
                                    const char* expectedThing,
                                    GetActual&& getActual,
                                    const std::vector<const uast::VarLikeDecl*>& actualDecls) {
  bool noActuals =
    (ci.numActuals() == 0 && !ci.isMethodCall()) ||
    (ci.numActuals() == 1 && ci.isMethodCall());

  // "decent" candidates are rejected with some explanation (as opposed
  // to a generic 'didn't match'). If we found a candidate with a decent
  // explanation, don't print other ones since they're unlikely to be
  // helpful.
  bool printedDecentCandidate = false;

  unsigned int printCount = 0;
  static const unsigned int maxPrintCount = 2;
  for (auto& candidate : rejected) {
    if (printCount == maxPrintCount) break;

    bool isThisCandidateDecent = true;

    auto reason = candidate.reason();
    if (/* skip printing detailed info_ here because computing the formal-actual
        map will go poorly with an unknown formal. */
        candidate.failedDueToWrongActual()) {
      auto fn = candidate.initialForErr();
      resolution::FormalActualMap fa(fn, ci);
      auto badPass = fa.byFormalIdx(candidate.formalIdx());
      auto formalDecl = badPass.formal();
      const uast::AstNode* actualExpr = getActual(badPass.actualIdx());

      wr.message("");
      wr.note(fn->id(), "the following candidate didn't match because ", passedThingArticle, " ", passedThing, " couldn't be passed to ", expectedThingArticle, " ", expectedThing, ":");
      wr.code(fn->id(), { formalDecl });

      std::string formalName;
      if (auto named = formalDecl->toNamedDecl()) {
        formalName = "'" + named->name().str() + "'";
      } else if (formalDecl->isTupleDecl()) {
        formalName = "'" + buildTupleDeclName(formalDecl->toTupleDecl()) + "'";
      }
      bool actualPrinted = false;
      const uast::VarLikeDecl* offendingActual = actualDecls.at(printCount);
      if (candidate.formalReason() == resolution::FAIL_VARARG_TQ_MISMATCH) {
        // a single vararg formal with a type query (like `x: ?t`) was used
        // to pass actuals of different types. Collect all applicable types.

        resolution::FormalActualMap fa(fn, ci);
        std::vector<std::pair<const types::Type*, size_t>> actuals;
        std::set<const types::Type*> seenTypes;
        for (size_t i = 0; i < ci.numActuals(); i++){
          auto entry = fa.byActualIdx(i);
          if (!entry) continue;
          if (seenTypes.insert(entry->actualType().type()).second) {
            actuals.emplace_back(entry->actualType().type(), i);
          }
        }

        CHPL_ASSERT(actuals.size() > 1);
        wr.message("The variable-argument ", expectedThing, " ", formalName,
                   ", which contains a type query, was used with ", passedThing, "s of "
                    "incompatible types '", actuals[0].first, "' and '",
                    actuals[1].first, "'.");
        wr.message("Variable-argument ", expectedThing, "s with type queries can only accept ",
                    passedThing, "s of a single type.");


        // TODO: we don't currently have all actuals available here, so
        // we can't print nice underlines showing the incompatible actuals.

      } else if (badPass.formalType().isUnknown()) {
        // The formal type can be unknown in an initial instantiation if it
        // depends on the previous formals' types. In that case, don't print it
        // and say something nicer.
        wr.message("The instantiated type of ", expectedThing, " ", formalName,
                   " does not allow ", passedThing, "s of type '", badPass.actualType().type(), "'.");
      } else if (badPass.actualType().isUnknown() &&
                 offendingActual &&
                 !offendingActual->initExpression() &&
                 !offendingActual->typeExpression()) {
        auto formalKind = badPass.formalType().kind();
        auto actualName = "'" + actualExpr->toIdentifier()->name().str() + "'";
        wr.note(offendingActual->id(), "The actual ", actualName,
                   " expects to be split-initialized because it is declared without a type or initialization expression here:");
        wr.codeForDef(offendingActual);
        wr.note(actualExpr, "The call to '", ci.name() ,"' occurs before any valid initialization points:");
        wr.code(actualExpr, { actualExpr });
        actualPrinted = true;
        wr.message("The call to '", ci.name(), "' cannot initialize ",
                   actualName,
                   " because only 'out' formals can be used to split-initialize. However, ",
                   actualName, " is passed to formal ", formalName, " which has intent '", formalKind, "'.");

      } else {
        wr.message("The ", expectedThing, " ", formalName, " expects ", badPass.formalType(),
                   ", but the ", passedThing, " was ", badPass.actualType(), ".");
      }

      if (!actualPrinted && actualExpr) {
        wr.code(actualExpr, { actualExpr });
      }
      auto formalReason = candidate.formalReason();
      if (formalReason == resolution::FAIL_INCOMPATIBLE_NILABILITY) {
        auto formalDec = badPass.formalType().type()->toClassType()->decorator();
        auto actualDec = badPass.actualType().type()->toClassType()->decorator();

        wr.message("The ", expectedThing, " expects a ", nilabilityStr(formalDec), " class, "
                   "but the ", passedThing, " is ", nilabilityStr(actualDec), ".");
      } else if (formalReason == resolution::FAIL_INCOMPATIBLE_MGR) {
        auto formalMgr = badPass.formalType().type()->toClassType()->manager();
        auto actualMgr = badPass.actualType().type()->toClassType()->manager();

        wr.message("A class with '", actualMgr, "' management cannot be passed to ", expectedThingArticle, " ", expectedThing, " with '", formalMgr, "' management.");
      } else if (formalReason == resolution::FAIL_EXPECTED_SUBTYPE) {
        wr.message("Formals with kind '", badPass.formalType().kind(),
                   "' expect the ", passedThing, " to be a subtype, but '", badPass.actualType().type(),
                   "' is not a subtype of '", badPass.formalType().type(), "'.");
      } else if (formalReason == resolution::FAIL_INCOMPATIBLE_TUPLE_SIZE) {
        auto formalTup = badPass.formalType().type()->toTupleType();
        auto actualTup = badPass.actualType().type()->toTupleType();

        wr.message("A tuple with ", actualTup->numElements(),
                   " elements cannot be passed to a tuple formal with ",
                   formalTup->numElements(), " elements.");
      } else if (formalReason == resolution::FAIL_INCOMPATIBLE_TUPLE_STAR) {
        auto formalTup = badPass.formalType().type()->toTupleType();
        auto actualTup = badPass.actualType().type()->toTupleType();

        const char* formalStr = formalTup->isStarTuple() ? "is" : "is not";
        const char* actualStr = actualTup->isStarTuple() ? "is" : "is not";

        wr.message("A ", expectedThing, " that ", formalStr, " a star tuple cannot accept ", passedThingArticle," ", passedThing," that ", actualStr, ".");
      } else if (formalReason == resolution::FAIL_NOT_EXACT_MATCH) {
        wr.message("The 'ref' intent requires the ", expectedThing, " and ", passedThing, " types to match exactly.");
      }
    } else if (reason == resolution::FAIL_FORMAL_ACTUAL_MISMATCH) {
      bool printedSpecial = false;
      if (auto fn = candidate.initialForErr()) {
        resolution::FormalActualMap fa(fn, ci);

        if (fa.failingActualIdx() != -1 && fa.failingFormalIdx() == -1) {
          auto& actual = ci.actual(fa.failingActualIdx());
          if (!actual.byName().isEmpty()) {
            wr.message("");
            wr.note(candidate.idForErr(), "the following candidate didn't match"
                    " because ", passedThing, " ", fa.failingActualIdx() + 1,
                    " was named '", actual.byName(), "', but no ", expectedThing,
                    " with that name was found.");
            printedSpecial = true;
          }
        } else if (noActuals) {
          auto numFormals = fn->numFormals() - (int) fn->isMethod();
          const char* usePlural = numFormals > 1 ? "s" : "";
          wr.message("");
          wr.note(candidate.idForErr(), "the following candidate didn't match because it expects ", numFormals, " ", passedThing, usePlural, ", but none were provided.");
          printedSpecial = true;
        }
      }

      if (!printedSpecial) {
        wr.message("");
        wr.note(candidate.idForErr(), "the following candidate didn't match ",
                "because the provided ", passedThing, "s could not be mapped to its ",
                expectedThing, "s:");
      }
      wr.code(candidate.idForErr());
    } else {
      std::string reasonStr = "";
      if (reason == resolution::FAIL_VARARG_MISMATCH) {
        reasonStr = "the number of varargs was incorrect:";
      } else if (reason == resolution::FAIL_WHERE_CLAUSE) {
        reasonStr = "the 'where' clause evaluated to 'false':";
      } else if (reason == resolution::FAIL_PARENLESS_MISMATCH) {
        if (ci.isParenless()) {
          reasonStr = "it is parenful, but the call was parenless:";
        } else {
          reasonStr = "it is parenless, but the call was parenful:";
        }
      } else if (reason == resolution::FAIL_INTERFACE_NOT_TYPE_INTENT) {
        reasonStr = "it did not return a type as required from associated type procedures:";
      }

      if (reasonStr.empty()) {
        isThisCandidateDecent = false;
        if (!printedDecentCandidate) {
          wr.message("");
          wr.note(candidate.idForErr(), "the following candidate didn't match:");
        } else {
          continue;
        }
      } else {
        wr.message("");
        wr.note(candidate.idForErr(), "the following candidate didn't match ",
                "because ", reasonStr);
      }
      wr.code(candidate.idForErr());
    }
    printedDecentCandidate |= isThisCandidateDecent;
    printCount++;
  }

  if (printCount < rejected.size()) {
    wr.message("");
    wr.note(locationOnly(anchorId), "omitting ", rejected.size() - printCount, " more candidates that didn't match.");
  }
}

void ErrorInterfaceAmbiguousFn::write(ErrorWriterBase& wr) const {
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<ID>(info_);
  auto fn = std::get<const uast::Function*>(info_);
  auto candidate = std::get<std::vector<const resolution::TypedFnSignature*>>(info_);

  wr.heading(kind_, type_, implPoint, "unable to disambiguate candidates for function '", fn->name(), "'.");
  wr.codeForDef(fn->id());
  wr.message("Required by the interface '", interface->name(), "':");
  wr.codeForDef(interface->id());
  wr.note(implPoint, "while checking the implementation point here:");
  wr.code<ID>(implPoint, { implPoint });

  unsigned int printCount = 0;
  static const unsigned int maxPrintCount = 2;
  for (auto sig : candidate) {
    if (printCount == maxPrintCount) break;
    printCount++;

    wr.message("");
    wr.note(sig->id(), "one candidate was here:");
    wr.code(sig->id());
  }
}

void ErrorInterfaceInvalidIntent::write(ErrorWriterBase& wr) const {
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<ID>(info_);
  auto fnTemplate = std::get<2>(info_);
  auto fnReal = std::get<3>(info_);

  wr.heading(kind_, type_, implPoint, "candidate for function '", fnTemplate->untyped()->name(), "' has mismatched return intent.");
  wr.codeForDef(fnTemplate->id());
  wr.message("Required by the interface '", interface->name(), "':");
  wr.codeForDef(interface->id());
  wr.note(implPoint, "while checking the implementation point here:");
  wr.code<ID>(implPoint, { implPoint });
  wr.note(fnReal->id(), "the provided candidate does not have a matching return intent:");
  wr.codeForDef(fnReal->id());
}

void ErrorInterfaceMissingAssociatedType::write(ErrorWriterBase& wr) const {
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<ID>(info_);
  auto var = std::get<const uast::Variable*>(info_);
  auto ci = std::get<resolution::CallInfo>(info_);
  auto rejected = std::get<std::vector<resolution::ApplicabilityResult>>(info_);
  auto actualDecls = std::vector<const uast::VarLikeDecl*>(rejected.size(), nullptr);
  wr.heading(kind_, type_, implPoint, "unable to find matching candidates for associated type '", var->name(), "'.");
  wr.codeForDef(var->id());
  wr.message("Required by the interface '", interface->name(), "':");
  wr.codeForDef(interface->id());
  wr.note(implPoint, "while checking the implementation point here:");
  wr.code<ID>(implPoint, { implPoint });
  wr.message("Associated types are resolved as 'type' calls on types constrained by the interface.");
  printRejectedCandidates(wr, implPoint, ci, rejected, "an", "actual", "a", "formal", [](int) -> const uast::AstNode* {
    return nullptr;
  }, actualDecls);
}

void ErrorInterfaceMissingFn::write(ErrorWriterBase& wr) const {
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<ID>(info_);
  auto fn = std::get<const resolution::TypedFnSignature*>(info_);
  auto ci = std::get<resolution::CallInfo>(info_);
  auto rejected = std::get<std::vector<resolution::ApplicabilityResult>>(info_);
  auto actualDecls = std::vector<const uast::VarLikeDecl*>(rejected.size(), nullptr);
  wr.heading(kind_, type_, implPoint, "unable to find matching candidates for function '", fn->untyped()->name(), "'.");
  wr.codeForDef(fn->id());
  wr.message("Required by the interface '", interface->name(), "':");
  wr.codeForDef(interface->id());
  wr.note(implPoint, "while checking the implementation point here:");
  wr.code<ID>(implPoint, { implPoint });
  printRejectedCandidates(wr, implPoint, ci, rejected, "a", "required formal", "a", "candidate formal", [fn](int idx) -> const uast::AstNode* {
    if (idx >= 0 && idx < fn->numFormals()) {
      return fn->untyped()->formalDecl(idx);
    }
    return nullptr;
  }, actualDecls);
}

void ErrorInterfaceMultipleImplements::write(ErrorWriterBase& wr) const {
  auto ad = std::get<const uast::AggregateDecl*>(info_);
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<2>(info_);
  auto otherImplPoint = std::get<3>(info_);

  wr.heading(kind_, type_, ad, "multiple implementations of interface '", interface->name(), "' found.");
  wr.message("While analyzing the definition of type '", ad->name(), "', defined here:");
  wr.codeForDef(ad);
  wr.note(implPoint, "the interface '", interface->name(), "' is first implemented here:");
  wr.code<ID>(implPoint, { implPoint });
  wr.note(otherImplPoint, "it is also implemented here:");
  wr.code<ID>(otherImplPoint, { otherImplPoint });
}

void ErrorInterfaceNaryInInherits::write(ErrorWriterBase& wr) const {
  auto ad = std::get<const uast::AggregateDecl*>(info_);
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<ID>(info_);

  wr.heading(kind_, type_, ad, "cannot use interface '", interface->name(), "' in inheritance expression as it is not a unary interface.");
  wr.message("While analyzing the definition of type '", ad->name(), "', defined here:");
  wr.codeForDef(ad);
  wr.note(implPoint, "found the interface '", interface->name(), "' in an inheritance list here:");
  wr.code<ID>(implPoint, { implPoint });
  wr.note(interface->id(), "However, the interface '", interface->name(), "' is defined to be a non-unary interface here:");
  wr.codeForDef(interface->id());
  wr.message("Only unary interfaces (those with a single type parameter like 'Self') can be used in inheritance expressions.");
  wr.message("To implement n-ary interfaces, consider using a standalone 'implements' statement.");
}

void ErrorInterfaceReorderedFnFormals::write(ErrorWriterBase& wr) const {
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto implPoint = std::get<ID>(info_);
  auto fnTemplate = std::get<2>(info_);
  auto fnReal = std::get<3>(info_);

  wr.heading(kind_, type_, implPoint, "candidate for function '", fnTemplate->untyped()->name(), "' does not have the same order of formal names.");
  wr.codeForDef(fnTemplate->id());
  wr.message("Required by the interface '", interface->name(), "':");
  wr.codeForDef(interface->id());
  wr.note(implPoint, "while checking the implementation point here:");
  wr.code<ID>(implPoint, { implPoint });
  wr.note(fnReal->id(), "the provided candidate defined here does not have matching formals:");
  wr.codeForDef(fnReal->id());
}

void ErrorInvalidClassCast::write(ErrorWriterBase& wr) const {
  auto primCall = std::get<const uast::PrimCall*>(info_);
  auto& type = std::get<types::QualifiedType>(info_);
  auto prim = primCall->prim();

  if (prim == uast::primtags::PRIM_TO_NILABLE_CLASS_CHECKED && !type.isType()) {
    wr.heading(kind_, type_, primCall, "cannot apply '?' operator to a non-type argument.");
    wr.code(primCall, { primCall->actual(0) });
    wr.message("The argument is ", type, ", but only types are allowed.");
    return;
  } else if (!type.isType()) {
    auto decoratorType = prim == uast::primtags::PRIM_TO_UNMANAGED_CLASS_CHECKED ?
      "unmanaged" : "borrowed";
    wr.heading(kind_, type_, primCall, "cannot use the '", decoratorType,
                "' decorator on values.");
    wr.message("The argument is ", type);
    return;
  }

  const char* primitiveConversion = nullptr;
  if (prim == uast::primtags::PRIM_TO_NILABLE_CLASS_CHECKED) {
    primitiveConversion = "to a nilable class";
  } else if (prim == uast::primtags::PRIM_TO_UNMANAGED_CLASS_CHECKED) {
    primitiveConversion = "to an unmanaged class";
  } else if (prim == uast::primtags::PRIM_TO_BORROWED_CLASS_CHECKED) {
    primitiveConversion = "to a borrowed class";
  }

  if (primitiveConversion) {
    wr.heading(kind_, type_, primCall, "unable to convert type '", type.type(),
               "' ", primitiveConversion, ".");
    wr.message("Only classes or class-like types are supported by this conversion.");
  } else {
    wr.heading(kind_, type_, primCall, "invalid use of class cast primitive.");
  }
}

void ErrorInvalidContinueBreakTarget::write(ErrorWriterBase& wr) const {
  auto stmt = std::get<const uast::AstNode*>(info_);
  auto target = std::get<ID>(info_);
  auto& qt = std::get<types::QualifiedType>(info_);

  const uast::AstNode* targetNode;
  const char* stmtType = nullptr;
  if (auto cont = stmt->toContinue()) {
    targetNode = cont->target();
    stmtType = "'continue'";
  } else if (auto brk = stmt->toBreak()) {
    targetNode = brk->target();
    stmtType = "'break'";
  } else {
    targetNode = stmt;
    stmtType = "'continue' or 'break'";
    CHPL_ASSERT(false && "invalid AST in error report");
  }
  wr.heading(kind_, type_, stmt, "invalid target for ", stmtType, " statement.");
  wr.code(stmt, { targetNode });
  wr.message("A ", stmtType, " statement can only refer to a loop. This is done by using the loop's label.");
  wr.message("However, the target is declared as ", qt, " here:");
  wr.codeForDef(target);
}

void ErrorInvalidDomainCall::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info_);
  auto actualTypes = std::get<std::vector<types::QualifiedType>>(info_);

  wr.heading(kind_, type_, fnCall, "invalid use of the 'domain' keyword.");
  wr.codeForLocation(fnCall);
  wr.message(
      "The 'domain' keyword should be used with a valid domain type "
      "expression.");

  if (fnCall->numActuals() == 0) {
    wr.message("However, 'domain' here did not have any actuals.");
  } else if (fnCall->numActuals() == 1) {
    // Could be rectangular or associative. Error if we have an actual type
    // that's wrong for both.
    auto qt = actualTypes[0];
    if (!qt.isType() && !(qt.type() && qt.type()->isIntType())) {
      wr.message("However, the first actual was ", decayToValue(qt),
                 " rather than an 'int' (for rectangular) or a type (for "
                 "associative).");
      wr.code(fnCall, {fnCall->actual(0)});
    }
  } else if (fnCall->numActuals() <= 3) {
    // Should be rectangular, must have one or more actual type(s) wrong after
    // the first.
    wr.message(
        "This 'domain' call is structured like a rectangular domain type.");
    bool erroredForIdxType = false;
    if (fnCall->numActuals() >= 2) {
      auto idxTypeQt = actualTypes[1];
      if (!idxTypeQt.isType()) {
        erroredForIdxType = true;
        wr.message("However, the second actual ('idxType') was ",
                   decayToValue(idxTypeQt), " rather than a type as required.");
        wr.code(fnCall, {fnCall->actual(1)});
      }
    }
    if (fnCall->numActuals() == 3) {
      auto stridesQt = actualTypes[2];
      wr.message((erroredForIdxType ? "Additionally" : "However"),
                 ", the third actual ('strides') was ", decayToValue(stridesQt),
                 " rather than a 'strideKind' as required.");
      wr.code(fnCall, {fnCall->actual(2)});
    }
  } else {
    wr.message("However, 'domain' here had too many actuals.");
  }
}

void ErrorInvalidImplementsActual::write(ErrorWriterBase& wr) const {
  auto impl = std::get<const uast::Implements*>(info_);
  auto actual = std::get<const uast::AstNode*>(info_);
  auto& qt = std::get<types::QualifiedType>(info_);

  wr.heading(kind_, type_, impl, "invalid use of 'implements' with an actual that is ", qt, ".");
  wr.codeForLocation(impl);
  wr.message("The actual is provided here:");
  wr.code(actual, { actual });
  wr.message("Only 'type' actuals are allowed in implementation points.");
}

void ErrorInvalidImplementsArity::write(ErrorWriterBase& wr) const {
  auto impl = std::get<const uast::Implements*>(info_);
  auto interface = std::get<const types::InterfaceType*>(info_);
  auto& actuals = std::get<std::vector<types::QualifiedType>>(info_);
  std::ignore = actuals;

  wr.heading(kind_, type_, impl, "wrong number of actuals in 'implements' statement for interface '", interface->name(), "'.");
  wr.codeForLocation(impl);
}

void ErrorInvalidImplementsInterface::write(ErrorWriterBase& wr) const {
  auto impl = std::get<const uast::Implements*>(info_);
  auto& qt = std::get<types::QualifiedType>(info_);

  wr.heading(kind_, type_, impl, "invalid 'implements' statement.");
  wr.codeForLocation(impl);
  wr.message("The statement attempts to implement ", qt, ", which is not an interface.");
}

static void printErrorInvalidDomainKeywordCall(ErrorWriterBase& wr,
                                               ErrorBase::Kind kind,
                                               ErrorType errType,
                                               const char* keyword,
                                               const uast::FnCall* fnCallLocation,
                                               const uast::FnCall* fnCallActuals,
                                               const types::QualifiedType& type) {
  wr.heading(kind, errType, fnCallLocation,
             "invalid use of the '", keyword, "' keyword.");
  wr.codeForLocation(fnCallLocation);
  wr.message("The '", keyword, "' keyword should be used with a domain: '", keyword, "(D)'.");

  if (fnCallActuals->numActuals() == 0) {
    wr.message("However, '", keyword, "' here did not have any actuals.");
  } else if (fnCallActuals->numActuals() > 1) {
    wr.message("However, '", keyword, "' here had more than one actual.");
    wr.code(fnCallLocation, { fnCallActuals->actual(1) });
  } else if (type.type() && !type.type()->isDomainType()) {
    wr.message("However, '", keyword, "' here is not called with a domain argument, but with ", decayToValue(type), ".");
    wr.code(fnCallLocation, { fnCallActuals->actual(0) });
  }

}

void ErrorInvalidIndexCall::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info_);
  auto& type = std::get<types::QualifiedType>(info_);

  printErrorInvalidDomainKeywordCall(wr, kind_, type_, "index", fnCall, fnCall, type);
}

void ErrorInvalidSubdomainCall::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info_);
  auto& type = std::get<types::QualifiedType>(info_);

  printErrorInvalidDomainKeywordCall(wr, kind_, type_, "subdomain", fnCall, fnCall, type);
}

void ErrorInvalidSparseSubdomainCall::write(ErrorWriterBase& wr) const {
  auto fnCallLocation = std::get<const uast::FnCall*>(info_);
  CHPL_ASSERT(fnCallLocation->numActuals() == 1);
  auto fnCallActuals = fnCallLocation->actual(0)->toFnCall();
  CHPL_ASSERT(fnCallActuals);
  auto& type = std::get<types::QualifiedType>(info_);

  printErrorInvalidDomainKeywordCall(wr, kind_, type_, "sparse subdomain", fnCallLocation, fnCallActuals, type);
}

void ErrorInvalidNewTarget::write(ErrorWriterBase& wr) const {
  auto newExpr = std::get<const uast::New*>(info_);
  auto type = std::get<types::QualifiedType>(info_);

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

void ErrorInvalidParamCast::write(ErrorWriterBase& wr) const {
  auto astForErr = std::get<0>(info_);
  auto& fromQt = std::get<1>(info_);
  auto& toQt = std::get<2>(info_);

  wr.heading(kind_, type_, astForErr,
             "cannot cast param value "
             "of type '", fromQt.type(), "' to type '", toQt.type(), "'.");
  wr.message("In the following expression:");
  wr.code(astForErr, { astForErr });
}

void ErrorInvalidSuper::write(ErrorWriterBase& wr) const {
  auto superExpr = std::get<const uast::Identifier*>(info_);
  auto qt = std::get<types::QualifiedType>(info_);

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

void ErrorIteratorsInOtherScopes::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto call = node->toCall();
  auto candidate = std::get<const resolution::TypedFnSignature*>(info_);
  auto& rejected = std::get<std::vector<const resolution::TypedFnSignature*>>(info_);

  UniqueString name = candidate->untyped()->name();

  wr.heading(kind_, type_, node, "found potentially compatible iterator overloads in other scopes.");
  wr.message("While resolving the call to iterator '", name ,"' here:");
  wr.codeForLocation(call);
  if (candidate) {
    wr.message("The following iterator was determined to match the given call:");
    wr.codeForLocation(candidate->id());
  }
  wr.message("However, other candidates were found in different scopes.");
  wr.message("");

  for (auto rejectedCandidate : rejected) {
    wr.note(rejectedCandidate->id(), "one candidate was found here:");
    wr.codeForLocation(rejectedCandidate->id());
  }
}

void ErrorLoopLabelOutsideBreakOrContinue::write(ErrorWriterBase& wr) const {
  auto label = std::get<const uast::AstNode*>(info_);
  auto loop = std::get<ID>(info_);

  wr.heading(kind_, type_, label, "invalid reference to loop label outside of a 'break' or 'continue' statement.");
  wr.message("Loop labels can only be referenced in 'break' or 'continue' statements.");
  wr.message("However, the expression here references a loop label in another context:");
  wr.code(label, { label });
  wr.message("The expression in question refers to a labeled loop declared here:");
  wr.codeForLocation(loop);
}

void ErrorMemManagementNonClass::write(ErrorWriterBase& wr) const {
  auto newCall = std::get<const uast::New*>(info_);
  auto type = std::get<const types::Type*>(info_);
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
  auto moduleInclude = std::get<const uast::Include*>(info_);
  auto& filePath = std::get<std::string>(info_);
  wr.heading(kind_, type_, moduleInclude, "cannot find included submodule");
  wr.code(moduleInclude);
  wr.note(moduleInclude, "expected file at path '", filePath, "'");
}

void ErrorMissingFormalInstantiation::write(ErrorWriterBase& wr) const {
  auto call = std::get<0>(info_);
  auto& genericFormals = std::get<1>(info_);

  wr.heading(kind_, type_, call, "call does not provide enough type information for a complete instantiation.");
  wr.code(call, {call});
  for (auto& formal : genericFormals) {
    auto decl = std::get<0>(formal);
    auto qt = std::get<1>(formal);

    std::string formalName = "formal";
    if (auto fieldDecl = decl->toVariable(); fieldDecl && fieldDecl->isField()) {
      formalName = std::string("field '") + fieldDecl->name().c_str() + "'";
    } else if (auto formalDecl = decl->toNamedDecl()) {
      formalName = formalName + " '" + formalDecl->name().c_str() + "'";
    }

    wr.note(decl, formalName, " has generic type '", qt.type(), "', but is expected to have a concrete type.");
    wr.codeForDef(decl);

    if (qt.type()) {
      if (auto ct = qt.type()->toClassType()) {
        if (ct->decorator().isUnknownManagement() && ct->basicClassType()) {
          wr.note(decl, "one reason that ", formalName, " is generic is that it doesn't have a specified memory management strategy like 'owned', 'shared' or 'unmanaged'.");
          wr.message("Consider explicitly specifying a memory management strategy, or adding a new type parameter to explicitly make the formal generic.");
        }
      }
    }

    wr.message("");
  }
}

void ErrorMismatchedInitializerResult::write(ErrorWriterBase& wr) const {
  auto node = std::get<0>(info_);
  auto initialCT = std::get<1>(info_);
  auto finalCT = std::get<2>(info_);
  auto& mismatches = std::get<3>(info_);

  wr.heading(kind_, type_, node, "final result of initialization does not match the initial type.");
  wr.message("In the following initialization: ");
  wr.code(justOneLine(node), { node });
  wr.message("The initializer was invoked with an instantiated type '", initialCT, "'.");
  wr.message("However, the initializer resulted in a value of type '", finalCT, "'.");

  for (auto& mismatch : mismatches) {
    auto& id = std::get<0>(mismatch);
    auto fieldName = std::get<1>(mismatch);
    auto& expectedType = std::get<2>(mismatch);
    auto& actualType = std::get<3>(mismatch);

    wr.message("");
    wr.note(id, "field '", fieldName, "' started with type '", expectedType.type(),
            "', but had incompatible type '", actualType.type(), "' "
            "after initialization.");
    wr.codeForDef(id);
  }
}

void ErrorModuleAsVariable::write(ErrorWriterBase& wr) const {
  auto node = std::get<0>(info_);
  auto parent = std::get<1>(info_);
  auto mod = std::get<const uast::Module*>(info_);
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
  auto enumAst = std::get<const uast::Enum*>(info_);
  auto elemName = std::get<UniqueString>(info_);
  auto& possibleElems = std::get<std::vector<ID>>(info_);

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

void ErrorMultipleInheritance::write(ErrorWriterBase& wr) const {
  auto theClass = std::get<const uast::Class*>(info_);
  auto firstParent = std::get<1>(info_);
  auto secondParent = std::get<2>(info_);

  wr.heading(kind_, type_, theClass,
             "invalid use of multiple inheritance in class '", theClass->name(),
             "'; only single inheritance is supported.");
  wr.message("The first class being inherited from is here:");
  wr.code(justOneLine(firstParent), { firstParent });
  wr.message("The second class being inherited from is here:");
  wr.code(justOneLine(secondParent), { secondParent });
  wr.message("Although a class can implement multiple interfaces, it cannot ",
             "inherit from multiple parent classes.");
}

void ErrorMultipleQuestionArgs::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::FnCall*>(info_);
  auto firstQuestion = std::get<1>(info_);
  auto secondQuestion = std::get<2>(info_);

  wr.heading(kind_, type_, call, "cannot have '?' more than once in a call");
  wr.message("The first ? argument occurs here:");
  wr.code(firstQuestion, { firstQuestion });
  wr.message("The second ? argument occurs here:");
  wr.code(secondQuestion, { secondQuestion });
}

void ErrorNestedClassFieldRef::write(ErrorWriterBase& wr) const {
  auto outerDecl = std::get<0>(info_);
  auto innerDecl = std::get<1>(info_);
  auto reference = std::get<2>(info_);
  auto id = std::get<3>(info_);

  auto getType = [](const uast::TypeDecl* typeDecl) {
    if (typeDecl->isEnum()) {
      return "enum";
    } else if (typeDecl->isClass()) {
      return "class";
    } else {
      return "record";
    }
  };

  auto getName = [](const uast::TypeDecl* typeDecl) {
    if (auto enumDecl = typeDecl->toEnum()) return enumDecl->name();
    CHPL_ASSERT(typeDecl->isAggregateDecl());
    return typeDecl->toAggregateDecl()->name();
  };

  const char* outerName = getType(outerDecl);
  const char* innerName = getType(innerDecl);
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
          getName(innerDecl), "', declared here:");
  wr.codeForLocation(innerDecl);
  wr.note(outerDecl, "however, the identifier refers to a field of an enclosing ",
          outerName, " '", getName(outerDecl), "', declared here:");
  wr.codeForLocation(outerDecl);
  wr.note(id, "field declared here:");
  wr.codeForDef(id);
}

void ErrorNoMatchingCandidates::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto call = node->toCall();
  auto& ci = std::get<resolution::CallInfo>(info_);
  auto& rejected = std::get<std::vector<resolution::ApplicabilityResult>>(info_);
  auto& actualDecls = std::get<std::vector<const uast::VarLikeDecl*>>(info_);

  wr.heading(kind_, type_, node, "unable to resolve call to '", ci.name(), "': no matching candidates.");
  wr.code(node);

  printRejectedCandidates(wr, node->id(), ci, rejected, "an", "actual", "a", "formal", [call](int idx) -> const uast::AstNode* {
    if (call && 0 <= idx && idx < call->numActuals()) {
      return call->actual(idx);
    }
    return nullptr;
  }, actualDecls);
}

void ErrorNoMatchingSuper::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto& ci = std::get<resolution::CallInfo>(info_);
  auto& rejected = std::get<std::vector<resolution::ApplicabilityResult>>(info_);
  auto& actualDecls = std::get<std::vector<const uast::VarLikeDecl*>>(info_);
  auto& superUses = std::get<std::vector<std::pair<ID, ID>>>(info_);

  wr.heading(kind_, type_, node, "failed to resolve implicit call to 'super.init': no matching candidates.");
  wr.code(node);
  wr.message("The call to 'super.init' was triggered by the use of fields from the parent type.");

  for (auto& superUse : superUses) {
    wr.note(superUse.first, "the field defined in a parent class here...");
    wr.codeForLocation(superUse.first);
    wr.note(superUse.second, "...was used in the child initializer here:");
    wr.codeForLocation(superUse.second);
  }

  wr.message("In order to use fields from a parent class, the parent's initializer must have been called.");
  wr.message("Chapel attempts to automatically call a zero-argument initializer on the parent class if an explicit call is not present.");

  printRejectedCandidates(wr, node->id(), ci, rejected, "an", "actual", "a", "formal", [](int idx) -> const uast::AstNode* {
    return nullptr;
  }, actualDecls);

  wr.message("If the parent type's initializer has formals, consider invoking it explicitly.");
}

void ErrorNonClassInheritance::write(ErrorWriterBase& wr) const {
  auto ad = std::get<const uast::AggregateDecl*>(info_);
  auto inheritanceExpr = std::get<const uast::AstNode*>(info_);
  auto& type = std::get<const types::Type*>(info_);

  wr.heading(kind_, type_, ad, "attempt for non-class type to inherit from a type.");
  wr.message("While analyzing the definition of type '", ad->name(), "', defined here:");
  wr.codeForDef(ad);
  wr.note(inheritanceExpr, "found an inheritance expression referring to type '", type, "' here:");
  wr.code(inheritanceExpr, { inheritanceExpr });
}

static void printTheseResults(
    ErrorWriterBase &wr,
    uast::Function::IteratorKind iterKind,
    const char* iterGroupStr,
    const char* iterKindStr,
    const resolution::TheseResolutionResult& tr) {
  using namespace resolution;

  bool firstLine = true;
  auto currentTr = &tr;

  int depth = 1; // print one nested zippering warning
  while (currentTr) {
    std::string failureReason = "";
    std::string start = "...because ";
    if (firstLine) {
      start = std::string("The ") +
              iterGroupStr +
              " version of the iterator could not be used because ";
    }
    const char* end = currentTr->zipperedFailure() && depth > 0 ? "..." : ".";

    auto reason = currentTr->reason();
    if (reason == TheseResolutionResult::THESE_NOT_ATTEMPTED) {
      bool isSerial = iterKind == uast::Function::SERIAL;
      wr.message(start, "this loop does not support ", isSerial ? "serial" : "parallel", " iterators", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_NO_LOOP_EXPR_STANDALONE) {
      wr.message(start, "loop expressions with more than one zippered iterand do not have standalone iterators", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_NO_PROMO_STANDALONE) {
      wr.message(start, "promoted expressions with more than one zippered iterand do not have standalone iterators", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_SERIAL_LOOP_EXPR) {
      wr.message(start, "the loop expression iterated over is explicitly serial", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_NO_ITERATOR_WITH_TAG) {
      wr.message(start, "an iterator of that kind was not defined for the type", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_LEADER_FOLLOWER_MISMATCH) {
      wr.message(start, "the yield type of the leader iterator does not match the type of the 'followThis' argument", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_ZIPPERED_ARG_FAILED) {
      auto iterandType = currentTr->iterandType().type();
      const char* combinedKind =
        iterandType && iterandType->isPromotionIteratorType() ?
        "promoted argument" :
        "zippered argument";
      wr.message(start, "one of the ", combinedKind, "s (", combinedKind ," number ",
                        currentTr->zipperedFailureIndex() + 1,
                        ", ", currentTr->zipperedFailure()->iterandType(),
                        ") does not support a ", iterKindStr, " iterator", end);
    } else if (reason == TheseResolutionResult::THESE_FAIL_PROMOTION_TYPE_YIELD_MISMATCH) {
      wr.message(start, "the claimed scalar type for promotion does not match the type yielded from the iterator", end);
    }

    depth--;
    firstLine = false;
    currentTr = currentTr->zipperedFailure();
  }
}

void ErrorNonIterable::write(ErrorWriterBase &wr) const {
  auto loop = std::get<0>(info_);
  auto iterand = std::get<1>(info_);
  auto& iterandType = std::get<types::QualifiedType>(info_);
  auto& resolutionResults = std::get<3>(info_);
  if (iterandType.isUnknown()) {
    wr.heading(kind_, type_, loop, "cannot iterate over this loop's iterand.");
  } else {
    wr.heading(kind_, type_, loop, "cannot iterate over ", decayToValue(iterandType), ".");
    wr.message("Used as an iterand in a loop here:");
  }
  wr.code(iterand, { iterand });

  bool failedLeader = false;
  for (auto& rr : resolutionResults) {
    auto iterKind = std::get<0>(rr);
    auto& tr = std::get<1>(rr);
    if (iterKind == uast::Function::LEADER) {
      failedLeader = true;
    } else if (iterKind == uast::Function::FOLLOWER &&
               tr.reason() == resolution::TheseResolutionResult::THESE_NOT_ATTEMPTED &&
               failedLeader) {
      // We didn't match a leader, so we didn't try the follower. Nothing to report.
      continue;
    }

    // The iter kind str will be "serial", "leader", "follower", to represent
    // the exact kind of iterator we failed to resolve.
    auto iterKindStr = uast::Function::iteratorKindToString(iterKind);

    // The iter group str will be "serial", "leader/follower" etc., to
    // represent the broad class of iterators we failed to resolve.
    auto iterGroupStr = iterKindStr;
    if (iterKind == uast::Function::LEADER ||
        iterKind == uast::Function::FOLLOWER) {
      iterGroupStr = "leader/follower";
    }

    wr.message("");
    printTheseResults(wr, iterKind, iterGroupStr, iterKindStr, tr);
  }
}

void ErrorNoMatchingEnumValue::write(ErrorWriterBase& wr) const {
  auto location = std::get<const uast::AstNode*>(info_);
  auto& enumType = std::get<const types::EnumType*>(info_);
  auto& numericValue = std::get<types::QualifiedType>(info_);

  wr.heading(kind_, type_, location, "the value '", numericValue.param(),
             "' of type '", numericValue.type(),
             "' is not associated with any constant of the enumeration '",
             enumType->name(), "'.");
  wr.codeForLocation(location);
  wr.message("Only values that were associated with an enumeration's constant "
             "can be converted back to the enumeration type.");
}

void ErrorNotInModule::write(ErrorWriterBase& wr) const {
  const uast::Dot* dot = std::get<0>(info_);
  //ID moduleId = std::get<1>(info_);
  UniqueString moduleName = std::get<2>(info_);
  ID renameClauseId = std::get<3>(info_);
  bool thereButPrivate = std::get<bool>(info_);

  if (thereButPrivate) {
    wr.heading(kind_, type_, dot,
               "cannot access '", dot->field(), "' as it is private to '", moduleName, "'.");
  } else {
    wr.heading(kind_, type_, dot,
               "cannot find '", dot->field(), "' in module '", moduleName, "'.");
  }

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
              " '", dotModName, "' here:");
      wr.code<ID>(renameClauseId, { renameClauseId });
    }
  }

  //wr.note(moduleId, "module '", moduleName, "' declared here");

  return;
}

void ErrorNoTypeForEnumElem::write(ErrorWriterBase& wr) const {
  auto enumAst = std::get<const uast::Enum*>(info_);
  auto signedElt = std::get<1>(info_);
  auto signedQt = std::get<2>(info_);
  auto unsignedElt = std::get<3>(info_);
  auto unsignedQt = std::get<4>(info_);

  wr.heading(kind_, type_, enumAst, "cannot pick single numeric type to represent the elements of enum '", enumAst->name(), "'");
  wr.note(signedElt, "the constant '", signedElt->name(), "' is associated with ", signedQt.param(), ", ", signedQt, ", which requires a signed integer type.");
  wr.codeForLocation(signedElt);
  wr.note(unsignedElt, "however, the constant '", unsignedElt->name(), "' is associated with ", unsignedQt.param(), ", ", unsignedQt, ", which requires an unsigned integer type.");
  wr.codeForLocation(unsignedElt);
}

void ErrorPhaseTwoInitMarker::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto& others = std::get<std::vector<ID>>(info_);

  const char* markerType = node->isInit() ? "init this" : "this.complete()";
  wr.heading(kind_, type_, node,
             "use of '", markerType, "' after type has been initialized.");
  wr.code(node, { node });

  auto previousMarker = others.at(0);
  wr.note(previousMarker, "the type was previously marked as initialized here:");
  wr.code<ID>(previousMarker, { previousMarker });
}

void ErrorPotentiallySurprisingShadowing::write(ErrorWriterBase& wr) const {
  auto id = std::get<0>(info_);
  auto name = std::get<1>(info_);
  auto& result = std::get<2>(info_);
  auto& traceResult = std::get<3>(info_);
  auto& shadowed = std::get<4>(info_);
  auto& traceShadowed = std::get<5>(info_);

  wr.heading(kind_, type_, id,
             "potentially surprising shadowing for '", name.c_str(), "'");
  wr.code<ID,ID>(id, { id });
  // only print out two matches
  if (result.numIds() > 0 && shadowed.numIds() > 0) {
    const char* intro = "it refers to a symbol found ";
    bool encounteredAutoModule = false;
    UniqueString from;
    bool needsIntroText = true;

    ID firstId = result.firstId();

    describeSymbolTrace(wr, id, name,
                        traceResult[0], /* start */ 0, intro,
                        encounteredAutoModule, from, needsIntroText);

    if (needsIntroText) {
      wr.note(locationOnly(firstId), "it refers to the symbol '", from, "' defined here:");
    } else {
      wr.note(locationOnly(firstId), "leading to '", from, "' defined here:");
    }
    wr.code<ID,ID>(firstId, { firstId });

    const char* intro2 = "but, there is a shadowed symbol found ";
    describeSymbolTrace(wr, id, name,
                        traceShadowed[0], /* start */ 0, intro2,
                        encounteredAutoModule, from, needsIntroText);

    ID otherId = shadowed.firstId();
    if (needsIntroText) {
      wr.note(locationOnly(otherId), "but, there is a shadowed symbol '", from, "' defined here:");
    } else {
      wr.note(locationOnly(otherId), "leading to '", from, "' defined here:");
    }
    wr.code<ID,ID>(otherId, { otherId });
  }
}

void ErrorPrivateToPublicInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info_);
  auto moduleDef = std::get<const uast::Module*>(info_);
  wr.heading(kind_, type_, moduleInclude,
             "cannot make a private module public through "
             "an include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "module declared private here:");
  wr.codeForLocation(moduleDef);
}

void ErrorProcDefExplicitAnonFormal::write(ErrorWriterBase& wr) const {
  auto fn = std::get<const uast::Function*>(info_);
  auto formal = std::get<const uast::Formal*>(info_);
  wr.heading(kind_, type_, formal, "formals in a procedure definition must "
                            "be named");
  wr.code(fn, {formal});
}

void ErrorProcTypeUnannotatedFormal::write(ErrorWriterBase& wr) const {
  auto sig = std::get<const uast::FunctionSignature*>(info_);
  auto formal = std::get<const uast::AnonFormal*>(info_);
  wr.heading(kind_, type_, formal, "unannotated formal is ambiguous in this "
                            "context");
  wr.code(sig, {formal});
  wr.message("The meaning of an unannotated formal (a formal not of the "
             "form '<name>:<type>') in a procedure type is ambiguous. "
             "It is currently undecided whether it represents a formal's "
             "type or name.");
}

void ErrorPrototypeInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info_);
  auto moduleDef = std::get<const uast::Module*>(info_);
  wr.heading(kind_, type_, moduleInclude,
             "cannot apply prototype to module in include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "put prototype keyword at module declaration here:");
  wr.codeForLocation(moduleDef);
}

// find the first ID not from use/import, returns true and sets result
// if one is found.
static bool firstIdFromDecls(
    const resolution::MatchingIdsWithName& matches,
    const std::vector<resolution::ResultVisibilityTrace>& trace,
    ID& result) {
  int n = matches.numIds();
  for (int i = 0; i < n; i++) {
    const auto& t = trace[i];
    if (t.visibleThrough.size() == 0) {
      // TODO: find the first non-function ID?
      // To do that, would use flags in MatchingIdsWithName
      // to filter Ids.
      result = matches.id(i);
      return true;
    }
  }
  return false;
}

void ErrorRecursion::write(ErrorWriterBase& wr) const {
  auto queryName = std::get<UniqueString>(info_);
  wr.heading(kind_, type_, ID(),
             "recursion detected in query '", queryName.c_str(), "'");
}

static const char* recursionMessage =
  "Recursion errors during resolution can sometimes be addressed by "
  "explicitly specifying variable and field types instead of "
  "relying on type inference.";

template <typename Loc>
static bool printRecursionTrace(ErrorWriterBase& wr,
                                const std::string& rootGoal,
                                const Loc& rootLocation,
                                const std::vector<TraceElement>& trace) {
  if (trace.size() == 0) return false;
  std::string prefix = rootGoal + " led to recursion because doing so required";
  for (size_t idx = 0; idx < trace.size(); idx++) {
    auto& te = trace[idx];
    wr.note(te.first, prefix, " ", te.second, ":");
    wr.codeForLocation(te.first);

    prefix = "which required";
  }

  return true;
}

void ErrorRecursionFieldDecl::write(ErrorWriterBase& wr) const {
  auto ast = std::get<const uast::AstNode*>(info_);
  auto ad = std::get<const uast::AggregateDecl*>(info_);
  auto ct = std::get<const types::CompositeType*>(info_);
  auto& trace = std::get<3>(info_);

  std::string rootGoalForTrace = "resolving the field";
  if (auto vld = ast->toVarLikeDecl()) {
    wr.heading(kind_, type_, ast, "encountered recursion while resolving field '",
               vld->name(),"' of type '", ad->name(),"':");
    rootGoalForTrace += std::string(" '") + vld->name().c_str() + "'";
  } else {
    wr.heading(kind_, type_, ast, "encountered recursion while resolving a field"
               "of type '", ad->name(),"':");
  }
  wr.codeForLocation(ast);
  if (ct->instantiatedFromCompositeType() != nullptr) {
    wr.note(ad, "the type '", ad->name(), "' was instantiated as '", ct, "'.");
  }

  if (printRecursionTrace(wr, rootGoalForTrace, ast, trace)) {
    wr.message("");
  }
  wr.message(recursionMessage);
}

void ErrorRecursionModuleStmt::write(ErrorWriterBase& wr) const {
  auto ast = std::get<const uast::AstNode*>(info_);
  auto& trace = std::get<2>(info_);

  wr.heading(kind_, type_, ast,
             "encountered recursion while resolving module statement:");
  wr.codeForLocation(ast);

  auto rootGoalForTrace = "resolving the module statement";
  if (printRecursionTrace(wr, rootGoalForTrace, ast, trace)) {
    wr.message("");
  }
  wr.message(recursionMessage);
}

void ErrorRedefinition::write(ErrorWriterBase& wr) const {
  auto scopeId = std::get<ID>(info_);
  auto name = std::get<UniqueString>(info_);
  auto& matches = std::get<resolution::MatchingIdsWithName>(info_);
  auto& trace = std::get<std::vector<resolution::ResultVisibilityTrace>>(info_);

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

  int n = matches.numIds();
  int lastTracePrinted = -1;
  for (int i = 0; i < n; i++) {
    bool encounteredAutoModule = false;
    UniqueString from;
    bool needsIntroText = false;
    std::string intro;
    if (i == 0) {
      intro = "it was first defined ";
    } else {
      intro = "redefined ";
    }
    if (i == 0 || trace[i] != trace[i-1]) {
      needsIntroText = true;
      describeSymbolTrace(wr, scopeId, name, trace[i], 0, intro.c_str(),
                          encounteredAutoModule, from, needsIntroText);
      if (!needsIntroText) {
        lastTracePrinted = i;
      }
    }

    // print out the other IDs
    const ID& matchId = matches.id(i);
    if (needsIntroText) {
      if (anchorToDef) {
        wr.message("It was first defined here:");
      } else {
        wr.note(locationOnly(matchId), intro + "here:");
      }
      needsIntroText = false;
    } else {
      if (lastTracePrinted != -1) {
        if (lastTracePrinted == i) {
          wr.note(locationOnly(matchId), "leading to the definition here:");
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

void ErrorReductionAssignNonIdentifier::write(ErrorWriterBase& wr) const {
  auto opCall = std::get<const uast::OpCall*>(info_);
  wr.heading(kind_, type_, opCall, "invalid reduction assignment to non-identifier:");
  wr.message("The left-hand side of a 'reduce=' statement must be an identifier referring to a variable with a 'reduce' intent.");
  wr.message("However, the left-hand side is not an identifier here:");
  wr.code(opCall, { opCall->actual(0) });
}

void ErrorReductionAssignNotReduceIntent::write(ErrorWriterBase& wr) const {
  auto opCall = std::get<const uast::OpCall*>(info_);
  auto lhs = std::get<const uast::AstNode*>(info_);
  wr.heading(kind_, type_, opCall, "invalid reduction assignment to variable declared outside of 'reduce' intent:");
  wr.codeForLocation(opCall);
  wr.message("The left-hand side of a 'reduce=' statement must be an identifier referring to a variable with a 'reduce' intent.");
  wr.message("However, the left-hand side is declared (outside a 'reduce' intent) here:");
  wr.codeForDef(lhs);
  wr.message("The 'reduce=' operator uses the 'reduce' intent to determine the reduction operation.");
}

void ErrorReductionAssignInvalidRhs::write(ErrorWriterBase& wr) const {
  auto opCall = std::get<const uast::OpCall*>(info_);
  auto reduceIntent = std::get<const uast::ReduceIntent*>(info_);
  auto shadowed = std::get<const uast::AstNode*>(info_);
  auto& ci = std::get<resolution::CallInfo>(info_);
  wr.heading(kind_, type_, opCall, "invalid reduction assignment:");
  wr.codeForLocation(opCall);
  wr.message("The right-hand side of the 'reduce=' statement refers to a variable '",
              reduceIntent->name(), "' declared '", reduceIntent->op(), " reduce' here:");
  wr.codeForDef(reduceIntent);
  wr.message("This shadows the declaration of '", reduceIntent->name(), "' with type '", ci.actual(1).type().type(), "' here:");
  wr.codeForDef(shadowed);
  wr.message("However, the reduction '", reduceIntent->op(), "' does not support an assignment from ", decayToValue(ci.actual(2).type()), ".");
  wr.code(opCall, { opCall->actual(1) });
}

void ErrorReductionIntentChangesType::write(ErrorWriterBase& wr) const {
  auto reduceIntent = std::get<const uast::ReduceIntent*>(info_);
  auto shadowed = std::get<const uast::AstNode*>(info_);
  auto& oldType = std::get<2>(info_);
  auto& newType = std::get<3>(info_);
  wr.heading(kind_, type_, reduceIntent, "cannot use non-closed reduction in intent:");
  wr.codeForLocation(reduceIntent);
  wr.message("A reduction is closed if it produces values of the same type as the values it operates on.");
  wr.message("The reduce intent shadows the declaration of '", reduceIntent->name(), "' with type '", oldType.type(), "' here:");
  wr.codeForDef(shadowed);
  wr.message("However, the reduction '", reduceIntent->op(), "' produces values of type '", newType.type(), "'.");
  wr.message("The reduced type of a variable cannot change, as the final reduced value is assigned back into the variable.");
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
  auto reduceOrScan = std::get<const uast::AstNode*>(info_);
  auto name = std::get<UniqueString>(info_);
  auto& iterType = std::get<types::QualifiedType>(info_);

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
  auto reduceOrScan = std::get<const uast::AstNode*>(info_);
  auto actualType = std::get<types::QualifiedType>(info_);
  const types::BasicClassType* actualClassType = nullptr;

  auto op = getReduceOrScanOp(reduceOrScan);
  wr.heading(kind_, type_, op, "invalid operation in 'reduce' expression.");
  wr.code(reduceOrScan, { op });

  // Don't print the details of managed / unmanaged / etc.
  if (auto classType = actualType.type()->toClassType()) {
    if (auto actualClassType = classType->basicClassType()) {
      while (auto instFrom = actualClassType->instantiatedFrom()) {
        actualClassType = instFrom;
      }
      actualType = types::QualifiedType(actualType.kind(), actualClassType);
    }
  }
  wr.message("The operation must be a type extending 'ReduceScanOp', but "
             "it is ", actualType);
  if (actualClassType) {
    wr.message("Did you mean for class '", actualClassType,
        "' to extend 'ReduceScanOp'?");
  }
}

void ErrorSelfDefinition::write(ErrorWriterBase& wr) const {
  const uast::NamedDecl* decl = std::get<0>(info_);
  const uast::Identifier* ident = std::get<1>(info_);
  auto name = ident->name();
  wr.heading(kind_, type_, decl,
            "statement uses variable '", name, "' to define itself.");
  wr.message("The following statement uses the variable '", name, "' to define itself:");
  wr.code(decl);
  wr.note(ident, "the variable '", name, "' is used to define itself here:");
  wr.codeForDef(ident);
  wr.message("Variables cannot be used to define themselves.");
}

void ErrorSplitInitMismatchedConditionalTypes::write(
    ErrorWriterBase& wr) const {
  const uast::Variable* var = std::get<0>(info_);
  const uast::AstNode* node = std::get<1>(info_);
  const types::QualifiedType thenType = std::get<2>(info_);
  const types::QualifiedType elseType = std::get<3>(info_);
  const int idx1 = std::get<4>(info_);
  const int idx2 = std::get<5>(info_);

  if (node->isConditional()) {
    const uast::Conditional* cond = node->toConditional();
    wr.heading(kind_, type_, var,
             "mismatched types for split-initialization of '", var->name(),
             "' in conditional branches.");
    wr.note(cond->thenBlock(), "initialized with ", thenType,
            " in 'then' branch");
    wr.note(cond->elseBlock(), "initialized with ", elseType,
            " in 'else' branch");
    wr.message(
        "Types of different initialization parts for split-initialized "
        "declarations must exactly match");
  } else {
    const uast::Select* sel = node->toSelect();
    const uast::When* branch1 = sel->whenStmt(idx1);
    const uast::When* branch2 = sel->whenStmt(idx2);
    wr.heading(kind_, type_, var,
             "mismatched types for split-initialization of '", var->name(),
             "' in select branches.");

    wr.note(branch1, "Initialized with ", thenType,
            " in one branch");
    wr.note(branch2, "Initialized with ", elseType,
            " in another branch");
    wr.message(
        "Types of different initialization parts for split-initialized "
        "declarations must exactly match");
  }
}

void ErrorSuperFromTopLevelModule::write(ErrorWriterBase& wr) const {
  auto use = std::get<const uast::AstNode*>(info_);
  auto mod = std::get<const uast::Module*>(info_);
  auto useOrImport = std::get<resolution::VisibilityStmtKind>(info_);
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

static const char* genericityToString(types::Type::Genericity genericity) {
  switch (genericity) {
    case types::Type::GENERIC:
      return "generic";
    case types::Type::GENERIC_WITH_DEFAULTS:
      return "generic with defaults";
    case types::Type::MAYBE_GENERIC:
      return "maybe generic";
    case types::Type::CONCRETE:
      return "concrete";
    default:
      CHPL_ASSERT(false && "shouldn't happen");
      return "";
  }
}

void ErrorSyntacticGenericityMismatch::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::Decl*>(info_);
  auto actualGenericity = std::get<1>(info_);
  auto syntacticGenericity = std::get<2>(info_);
  auto& type = std::get<types::QualifiedType>(info_);

  CHPL_ASSERT(decl->isVarLikeDecl());
  auto var = decl->toVarLikeDecl();

  wr.heading(kind_, type_, decl, "field '", var->name(), "' appears to be ",
             genericityToString(syntacticGenericity), " in the program's syntax, but it has been inferred to be ",
             type, ", which is ", genericityToString(actualGenericity), ".");
  wr.codeForLocation(decl);
}

void ErrorFieldWithGenericManagement::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::Decl*>(info_);

  wr.heading(kind_, type_, decl, "field is declared with generic memory management.");
  wr.codeForLocation(decl);
  wr.note(decl, "Consider adding 'owned', 'shared', or 'borrowed'.");
  wr.note(decl, "If generic memory management is desired, use a 'type' field to store the class type.");
}

void ErrorTertiaryUseImportUnstable::write(ErrorWriterBase& wr) const {
  auto name = std::get<UniqueString>(info_);
  auto node = std::get<const uast::AstNode*>(info_);
  auto clause = std::get<const uast::VisibilityClause*>(info_);
  auto searchedScope = std::get<const resolution::Scope*>(info_);
  auto useOrImport = std::get<resolution::VisibilityStmtKind>(info_);
  auto useOrImportStr = (useOrImport == resolution::VIS_USE) ? "a 'use'"
                                                             : "an 'import'";
  wr.heading(kind_, type_, clause,
             "using a type's name ('", name, "' in this case) in ", useOrImportStr,
             " statement to access its tertiary methods is an unstable feature.");
  wr.message("In the following clause:");
  wr.code(clause, { node });
  wr.message("The type '", name, "' is not defined in '", searchedScope->name(), "'.");
}

void ErrorTupleDeclMismatchedElems::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info_);
  auto type = std::get<const types::TupleType*>(info_);
  wr.heading(kind_, type_, decl,
            "tuple size mismatch in split tuple declaration.");
  wr.code(decl);
  wr.message("The left-hand side of the declaration expects a ",
             decl->numDecls(), "-tuple, but the right-hand side is a ",
             type->numElements(), "-tuple, '", type, "'.");
}

void ErrorTupleDeclNotTuple::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info_);
  auto type = std::get<const types::Type*>(info_);
  wr.heading(kind_, type_, decl,
            "value of non-tuple type '", type, "' cannot be split using a tuple "
            "declaration.");
  wr.message("In the following tuple declaration:");
  wr.code(decl);
  wr.message("the initialization expression has type '", type, "', while it is expected "
             "to be a ", decl->numDecls(), "-tuple.");
}

void ErrorTupleDeclUnknownType::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info_);
  wr.heading(kind_, type_, decl,
             "value of unknown type cannot be split using tuple assignment.");
  wr.code(decl);
}

void ErrorTupleExpansionNamedArgs::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info_);
  auto tupleOp = std::get<const uast::OpCall*>(info_);

  wr.heading(kind_, type_, fnCall, "tuple expansion cannot be used to pass "
             "values to a non-variadic named argument.");
  wr.message("A tuple is being expanded here:");
  wr.code(fnCall, { tupleOp });
}

void ErrorTupleExpansionNonTuple::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::FnCall*>(info_);
  auto expansion = std::get<const uast::OpCall*>(info_);
  auto& type = std::get<types::QualifiedType>(info_);

  wr.heading(kind_, type_, call, "cannot apply tuple expansion to an "
             "expression of non-tuple type");
  wr.message("In the following function call:");
  wr.code(call, { expansion });
  wr.message("the expanded element has non-tuple type '", type.type(), "', "
             "but expansion can only be used on tuples.");
}

void ErrorTupleIndexOOB::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::Call*>(info_);
  auto type = std::get<const types::TupleType*>(info_);
  auto index = std::get<const int>(info_);

  wr.heading(kind_, type_, call, "tuple index ", index, " is out of bounds");
  wr.message("In the following expression:");
  wr.code(call, { call });
  wr.message("the index value is '", index, "' but the valid indices for this",
             " tuple are in the range 0..", type->numElements()-1, " (inclusive)");
}

void ErrorUnknownEnumElem::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info_);
  auto elemName = std::get<UniqueString>(info_);
  auto enumAst = std::get<const uast::Enum*>(info_);

  wr.heading(kind_, type_, node, "enum '", enumAst->name(),
             "' has no element named '", elemName, "'.");
  wr.code(node, { node });
  wr.note(enumAst->id(), "'", enumAst->name(), "' is declared here.");
  wr.codeForLocation(enumAst->id());
}

void ErrorUnknownIdentifier::write(ErrorWriterBase& wr) const {
  auto ident = std::get<const uast::Identifier*>(info_);
  auto mentionedMoreThanOnce = std::get<bool>(info_);

  wr.heading(kind_, type_, ident,
             "'", ident->name(), "' cannot be found",
             mentionedMoreThanOnce?" (first mention this function)":"");

  wr.code(ident, { ident });

  return;
}

void ErrorUnstable::write(ErrorWriterBase& wr) const {
  auto msg = std::get<std::string>(info_);
  auto mention = std::get<const uast::AstNode*>(info_);
  auto target = std::get<const uast::NamedDecl*>(info_);
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
  auto symbolName = std::get<UniqueString>(info_);
  auto firstOccurrence = std::get<1>(info_);
  auto secondOccurrence = std::get<2>(info_);

  wr.heading(kind_, type_, secondOccurrence, "'",
             symbolName, "' is multiply defined.");
  wr.message("'", symbolName, "' was first defined here:");
  wr.codeForDef(firstOccurrence);
  wr.message("Redefined here:");
  wr.codeForDef(secondOccurrence);
}

void ErrorUseImportMultiplyMentioned::write(ErrorWriterBase& wr) const {
  auto symbolName = std::get<UniqueString>(info_);
  auto firstOccurrence = std::get<1>(info_);
  auto secondOccurrence = std::get<2>(info_);

  wr.heading(kind_, type_, secondOccurrence, "'",
             symbolName, "' is repeated.");
  wr.message("'", symbolName, "' was first mentioned here:");
  wr.codeForDef(firstOccurrence);
  wr.message("Mentioned again here:");
  wr.codeForDef(secondOccurrence);
}

void ErrorUseImportNotModule::write(ErrorWriterBase& wr) const {
  auto id = std::get<const ID>(info_);
  auto moduleName = std::get<std::string>(info_);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info_);

  wr.heading(kind_, type_, id, "cannot '", useOrImport, "' symbol '", moduleName,
             "', which is not a ", allowedItem(useOrImport), ".");
  wr.message("In the following '", useOrImport, "' statement:");
  wr.code<ID>(id, { id });
  wr.message("Only ", allowedItems(useOrImport), " can be used with '",
             useOrImport, "' statements.");
}

void ErrorUseImportTransitiveRename::write(ErrorWriterBase& wr) const {
  auto from = std::get<0>(info_);
  auto middle = std::get<1>(info_);
  auto to = std::get<2>(info_);

  auto firstRename = std::get<3>(info_);
  auto secondRename = std::get<4>(info_);

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
  auto id = std::get<const ID>(info_);
  auto moduleName = std::get<2>(info_);
  auto previousPartName = std::get<3>(info_);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info_);
  auto& improperMatches = std::get<std::vector<const uast::AstNode*>>(info_);

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
  auto visibilityClause = std::get<const uast::VisibilityClause*>(info_);
  auto symbolName = std::get<std::string>(info_);
  auto searchedScope = std::get<const resolution::Scope*>(info_);
  auto useOrImport = std::get<const resolution::VisibilityStmtKind>(info_);
  auto isRename = std::get<bool>(info_);

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
  auto stmt = std::get<const uast::AstNode*>(info_);
  auto laterId = std::get<ID>(info_);
  auto name = std::get<UniqueString>(info_);
  wr.heading(kind_, type_, stmt,
             "statement references variable '", name, "' before it is defined.");
  wr.message("The following statement references the variable '", name, "', which is defined later:");
  wr.code(stmt);
  wr.note(laterId, "the variable '", name, "' is defined here:");
  wr.codeForDef(laterId);
  wr.message("Variables cannot be referenced before they are defined.");
}


void ErrorVariableWithoutInitOrType::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::AstNode*>(info_);
  auto& useOf = std::get<ID>(info_);
  auto name = std::get<UniqueString>(info_);
  wr.heading(kind_, type_, decl,
             "variable '", name, "' is declared without an initializer or type.");
  if (!useOf.isEmpty()) {
    wr.note(useOf, "cannot find initialization point to split-init this variable");
  }
  wr.codeForDef(decl);
  if (!useOf.isEmpty()) {
    wr.note(useOf, "the variable '", name, "' is used here");
    wr.codeForLocation(useOf);
  }
}

void ErrorUserDiagnosticEncounterError::write(ErrorWriterBase& wr) const {
  auto msg = std::get<UniqueString>(info_);
  auto& node = std::get<ID>(info_);
  wr.heading(kind_, type_, node, msg);
  wr.codeForLocation(node);
}

void ErrorUserDiagnosticEncounterWarning::write(ErrorWriterBase& wr) const {
  auto msg = std::get<UniqueString>(info_);
  auto& node = std::get<ID>(info_);
  wr.heading(kind_, type_, node, msg);
  wr.codeForLocation(node);
}

void ErrorUserDiagnosticEmitError::write(ErrorWriterBase& wr) const {
  auto msg = std::get<UniqueString>(info_);
  auto& node = std::get<ID>(info_);
  wr.heading(kind_, type_, node, msg);
  wr.codeForLocation(node);
}

void ErrorUserDiagnosticEmitWarning::write(ErrorWriterBase& wr) const {
  auto msg = std::get<UniqueString>(info_);
  auto& node = std::get<ID>(info_);
  wr.heading(kind_, type_, node, msg);
  wr.codeForLocation(node);
}

void ErrorValueUsedAsType::write(ErrorWriterBase& wr) const {
  auto typeExpr = std::get<const uast::AstNode*>(info_);
  auto type = std::get<types::QualifiedType>(info_);
  wr.heading(kind_, type_, typeExpr,
             "type specifier is ", type, ", but it was expected to be a type.");
  wr.message("In the following type specifier:");
  wr.code(typeExpr, { typeExpr });
  // wr.message("Did you mean to use '.type'?");
}

/* end resolution errors */

} // end namespace 'chpl'
