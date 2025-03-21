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

// The order here should be kept in sync with the order in the param list.
// Not all param kinds need methods exposed to python, so not all param kinds are listed here.
// See list in frontend/include/chpl/types/param-classes-list.h

//
// Inside each method table, methods should be listed in alphabetical order
//

CLASS_BEGIN(Param)
CLASS_END(Param)

CLASS_BEGIN(BoolParam)
  PLAIN_GETTER(BoolParam, value, "Get the value of this boolean Param",
               bool, return node->value())
CLASS_END(BoolParam)

CLASS_BEGIN(EnumParam)
  PLAIN_GETTER(EnumParam, value, "Get the value of this enum Param",
               const chpl::uast::AstNode*, return parsing::idToAst(context, node->value().id))
CLASS_END(EnumParam)

CLASS_BEGIN(IntParam)
  PLAIN_GETTER(IntParam, value, "Get the value of this integer Param",
               int, return node->value())
CLASS_END(IntParam)

CLASS_BEGIN(StringParam)
  PLAIN_GETTER(StringParam, value, "Get the value of this string Param",
               chpl::UniqueString, return node->value())
CLASS_END(StringParam)
