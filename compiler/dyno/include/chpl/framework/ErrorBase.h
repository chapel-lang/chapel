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

#define ERROR_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, ERROR, EINFO)
#define WARNING_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, WARNING, EINFO)
#define SYNTAX_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, SYNTAX, EINFO)
#define NOTE_CLASS(NAME, EINFO...) DIAGNOSTIC_CLASS(NAME, NOTE, EINFO)

class ErrorWriter;

enum ErrorType {
  PARSE,
  GENERAL,
#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) NAME,
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS
};

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

  virtual bool contentsMatchInner(const ErrorBase* other) const = 0;
 public:
  virtual ~ErrorBase() = default;

  template <typename T>
  static bool update(owned<T>& old, owned<T>& addin) {
    return defaultUpdateOwned(old, addin);
  }

  Kind kind() const { return kind_; }
  ErrorType type() const { return type_; }

  std::string message() const;
  Location location(Context* context) const;
  ID id() const;

  bool operator==(const ErrorBase& other) const {
    if (kind_ != other.kind_ || type_ != other.type_) return false;
    return contentsMatchInner(&other);
  }
  bool operator!=(const ErrorBase& other) const {
    return !(*this == other);
  }

  virtual void write(ErrorWriter& wr) const = 0;
  virtual void mark(Context* context) const = 0;
};

class ParseError : public ErrorBase {
 private:
  ID id_;
  Location loc_;
  std::string message_;

 protected:
  ParseError(Kind kind, ID id, Location loc, std::string message)
    : ErrorBase(kind, PARSE), id_(std::move(id)),
      loc_(std::move(loc)), message_(std::move(message)) {}

  static const owned<ParseError>&
  getParseError(Context* context, Kind kind, ID id, Location loc, std::string message);

  bool contentsMatchInner(const ErrorBase* other) const override {
    auto otherParse = static_cast<const ParseError*>(other);
    return id_ == otherParse->id_ &&
      loc_ == otherParse->loc_ &&
      message_ == otherParse->message_;
  }
 public:

  static const ParseError* get(Context* context, const ErrorMessage&);

  void write(ErrorWriter& eq) const override;
  void mark(Context* context) const override;
};

class GeneralError : public ErrorBase {
 private:
  ID id_;
  Location loc_;
  std::string message_;

 protected:
  GeneralError(Kind kind, ID id, std::string message)
    : ErrorBase(kind, GENERAL), id_(std::move(id)), message_(std::move(message)) {}

  GeneralError(Kind kind, Location loc, std::string message)
    : ErrorBase(kind, GENERAL), loc_(std::move(loc)), message_(std::move(message)) {}

  static const owned<GeneralError>&
  getGeneralErrorID(Context* context, Kind kind, ID id, std::string message);

  static const owned<GeneralError>&
  getGeneralErrorLocation(Context* context, Kind kind, Location loc, std::string message);

  bool contentsMatchInner(const ErrorBase* other) const override {
    auto otherGeneral = static_cast<const GeneralError*>(other);
    return id_ == otherGeneral->id_ &&
      loc_ == otherGeneral->loc_ &&
      message_ == otherGeneral->message_;
  }
 public:

  static const GeneralError* vbuild(Context* context,
                                    Kind kind, ID id,
                                    const char* fmt,
                                    va_list vl);
  static const GeneralError* vbuild(Context* context,
                                    Kind kind, Location loc,
                                    const char* fmt,
                                    va_list vl);

  void write(ErrorWriter& eq) const override;
  void mark(Context* context) const override;
};

#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...)\
  class Error##NAME : public ErrorBase {\
   private:\
    using ErrorInfo = std::tuple<EINFO>;\
    ErrorInfo info;\
\
    Error##NAME(ErrorInfo info) :\
      ErrorBase(KIND, NAME), info(std::move(info)) {}\
\
    static const owned<Error##NAME>&\
    getError##NAME(Context* context, ErrorInfo info);\
   protected:\
    bool contentsMatchInner(const ErrorBase* other) const override {\
      auto otherCast = static_cast<const Error##NAME*>(other);\
      return info == otherCast->info;\
    }\
   public:\
    ~Error##NAME() = default;\
    static const Error##NAME* get(Context* context, ErrorInfo info);\
\
    void write(ErrorWriter& writer) const override;\
    void mark(Context* context) const override {\
      ::chpl::mark<ErrorInfo> marker;\
      marker(context, info);\
    }\
  };
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

#define REPORT(CONTEXT, NAME, EINFO...)\
  context->report(Error##NAME::get(CONTEXT, std::make_tuple(EINFO)))

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
