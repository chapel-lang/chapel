/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CALL_INIT_DEINIT_H
#define CALL_INIT_DEINIT_H

#include <tuple>
#include "chpl/uast/AstNode.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace resolution {


struct Resolver;

std::tuple<CallInfo, CallScopeInfo>
setupCallForCopyOrMove(Resolver& resolver,
                       const uast::AstNode* ast,
                       const uast::AstNode* rhsAst,
                       const types::QualifiedType& lhsType,
                       const types::QualifiedType& rhsType,
                       bool forMoveInit,
                       std::vector<const uast::AstNode*>& outAsts);

/* Add calls to default init, copy init, deinit, etc to the Resolver's
   results after analyzing for split-init and copy-elision. */
void callInitDeinit(Resolver& resolver);


} // end namespace resolution
} // end namespace chpl

#endif
