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
 * limitations under the License.
 */
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/query-impl.h"
#include "chpl/uast/VisibilityClause.h"
#include "chpl/types/all-types.h"
#include <sstream>

namespace chpl {

const char* ErrorBase::getKindName(ErrorBase::Kind kind) {
  switch (kind) {
    case ErrorBase::Kind::NOTE: return "note";
    case ErrorBase::Kind::WARNING: return "warning";
    case ErrorBase::Kind::SYNTAX: return "syntax";
    case ErrorBase::Kind::ERROR: return "error";
  }
  return "(unknown kind)";
}

const char* ErrorBase::getTypeName(ErrorType type) {
  switch (type) {
#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) case NAME: return #NAME;
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS
    default: return nullptr;
  }
}

std::string ErrorBase::message() const {
  CompatibilityWriter ew(/* context */ nullptr);
  write(ew);
  return ew.message();
}

Location ErrorBase::location(Context* context) const {
  CompatibilityWriter ew(context);
  write(ew);
  return ew.computedLocation();
}

ErrorMessage ErrorBase::toErrorMessage(Context* context) const {
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
  for (const auto& note : ew.notes()) {
    auto detailKind = ErrorMessage::NOTE;
    auto detailmessage = std::get<std::string>(note);
    message.addDetail(
        ErrorMessage(detailKind,
                     std::get<IdOrLocation>(note),
                     std::get<std::string>(note)));
  }
  return message;
}

void BasicError::write(ErrorWriterBase& wr) const {
  // if the ID is set, determine the location from that
  wr.heading(kind_, type_, idOrLoc_, message_);
  for (const auto& note : notes_) {
    auto& idOrLoc = std::get<IdOrLocation>(note);
    auto& message = std::get<std::string>(note);
    wr.note(idOrLoc, message);
  }
}

void BasicError::mark(Context* context) const {
  chpl::mark<ErrorNote> marker;
  idOrLoc_.mark(context);
  for (auto& note : notes_) {
    marker(context, note);
  }
}

owned<GeneralError> GeneralError::vbuild(Kind kind, ID id, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return owned<GeneralError>(new GeneralError(kind, std::move(id), std::move(message), {}));
}

owned<GeneralError> GeneralError::vbuild(Kind kind, Location loc, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return owned<GeneralError>(new GeneralError(kind, std::move(loc), std::move(message), {}));
}

owned<GeneralError> GeneralError::vbuild(Kind kind, IdOrLocation loc, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return owned<GeneralError>(new GeneralError(kind, std::move(loc), std::move(message), {}));
}

owned<GeneralError> GeneralError::get(Kind kind, Location loc, std::string msg) {
  return owned<GeneralError>(new GeneralError(kind, std::move(loc), std::move(msg), {}));
}

owned<GeneralError> GeneralError::error(Location loc, std::string msg) {
  return GeneralError::get(ErrorBase::ERROR, std::move(loc), std::move(msg));
}

owned<ErrorBase> GeneralError::clone() const {
  return owned<ErrorBase>(new GeneralError(*this));
}

} // end namespace 'chpl'
