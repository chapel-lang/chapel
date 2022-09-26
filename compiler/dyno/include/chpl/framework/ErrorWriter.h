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

class ErrorWriter {
 public:
  enum OutputFormat {
    DETAILED,     // All known details about the error
    BRIEF,        // Condensed version of the error
  };
 protected:
  Context* context;
  std::ostream& oss_;
  OutputFormat outputFormat_;
  bool useColor_;

  std::string fileText(const Location& loc);

  void setColor(TermColorName color);

  virtual void writeErrorHeading(ErrorBase::Kind kind, Location loc, const std::string& message);
  virtual void writeErrorHeading(ErrorBase::Kind kind, const ID& id, const std::string& message);
  virtual void writeErrorHeading(ErrorBase::Kind kind, const uast::AstNode* ast, const std::string& message);

  virtual void writeNoteHeading(Location loc, const std::string& message);
  virtual void writeNoteHeading(const ID& id, const std::string& message);
  virtual void writeNoteHeading(const uast::AstNode* ast, const std::string& message);

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
  ErrorWriter(Context* context, std::ostream& oss,
              OutputFormat outputFormat, bool useColor) :
    context(context), oss_(oss),
    outputFormat_(outputFormat), useColor_(useColor) {}

  template <typename LocationType, typename ... Ts>
  void writeHeading(ErrorBase::Kind kind, LocationType loc, Ts ... ts) {
    writeErrorHeading(kind, loc, toString(std::forward<Ts>(ts)...));
  }

  template <typename ... Ts>
  void writeMessage(Ts ... ts) {
    if (outputFormat_ == DETAILED) {
      oss_ << toString(std::forward<Ts>(ts)...) << std::endl;
    }
  }

  template <typename LocationType, typename ... Ts>
  void writeNote(LocationType loc, Ts ... ts) {
    writeNoteHeading(loc, toString(std::forward<Ts>(ts)...));
  }

  void writeCode(const Location& place,
                const std::vector<Location>& toHighlight = {});
  template <typename LocPlace, typename LocHighlight>
  void writeCode(const LocPlace& place,
                const std::vector<LocHighlight>& toHighlight = {}) {
    std::vector<Location> ids(toHighlight.size());
    std::transform(toHighlight.cbegin(), toHighlight.cend(), ids.begin(), [&](auto node) {
      return errordetail::locate(context, node);
    });
    writeCode(errordetail::locate(context, place), ids);
  }
  // Specialize for AstNode* to support brace-only construction of highlight
  // lists (e.g. { binOp, fnCall })
  void writeCode(const uast::AstNode* place,
                const std::vector<const uast::AstNode*>& toHighlight = {});

  void writeNewline();
};

} // end namespace chpl

#endif
