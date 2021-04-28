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

#ifndef CHPL_FRONTEND_PARSER_H
#define CHPL_FRONTEND_PARSER_H

#include "chpl/AST/ASTBase.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Exp.h"
#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Location.h"
#include "chpl/Util/memory.h"

#include <vector>

namespace chpl {
namespace frontend {

/**
  A class for parsing
 */
class Parser final {
 private:
   // TODO: stuff to do with module search paths
   // and then connect parsed modules to a query

   // TODO: compile-time configuration variable settings
   // need to be stored in here.

   Context* context_;
   Parser(Context* context);

 public:
   static owned<Parser> build(Context* context);
   ~Parser() = default;

   /**
     Return the AST Context used by this Parser.
    */
   Context* context() { return context_; }

   /**
     Parse a file at a particular path.
    */
   uast::Builder::Result parseFile(const char* path);
   /**
     Parse source code in a string.
     'path' is only used for certain errors.
    */
   uast::Builder::Result parseString(const char* path, const char* str);
};

} // end namespace frontend
} // end namespace chpl

#endif
