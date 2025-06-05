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

#ifndef CHAPEL_PY_PYTHON_TYPE_HELPER_H
#define CHAPEL_PY_PYTHON_TYPE_HELPER_H

#include "PythonWrapper.h"

static inline void callPyTypeSlot_tp_free(PyTypeObject* type, PyObject* self) {
  auto tp_free = (freefunc)PyType_GetSlot(type, Py_tp_free);
  if (!tp_free) {
    PyErr_SetString(PyExc_RuntimeError, "Could not free object");
    return;
  }
  tp_free(self);
}
static inline int callPyTypeSlot_tp_init(PyTypeObject* type, PyObject* self, PyObject* args, PyObject* kwargs) {
  auto func = (initproc)PyType_GetSlot(type, Py_tp_init);
  if (!func) {
    PyErr_SetString(PyExc_TypeError, "Cannot initialize object");
    return -1;
  }
  return func((PyObject*) self, args, kwargs);
}


static inline const char* getCStringFromPyObjString(PyObject* obj) {
  const char* str;
  if (!PyArg_Parse(obj, "s", &str)) {
    PyErr_SetString(PyExc_RuntimeError, "Could not get string from object");
    return nullptr;
  }
  return str;
}

static const char* getTypeName(PyTypeObject* obj) {
  PyObject* name_attr = PyObject_GetAttrString((PyObject*) obj, "__name__");
  if (!name_attr) {
    PyErr_SetString(PyExc_RuntimeError, "Could not get type name");
    return nullptr;
  }
  const char* name = getCStringFromPyObjString(name_attr);
  Py_XDECREF(name_attr);
  return name;
}

// In python3.12, Py_None is immortal and this is not needed
#define chpl_PY_RETURN_NONE \
  do { \
    Py_INCREF(Py_None); \
    return Py_None; \
  } while(0)

#endif
