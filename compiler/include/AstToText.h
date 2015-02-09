/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _AST_TO_TEXT_H_
#define _AST_TO_TEXT_H_

/************************************ | *************************************
*                                                                           *
* This is the start of an effort to render AST to human-readable text.      *
*                                                                           *
* The initial implementation is focussed on function names and the formals  *
* list as a lever to deprecate FnSymbol::userString and to eliminate the    *
* need for unfortunate coupling between the Chapel parser and lexer.        *
*                                                                           *
* Longer term it could be built out to handle the function return type,     *
* the body of a function, and non-function constructs.                      *
*                                                                           *
* This "class" is largely a wrapper around an internal std::string plus a   *
* namespace for functions that mediate between AST and the string.  The     *
* general approach is to call functions in the correct order to build up    *
* the desired string.                                                       *
*                                                                           *
* The initial implementation is focussed on producing a string that can be  *
* used to generate effective user-facing error messages. The focus for      *
* these is a useful view of the function name and the formals list. The     *
* entry point for this is                                                   *
*     appendNameAndFormals() followed by                                    *
*     text()                                                                *
*                                                                           *
* It is intended that this work should benefit the revisions to chpl-doc.   *
* To this end there are entry points that simply accumulate the function's  *
* name or a given formal.  These entry points hide the internal formals     *
* that are used by methods to represent _mt and this.                       *
*                                                                           *
************************************* | ************************************/

class ArgSymbol;
class FnSymbol;

#include <string>

class AstToText
{
public:
                         AstToText();
                        ~AstToText();

  // A reference to the generate text
  const std::string&     text()                                        const;



  // These functions assume a user facing requirement for enumerating the
  // formals i.e. the internal formal(s) associated with a method are not
  // shown or accessible through this portion of the API.
  //
  void                   appendNameAndFormals(FnSymbol* fn);
  void                   appendName(FnSymbol* fn);
  void                   appendFormals(FnSymbol* fn);

  int                    numFormals(FnSymbol* fn)                      const;

  void                   appendFormal(FnSymbol* fn, int oneBasedIndex);

private:

  // Index of formal "this"            -> [0 .. 2]
  int                    indexForThis(FnSymbol* fn)                    const;

  // Index of first user facing formal -> [1 .. 3]
  int                    indexOfFirstFormal(FnSymbol* fn)              const;

  // Get user facing formal by one-based-index
  ArgSymbol*             formalGet(FnSymbol* fn, int oneBasedIndex)    const;

  std::string            mText;
};

#endif
