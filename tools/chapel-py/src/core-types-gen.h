/*
 * Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHAPEL_PY_CORE_TYPES_GEN_H
#define CHAPEL_PY_CORE_TYPES_GEN_H

#include "core-types.h"
#include "error-tracker.h"
#include "chpl/uast/all-uast.h"
#include "python-types.h"

/**
  Declare a Python PyTypeObject that corresponds to a generated type
  (AST node, Chapel type, etc.) of a given name.
 */
#define DECLARE_PY_OBJECT_FOR(ROOT, NAME)\
  struct NAME##Object { \
    ROOT##Object parent; \
  \
    static constexpr const char* QualifiedName = "chapel." #NAME; \
    static constexpr const char* Name = #NAME; \
    static constexpr const char* DocStr = "An object that represents a Chapel " #NAME; \
  \
    const auto unwrap() const { \
      if (parent.value_) { \
        PyErr_Format(PyExc_RuntimeError, \
                     "invalid instance of class '%s'; please do not manually construct instances of this class.", \
                      #NAME); \
        return static_cast<decltype(parent.value_->to##NAME())>(nullptr); \
      } \
      return parent.value_->to##NAME(); \
    } \
    ContextObject* context() const { return (ContextObject*) parent.contextObject; } \
  }; \
  \
  extern PyTypeObject NAME##Type;

/* Generate a Python object for reach AST node type. */
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) DECLARE_PY_OBJECT_FOR(ROOT, NAME)
#include "generated-types-list.h"
#undef DECLARE_PY_OBJECT_FOR

/* Forward declare methods declared for each AST node. */
#define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY)\
  PyObject* NODE##Object_##NAME(PyObject *self, PyObject *argsTup);
#define METHOD_PROTOTYPE(NODE, NAME, DOCSTR) \
  PyObject* NODE##Object_##NAME(PyObject *self, PyObject *argsTup);
#include "method-tables.h"

/* The following code is used to help set up the (Python) method tables for
   each Chapel class. Each node class needs a table, but not all
   classes have Python methods we want to expose. We thus want to default
   to an empty method table (to save on typing / boilerplate), but at the same
   time to make it easy to override a node's method table.

   To this end, we use template specialization of the PerTypeMethods struct. The
   default template provides an empty table; it can be specialized per-node to
   change the table for that node.

   Macros below take this a step further and compiler-generate the template
   specializations. */

template <typename ObjectType>
struct PerTypeMethods {
  static constexpr PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}  /* Sentinel */
  };
};

#define CLASS_BEGIN(NAME) \
  template <> \
  struct PerTypeMethods<NAME##Object> { \
    static constexpr PyMethodDef methods[] = {
#define CLASS_END(NAME) \
      {NULL, NULL, 0, NULL}  /* Sentinel */ \
    }; \
  };
#define METHOD(NODE, NAME, DOCSTR, TYPE, BODY) \
  {#NAME, NODE##Object_##NAME, PythonFnHelper<TYPE>::PyArgTag, DOCSTR},
#define METHOD_PROTOTYPE(NODE, NAME, DOCSTR) \
  {#NAME, NODE##Object_##NAME, METH_NOARGS, DOCSTR},
#include "method-tables.h"

void setupGeneratedTypes();

#endif
