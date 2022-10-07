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
#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...)\
  const owned<Error##NAME>&\
  Error##NAME::getError##NAME(Context* context, std::tuple<EINFO> tuple) {\
    QUERY_BEGIN(getError##NAME, context, tuple);\
    auto result = owned<Error##NAME>(new Error##NAME(tuple));\
    return QUERY_END(result);\
  }\
\
  const Error##NAME*\
  Error##NAME::get(Context* context, std::tuple<EINFO> tuple) {\
    return Error##NAME::getError##NAME(context, std::move(tuple)).get();\
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

  wr.heading(kind_, ifExpr, "branches of if-expression have incompatible types.");
  wr.message("in the following if-expression:");
  wr.code(ifExpr, { ifExpr->thenBlock(), ifExpr->elseBlock() });
  wr.message("the first branch has type ", qt1,
              ", while the second has type ", qt2);
}

void ErrorTupleExpansionNamedArgs::write(ErrorWriterBase& wr) const {
  auto fnCall = std::get<const uast::FnCall*>(info);
  auto tupleOp = std::get<const uast::OpCall*>(info);

  wr.heading(kind_, fnCall, "tuple expansion cannot be used with named arguments.");
  wr.message("a tuple is being expanded here:");
  wr.code(fnCall, { tupleOp });
}

void ErrorMemManagementRecords::write(ErrorWriterBase& wr) const {
  auto newCall = std::get<const uast::New*>(info);
  auto record = std::get<const types::RecordType*>(info);

  wr.heading(kind_, newCall,
             "cannot use memory management strategy ",
             uast::New::managementToString(newCall->management()),
             " with record ",
             record->name());
  wr.code(newCall, { newCall->typeExpression() });
  wr.note(record->id(), "declared as record here");
  wr.code<ID, ID>(record->id(), {});
}

void ErrorPrivateToPublicInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, moduleInclude,
             "cannot make a private module public through "
             "an include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "module declared private here");
  wr.code(moduleDef);
}

void ErrorPrototypeInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto moduleDef = std::get<const uast::Module*>(info);
  wr.heading(kind_, moduleInclude,
             "cannot apply prototype to module in include statement");
  wr.code(moduleInclude);
  wr.note(moduleDef, "put prototype keyword at module declaration here");
  wr.code(moduleDef);
}

void ErrorMissingInclude::write(ErrorWriterBase& wr) const {
  auto moduleInclude = std::get<const uast::Include*>(info);
  auto& filePath = std::get<std::string>(info);
  wr.heading(kind_, moduleInclude, "cannot find included submodule");
  wr.note(moduleInclude, "expected file at path '", filePath, "'");
}

void ErrorRedefinition::write(ErrorWriterBase& wr) const {
  auto decl = std::get<const uast::NamedDecl*>(info);
  auto& ids = std::get<std::vector<ID>>(info);
  wr.heading(kind_, decl, "'", decl->name().c_str(), "' has multiple definitions");
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
  wr.heading(kind_, locationOnly(variable), "ambiguous config name (", name, ")");
  wr.code(variable);
  wr.note(locationOnly(otherId), "also defined here");
  wr.code(otherId);
  wr.note(locationOnly(otherId), "(disambiguate using -s<modulename>.", name, "...)");
}

void ErrorAmbiguousConfigSet::write(ErrorWriterBase& wr) const {
  auto& name1 = std::get<1>(info);
  auto& name2 = std::get<2>(info);
  auto variable = std::get<const uast::Variable*>(info);
  wr.heading(kind_, locationOnly(variable),
            "config set ambiguously via '-s", name1, "' and '-s", name2, "'");
}

void ErrorImplicitSubModule::write(ErrorWriterBase& wr) const {
  const char* stmtKind = "require', 'use', and/or 'import";
  auto mod = std::get<const uast::Module*>(info);
  auto path = std::get<UniqueString>(info);
  wr.heading(kind_, mod,
             "as written, '", mod->name().c_str(), "' is a sub-module of the "
             "module created for file '", path.c_str(), "' due to the "
             "file-level '", stmtKind, "' statements.  If you meant for '",
             mod->name().c_str(), "' to be a top-level module, move the '",
             stmtKind, "' statements into its scope.");
}

void ErrorImplicitFileModule::write(ErrorWriterBase& wr) const {
  auto code = std::get<const uast::AstNode*>(info);
  auto lastModule = std::get<1>(info);
  auto implicitModule = std::get<2>(info);
  wr.heading(kind_, code,
             "This file-scope code is outside of any "
             "explicit module declarations (e.g., module ",
             lastModule->name().c_str(), "), "
             "so an implicit module named '",
             implicitModule->name().c_str(), "' is being "
             "introduced to contain the file's contents.");
}

void ErrorProcTypeUnannotatedFormal::write(ErrorWriterBase& wr) const {
  auto ast = std::get<const uast::AstNode*>(info);
  wr.heading(kind_, ast, "unannotated formal is ambiguous in this context");
  wr.code(ast);
  wr.message("The meaning of an unannotated formal (a formal not of the "
             "form '<name>:<type>') in a procedure type is ambiguous."
             "It is currently undecided whether it represents a formal's "
             "type or name.");
}

void ErrorProcDefExplicitAnonFormal::write(ErrorWriterBase& wr) const {
  auto ast = std::get<const uast::AstNode*>(info);
  wr.heading(kind_, ast, "formals in a procedure definition must be named");
  wr.code(ast);
}

} // end namespace 'chpl'
