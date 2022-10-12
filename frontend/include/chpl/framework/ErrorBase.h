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
#ifndef CHPL_FRAMEWORK_ERRORBASE_H
#define CHPL_FRAMEWORK_ERRORBASE_H

#include "chpl/framework/ErrorMessage.h"
#include "chpl/uast/all-uast.h"
#include "chpl/types/all-types.h"
#include "llvm/ADT/Optional.h"

namespace chpl {

// Convenience shorthands for DIAGNOSTIC_CLASS.
#define ERROR_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, ERROR, EINFO)
#define WARNING_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, WARNING, EINFO)
#define SYNTAX_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, SYNTAX, EINFO)
#define NOTE_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, NOTE, EINFO)

class ErrorWriterBase;

using Note = std::tuple<ID, Location, std::string>;

/** Enum representing the different types of errors in Dyno. */
enum ErrorType {
  // The ParseError and GeneralError are not defined via macros to
  // make it easier to provide special behavior for them (e.g. vbuild
  // for GeneralError). Their tags are thus also not provided via the macro.
  PARSE,
  GENERAL,
// Include each error specified in error-classes-list.h as an enum element here
#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) NAME,
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS
};

/**
  Parent class for all errors in Dyno.

  The most important part of defining a new error is implementing the
  ErrorBase::write method; see its documentation for the implementation
  strategy.

  Most sub-classes of ErrorBase should be defined by adding them to
  error-classes-list
 */
class ErrorBase {
 public:
  enum Kind {
    NOTE,
    WARNING,
    SYNTAX,
    ERROR
  };
 protected:
  const Kind kind_;
  const ErrorType type_;

  ErrorBase(Kind kind, ErrorType type) : kind_(kind), type_(type) {}

  /**
    This function needs to be defined by subclasses.
    It should check only those fields defined in subclasses
    (it should not check the ErrorBase fields such as type_).

    It can assume that other has the same type as the receiver.
   */
  virtual bool contentsMatchInner(const ErrorBase* other) const = 0;
 public:
  virtual ~ErrorBase() = default;

  static const char* getTypeName(ErrorType type);

  template <typename T>
  static bool update(owned<T>& old, owned<T>& addin) {
    return defaultUpdateOwned(old, addin);
  }

  /** Get the error's kind */
  Kind kind() const { return kind_; }
  /** Get the error's type */
  ErrorType type() const { return type_; }

  /** Extract the error's summary error message. */
  std::string message() const;
  /** Extract the error's location. */
  Location location(Context* context) const;
  /**
    Convert the error to an ErrorMessage. This is mostly needed
    for compatibility reasons, since the production compiler currently
    knows how to print ErrorMessage, and does it differently from
    our ErrorBase::write methods.
   */
  ErrorMessage toErrorMessage(Context* context) const;

  bool operator==(const ErrorBase& other) const {
    if (kind_ != other.kind_ || type_ != other.type_) return false;
    return contentsMatchInner(&other);
  }
  bool operator!=(const ErrorBase& other) const {
    return !(*this == other);
  }

  /**
    Write information about this error to the given writer. See
    the ErrorWriterBase class for the API available to the error messages.

    This method should call methods on the ErrorWriterBase class to
    provide information about the error.
    * All ErrorBase::write implemenations must call ErrorWriterBase::heading to
      provide a concise description of the error message.
    * After this, ErrorWriterBase::note can be used to print notes, which are
      always shown to the user.
    * ErrorWriterBase::message along with ErrorWriterBase::code can be used to
      print additional text and code, respectively, which is only shown when
      the error is printed in detail.
   */
  virtual void write(ErrorWriterBase& wr) const = 0;
  virtual void mark(Context* context) const = 0;
};

/**
  An error without a specific type, and lacking (typed) additional information
  about what happened.
 */
class BasicError : public ErrorBase {
 private:
  /**
    The ID where the error occurred. Much like ErrorMessage, if the ID
    is empty, then BasicError::loc_ should be used instead.
   */
  ID id_;
  /**
    The location of the error message. Should be used only if BasicError::id_
    is empty.
   */
  Location loc_;
  /** The error's message. */
  std::string message_;
  /** Additional notes / details attached to the error. */
  std::vector<Note> notes_;

 protected:
  BasicError(Kind kind, ErrorType type, ID id, Location loc,
             std::string message,
             std::vector<Note> notes) :
    ErrorBase(kind, type), id_(std::move(id)), loc_(std::move(loc)),
    message_(std::move(message)), notes_(std::move(notes)) {}

  bool contentsMatchInner(const ErrorBase* other) const override {
    auto otherBasic = static_cast<const BasicError*>(other);
    return id_ == otherBasic->id_ &&
      loc_ == otherBasic->loc_ &&
      message_ == otherBasic->message_ &&
      notes_ == otherBasic->notes_;
  }
 public:
  void write(ErrorWriterBase& eq) const override;
  void mark(Context* context) const override;
};

/**
  An error originating from the parser.
 */
class ParseError : public BasicError {
 protected:
  ParseError(Kind kind, ID id, Location loc, std::string message, std::vector<Note> notes)
    : BasicError(kind, PARSE, std::move(id), std::move(loc), std::move(message),
        std::move(notes)) {}

  static const owned<ParseError>&
  getParseError(Context* context, Kind kind, ID id,
                Location loc, std::string message,
                std::vector<Note> notes);
 public:
  static const ParseError* get(Context* context, const ErrorMessage&);
};

/**
  An error without specific information. Used when no error class has yet
  been specified for the specific error condition.
 */
class GeneralError : public BasicError {
 protected:
  GeneralError(ErrorBase::Kind kind, ID id,
               std::string message, std::vector<Note> notes)
    : BasicError(kind, GENERAL, std::move(id), Location(),
                 std::move(message), std::move(notes)) {}

  GeneralError(ErrorBase::Kind kind, Location loc,
               std::string message, std::vector<Note> notes)
    : BasicError(kind, GENERAL, ID(), std::move(loc),
                 std::move(message), std::move(notes)) {}

  static const owned<GeneralError>&
  getGeneralErrorForID(Context* context, ErrorBase::Kind kind, ID id, std::string message);

  static const owned<GeneralError>&
  getGeneralErrorForLocation(Context* context, ErrorBase::Kind kind, Location loc, std::string message);
 public:

  static const GeneralError* vbuild(Context* context,
                                    ErrorBase::Kind kind, ID id,
                                    const char* fmt,
                                    va_list vl);
  static const GeneralError* vbuild(Context* context,
                                    ErrorBase::Kind kind, Location loc,
                                    const char* fmt,
                                    va_list vl);

  static const GeneralError* get(Context* context,
                                 ErrorBase::Kind kind,
                                 Location loc,
                                 std::string msg);
};

// The error-classes-list.h header will expand the DIAGNOSTIC_CLASS macro
// for each error message defined in the list. In this case, we use this
// macro to generate class declarations for each error. This way, the only
// steps needed to add a new error are to add a line to error-classes-list.h,
// and to implement the new error classes' ErrorBase::write method.
#define DIAGNOSTIC_CLASS(NAME__, KIND__, EINFO__...)\
  class Error##NAME__ : public ErrorBase {\
   private:\
    using ErrorInfo = std::tuple<EINFO__>;\
    ErrorInfo info;\
\
    Error##NAME__(ErrorInfo info) :\
      ErrorBase(KIND__, NAME__), info(std::move(info)) {}\
\
    static const owned<Error##NAME__>&\
    getError##NAME__(Context* context, ErrorInfo info);\
   protected:\
    bool contentsMatchInner(const ErrorBase* other) const override {\
      auto otherCast = static_cast<const Error##NAME__*>(other);\
      return info == otherCast->info;\
    }\
   public:\
    ~Error##NAME__() = default;\
    static const Error##NAME__* get(Context* context, ErrorInfo info);\
\
    void write(ErrorWriterBase& writer) const override;\
    void mark(Context* context) const override {\
      ::chpl::mark<ErrorInfo> marker;\
      marker(context, info);\
    }\
  };
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

/**
  Helper macro to report an error message in a Context. Accepts
  the context, error type, and the additional error information specific
  to the error type (see error-classes-list).

  This macro takes care of packaging the error information into a tuple;
  it's sufficient to provide it via varargs.
 */
#define CHPL_REPORT(CONTEXT__, NAME__, EINFO__...)\
  CONTEXT__->report(Error##NAME__::get(CONTEXT__, std::make_tuple(EINFO__)))

template <>
struct stringify<chpl::ErrorBase::Kind> {
  void operator()(std::ostream& ss,
                StringifyKind stringifyKind,
                chpl::ErrorBase::Kind kind) {
    switch (kind) {
      case ErrorBase::NOTE: ss << "NOTE"; return;
      case ErrorBase::WARNING: ss << "WARNING"; return;
      case ErrorBase::ERROR: ss << "ERROR"; return;
      case ErrorBase::SYNTAX: ss << "SYNTAX"; return;
    }
  }
};

} // end namespace chpl

namespace std {
  template <>
  struct hash<chpl::ErrorBase::Kind> {
    size_t operator()(const chpl::ErrorBase::Kind& key) {
      return key;
    }
  };
} // end namesapce std

#endif
