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

#ifndef CHPL_FRONTEND_FRONTEND_QUERIES_H
#define CHPL_FRONTEND_FRONTEND_QUERIES_H

#include "chpl/queries/Context.h"
#include "chpl/queries/ID.h"
#include "chpl/queries/Location.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Module.h"

#include <vector>

namespace chpl {
namespace frontend {


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
  const FileContents& fileText(Context* context, UniqueString path);
  void setFileText(Context* context, UniqueString path, FileContents result);
  void setFileText(Context* context, UniqueString path, std::string text);

  const uast::Builder::Result& parseFile(Context* context, UniqueString path);

  using LocationsMap = std::unordered_map<ID, Location>;
  const LocationsMap& fileLocations(Context* context, UniqueString path);

  // These functions can't return the Location for a Comment
  // because Comments don't have IDs. If Locations for Comments are needed,
  // instead use the locations field from the result of parseFile.
  const Location& locateID(Context* context, ID id);
  const Location& locate(Context* context, const uast::ASTNode* ast);

  using ModuleVec = std::vector<const uast::Module*>;
  const ModuleVec& parse(Context* context, UniqueString path);


} // end namespace frontend
} // end namespace chpl
#endif
