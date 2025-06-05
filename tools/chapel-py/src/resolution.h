/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/AstNode.h"
#include "chpl/types/Type.h"
#include "chpl/resolution/resolution-types.h"

const chpl::resolution::ResolvedExpression*
scopeResolveResultsForNode(chpl::Context* context, const chpl::uast::AstNode* node);

const chpl::resolution::ResolvedExpression*
resolveResultsForNode(chpl::Context* context, const chpl::uast::AstNode* node);

const chpl::uast::AstNode* const&
nodeOrNullFromToId(chpl::Context* context, const chpl::uast::AstNode* node);

chpl::types::QualifiedType const&
typeForNode(chpl::Context* context, const chpl::uast::AstNode* node);

const chpl::uast::AstNode* const&
calledFnForNode(chpl::Context* context, const chpl::uast::AstNode* node);

std::vector<int> const&
actualOrderForNode(chpl::Context* context, const chpl::uast::AstNode* node);
