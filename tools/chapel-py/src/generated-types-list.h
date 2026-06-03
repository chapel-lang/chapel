/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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

/*
 The macros have the following arguments:

  1. ROOT: The name of the (Python) root type for the type hierarchy (e.g. AstNode).
  2. ROOT_TYPE: The C++ type (without pointers) of values contained in the type hiearrchy (e.g. chpl::uast::AstNode).
  3. NAME: The name of the type (e.g. Module).
  4. TYPE: The C++ type (without pointers) for the generated type (e.g. chpl::uast::Module).
  5. TAG: The name of the tag for the type (e.g. chpl::uast::asttags::Module).
  6. FLAGS: The Python flags for the type (e.g. Py_TPFLAGS_DEFAULT).
 */

#ifndef GENERATED_TYPE
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS)
#endif

#ifndef GENERATED_TYPE_BEGIN
#define GENERATED_TYPE_BEGIN(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS)
#endif

#ifndef GENERATED_TYPE_END
#define GENERATED_TYPE_END(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS)
#endif

#define TYPE_NODE(NAME) GENERATED_TYPE(ChapelType, chpl::types::Type, NAME, chpl::types::NAME, chpl::types::typetags::NAME, Py_TPFLAGS_DEFAULT)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME) GENERATED_TYPE(ChapelType, chpl::types::Type, NAME, chpl::types::NAME, chpl::types::typetags::NAME, Py_TPFLAGS_DEFAULT)
#define TYPE_BEGIN_SUBCLASSES(NAME) GENERATED_TYPE(ChapelType, chpl::types::Type, NAME, chpl::types::NAME, chpl::types::typetags::START_##NAME, Py_TPFLAGS_BASETYPE) GENERATED_TYPE_BEGIN(ChapelType, chpl::types::Type, NAME, chpl::types::NAME, chpl::types::typetags::START_##NAME, Py_TPFLAGS_BASETYPE)
#define TYPE_END_SUBCLASSES(NAME) GENERATED_TYPE_END(ChapelType, chpl::types::Type, NAME, chpl::types::NAME, chpl::types::typetags::END_##NAME, Py_TPFLAGS_BASETYPE)
#include "chpl/types/type-classes-list.h"
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES

#define AST_NODE(NAME) GENERATED_TYPE(AstNode, chpl::uast::AstNode, NAME, chpl::uast::NAME, chpl::uast::asttags::NAME, Py_TPFLAGS_DEFAULT)
#define AST_LEAF(NAME) GENERATED_TYPE(AstNode, chpl::uast::AstNode, NAME, chpl::uast::NAME, chpl::uast::asttags::NAME, Py_TPFLAGS_DEFAULT)
#define AST_BEGIN_SUBCLASSES(NAME) GENERATED_TYPE(AstNode, chpl::uast::AstNode, NAME, chpl::uast::NAME, chpl::uast::asttags::START_##NAME, Py_TPFLAGS_BASETYPE) GENERATED_TYPE_BEGIN(AstNode, chpl::uast::AstNode, NAME, chpl::uast::NAME, chpl::uast::asttags::START_##NAME, Py_TPFLAGS_BASETYPE)
#define AST_END_SUBCLASSES(NAME) GENERATED_TYPE_END(AstNode, chpl::uast::AstNode, NAME, chpl::uast::NAME, chpl::uast::asttags::END_##NAME, Py_TPFLAGS_BASETYPE)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES

#define PARAM_NODE(NAME, TYPE) GENERATED_TYPE(Param, chpl::types::Param, NAME, chpl::types::NAME, chpl::types::paramtags::NAME, Py_TPFLAGS_DEFAULT)
#include "chpl/types/param-classes-list.h"
#undef PARAM_NODE

#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) GENERATED_TYPE(Error, chpl::ErrorBase, NAME, chpl::Error##NAME, chpl::ErrorType::NAME, Py_TPFLAGS_DEFAULT)
GENERATED_TYPE(Error, chpl::ErrorBase, GeneralError, chpl::GeneralError, chpl::ErrorType::General, Py_TPFLAGS_DEFAULT)
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

#undef GENERATED_TYPE
#undef GENERATED_TYPE_BEGIN
#undef GENERATED_TYPE_END
