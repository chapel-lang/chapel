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

template <typename T>
struct AsFileName {
  T t; /* The thing to locate */

  Location location(Context* context) const {
    return locate(context, t);
  }
};

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
    MESSAGE_ONLY, // Only the error text, without details or location
  };
 private:
  Context* context;
  std::ostream& oss_;
  OutputFormat outputFormat_;
  Location lastLocation_;
  ID lastId_;

  std::string fileText(const Location& loc);

  const char* kindText(ErrorBase::Kind kind);

  template <typename T>
  struct Writer {
    void operator()(ErrorWriter& ew, const T& t) {
      stringify<T> str;
      str(ew.oss_, CHPL_SYNTAX, t);
    }
  };

  template <typename T>
  void write(T t) {
    Writer<T> writer;
    writer(*this, t);
  }

  void writeErrorHeading(ErrorBase::Kind kind, Location loc);
  void writeErrorHeading(ErrorBase::Kind kind, const ID& id);
  void writeErrorHeading(ErrorBase::Kind kind, const uast::AstNode* ast);

  template <typename T, typename ... Rest>
  void writeAll(T t, Rest ... rest) {
    write(t);
    writeAll(std::forward<Rest>(rest)...);
  }

  void writeAll() {
    if (outputFormat_ != MESSAGE_ONLY) {
      // Don't write newlines if we're only concerned about
      oss_ << std::endl;
    }
  }

 public:
  ErrorWriter(Context* context, std::ostream& oss, OutputFormat outputFormat) :
    context(context), oss_(oss), outputFormat_(outputFormat) {}

  template <typename LocationType, typename ... Ts>
  void writeHeading(ErrorBase::Kind kind, LocationType loc, Ts ... ts) {
    writeErrorHeading(kind, loc);
    writeAll(std::forward<Ts>(ts)...);
  }

  template <typename ... Ts>
  void writeMessage(Ts ... ts) {
    if (outputFormat_ == DETAILED) {
      writeAll(std::forward<Ts>(ts)...);
    }
  }

  template <typename ... Ts>
  void writeNote(Ts ... ts) {
    if (outputFormat_ == DETAILED) {
      writeAll(std::forward<Ts>(ts)...);
    } else if (outputFormat_ == BRIEF){
      // Indent notes in brief mode to make things easier to organize
      writeAll("  note: ", std::forward<Ts>(ts)...);
    } else {
      // In message-only mode, don't print notes either.
    }
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

  inline Location lastLocation() const { return lastLocation_; }
  inline ID lastId() const { return lastId_; }
};

template <>
struct ErrorWriter::Writer<const char*> {
  void operator()(ErrorWriter& ew, const char* t) {
    ew.oss_ << t;
  }
};

template <>
struct ErrorWriter::Writer<std::string> {
  void operator()(ErrorWriter& ew, const std::string& t) {
    ew.oss_ << t;
  }
};

template <typename T>
struct ErrorWriter::Writer<errordetail::AsFileName<T>> {
  void operator()(ErrorWriter& ew, const errordetail::AsFileName<T>& e) {
    auto loc = e.location(ew.context);
    ew.write(loc.path().c_str());
    ew.write(":");
    ew.write(loc.firstLine());
  }
};

} // end namespace chpl

#endif
