/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _ASTLOCS_H_
#define _ASTLOCS_H_

#include <cassert>
#include <cstring>

#include "chpl/queries/ID.h"
#include "stringutil.h"

class BaseAST;
class Expr;

// how an AST node knows its location in the source code
// (assumed to get copied upon assignment and parameter passing)
class astlocT {
private:
  mutable const char* filename_;  // filename of location
  mutable int         lineno_;    // line number of location
  chpl::ID            id_;        // id from compiler/next

public:
  astlocT(int linenoArg, const char* filenameArg)
    : filename_(filenameArg), lineno_(linenoArg), id_()
  {
    if (filenameArg != nullptr)
      assert(astr(filename_) == filename_);
  }

  astlocT(chpl::ID id)
    : filename_(nullptr), lineno_(0), id_(std::move(id))
  { }

  int compare(const astlocT& other) const;
  void convertIdToFileLine(const char*& filename, int& lineno) const;
  const char* stringLoc() const;

  bool isEmpty() const {
    if (filename_ != nullptr)
      return false;

    return id_.isEmpty();
  }

  chpl::ID id() const {
    return id_;
  }

  // always returns an astr or nullptr
  const char* filename() const {
    if (filename_ != nullptr || id_.isEmpty())
      return filename_;

    // otherwise, get the filename from the id
    const char* name = nullptr;
    int line = 0;
    convertIdToFileLine(name, line);
    return name;
  }

  int lineno() const {
    if (filename_ != nullptr || id_.isEmpty())
      return lineno_;

    // otherwise, get the lineno from the id
    const char* name = nullptr;
    int line = 0;
    convertIdToFileLine(name, line);
    return line;
  }

  bool operator==(const astlocT& other) const {
    return this->filename_ == other.filename_ &&
           this->lineno_ == other.lineno_ &&
           this->id_ == other.id_;
  }
  bool operator!=(const astlocT& other) const {
    return !(*this == other);
  }

  bool operator<(const astlocT& other) const {
    return this->compare(other) < 0;
  }
  bool operator>(const astlocT other) const {
    return this->compare(other) > 0;
  }
  bool operator<=(const astlocT other) const {
    return this->compare(other) <= 0;
  }
  bool operator>=(const astlocT other) const {
    return this->compare(other) >= 0;
  }
};

//
// macro to update the global line number used to set the line number
// of an AST node when it is constructed - or to print out the line
// number of code related to a core dump.
//
// This should be used before constructing new nodes to make sure the
// line number is correctly set. The global line number reverts to
// its previous value upon leaving the scope where the macro is used.
// The fixed variable name ensures a single macro per scope.
// Users of the macro are to create additional scopes when needed.
// todo - should we add it to DECLARE_COPY/DECLARE_SYMBOL_COPY ?
//
#define SET_LINENO(ast) astlocMarker markAstLoc(ast->astloc)

extern astlocT currentAstLoc;

class astlocMarker {
public:
  astlocMarker(astlocT newAstLoc);
  astlocMarker(int lineno, const char* filename);
  ~astlocMarker();

  astlocT previousAstLoc;
};

Expr* findLocationIgnoringInternalInlining(Expr* cur);
bool printsUserLocation(const BaseAST* astIn);

astlocT getUserInstantiationPoint(const BaseAST* ast);

#endif
