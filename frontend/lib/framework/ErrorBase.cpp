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

/**
  Implementation of ErrorWriterBase that records calls
  to the various API functions in order to convert ErrorBase
  subclasses into backwards-compatible ErrorMessage instances.
 */
class CompatibilityWriter : public ErrorWriterBase {
 private:
  IdOrLocation idOrLoc_;
  /** The computed location (derived from the ::id_ or ::loc_) */
  Location computedLoc_;
  /** The error's brief message */
  std::string message_;
  /** A list of notes associated with this error (aka details) */
  std::vector<Note> notes_;

 public:
  CompatibilityWriter(Context* context)
    : ErrorWriterBase(context, OutputFormat::BRIEF) {}

  void writeHeading(ErrorBase::Kind kind, ErrorType type,
                    IdOrLocation idOrLoc, const std::string& message) override {
    // We may not have a context e.g. if we are just figuring out the error
    // message text. Trust that `computedLoc_` is not important for that.
    if (context_) this->computedLoc_ = errordetail::locate(context_, idOrLoc);
    this->idOrLoc_ = std::move(idOrLoc);
    this->message_ = message;
  }

  void writeMessage(const std::string& message) override {}
  void writeCode(const Location& loc,
                 const std::vector<Location>& hl) override {}

  void writeNote(IdOrLocation loc, const std::string& message) override {
    this->notes_.push_back(std::make_tuple(std::move(loc), message));
  }

  /**
    Get the error's ID (could be empty in favor of the location).

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
  inline ID id() const { return idOrLoc_.id(); }
  /**
    Get the error's location (could be empty in favor of the ID)

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
  inline Location location() const { return idOrLoc_.location(); }
  /**
    Return the location that should be reported to the user.

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
  inline Location computedLocation() const { return computedLoc_; }
  /**
    Return the error's brief message.

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
  inline const std::string& message() const { return message_; }
  /**
    Return the error's notes / details.

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
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
  chpl::mark<Note> marker;
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
