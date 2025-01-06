/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_PARSING_PARSER_H
#define CHPL_PARSING_PARSER_H

#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Builder.h"
#include "chpl/util/memory.h"
#include "chpl/parsing/parser-stats.h"

#include <vector>

namespace chpl {
namespace parsing {

/**
  A class for parsing
*/
class Parser final {
 private:
  Context* context_;
  UniqueString parentSymbolPath_;

  Parser(Context* context, UniqueString parentSymbolPath);

 public:
  /** Construct a parser for parsing a top-level module */
  static Parser createForTopLevelModule(Context* context);

  /** Construct a parser for parsing an included module.
      'parentSymbolPath' is the symbol path component of the ID
      of the module containing the 'module include' statement.
   */
  static Parser createForIncludedModule(Context* context,
                                        UniqueString parentSymbolPath);

  ~Parser() = default;

  /**
    Return the AST Context used by this Parser.
  */
  Context* context() { return context_; }

  /**
   Parse a file at a particular path.
  */
  uast::BuilderResult parseFile(const char* path,
                                ParserStats* parseStats=nullptr);
  /**
   Parse source code in a string.
   'path' is only used for certain errors.
  */
  uast::BuilderResult parseString(const char* path, const char* str,
                                  ParserStats* parseStats=nullptr);
};

} // end namespace parsing
} // end namespace chpl

#endif
