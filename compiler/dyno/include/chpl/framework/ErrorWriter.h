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
#ifndef CHPL_FRAMEWORK_ERRORWRITER_H
#define CHPL_FRAMEWORK_ERRORWRITER_H

#include <ostream>

#include "chpl/framework/ErrorBase.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/util/terminal.h"

namespace chpl {

class Context;

namespace errordetail {

template <typename T>
struct LocationOnly {
  T t; /* The thing whose location to compute */
};

inline Location locate(Context* context, const ID& id) {
  if (!context) return Location();
  return parsing::locateId(context, id);
}
inline Location locate(Context* context, const uast::AstNode* node) {
  return locate(context, node->id());
}

/// \cond DO_NOT_DOCUMENT
template <typename T>
struct AsFileName {
  T t; /* The thing to locate */

  Location location(Context* context) const {
    return locate(context, t);
  }
};

template <typename T>
struct Writer {
  void operator()(Context* context, std::ostream& oss, const T& t) {
    stringify<T> str;
    str(oss, CHPL_SYNTAX, t);
  }
};

template <>
struct Writer<const char*> {
  void operator()(Context* context, std::ostream& oss, const char* t) {
    oss << t;
  }
};

template <>
struct Writer<std::string> {
  void operator()(Context* context, std::ostream& oss, const std::string& t) {
    oss << t;
  }
};

template <typename T>
struct Writer<errordetail::AsFileName<T>> {
  void operator()(Context* context, std::ostream& oss, const errordetail::AsFileName<T>& e) {
    auto loc = e.location(context);
    oss << loc.path().c_str() << ":" << loc.firstLine();
  }
};
/// \endcond

} // end namespace errordetail

template <typename T>
errordetail::AsFileName<T> fileNameOf(T t) {
  return errordetail::AsFileName<T> { std::move(t) };
}

template <typename T>
errordetail::LocationOnly<T> locationOnly(T t) {
  return errordetail::LocationOnly<T> { std::move(t) };
}

class ErrorWriterBase {
 protected:
  Context* context;

  ErrorWriterBase(Context* context) : context(context) {}

  virtual void writeHeading(ErrorBase::Kind kind, Location loc, const std::string& message) = 0;
  virtual void writeHeading(ErrorBase::Kind kind, const ID& id, const std::string& message);
  virtual void writeHeading(ErrorBase::Kind kind, const uast::AstNode* ast, const std::string& message);
  template <typename T>
  void writeHeading(ErrorBase::Kind kind, errordetail::LocationOnly<T> t, const std::string& message) {
    writeHeading(kind, errordetail::locate(context, t.t), message);
  }

  virtual void writeMessage(const std::string& message) = 0;

  virtual void writeNote(Location loc, const std::string& message) = 0;
  virtual void writeNote(const ID& id, const std::string& message);
  virtual void writeNote(const uast::AstNode* ast, const std::string& message);
  template <typename T>
  void writeNote(errordetail::LocationOnly<T> t, const std::string& message) {
    writeNote(errordetail::locate(context, t.t), message);
  }

  virtual void writeCode(const Location& place,
                         const std::vector<Location>& toHighlight = {}) = 0;

  template <typename ... Ts>
  std::string toString(Ts ... ts) {
    std::ostringstream oss;
    auto write = [&](auto t) {
      errordetail::Writer<decltype(t)> writer;
      writer(context, oss, t);
    };

    auto dummy = { (write(ts), 0)..., };
    (void) dummy;
    return oss.str();
  }
 public:
  template <typename LocationType, typename ... Ts>
  void heading(ErrorBase::Kind kind, LocationType loc, Ts ... ts) {
    writeHeading(kind, loc, toString(std::forward<Ts>(ts)...));
  }

  template <typename ... Ts>
  void message(Ts ... ts) {
    writeMessage(toString(std::forward<Ts>(ts)...));
  }

  template <typename LocationType, typename ... Ts>
  void note(LocationType loc, Ts ... ts) {
    writeNote(loc, toString(std::forward<Ts>(ts)...));
  }

  template <typename LocPlace, typename LocHighlight = const uast::AstNode*>
  void code(const LocPlace& place,
                const std::vector<LocHighlight>& toHighlight = {}) {
    std::vector<Location> ids(toHighlight.size());
    std::transform(toHighlight.cbegin(), toHighlight.cend(), ids.begin(), [&](auto node) {
      return errordetail::locate(context, node);
    });
    writeCode(errordetail::locate(context, place), ids);
  }

};

class ErrorWriter : public ErrorWriterBase {
 public:
  enum OutputFormat {
    DETAILED,     // All known details about the error
    BRIEF,        // Condensed version of the error
  };
 protected:
  std::ostream& oss_;
  OutputFormat outputFormat_;
  bool useColor_;

  void setColor(TermColorName color);

  void writeHeading(ErrorBase::Kind kind, Location loc,
                    const std::string& message) override;
  void writeMessage(const std::string& message) override {
    if (outputFormat_ == DETAILED) {
      oss_ << message << std::endl;
    }
  }
  void writeNote(Location loc, const std::string& message) override;
  void writeCode(const Location& place,
                 const std::vector<Location>& toHighlight = {}) override;
 public:
  ErrorWriter(Context* context, std::ostream& oss,
              OutputFormat outputFormat, bool useColor) :
    ErrorWriterBase(context), oss_(oss),
    outputFormat_(outputFormat), useColor_(useColor) {}

  void writeNewline();
};

} // end namespace chpl

#endif
