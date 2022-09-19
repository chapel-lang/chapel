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

#include "ErrorBase.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/parsing-queries.h"

namespace chpl {

class Context;

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

  inline Location locate(const ID& id) {
    if (!context) return Location();
    return parsing::locateId(context, id);
  }
  inline Location locate(const uast::AstNode* node) {
    return locate(node->id());
  }

  std::string fileText(const Location& loc);

  const char* kindText(ErrorBase::Kind kind);

  template <typename T>
  void write(T t) {
    stringify<T> str;
    str(oss_, CHPL_SYNTAX, t);
  }

  void write(const char* str) {
    oss_ << str;
  }

  void write(const std::string& str) {
    oss_ << str;
  }

  void printErrorHeading(ErrorBase::Kind kind, Location loc);
  template <typename Locatable>
  void printErrorHeading(ErrorBase::Kind kind, const Locatable& loc) {
    printErrorHeading(kind, locate(loc));
  }

  template <typename ... Ts>
  void print(Ts ... ts);

  template <typename T, typename ... Rest>
  void print(T t, Rest ... rest) {
    write(t);
    print(std::forward<Rest>(rest)...);
  }

  template <>
  void print() {
    if (outputFormat_ != MESSAGE_ONLY) {
      // Don't print newlines if we're only concerned about
      oss_ << std::endl;
    }
  }

 public:
  ErrorWriter(Context* context, std::ostream& oss, OutputFormat outputFormat) :
    context(context), oss_(oss), outputFormat_(outputFormat) {}

  template <typename LocationType, typename ... Ts>
  void printBrief(ErrorBase::Kind kind, LocationType loc, Ts ... ts) {
    printErrorHeading(kind, loc);
    print(std::forward<Ts>(ts)...);
  }

  template <typename ... Ts>
  void printMessage(Ts ... ts) {
    if (outputFormat_ == DETAILED) {
      print(std::forward<Ts>(ts)...);
    }
  }

  void printAst(const Location& place,
                const std::vector<Location>& toHighlight = {});
  template <typename LocPlace, typename LocHighlight>
  void printAst(const LocPlace& place,
                const std::vector<LocHighlight>& toHighlight = {}) {
    std::vector<Location> ids(toHighlight.size());
    std::transform(toHighlight.cbegin(), toHighlight.cend(), ids.begin(), [&](auto node) {
      return locate(node);
    });
    printAst(locate(place), ids);
  }
  // Specialize for AstNode* to support brace-only construction of highlight
  // lists (e.g. { binOp, fnCall })
  void printAst(const uast::AstNode* place,
                const std::vector<const uast::AstNode*>& toHighlight = {});

  void printNewline();

  inline Location lastLocation() const { return lastLocation_; }
};

} // end namespace chpl

#endif
