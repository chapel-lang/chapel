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

  wr.heading(kind_, ifExpr, "Branches of if-expression have incompatible types.");
  wr.message("In the following if-expression:");
  wr.code(ifExpr, { ifExpr->thenBlock(), ifExpr->elseBlock() });
  wr.message("The first branch has type ", qt1,
              ", while the second has type ", qt2);
}

void ErrorTupleExpansionNamedArgs::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info);
  auto tupleOp = std::get<const uast::OpCall*>(info);

  wr.heading(kind_, fnCall, "Tuple expansion cannot be used with named arguments.");
  wr.message("A tuple is being expanded here:");
  wr.code(fnCall, { tupleOp });
}

void ErrorMemManagementRecords::write(ErrorWriterBase& wr) const {
  auto newCall = std::get<const uast::New*>(info);
  auto record = std::get<const types::RecordType*>(info);

  wr.heading(kind_, newCall,
             "Cannot use memory management strategy '",
             uast::New::managementToString(newCall->management()),
             "' with record '",
             record->name(), "'.");
  wr.code(newCall, { newCall->typeExpression() });
  wr.note(record->id(), "'", record->name(), "' declared as record here");
  wr.code<ID, ID>(record->id(), {});
  wr.message("Memory management strategies can only be used with classes. "
             "Remove the '", uast::New::managementToString(newCall->management()),
             "' keyword to fix this error, " "or define '", record->name(),
             "' as a class");
}

void ErrorPrivateToPublicInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, moduleInclude,
             "Cannot make a private module public through "
             "an include statement.");
  wr.code(moduleInclude);
  wr.note(moduleDef, "Module declared private here");
  wr.code(moduleDef);
}

void ErrorPrototypeInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, moduleInclude,
             "Cannot apply prototype to module in include statement.");
  wr.code(moduleInclude);
  wr.note(moduleDef, "Put prototype keyword at module declaration here");
  wr.code(moduleDef);
}

void ErrorMissingInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto& filePath = std::get<std::string>(info);
  wr.heading(kind_, moduleInclude, "Cannot find included submodule.");
  wr.note(moduleInclude, "Expected file at path '", filePath, "'");
}

void ErrorRedefinition::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::NamedDecl*>(info);
  auto& ids = std::get<std::vector<ID>>(info);
  wr.heading(kind_, decl, "'", decl->name(), "' has multiple definitions.");
  wr.code(decl);
  for (const ID& id : ids) {
    if (id != decl->id()) {
      wr.note(id, "redefined here");
      wr.code<ID, ID>(id);
    }
  }
}

void ErrorAmbiguousConfigName::write(ErrorWriterBase& wr) const {
  auto& name = std::get<std::string>(info);
  auto variable = std::get<const uast::Variable*>(info);
  auto otherId = std::get<ID>(info);
  wr.heading(kind_, locationOnly(variable), "Ambiguous config name (", name, ").");
  wr.code(variable);
  wr.note(locationOnly(otherId), "Also defined here");
  wr.code(otherId);
  wr.note(locationOnly(otherId), "(disambiguate using -s<modulename>.", name, "...)");
}

void ErrorAmbiguousConfigSet::write(ErrorWriterBase& wr) const {
  auto& name1 = std::get<1>(info);
  auto& name2 = std::get<2>(info);
  auto variable = std::get<const uast::Variable*>(info);
  wr.heading(kind_, locationOnly(variable),
            "Config set ambiguously via '-s", name1, "' and '-s", name2, "'.");
}

void ErrorImplicitSubModule::write(ErrorWriterBase& wr) const {
  const char* stmtKind = "require', 'use', and/or 'import";
  auto mod = std::get<const uast::Module*>(info);
  auto path = std::get<UniqueString>(info);
  wr.heading(kind_, mod,
             "As written, '", mod->name(), "' is a sub-module of the "
             "module created for file '", path, "' due to the "
             "file-level '", stmtKind, "' statements.  If you meant for '",
             mod->name(), "' to be a top-level module, move the '",
             stmtKind, "' statements into its scope.");
}

void ErrorImplicitFileModule::write(ErrorWriterBase& wr) const {
  auto code = std::get<const uast::AstNode*>(info);
  auto lastModule = std::get<1>(info);
  auto implicitModule = std::get<2>(info);
  wr.heading(kind_, code,
             "This file-scope code is outside of any "
             "explicit module declarations (e.g., module ",
             lastModule->name(), ")");
  wr.message("An implicit module named '",
             implicitModule->name(), "' is therefore being "
             "introduced to contain the file's contents.");
  wr.message("The following statement is the first to be placed in the implict module");
  wr.code(code);
}

void ErrorValueUsedAsType::write(ErrorWriterBase& wr) const {
  auto typeExpr = std::get<const uast::AstNode*>(info);
  auto type = std::get<types::QualifiedType>(info);
  wr.heading(kind_, typeExpr, "Type expression produces a value while type was expected.");
  wr.message("In the following type expression:");
  wr.code(typeExpr, { typeExpr });
  wr.message("The result of evaluating the expression is a value of type '",
            type, "'");
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
    wr.heading(kind_, initExpr, "Attempt to initialize a type variable with a value.");
  } else if (typeToValue) {
    wr.heading(kind_, initExpr, "Attempt to initialize a value with a type.");
  } else if (nonParamToParam) {
    wr.heading(kind_, initExpr, "Attempt to initialize a param with a non-param.");
  }
  wr.message("In the following initialization expression:");
  wr.code(initExpr, { initExpr });
  wr.message("The result of evaluating the expression is a ", initType,
             ", but it's being assigned to something with kind '", declKind, "'");
  if (typeToValue && initType.type() && initType.type()->isCompositeType()) {
    auto compsiteType = initType.type()->toCompositeType();
    auto initIdent = initExpr->toIdentifier();
    if (initIdent && initIdent->name() == compsiteType->name()) {
      // There's no aliasing involved, the user is just using the record name.
      wr.message("If you're trying to create a new value of type '",
          initType.type()->toCompositeType()->name(), "', try writing 'new ",
          compsiteType->name(), "()' instead");
    } else {
      // They are referring to a composite type, but through an alias. Not
      // quite sure how to report this better.
      wr.message("If you're trying to create a new value of type '",
          initType.type()->toCompositeType()->name(), "', try using the 'new' keyword");
    }
  }
}

void ErrorIncompatibleTypeAndInit::write(ErrorWriterBase& wr) const {
  auto decl = std::get<0>(info);
  auto type = std::get<1>(info);
  auto init = std::get<2>(info);
  auto typeExprType = std::get<3>(info);
  auto initExprType = std::get<4>(info);

  wr.heading(kind_, decl, "Type mismatch in declared type and initialization expression");
  wr.message("In the following declaration:");
  wr.code(decl, { type, init });
  wr.message("The type expression has type '", typeExprType, "', while the "
             "initialization expression has type '", initExprType, "'");
}

void ErrorTupleDeclUnknownType::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  wr.heading(kind_, decl, "Attempt to split unknown type using split tuple assignment.");
  wr.code(decl);
}

void ErrorTupleDeclNotTuple::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::TupleDecl*>(info);
  auto type = std::get<const types::Type*>(info);
  wr.heading(kind_, decl,
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
  wr.heading(kind_, decl,
            "Tuple size mismatch in split tuple declaration.");
  wr.code(decl);
  wr.message("The left-hand side of the declaration expects a tuple with ",
             decl->numDecls(), " elements, but the right-hand side, which is a '",
             type, "' has ", type->numElements(), " elements");
}

void ErrorUseOfLaterVariable::write(ErrorWriterBase& wr) const {
  auto stmt = std::get<const uast::AstNode*>(info);
  auto laterId = std::get<ID>(info);
  wr.heading(kind_, stmt, "Statement uses a later variable, the type of which is not yet established.");
  wr.message("In the following statement:");
  wr.code(stmt);
  wr.message("There is a reference to a variable declared later:");
  wr.code(laterId);
}

void ErrorProcTypeUnannotatedFormal::write(ErrorWriterBase& wr) const {
  auto sig = std::get<const uast::FunctionSignature*>(info);
  auto formal = std::get<const uast::AnonFormal*>(info);
  wr.heading(kind_, formal, "unannotated formal is ambiguous in this "
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
  wr.heading(kind_, formal, "formals in a procedure definition must "
                            "be named");
  wr.code(fn, {formal});
}

} // end namespace 'chpl'
