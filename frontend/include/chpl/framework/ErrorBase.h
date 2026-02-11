/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl/resolution/scope-types.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {

// Convenience shorthands for DIAGNOSTIC_CLASS.
#define ERROR_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, ERROR, EINFO)
#define WARNING_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, WARNING, EINFO)

// Shorthands specific to parser errors, which provide explicit Locations
#define PARSER_DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) \
  DIAGNOSTIC_CLASS(NAME, KIND, const Location, ##EINFO)
#define PARSER_ERROR_CLASS(NAME, EINFO...) \
  PARSER_DIAGNOSTIC_CLASS(NAME, ERROR, ##EINFO)
#define PARSER_WARNING_CLASS(NAME, EINFO...) \
  PARSER_DIAGNOSTIC_CLASS(NAME, WARNING, ##EINFO)
#define PARSER_SYNTAX_CLASS(NAME, EINFO...) \
  PARSER_DIAGNOSTIC_CLASS(NAME, SYNTAX, ##EINFO)

// Shorthands specific to post-parse-checks errors, which provide node IDs
// that should connect to locations by the time we report out errors
#define POSTPARSE_DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) \
  DIAGNOSTIC_CLASS(NAME, KIND, const uast::AstNode*, ##EINFO)
#define POSTPARSE_ERROR_CLASS(NAME, EINFO...) \
  POSTPARSE_DIAGNOSTIC_CLASS(NAME, ERROR, ##EINFO)
#define POSTPARSE_WARNING_CLASS(NAME, EINFO...) \
  POSTPARSE_DIAGNOSTIC_CLASS(NAME, WARNING, ##EINFO)

class ErrorWriterBase;

using ErrorNote = std::tuple<IdOrLocation, std::string>;
using ErrorCodeSnippet = std::tuple<Location, std::vector<Location>>;

/** Enum representing the different types of errors in Dyno. */
enum ErrorType {
  // GeneralError is not defined via macro to make it easier to provide special
  // behavior for it (e.g. vbuild). Its tags are thus also not provided via the
  // macro.
  General,
// Include each error specified in error-classes-list.h as an enum element here
#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) NAME,
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS
};

/* Forward-declare subclasses of ErrorBase, so that we can use them in
   ErrorBase::toBla() methods. */
class GeneralError;
#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) class Error##NAME;
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

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

  static const char* getKindName(Kind kind);
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
    * All ErrorBase::write implementations must call ErrorWriterBase::heading to
      provide a concise description of the error message.
    * After this, ErrorWriterBase::note can be used to print notes, which are
      always shown to the user.
    * ErrorWriterBase::message along with ErrorWriterBase::code can be used to
      print additional text and code, respectively, which is only shown when
      the error is printed in detail.
   */
  virtual void write(ErrorWriterBase& wr) const = 0;
  virtual void mark(Context* context) const = 0;
  virtual owned<ErrorBase> clone() const = 0;

  bool isGeneralError() const { return type_ == ErrorType::General; }
  const GeneralError* toGeneralError() const {
    return isGeneralError() ? (const GeneralError*)(this) : nullptr;
  }
  GeneralError* toGeneralError() {
    return isGeneralError() ? (GeneralError*)(this) : nullptr;
  }

  /// \cond DO_NOT_DOCUMENT
  #define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) \
    bool is##NAME() const { return type_ == ErrorType::NAME; } \
    const Error##NAME* to##NAME() const { \
      return is##NAME() ? (const Error##NAME*)(this) : nullptr; \
    } \
    Error##NAME* to##NAME() { \
      return is##NAME() ? (Error##NAME*)(this) : nullptr; \
    }
  #include "chpl/framework/error-classes-list.h"
  #undef DIAGNOSTIC_CLASS
  /// \endcond DO_NOT_DOCUMENT
};

/* Converts a given ErrorType to its corresponding ErrorBase::Kind */
ErrorBase::Kind errorKindForErrorType(ErrorType type);

/* Returns whether the given kind represents an error (as opposed to
   a warning or note) */
bool errorKindIsError(ErrorBase::Kind kind);

/**
  An error without a specific type, and lacking (typed) additional information
  about what happened.
 */
class BasicError : public ErrorBase {
 private:
  IdOrLocation idOrLoc_;
  /** The error's message. */
  std::string message_;
  /** Additional notes / details attached to the error. */
  std::vector<ErrorNote> notes_;

 protected:
  BasicError(Kind kind, ErrorType type, IdOrLocation idOrLoc,
             std::string message,
             std::vector<ErrorNote> notes) :
    ErrorBase(kind, type), idOrLoc_(std::move(idOrLoc)),
    message_(std::move(message)), notes_(std::move(notes)) {}

  bool contentsMatchInner(const ErrorBase* other) const override {
    auto otherBasic = static_cast<const BasicError*>(other);
    return idOrLoc_ == otherBasic->idOrLoc_ &&
      message_ == otherBasic->message_ &&
      notes_ == otherBasic->notes_;
  }
 public:
  void write(ErrorWriterBase& eq) const override;
  void mark(Context* context) const override;
};

/**
  An error without specific information. Used when no error class has yet
  been specified for the specific error condition.
 */
class GeneralError : public BasicError {
 protected:
  GeneralError(const GeneralError& other) = default;
  GeneralError(ErrorBase::Kind kind, IdOrLocation idOrLoc,
               std::string message, std::vector<ErrorNote> notes)
    : BasicError(kind, General, std::move(idOrLoc),
                 std::move(message), std::move(notes)) {}

  static const owned<GeneralError>&
  getGeneralErrorForID(ErrorBase::Kind kind, ID id, std::string message);

  static const owned<GeneralError>&
  getGeneralErrorForLocation(ErrorBase::Kind kind, Location loc, std::string message);
 public:

  static owned<GeneralError> vbuild(ErrorBase::Kind kind, ID id,
                                    const char* fmt,
                                    va_list vl);
  static owned<GeneralError> vbuild(ErrorBase::Kind kind, Location loc,
                                    const char* fmt,
                                    va_list vl);
  static owned<GeneralError> vbuild(ErrorBase::Kind kind, IdOrLocation loc,
                                    const char* fmt,
                                    va_list vl);

  static owned<GeneralError> get(ErrorBase::Kind kind,
                                 Location loc,
                                 std::string msg);

  /* Convenience overload to call ::get with the ERROR kind. */
  static owned<GeneralError> error(Location loc,
                                   std::string msg);

  owned<ErrorBase> clone() const override;
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
    ErrorInfo info_;\
\
    Error##NAME__(const Error##NAME__& other) = default;\
    Error##NAME__(ErrorInfo info) :\
      ErrorBase(KIND__, NAME__), info_(std::move(info)) {}\
\
   protected:\
    bool contentsMatchInner(const ErrorBase* other) const override {\
      auto otherCast = static_cast<const Error##NAME__*>(other);\
      return info_ == otherCast->info_;\
    }\
   public:\
    ~Error##NAME__() = default;\
    static owned<Error##NAME__> get(ErrorInfo info);\
\
    void write(ErrorWriterBase& writer) const override;\
    void mark(Context* context) const override {\
      ::chpl::mark<ErrorInfo> marker;\
      marker(context, info_);\
    }\
    owned<ErrorBase> clone() const override {\
      return owned<ErrorBase>(new Error##NAME__(*this));\
    }\
\
    const ErrorInfo& info() const { return info_; }\
  };
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

// Generate query function implementations, like ErrorMessage::get for an
// error type. This is meant to be invoked via DIAGNOSTIC_CLASS and
// including error-classes-list.h
#define DIAGNOSTIC_CLASS_IMPL(NAME__, KIND__, EINFO__...)\
  owned<Error##NAME__> Error##NAME__::get(std::tuple<EINFO__> tuple) {\
    return owned<Error##NAME__>(new Error##NAME__(std::move(tuple)));\
  }

/**
  Helper macro to report an error message in a Context. Accepts
  the context, error type, and the additional error information specific
  to the error type (see error-classes-list).

  This macro takes care of packaging the error information into a tuple;
  it's sufficient to provide it via varargs.
 */
#define CHPL_REPORT(CONTEXT__, NAME__, EINFO__...) \
  CONTEXT__->report(CHPL_GET_ERROR(NAME__, EINFO__))
#define CHPL_GET_ERROR(NAME__, EINFO__...) \
  Error##NAME__::get(std::make_tuple(EINFO__))

/**
  Helper macro to report an error to the context, and produce an
  erroneous QualifiedType. Accepts the pointer to the context,
  the name of the error to report, and additional error information arguments,
  the exact types of which depend on the type of error (see error-classes-list.h)
 */
#define CHPL_TYPE_ERROR(CONTEXT, NAME, EINFO...)\
  (CHPL_REPORT(CONTEXT, NAME, EINFO),\
   QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(CONTEXT)))

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
} // end namespace std

#endif
