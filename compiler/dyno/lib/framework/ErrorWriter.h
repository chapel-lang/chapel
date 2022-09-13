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

#include <sstream>

#include "ErrorBase.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/parsing-queries.h"

namespace chpl {

class Context;

class ErrorWriter {
 private:
  std::ostringstream oss;
  Context* context;
  bool detailed;

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
    str(oss, CHPL_SYNTAX, t);
  }

  template <>
  void write(const char* str) {
    oss << str;
  }

  template <>
  void write(const std::string& str) {
    oss << str;
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
    oss << std::endl;
  }

 public:
  ErrorWriter(Context* context, bool detailed) :
    context(context), detailed(detailed) {}

  template <typename LocationType, typename ... Ts>
  void printBrief(ErrorBase::Kind kind, LocationType loc, Ts ... ts) {
    printErrorHeading(kind, loc);
    printMessage(std::forward<Ts>(ts)...);
  }

  template <typename ... Ts>
  void printMessage(Ts ... ts) {
    if (detailed) {
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

  std::string message() const;
};

} // end namespace chpl

#endif
