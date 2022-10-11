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
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/query-impl.h"
#include "chpl/types/all-types.h"
#include <sstream>

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

//
// Below are the implementations of 'write' for each error class, which does
// the specialized work.
//

void ErrorIncompatibleIfBranches::write(ErrorWriterBase& wr) const {
  auto ifExpr = std::get<const uast::Conditional*>(info);
  auto qt1 = std::get<1>(info);
  auto qt2 = std::get<2>(info);

  wr.heading(kind_, type_, ifExpr, "Branches of if-expression have incompatible types.");
  wr.message("In the following if-expression:");
  wr.code(ifExpr, { ifExpr->thenBlock(), ifExpr->elseBlock() });
  wr.message("the first branch is a ", qt1,
              ", while the second is a ", qt2);
}

void ErrorTupleExpansionNamedArgs::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info);
  auto tupleOp = std::get<const uast::OpCall*>(info);

  wr.heading(kind_, type_, fnCall, "Tuple expansion cannot be used with named arguments.");
  wr.message("A tuple is being expanded here:");
  wr.code(fnCall, { tupleOp });
}

void ErrorMemManagementNonClass::write(ErrorWriterBase& wr) const {
  auto newCall = std::get<const uast::New*>(info);
  auto type = std::get<const types::Type*>(info);
  auto record = type ? type->toRecordType() : nullptr;

  if (record) {
    wr.heading(kind_, type_, newCall,
               "Cannot use memory management strategy '",
               uast::New::managementToString(newCall->management()),
               "' with record '",
               record->name(), "'.");
  } else {
    wr.heading(kind_, type_, newCall,
               "Cannot use memory management strategy '",
               uast::New::managementToString(newCall->management()),
               "' with non-class type '", type, "'.");
  }
  wr.code(newCall, { newCall->typeExpression() });
  wr.message("Memory management strategies can only be used with classes.");
  if (record) {
    wr.note(record->id(), "'", record->name(), "' declared as record here:");
    wr.code(record->id());
    wr.message(
               "Remove the '", uast::New::managementToString(newCall->management()),
               "' keyword to fix this error, " "or define '", record->name(),
               "' as a class");
  }
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

void ErrorPrototypeInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, type_, moduleInclude,
             "cannot apply prototype to module in include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "put prototype keyword at module declaration here:");
  wr.code(moduleDef);
}

void ErrorMissingInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto& filePath = std::get<std::string>(info);
  wr.heading(kind_, type_, moduleInclude, "cannot find included submodule");
  wr.code(moduleInclude);
  wr.note(moduleInclude, "expected file at path '", filePath, "'");
}

void ErrorRedefinition::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::NamedDecl*>(info);
  auto& ids = std::get<std::vector<ID>>(info);
  wr.heading(kind_, type_, decl, "'", decl->name(), "' has multiple definitions.");
  wr.code(decl);
  for (const ID& id : ids) {
    if (id != decl->id()) {
      wr.note(id, "redefined here:");
      wr.code<ID, ID>(id);
    }
  }
}

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

void ErrorImplicitFileModule::write(ErrorWriterBase& wr) const {
  auto code = std::get<const uast::AstNode*>(info);
  auto lastModule = std::get<1>(info);
  auto implicitModule = std::get<2>(info);
  wr.heading(kind_, type_, code, "an implicit module named '",
             implicitModule->name(), "' is being introduced to contain this "
             "file-scope code.");
  wr.message("The following is the first file-scope statement:");
  wr.code(code);
  wr.message("The implicit module '", implicitModule->name(), "' is being "
             "created because the above code is outside of any module "
             "declarations (e.g., 'module ", lastModule->name(), "').");
  wr.message("Note that all of the file's contents will be placed into the "
             "new '", implicitModule->name(), "' module.");
}

void ErrorValueUsedAsType::write(ErrorWriterBase& wr) const {
  auto typeExpr = std::get<const uast::AstNode*>(info);
  auto type = std::get<types::QualifiedType>(info);
  wr.heading(kind_, type_, typeExpr,
             "Type expression produces a ", type, " while a type was expected.");
  wr.message("In the following type expression:");
  wr.code(typeExpr, { typeExpr });
  // wr.message("Did you mean to use '.type'?");
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
               "A type variable cannot be initialized using a regular value.");
  } else if (typeToValue) {
    wr.heading(kind_, type_, initExpr,
               "A regular variable cannot be initialized with a type.");
  } else if (nonParamToParam) {
    wr.heading(kind_, type_, initExpr,
               "A 'param' cannot be initialized using a non-'param' value.");
  }
  wr.message("In the following initialization expression:");
  wr.code(initExpr, { initExpr });
  wr.message("the initialization expression is a ", initType);
  if (valueToType) {
    wr.message("If you were trying to extract the type of the expression on "
               "the left of the '=', try using '.type'?");
  } else if (typeToValue) {
    if (initType.type() && initType.type()->isCompositeType()) {
      auto compsiteType = initType.type()->toCompositeType();
      auto initIdent = initExpr->toIdentifier();
      if (initIdent && initIdent->name() == compsiteType->name()) {
        // There's no aliasing involved, the user is just using the record name.
        wr.message("If you're trying to create a new value of type '",
                   initType.type()->toCompositeType()->name(),
                   "', try writing 'new ", compsiteType->name(), "()' instead");
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

void ErrorIncompatibleTypeAndInit::write(ErrorWriterBase& wr) const {
  auto decl = std::get<0>(info);
  auto type = std::get<1>(info);
  auto init = std::get<2>(info);
  auto typeExprType = std::get<3>(info);
  auto initExprType = std::get<4>(info);

  wr.heading(kind_, type_, decl,
             "Type mismatch in declared type and initialization expression");
  wr.message("In the following declaration:");
  wr.code(decl, { type, init });
  wr.message("the type expression has type '", typeExprType, "', while the "
             "initial value has type '", initExprType, "'");
}

void ErrorTupleDeclUnknownType::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  wr.heading(kind_, type_, decl,
             "Attempt to split unknown type using split tuple assignment.");
  wr.code(decl);
}

void ErrorTupleDeclNotTuple::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  auto type = std::get<const types::Type*>(info);
  wr.heading(kind_, type_, decl,
            "Attempt to use tuple declaration to split a value of "
            "non-tuple type '", type, "'.");
  wr.message("In the following tuple declaration:");
  wr.code(decl);
  wr.message("The value being assigned has type '", type, "', while it is expected "
             "to be a ", decl->numDecls(), "-element tuple");
}

void ErrorTupleDeclMismatchedElems::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  auto type = std::get<const types::TupleType*>(info);
  wr.heading(kind_, type_, decl,
            "Tuple size mismatch in split tuple declaration.");
  wr.code(decl);
  wr.message("The left-hand side of the declaration expects a tuple with ",
             decl->numDecls(), " elements, but the right-hand side, which is a tuple '",
             type, "', has ", type->numElements(), " elements");
}

void ErrorUseOfLaterVariable::write(ErrorWriterBase& wr) const {
  auto stmt = std::get<const uast::AstNode*>(info);
  auto laterId = std::get<ID>(info);
  wr.heading(kind_, type_, stmt,
             "Statement references a variable before it is defined.");
  wr.message("In the following statement:");
  wr.code(stmt);
  wr.message("There is a reference to a variable defined later:");
  wr.code(laterId);
  wr.message("Chapel doesn't allow references to variables before they are defined.");
}

void ErrorIncompatibleRangeBounds::write(ErrorWriterBase& wr) const {
  auto range = std::get<const uast::Range*>(info);
  auto qt1 = std::get<1>(info);
  auto qt2 = std::get<2>(info);

  wr.heading(kind_, type_, range,
            "Upper and lower bounds of range expression have incompatible types.");
  wr.message("In the following if-expression:");
  wr.code(range, { range->lowerBound(), range->upperBound() });
  wr.message("the lower bound is a ", qt1, ", while the upper bound is a ", qt2);
}

void ErrorUnknownEnumElem::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto elemName = std::get<UniqueString>(info);
  auto enumType = std::get<const types::EnumType*>(info);

  wr.heading(kind_, type_, node, "The enum '", enumType->name(),
             "' has no element named '", elemName, "'.");
  wr.code(node, { node });
  wr.note(enumType->id(), "the enum '", enumType->name(), "' is declared here.");
  wr.code(enumType->id());
}

void ErrorMultipleEnumElems::write(ErrorWriterBase& wr) const {
  auto node = std::get<const uast::AstNode*>(info);
  auto enumType = std::get<const types::EnumType*>(info);
  auto elemName = std::get<UniqueString>(info);
  auto& possibleElems = std::get<std::vector<ID>>(info);

  wr.heading(kind_, type_, node, "The enum '", enumType->name(),
             "' has multiple elements named '", elemName, "'.");
  wr.code(node, { node } );
  for (auto& id : possibleElems) {
    wr.note(id, "one instance occurs here");
    wr.code<ID, ID>(id, { id });
  }
  wr.message("In Chapel, an enum should not have repeated elements of the same name.");
}

void ErrorInvalidNew::write(ErrorWriterBase& wr) const {
  auto newExpr = std::get<const uast::New*>(info);
  auto type = std::get<types::QualifiedType>(info);

  // TODO: Specialize this error to more types (e.g. enum).
  if (auto primType = type.type()->toPrimitiveType()) {
    wr.heading(kind_, type_, newExpr,
               "Invalid use of 'new' on primitive '", primType, "'");
  } else {
    wr.heading(kind_, type_, newExpr, "Invalid use of 'new' with type '",
               type.type(), "', which is neither a class nor a record.");
  }
  wr.code(newExpr, { newExpr->typeExpression() });
  wr.message("The 'new' expression can only be used with records or classes.");
}

void ErrorMultipleQuestionArgs::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::FnCall*>(info);
  auto firstQuestion = std::get<1>(info);
  auto secondQuestion = std::get<2>(info);

  wr.heading(kind_, type_, call, "cannot have ? more than once in a call");
  wr.message("The first ? argument occurs here:");
  wr.code(firstQuestion, { firstQuestion });
  wr.message("The second ? argument occurs here:");
  wr.code(secondQuestion, { secondQuestion });
}

void ErrorTupleExpansionNonTuple::write(ErrorWriterBase& wr) const {
  auto call = std::get<const uast::FnCall*>(info);
  auto expansion = std::get<const uast::OpCall*>(info);
  auto& type = std::get<types::QualifiedType>(info);

  wr.heading(kind_, type_, call, "cannot apply tuple expansion to non-tuple argument");
  wr.message("In the following function call:");
  wr.code(call, { expansion });
  wr.message("the expanded element has type '", type.type(), "' while it's ",
             "expected to be a tuple.");
}

void ErrorNonIterable::write(ErrorWriterBase &wr) const {
  auto loop = std::get<const uast::IndexableLoop*>(info);
  auto iterand = std::get<const uast::AstNode*>(info);
  auto& iterandType = std::get<types::QualifiedType>(info);
  wr.heading(kind_, type_, loop, "cannot iterate over ", iterandType);
  wr.message("In the following loop:");
  wr.code(loop, { iterand });
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

void ErrorProcDefExplicitAnonFormal::write(ErrorWriterBase& wr) const {
  auto fn = std::get<const uast::Function*>(info);
  auto formal = std::get<const uast::Formal*>(info);
  wr.heading(kind_, type_, formal, "formals in a procedure definition must "
                            "be named");
  wr.code(fn, {formal});
}

} // end namespace 'chpl'
