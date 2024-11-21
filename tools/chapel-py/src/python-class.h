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

#ifndef CHAPEL_PY_PYTHON_CLASS_H
#define CHAPEL_PY_PYTHON_CLASS_H
#include "Python.h"
#include "chpl/framework/Context.h"
#include <optional>

template <typename ObjectType>
struct PerTypeMethods;

struct ContextObject;

template <typename Self, typename T>
struct PythonClass {
  PyObject_HEAD
  T value_;

  /** ===== Support for the method-tables API ===== */

  using UnwrappedT = typename std::conditional<std::is_pointer_v<T>, T, T&>::type;

  // If T is a pointer, allow ::create() to be called with nullptr, and
  // just return None.
  template <typename Q = T>
  static typename std::enable_if<std::is_pointer_v<Q>, bool>::type shouldReturnNone(const Q& val) {
    return val == nullptr;
  }

  // If T is a not a pointer, we can't detect 'nullptr'.
  template <typename Q = T>
  static typename std::enable_if<!std::is_pointer_v<Q>, bool>::type shouldReturnNone(const Q& val) {
    return false;
  }

  UnwrappedT unwrap() { return value_; }
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
    // tp_name must be the qualified name
    configuring.tp_name = Self::QualifiedName;
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

  static Self* create(T createFrom) {
    if (PythonClass<Self, T>::shouldReturnNone(createFrom)) {
      PyErr_SetString(PyExc_RuntimeError, "Attempt to create a Python object from nullptr.");
      return nullptr;
    }

    auto selfObjectPy = PyObject_CallObject((PyObject *) &Self::PythonType, nullptr);
    auto& val = ((Self*) selfObjectPy)->value_;

    val = std::move(createFrom);
    return (Self*) selfObjectPy;
  }

  static std::optional<Self*> tryCreate(T createFrom) {
    if (PythonClass<Self, T>::shouldReturnNone(createFrom)) {
      return {};
    }

    return Self::create(std::move(createFrom));
  }
};

template <typename Self, typename T>
PyTypeObject PythonClass<Self,T>::PythonType = Self::configurePythonType();

// Because the ContextObject is essential to other pieces of the API (it's attached
// to many other Chapel objects to save the user the work of threading through
// a Context instance), we need to declare and define it here for the next
// template (PythonClassWithContext) to use.
//
// Forward-declaring doesn't cut it because we need ContextObject::PythonType.

struct ContextObject : public PythonClass<ContextObject, chpl::Context> {
  static constexpr const char* QualifiedName = "chapel.Context";
  static constexpr const char* Name = "Context";
  static constexpr const char* DocStr = "The Chapel context object that tracks various frontend state";

  // override init to set CHPL_HOME and install an error handler.
  static int init(ContextObject* self, PyObject* args, PyObject* kwargs);

  // override context() because this object is expected to return itself.
  ContextObject* context() { return this; }
};

template <typename Self, typename T>
struct PythonClassWithContext : public PythonClass<Self, T> {
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

  static Self* create(T createFrom) = delete; /* Don't you mention Liskov to me! */
  static Self* create(ContextObject* context, T createFrom) {
    if (PythonClass<Self, T>::shouldReturnNone(createFrom)) {
      PyErr_SetString(PyExc_RuntimeError, "Attempt to create a Python object from nullptr.");
      return nullptr;
    }

    PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
    auto selfObjectPy = PyObject_CallObject((PyObject *) &Self::PythonType, args);
    auto& val = ((Self*) selfObjectPy)->value_;

    val = std::move(createFrom);
    return (Self*) selfObjectPy;
  }
  static std::optional<Self*> tryCreate(ContextObject* context, T createFrom) {
    if (PythonClass<Self, T>::shouldReturnNone(createFrom)) {
      return {};
    }
    return create(context, std::move(createFrom));
  }
};


#endif
