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

#ifndef CHAPEL_PY_CORE_TYPES_H
#define CHAPEL_PY_CORE_TYPES_H

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/AstTag.h"
#include "error-tracker.h"

PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag);
PyTypeObject* parentTypeFor(chpl::types::typetags::TypeTag tag);
PyTypeObject* parentTypeFor(chpl::types::paramtags::ParamTag tag);

template <typename ObjectType>
struct PerTypeMethods;

struct ContextObject;

template <typename Self, typename T>
struct PythonClass {
  PyObject_HEAD
  T value_;

  /** ===== Support for the method-tables API ===== */

  using UnwrappedT = typename std::conditional<std::is_pointer_v<T>, T, T*>::type;

  // If T is a pointer, return it by value, since copying pointers is cheap.
  template <typename Q = T>
  typename std::enable_if<std::is_pointer_v<Q>, UnwrappedT>::type unwrapImpl() {
    return value_;
  }

  // If T is a not a pointer, return it by pointer, since values could be large.
  template <typename Q = T>
  typename std::enable_if<!std::is_pointer_v<Q>, UnwrappedT>::type unwrapImpl() {
    return &value_;
  }

  // If T is a pointer, allow ::create() to be called with nullptr, and
  // just return None.
  template <typename Q = T>
  static typename std::enable_if<std::is_pointer_v<Q>, PyObject*>::type returnNoneIfNeeded(const Q& val) {
    if (val == nullptr) {
      Py_RETURN_NONE;
    }
    return nullptr;
  }

  // If T is a not a pointer, we can't detect 'nullptr'.
  template <typename Q = T>
  static typename std::enable_if<!std::is_pointer_v<Q>, PyObject*>::type returnNoneIfNeeded(const Q& val) {
    return nullptr;
  }

  UnwrappedT unwrap() { return unwrapImpl(); }
  ContextObject* context() { return nullptr; }

  /** ===== CPython API support ===== */

  static void dealloc(Self* self) {
    ((PythonClass*) self)->value_.~T();
    Py_TYPE(self)->tp_free((PyObject *) self);
  }

  static int init(Self* self, PyObject* args, PyObject* kwargs) {
    new (&((PythonClass*) self)->value_) T();
    return 0;
  }

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = {
      PyVarObject_HEAD_INIT(NULL, 0)
    };
    configuring.tp_name = Self::Name;
    configuring.tp_basicsize = sizeof(Self);
    configuring.tp_itemsize = 0;
    configuring.tp_dealloc = (destructor) Self::dealloc;
    configuring.tp_flags = Py_TPFLAGS_DEFAULT;
    configuring.tp_doc = PyDoc_STR(Self::DocStr);
    configuring.tp_methods = (PyMethodDef*) PerTypeMethods<Self>::methods;
    configuring.tp_init = (initproc) Self::init;
    configuring.tp_new = PyType_GenericNew;
    return configuring;
  }

  static PyTypeObject PythonType;

  static int ready() {
    return PyType_Ready(&Self::PythonType);
  }

  static int addToModule(PyObject* mod) {
    return PyModule_AddObject(mod, Self::Name, (PyObject*) &Self::PythonType);
  }

  /** ===== Public convenience methods for using this object ===== */

  static PyObject* create(T createFrom) {
    if (auto obj = PythonClass<Self, T>::returnNoneIfNeeded(createFrom)) {
      return obj;
    }

    auto selfObjectPy = PyObject_CallObject((PyObject *) &Self::PythonType, nullptr);
    auto& val = ((Self*) selfObjectPy)->value_;

    val = std::move(createFrom);
    return selfObjectPy;
  }
};

template <typename Self, typename T>
PyTypeObject PythonClass<Self,T>::PythonType = Self::configurePythonType();

struct ContextObject : public PythonClass<ContextObject, chpl::Context> {
  static constexpr const char* Name = "Context";
  static constexpr const char* DocStr = "The Chapel context object that tracks various frontend state";

  // override init to set CHPL_HOME and install an error handler.
  static int init(ContextObject* self, PyObject* args, PyObject* kwargs);

  // override context() because this object is expected to return itself.
  ContextObject* context() { return this; }
};

struct LocationObject : public PythonClass<LocationObject, chpl::Location> {
  static constexpr const char* Name = "Location";
  static constexpr const char* DocStr = "An object that represents the location of an AST node in a source file.";
};

using LineColumnPair = std::tuple<int, int>;

template <typename Self, typename T>
struct PythonClassWithObject : public PythonClass<Self, T> {
  PyObject* contextObject;

  static void dealloc(Self* self) {
    Py_DECREF(self->contextObject);
    PythonClass<Self, T>::dealloc(self);
  }

  static int init(Self* self, PyObject* args, PyObject* kwargs) {
    PyObject* contextObjectPy;
    if (!PyArg_ParseTuple(args, "O", &contextObjectPy))
        return -1;

    if (contextObjectPy->ob_type != &ContextObject::PythonType) {
      PyErr_SetString(PyExc_TypeError, "Expected a chapel.Context object as the only argument.");
      return -1;
    }

    Py_INCREF(contextObjectPy);
    self->contextObject = contextObjectPy;
    return PythonClass<Self, T>::init(self, args, kwargs);
  }

  ContextObject* context() { return (ContextObject*) contextObject; }

  static PyObject* create(T createFrom) = delete; /* Don't you mention Liskov to me! */
  static PyObject* create(ContextObject* context, T createFrom) {
    if (auto obj = PythonClass<Self, T>::returnNoneIfNeeded(createFrom)) {
      return obj;
    }

    PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
    auto selfObjectPy = PyObject_CallObject((PyObject *) &Self::PythonType, args);
    auto& val = ((Self*) selfObjectPy)->value_;

    val = std::move(createFrom);
    return selfObjectPy;
  }
};

struct ScopeObject : public PythonClassWithObject<ScopeObject, const chpl::resolution::Scope*> {
  static constexpr const char* Name = "Scope";
  static constexpr const char* DocStr = "A scope in the Chapel program, such as a block.";
};

struct AstNodeObject : public PythonClassWithObject<AstNodeObject, const chpl::uast::AstNode*> {
  static constexpr const char* Name = "AstNode";
  static constexpr const char* DocStr = "The base type of Chapel AST nodes";

  static PyObject* iter(AstNodeObject *self);

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = PythonClassWithObject<AstNodeObject, const chpl::uast::AstNode*>::configurePythonType();
    configuring.tp_iter = (getiterfunc) AstNodeObject::iter;
    configuring.tp_flags = Py_TPFLAGS_BASETYPE;
    return configuring;
  }
};

using QualifiedTypeTuple = std::tuple<const char*, const chpl::types::Type*, const chpl::types::Param*>;

struct ChapelTypeObject  : public PythonClassWithObject<ChapelTypeObject, const chpl::types::Type*> {
  static constexpr const char* Name = "ChapelType";
  static constexpr const char* DocStr = "The base type of Chapel types";

  static PyObject* str(ChapelTypeObject* self);

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = PythonClassWithObject<ChapelTypeObject, const chpl::types::Type*>::configurePythonType();
    configuring.tp_str = (reprfunc) ChapelTypeObject::str;
    configuring.tp_flags = Py_TPFLAGS_BASETYPE;
    return configuring;
  }
};

struct ParamObject : public PythonClassWithObject<ParamObject, const chpl::types::Param*> {
  static constexpr const char* Name = "Param";
  static constexpr const char* DocStr = "The base type of Chapel parameters (compile-time known values)";

  static PyObject* str(ParamObject* self);

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = PythonClassWithObject<ParamObject, const chpl::types::Param*>::configurePythonType();
    configuring.tp_str = (reprfunc) ParamObject::str;
    configuring.tp_flags = Py_TPFLAGS_BASETYPE;
    return configuring;
  }
};

template<typename IntentType>
const char* intentToString(IntentType intent) {
  return qualifierToString(chpl::uast::Qualifier(int(intent)));
}

/**
 Using the various definitions, templates, and method tables we have, generate
 a Python .pyi file describing the various methods and types we provide.
 */
std::string generatePyiFile();

/**
  Create a Python object of the class corresponding to the given AST node's
  type. For example, an Identifier node will be wrapped in a chapel.Identifier.
 */
PyObject* wrapAstNode(ContextObject* context, const chpl::uast::AstNode* node);

/**
  Create a Python object of the class corresponding to the given Type*.
  For example, an ArrayType type will be wrapped in a chapel.ArrayType.
 */
PyObject* wrapType(ContextObject* context, const chpl::types::Type* node);

/**
  Creates a Python object of the class corresponding to the given Param*.
 */
PyObject* wrapParam(ContextObject* context, const chpl::types::Param* node);

#endif
