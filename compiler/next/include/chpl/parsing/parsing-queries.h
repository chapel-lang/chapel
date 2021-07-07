/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_PARSING_PARSING_QUERIES_H
#define CHPL_PARSING_PARSING_QUERIES_H

#include "chpl/queries/Context.h"
#include "chpl/queries/ID.h"
#include "chpl/queries/Location.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Module.h"

#include <vector>

namespace chpl {
namespace parsing {


  struct FileContents {
    std::string text;
    ErrorMessage error;
    FileContents()
      : text(), error() { }
    FileContents(std::string text)
      : text(std::move(text)), error() { }
    FileContents(std::string text, ErrorMessage error)
      : text(std::move(text)), error(std::move(error)) { }
  };

  /**
   This query returns the contents of a file as the string field in the
   FileContents.
   In case there is an error reading the file, the error is stored in the error
   field of the FileContents.
   */
  const FileContents& fileText(Context* context, UniqueString path);

  /**
   This function sets the FileContents that will be returned by the fileText
   query above.
   */
  void setFileText(Context* context, UniqueString path, FileContents result);
  /**
   This function sets the string that will be returned by the fileText
   query above. The FileContents stored will have an empty ErrorMessage field.
   */
  void setFileText(Context* context, UniqueString path, std::string text);

  /**
   This query reads a file (with the fileText query) and then parses it.
   */
  const uast::Builder::Result& parseFile(Context* context, UniqueString path);


  // These functions can't return the Location for a Comment
  // because Comments don't have IDs. If Locations for Comments are needed,
  // instead use the astToLocation field from the result of parseFile.

  /**
   This query returns the Location where a particular ID appeared.
   It cannot be used for Comments because Comments don't have IDs set.
   If Locations for Comments are needed, use the locations field from
   the result of parseFile.
   */
  const Location& locateId(Context* context, ID id);
  /**
   This function just runs locateId on ast->id(). Similarly to locateID,
   it cannot be used to get a Location for a Comment.
   */
  const Location& locateAst(Context* context, const uast::ASTNode* ast);

  using ModuleVec = std::vector<const uast::Module*>;
  /**
   This query returns a vector of parsed modules given a file path.
   */
  const ModuleVec& parse(Context* context, UniqueString path);

  /**
   This query parses a toplevel module by name. Returns nullptr
   if no such toplevel module can be found.
   */
  const uast::Module* getToplevelModule(Context* context, UniqueString name);

  /**
   Returns the uast node with the given ID.
   */
  const uast::ASTNode* idToAst(Context* context, ID id);

  /**
   Returns the parent ID given an ID
   */
  const ID& idToParentId(Context* context, ID id);


  // TODO: noteError for an error attached to a particular AST node

} // end namespace parsing
} // end namespace chpl
#endif
