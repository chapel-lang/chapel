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

class CompatibilityWriter : public ErrorWriterBase {
 private:
  ID id_;
  Location loc_;
  Location computedLoc_;
  std::string message_;
  std::vector<Note> notes_;

 public:
  CompatibilityWriter(Context* context)
    : ErrorWriterBase(context) {}

  void writeHeading(ErrorBase::Kind kind, Location loc, const std::string& message) override {
    this->loc_ = loc;
    this->computedLoc_ = std::move(loc);
    this->message_ = message;
  }
  void writeHeading(ErrorBase::Kind kind, const ID& id, const std::string& message) override {
    // Just store the ID, but don't pollute the output stream.
    this->id_ = id;
    this->computedLoc_ = errordetail::locate(context, id);
    this->message_ = message;
  }

  void writeMessage(const std::string& message) override {}
  void writeCode(const Location& loc,
                 const std::vector<Location>& hl) override {}

  void writeNote(Location loc, const std::string& message) override {
    this->notes_.push_back(std::make_tuple(ID(), std::move(loc), message));
  }
  void writeNote(const ID& id, const std::string& message) override {
    this->notes_.push_back(std::make_tuple(std::move(id), Location(), message));
  }

  inline ID id() const { return id_; }
  inline Location location() const { return loc_; }
  inline Location computedLocation() const { return computedLoc_; }
  inline const std::string& message() const { return message_; }
  const std::vector<Note>& notes() const { return notes_; }
};

std::string ErrorBase::message() const {
  std::ostringstream oss;
  CompatibilityWriter ew(/* context */ nullptr);
  write(ew);
  return ew.message();
}

Location ErrorBase::location(Context* context) const {
  std::ostringstream oss;
  CompatibilityWriter ew(context);
  write(ew);
  return ew.computedLocation();
}

ID ErrorBase::id() const {
  std::ostringstream oss;
  CompatibilityWriter ew(/* context */ nullptr);
  write(ew);
  return ew.id();
}

ErrorMessage ErrorBase::toErrorMessage(Context* context) const {
  std::ostringstream oss;
  CompatibilityWriter ew(context);
  write(ew);
  ErrorMessage::Kind kind = ErrorMessage::NOTE;
  switch (kind_) {
    case ERROR: kind = ErrorMessage::ERROR; break;
    case WARNING: kind = ErrorMessage::WARNING; break;
    case NOTE: kind = ErrorMessage::NOTE; break;
    case SYNTAX: kind = ErrorMessage::SYNTAX; break;
  }
  auto message = ew.id().isEmpty() ?
    ErrorMessage(kind, ew.location(), ew.message()) :
    ErrorMessage(kind, ew.id(), ew.message());
  for (auto note : ew.notes()) {
    auto detailKind = ErrorMessage::NOTE;
    auto detailmessage = std::get<std::string>(note);
    message.addDetail(std::get<ID>(note).isEmpty() ?
        ErrorMessage(detailKind, std::get<Location>(note), std::move(detailmessage)) :
        ErrorMessage(detailKind, std::get<ID>(note), std::move(detailmessage))
    );
  }
  return message;
}

void BasicError::write(ErrorWriterBase& wr) const {
  // if the ID is set, determine the location from that
  if (!id_.isEmpty()) {
    wr.heading(kind_, id_, message_);
  } else {
    wr.heading(kind_, loc_, message_);
  }
  for (auto note : notes_) {
    auto& id = std::get<ID>(note);
    auto& location = std::get<Location>(note);
    auto& message = std::get<std::string>(note);
    if (!id.isEmpty()) {
      wr.note(id, message);
    } else {
      wr.note(location, message);
    }
  }
}

void BasicError::mark(Context* context) const {
  chpl::mark<Note> marker;
  id_.mark(context);
  loc_.mark(context);
  for (auto& note : notes_) {
    marker(context, note);
  }
}

const owned<ParseError>&
ParseError::getParseError(Context* context,
                          ErrorBase::Kind kind,
                          ID id,
                          Location loc,
                          std::string message,
                          std::vector<Note> notes) {
  QUERY_BEGIN(getParseError, context, kind, id, loc, message, notes);
  auto result = std::unique_ptr<ParseError>(new ParseError(kind, id, loc, message, notes));
  return QUERY_END(result);
}

const ParseError* ParseError::get(Context* context, const ErrorMessage& error) {
  Kind kind = NOTE;
  switch (error.kind()) {
    case ErrorMessage::ERROR: kind = ERROR; break;
    case ErrorMessage::WARNING: kind = WARNING; break;
    case ErrorMessage::NOTE: kind = NOTE; break;
    case ErrorMessage::SYNTAX: kind = SYNTAX; break;
  }
  std::vector<Note> notes;
  for (auto& note : error.details()) {
    assert(note.kind() == ErrorMessage::NOTE);
    notes.push_back(std::make_tuple(note.id(), note.location(), note.message()));
  }
  return ParseError::getParseError(context, kind, error.id(),
                                   error.location(), error.message(),
                                   std::move(notes)).get();
}

const owned<GeneralError>&
GeneralError::getGeneralErrorID(Context* context, Kind kind, ID id, std::string message) {
  QUERY_BEGIN(getGeneralErrorID, context, kind, id, message);
  auto result = owned<GeneralError>(new GeneralError(kind, id, std::move(message), {}));
  return QUERY_END(result);
}

const owned<GeneralError>&
GeneralError::getGeneralErrorLocation(Context* context, Kind kind, Location loc, std::string message) {
  QUERY_BEGIN(getGeneralErrorLocation, context, kind, loc, message);
  auto result = owned<GeneralError>(new GeneralError(kind, loc, std::move(message), {}));
  return QUERY_END(result);
}

const GeneralError* GeneralError::vbuild(Context* context, Kind kind, ID id, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return getGeneralErrorID(context, kind, id, message).get();
}

const GeneralError* GeneralError::vbuild(Context* context, Kind kind, Location loc, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return getGeneralErrorLocation(context, kind, loc, message).get();
}

const GeneralError* GeneralError::get(Context* context, Kind kind, Location loc, std::string msg) {
  return getGeneralErrorLocation(context, kind, loc, std::move(msg)).get();
}

#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...)\
  const owned<Error##NAME>& Error##NAME::getError##NAME(Context* context, std::tuple<EINFO> tuple) {\
    QUERY_BEGIN(getError##NAME, context, tuple);\
    auto result = owned<Error##NAME>(new Error##NAME(tuple));\
    return QUERY_END(result);\
  }\
\
  const Error##NAME* Error##NAME::get(Context* context, std::tuple<EINFO> tuple) {\
    return Error##NAME::getError##NAME(context, std::move(tuple)).get();\
  }
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

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

}
