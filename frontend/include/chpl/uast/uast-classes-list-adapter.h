/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

// this is a convenient wrapper around uast-classes-list.h. See that file
// for documentation on how to use it. This file simplifies using the
// header by auto-defining undefined X-macros to sensible defaults, and
// by clearing the macro definitions at the end of the file.

#ifndef AST_NODE
#define AST_NODE(NAME)
#endif

#ifndef AST_LEAF
#define AST_LEAF(NAME) AST_NODE(NAME)
#endif

#ifndef AST_BEGIN_SUBCLASSES
#define AST_BEGIN_SUBCLASSES(NAME) AST_NODE(NAME)
#endif

#ifndef AST_END_SUBCLASSES
#define AST_END_SUBCLASSES(NAME)
#endif

#include "uast-classes-list.h"

#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
