/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

#include "core-types-gen-help.h"

/* In this file are generated struct definitions for each AST node using
   the X-Macros pattern. Each 'include' of a header file, such
   as generated-types-list.h, is used to generate a bunch of similar code for many
   classes.

   The C++ implemenations for the methods are generated in individual
   files corresponding to the method table, to reduce the memory usage
   while compiling. E.g., uast-methods.h has a corresponding uast-methods.cpp.
*/

/* First, generate a Node_init for each type of class we generate from Dyno,
   implemented in the DEFINE_INIT_FOR macro.

   We particularly want this to make sure we call the AstNode constructor,
   which sets the context object etc.
 */
#define DEFINE_INIT_FOR(NAME, TAG) \
  int NAME##Object_init(NAME##Object* self, PyObject* args, PyObject* kwargs) { \
    return callPyTypeSlot_tp_init(parentTypeFor(TAG), (PyObject*) self, args, kwargs); \
  }

/* Use the X-macros pattern to invoke DEFINE_INIT_FOR for each AST node type. */
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) DEFINE_INIT_FOR(NAME, TAG)
#include "generated-types-list.h"

/* Helper macro to set up actual iterators. Needs to be a macro because nodes
   that have actuals don't all share a parent class (Attribute vs FnCall, e.g.).
   */
#define ACTUAL_ITERATOR(NAME)\
  PyObject* NAME##Object_actuals(PyObject *self, PyObject *Py_UNUSED(ignored)) { \
    auto node = ((NAME##Object*) self)->unwrap(); \
    if (!node) return nullptr; \
    \
    auto argList = Py_BuildValue("(O)", (PyObject*) self); \
    auto astCallIterObjectPy = PyObject_CallObject((PyObject *) AstCallIterType, argList); \
    Py_XDECREF(argList); \
    \
    auto astCalliterObject = (AstCallIterObject*) astCallIterObjectPy; \
    astCalliterObject->current = 0; \
    astCalliterObject->num = node->numActuals(); \
    astCalliterObject->container = node; \
    astCalliterObject->childGetter = [](const void* n, int child) { \
      return ((NAME*) n)->actual(child); \
    }; \
    astCalliterObject->nameGetter = [](const void* n, int child) { \
      return ((NAME*) n)->actualName(child); \
    }; \
    \
    return astCallIterObjectPy; \
  }

ACTUAL_ITERATOR(Attribute);
ACTUAL_ITERATOR(FnCall);

/* Having generated the initializers and iterators, we can now
   generate the Python type objects for each generated type . The DEFINE_PY_TYPE_FOR
   macro declares the PyTypeObject*. Later, we dynamically
   construct the object in INITIALIZE_PY_TYPE_FOR and set this
   variable with what a type object for a Dyno class (abstract or not) should
   look like. Doing so dynamically makes it possible to use Python's stable
   ABI, and not link against a particular version. */
#define DEFINE_PY_TYPE_FOR(NAME) PyTypeObject* NAME##Type = NULL;

/* Now, invoke DEFINE_PY_TYPE_FOR for each AST node to get our type objects. */
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) DEFINE_PY_TYPE_FOR(NAME)
#include "generated-types-list.h"

#define INITIALIZE_PY_TYPE_FOR(NAME, TYPE, TAG, FLAGS)\
  do { \
    PyType_Slot slots[] = { \
      {Py_tp_doc, (void*) PyDoc_STR("A Chapel " #NAME " AST node")}, \
      {Py_tp_methods, (void*) PerTypeMethods<NAME##Object>::methods}, \
      {Py_tp_init, (void*) NAME##Object_init}, \
      {Py_tp_new, (void*) PyType_GenericNew}, \
      {0, nullptr} \
    }; \
    PyType_Spec spec = { \
      /*name*/ "chapel."#NAME, \
      /*basicsize*/ sizeof(NAME##Object), \
      /*itemsize*/ 0, \
      /*flags*/ FLAGS, \
      /*slots*/ slots \
    }; \
    auto parentType = parentTypeFor(TAG); \
    auto bases = PyTuple_Pack(1, parentType); \
    TYPE = (PyTypeObject*)PyType_FromSpecWithBases(&spec, bases); \
    if (!TYPE || PyType_Ready(TYPE) < 0) return false; \
  } while(0);

bool setupGeneratedTypes() {
  #define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) INITIALIZE_PY_TYPE_FOR(NAME, NAME##Type, TAG, FLAGS)
  #include "generated-types-list.h"
  return true;
}
