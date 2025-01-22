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

#include "core-types-gen.h"
#include "resolution.h"
#include "chpl/uast/all-uast.h"
#include "python-types.h"
#include "chpl/framework/query-impl.h"
#include "chpl/framework/ErrorWriter.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/util/version-info.h"

using namespace chpl;
using namespace uast;

/* In this file are generated method and struct definitions for each AST node using
   the X-Macros pattern. Each 'include' of a header file, either uast-classes-list.h
   or method-tables.h, is used to generate a bunch of similar code for many
   classes. */

/* First, generate a Node_init for each type of node in the Dyno AST,
   implemented in the DEFINE_INIT_FOR macro.

   We particularly want this to make sure we call the AstNode constructor,
   which sets the context object etc.
 */
#define DEFINE_INIT_FOR(NAME, TAG)\
  int NAME##Object_init(NAME##Object* self, PyObject* args, PyObject* kwargs) { \
    auto func = ((int(*)(PyObject*, PyObject*, PyObject*)) PyType_GetSlot(parentTypeFor(TAG), Py_tp_init)); \
    if (!func) { \
      PyErr_SetString(PyExc_TypeError, "Cannot initialize Chapel AST node"); \
      return -1; \
    } \
    return func((PyObject*) self, args, kwargs); \
  } \

/* Use the X-macros pattern to invoke DEFINE_INIT_FOR for each AST node type. */
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) DEFINE_INIT_FOR(NAME, TAG)
#include "generated-types-list.h"

static const char* blockStyleToString(BlockStyle blockStyle) {
  switch (blockStyle) {
    case BlockStyle::EXPLICIT: return "explicit";
    case BlockStyle::IMPLICIT: return "implicit";
    case BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK: return "unnecessary";
    default: return "";
  }
}

static const char* opKindToString(Range::OpKind kind) {
  switch (kind) {
    case Range::DEFAULT: return "..";
    case Range::OPEN_HIGH: return "..<";
    default: return "";
  }
}

static std::optional<chpl::Location> getValidLocation(const chpl::Location& loc) {
  /*isEmpty doesn't work since that only relies upon path, which is set*/
  if (loc.line() != -1) {
    return loc;
  }
  return std::nullopt;
}

template <typename T> struct InvokeHelper {};

template <typename Ret, typename... Args>
struct InvokeHelper<Ret(Args...)> {
  template <typename F>
  static PyObject* invoke(ContextObject* contextObject, F&& fn) {
    auto result = fn();
    return PythonFnHelper<Ret(Args...)>::ReturnTypeInfo::wrap(contextObject, std::move(result));
  }
};

template <typename... Args>
struct InvokeHelper<void(Args...)> {
  template <typename F>
  static PyObject* invoke(ContextObject* contextObject, F&& fn) {
    fn();
    // In python3.12, Py_None is immortal and this is not needed
    Py_INCREF(Py_None);
    return Py_None;
  }
};

/* The METHOD macro is overridden here to actually create a Python-compatible
   function to insert into the method table. Each such function retrieves
   a node's context object, calls the method body, and wraps the result
   in a Python-compatible type.
 */
#define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY)\
  PyObject* NODE##Object_##NAME(PyObject *self, PyObject *argsTup) {\
    auto* node = ((NODE##Object*) self)->unwrap(); \
    if (!node) return nullptr; \
    auto contextObject = ((NODE##Object*) self)->context(); \
    auto context = &contextObject->value_; \
    auto args = PythonFnHelper<TYPEFN>::unwrapArgs(contextObject, argsTup); \
    return InvokeHelper<TYPEFN>::invoke(contextObject, \
      [&node, context, contextObject, &args]() -> PythonFnHelper<TYPEFN>::ReturnType { \
        (void) context, contextObject; \
        BODY; \
      }); \
  }

/* Call METHOD on each method in the method-tables.h header to generate
   the Node_method(...) functions. */
#include "method-tables.h"

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

/* Having generated the method calls and the method tables, we can now
   generate the Python type objects for each AST node. The DEFINE_PY_TYPE_FOR
   macro defines what a type object for an AST node (abstract or not) should
   look like. */

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
    TYPE = (PyTypeObject*)PyType_FromSpecWithBases(&spec, (PyObject*)parentType); \
    if (!TYPE || PyType_Ready(TYPE) < 0) return false; \
  } while(0);

bool setupGeneratedTypes() {
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) INITIALIZE_PY_TYPE_FOR(NAME, NAME##Type, TAG, FLAGS)
#include "generated-types-list.h"
return true;
}
