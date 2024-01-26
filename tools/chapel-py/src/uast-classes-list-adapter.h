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

#ifndef GENERATED_TYPE
#define GENERATED_TYPE(NAME, TAG, FLAGS)
#endif

#ifndef GENERATED_TYPE_BEGIN
#define GENERATED_TYPE_BEGIN(NAME, TAG, FLAGS)
#endif

#ifndef GENERATED_TYPE_END
#define GENERATED_TYPE_END(NAME, TAG, FLAGS)
#endif

#define AST_NODE(NAME) GENERATED_TYPE(NAME, chpl::uast::asttags::NAME, Py_TPFLAGS_DEFAULT)
#define AST_LEAF(NAME) GENERATED_TYPE(NAME, chpl::uast::asttags::NAME, Py_TPFLAGS_DEFAULT)
#define AST_BEGIN_SUBCLASSES(NAME) GENERATED_TYPE(NAME, chpl::uast::asttags::START_##NAME, Py_TPFLAGS_BASETYPE) GENERATED_TYPE_BEGIN(NAME, chpl::uast::asttags::START_##NAME, Py_TPFLAGS_BASETYPE)
#define AST_END_SUBCLASSES(NAME) GENERATED_TYPE_END(NAME, chpl::uast::asttags::END_##NAME, Py_TPFLAGS_BASETYPE)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES

#undef GENERATED_TYPE
#undef GENERATED_TYPE_BEGIN
#undef GENERATED_TYPE_END
