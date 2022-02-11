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

#ifndef SCOPE_HELP_H
#define SCOPE_HELP_H

#include "chpl/resolution/scope-types.h"

namespace chpl {
namespace uast {
  class ASTNode;
}
namespace resolution {


void gatherDeclsWithin(const uast::ASTNode* ast,
                       DeclMap& declared,
                       bool& containsUseImport,
                       bool& containsFunctionDecls);


} // end namespace resolution
} // end namespace chpl

#endif
