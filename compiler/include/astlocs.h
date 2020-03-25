/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

class BaseAST;
class Expr;

// how an AST node knows its location in the source code
// (assumed to get copied upon assignment and parameter passing)
class astlocT {
public:
  astlocT(int linenoArg, const char* filenameArg) :
    filename(filenameArg), lineno(linenoArg)
    {}

  const char* filename;  // filename of location
  int         lineno;    // line number of location

  inline bool operator==(const astlocT other) const {
    return this->filename == other.filename && this->lineno == other.lineno;
  }
  inline bool operator!=(const astlocT other) const {
    return this->filename != other.filename || this->lineno != other.lineno;
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
