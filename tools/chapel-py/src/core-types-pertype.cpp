/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

#include "core-types.h"
#include "chpl/types/all-types.h"
#include "python-types.h"

using namespace chpl;
using namespace types;

/* In this file are generated method and struct definitions for each C++
   class corresponding to a Chapel type. This is done using
   the X-Macros pattern. Each 'include' of a header file, either type-classes-list.h
   or method-tables.h, is used to generate a bunch of similar code for many
   classes. */

/* First, generate a Type_init for each Chapel type in Dyno,
   implemented in the DEFINE_INIT_FOR macro.

   We particularly want this to make sure we call the ChapelType constructor,
   which sets the context object etc.
 */
#define DEFINE_INIT_FOR(NAME, TAG)\
  int NAME##Object_init(NAME##Object* self, PyObject* args, PyObject* kwargs) { \
    return parentTypeFor(typetags::TAG)->tp_init((PyObject*) self, args, kwargs); \
  } \

#define TYPE_NODE(NAME) DEFINE_INIT_FOR(NAME, NAME)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME) DEFINE_INIT_FOR(NAME, NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME) DEFINE_INIT_FOR(NAME, START_##NAME)
#define TYPE_END_SUBCLASSES(NAME)
#include "chpl/types/type-classes-list.h"
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef DEFINE_INIT_FOR

/* See the explanation in core-types-pernode.cpp (where a similar template
   is defined) for an explanation of how we use this to easily extend
   methods for each Python type we declare. */
template <typetags::TypeTag tag>
struct PerNodeInfo {
  static constexpr PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}  /* Sentinel */
  };
};

#define DEFINE_PY_TYPE_FOR(NAME, TAG, FLAGS)\
  PyTypeObject NAME##Type = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
  }; \

/* Now, invoke DEFINE_PY_TYPE_FOR for each AST node to get our type objects. */
#define TYPE_NODE(NAME) DEFINE_PY_TYPE_FOR(NAME, typetags::NAME, Py_TPFLAGS_DEFAULT)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME) DEFINE_PY_TYPE_FOR(NAME, typetags::NAME, Py_TPFLAGS_DEFAULT)
#define TYPE_BEGIN_SUBCLASSES(NAME) DEFINE_PY_TYPE_FOR(NAME, typetags::NAME, Py_TPFLAGS_BASETYPE)
#define TYPE_END_SUBCLASSES(NAME)
#include "chpl/types/type-classes-list.h"
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef DEFINE_PY_TYPE_FOR

#define INITIALIZE_PY_TYPE_FOR(NAME, TYPE, TAG, FLAGS)\
  TYPE.tp_name = #NAME; \
  TYPE.tp_basicsize = sizeof(NAME##Object); \
  TYPE.tp_itemsize = 0; \
  TYPE.tp_flags = FLAGS; \
  TYPE.tp_doc = PyDoc_STR("A Chapel " #NAME " type"); \
  TYPE.tp_methods = (PyMethodDef*) PerNodeInfo<TAG>::methods; \
  TYPE.tp_base = parentTypeFor(TAG); \
  TYPE.tp_init = (initproc) NAME##Object_init; \
  TYPE.tp_new = PyType_GenericNew; \

void setupPerTypeTypes() {
#define TYPE_NODE(NAME) INITIALIZE_PY_TYPE_FOR(NAME, NAME##Type, typetags::NAME, Py_TPFLAGS_DEFAULT)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME) INITIALIZE_PY_TYPE_FOR(NAME, NAME##Type, typetags::NAME, Py_TPFLAGS_DEFAULT)
#define TYPE_BEGIN_SUBCLASSES(NAME) INITIALIZE_PY_TYPE_FOR(NAME, NAME##Type, typetags::START_##NAME, Py_TPFLAGS_BASETYPE)
#define TYPE_END_SUBCLASSES(NAME)
#include "chpl/types/type-classes-list.h"
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef INITIALIZE_PY_TYPE_FOR
}
