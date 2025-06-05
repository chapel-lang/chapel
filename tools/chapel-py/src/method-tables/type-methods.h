/*
 * Copyright 2023-2025 Hewlett Packard Enterprise Development LP
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

// See method-tables.h for a top-level description of what the X-macros
// in this file do.

// The order here should be kept in sync with the order in the types list.
// Not all types need methods exposed to python, so not all type classes are listed here.
// See list in frontend/include/chpl/types/type-classes-list.h.

//
// Inside each method table, methods should be listed in alphabetical order
//

CLASS_BEGIN(ChapelType)
CLASS_END(ChapelType)

CLASS_BEGIN(CompositeType)
  PLAIN_GETTER(CompositeType, decl, "Get the chpl::uast::AstNode that declares this CompositeType",
               Nilable<const chpl::uast::AstNode*>,

               // For completely builtin types, the ID could be empty.
               // They don't have code-level declarations, so return 'None'.
               auto& id = node->id();
               if (id.isEmpty()) return nullptr;

               return parsing::idToAst(context, id))
CLASS_END(CompositeType)
