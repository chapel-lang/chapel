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
#include "llvm/ADT/Optional.h"

namespace chpl {

class ErrorWriter;

enum ErrorType {
  PARSE,
#define ERROR_CLASS(name__, kind__, info__...) name__,
#include "error-classes-list.h"
#undef ERROR_CLASS
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
 public:
  virtual ~ErrorBase() = default;

  template <typename T>
  static bool update(owned<T>& old, owned<T>& addin) {
    return defaultUpdateOwned(old, addin);
  }

  Kind kind() const { return kind_; }
  ErrorType type() const { return type_; }

  virtual void write(ErrorWriter& wr) const = 0;
  virtual bool operator ==(const ErrorBase& other) {
    // TODO: how to make this work?
    return false;
  }
  virtual void mark(Context* context) const = 0;
};

class ParseError : public ErrorBase {
 private:
  ID id_;
  Location loc_;
  std::string message_;

 protected:
  ParseError(Kind kind, ErrorType type, ID id, Location loc, std::string message)
    : ErrorBase(kind, type), id_(id), loc_(std::move(loc)), message_(std::move(message)) {}
  ParseError(Kind kind, ID id, Location loc, std::string message)
    : ParseError(kind, PARSE, std::move(id), std::move(loc), std::move(message)) {}

  static const owned<ParseError>&
  getParseError(Context* context, Kind kind, ID id, Location loc, std::string message);
 public:

  static const ParseError* get(Context* context, const ErrorMessage&);

  Location location(Context* context) const;
  const std::string& message() const { return message_; }

  void write(ErrorWriter& eq) const override;
  void mark(Context* context) const override;
};

#define ERROR_CLASS(name__, kind__, info__...)\
  class Error##name__ : public ErrorBase {\
   private:\
    using ErrorInfo = std::tuple<info__>;\
    ErrorInfo info;\
\
    Error##name__(ErrorInfo info) :\
      ErrorBase(kind__, name__), info(std::move(info)) {}\
\
    static const owned<Error##name__>&\
    getError##name__(Context* context, ErrorInfo info);\
   public:\
    ~Error##name__() = default;\
    static const Error##name__* get(Context* context, ErrorInfo info);\
\
    void write(ErrorWriter& writer) const override;\
    void mark(Context* context) const override {\
      ::chpl::mark<ErrorInfo> marker;\
      marker(context, info);\
    }\
  };
#include "error-classes-list.h"
#undef ERROR_CLASS

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
  struct std::hash<chpl::ErrorBase::Kind> {
    size_t operator()(const chpl::ErrorBase::Kind& key) {
      return key;
    }
  };
} // end namesapce std

#endif
