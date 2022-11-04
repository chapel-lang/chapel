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
#include "chpl/uast/VisibilityClause.h"
#include "chpl/types/all-types.h"
#include <sstream>

namespace chpl {

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
  /** The ID of where the error occurred. */
  ID id_;
  /** The location of where the error occurred. Only used if ::id_ is empty. */
  Location loc_;
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
                    Location loc, const std::string& message) override {
    this->loc_ = loc;
    this->computedLoc_ = std::move(loc);
    this->message_ = message;
  }
  void writeHeading(ErrorBase::Kind kind, ErrorType type,
                    const ID& id, const std::string& message) override {
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

  /**
    Get the error's ID (could be empty in favor of the location).

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
  inline ID id() const { return id_; }
  /**
    Get the error's location (could be empty in favor of the ID)

    This only works after ErrorBase::write was invoked with this
    CompatibilityWriter.
   */
  inline Location location() const { return loc_; }
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
    wr.heading(kind_, type_, id_, message_);
  } else {
    wr.heading(kind_, type_, loc_, message_);
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

const owned<ParseError>& ParseError::getParseError(Context* context,
                                                   const Location loc,
                                                   const std::string message) {
  QUERY_BEGIN(getParseError, context, loc, message);
  auto result = std::unique_ptr<ParseError>(new ParseError(loc, message));
  return QUERY_END(result);
}

const ParseError* ParseError::get(Context* context, const Location loc,
                                  const std::string message) {
  return ParseError::getParseError(context, loc, message).get();
}

const owned<GeneralError>&
GeneralError::getGeneralErrorForID(Context* context, Kind kind, ID id, std::string message) {
  QUERY_BEGIN(getGeneralErrorForID, context, kind, id, message);
  auto result = owned<GeneralError>(new GeneralError(kind, id, std::move(message), {}));
  return QUERY_END(result);
}

const owned<GeneralError>&
GeneralError::getGeneralErrorForLocation(Context* context, Kind kind, Location loc, std::string message) {
  QUERY_BEGIN(getGeneralErrorForLocation, context, kind, loc, message);
  auto result = owned<GeneralError>(new GeneralError(kind, loc, std::move(message), {}));
  return QUERY_END(result);
}

const GeneralError* GeneralError::vbuild(Context* context, Kind kind, ID id, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return getGeneralErrorForID(context, kind, id, message).get();
}

const GeneralError* GeneralError::vbuild(Context* context, Kind kind, Location loc, const char* fmt, va_list vl) {
  auto message = vprintToString(fmt, vl);
  return getGeneralErrorForLocation(context, kind, loc, message).get();
}

const GeneralError* GeneralError::get(Context* context, Kind kind, Location loc, std::string msg) {
  return getGeneralErrorForLocation(context, kind, loc, std::move(msg)).get();
}

} // end namespace 'chpl'
