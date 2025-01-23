/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef CHPL_PYTHON_HELPER_H_
#define CHPL_PYTHON_HELPER_H_

#define PY_SSIZE_T_CLEAN
#include <Python.h>

const uint64_t chpl_PY_VERSION_HEX = PY_VERSION_HEX;
const char* chpl_PY_VERSION = PY_VERSION;
const unsigned long chpl_PY_MAJOR_VERSION = PY_MAJOR_VERSION;
const unsigned long chpl_PY_MINOR_VERSION = PY_MINOR_VERSION;
const unsigned long chpl_PY_MICRO_VERSION = PY_MICRO_VERSION;

static inline PyObject* chpl_PyEval_GetFrameGlobals(void) {
#if PY_VERSION_HEX >= 0x030d0000 /* Python 3.13 */
  return PyEval_GetFrameGlobals();
#else
  return PyEval_GetGlobals();
#endif
}

static inline PyObject* chpl_PyErr_GetRaisedException(void) {
#if PY_VERSION_HEX >= 0x030c0000 /* Python 3.12 */
  return PyErr_GetRaisedException();
#else
  // use PyErr_Fetch
  PyObject* ptype;
  PyObject* pvalue;
  PyObject* ptraceback;
  PyErr_Fetch(&ptype, &pvalue, &ptraceback);
  // for now, don't care about type and traceback
  if (ptype) Py_DECREF(ptype);
  if (ptraceback) Py_DECREF(ptraceback);

  if (pvalue) return pvalue;
  return NULL;
#endif
}

static inline void chpl_Py_INCREF(PyObject* o) { Py_INCREF(o); }
static inline void chpl_Py_DECREF(PyObject* o) { Py_DECREF(o); }
static inline void chpl_Py_CLEAR(PyObject** o) { Py_CLEAR(*o); }

static inline int chpl_PyList_Check(PyObject* o) { return PyList_Check(o); }
static inline int chpl_PyGen_Check(PyObject* o) { return PyGen_Check(o); }

static inline PyObject* chpl_Py_None(void) { return (PyObject*)Py_None; }
static inline PyObject* chpl_Py_True(void) { return (PyObject*)Py_True; }
static inline PyObject* chpl_Py_False(void) { return (PyObject*)Py_False; }


#include <stdint.h>
typedef struct {
  PyObject_HEAD
  int64_t* data;
  Py_ssize_t size;
} ArrayShimObject;
static PyTypeObject* ArrayShimType = NULL;

static int ArrayShimObject_init(ArrayShimObject* self, PyObject* args, PyObject* kwargs) {
  return 0;
}
static void ArrayShimObject_dealloc(ArrayShimObject* self) {
  void (*tp_free)(PyObject*) = (void (*)(PyObject*))(PyType_GetSlot(ArrayShimType, Py_tp_free));
  if (!tp_free) PyErr_SetString(PyExc_RuntimeError, "Could not free object");
  else          tp_free((PyObject*) self);
}
static PyObject* ArrayShimObject_str(ArrayShimObject* self) {
  return PyUnicode_FromFormat("ArrayI64(data: 0x%p, size: %zu)", self->data, self->size);
}
static int ArrayShimObject_setitem(ArrayShimObject* self, Py_ssize_t index, PyObject* value) {
  if (!PyLong_Check(value)) {
    PyErr_SetString(PyExc_TypeError, "ArrayShimObject only supports setting items with integers");
    return -1;
  }
  int64_t val = PyLong_AsLong(value);
  if (val == -1 && PyErr_Occurred()) return -1;
  if (index < 0 || index >= self->size) {
    PyErr_SetString(PyExc_IndexError, "ArrayShimObject index out of range");
    return -1;
  }
  self->data[index] = val;
  return 0;
}
static PyObject* ArrayShimObject_getitem(ArrayShimObject* self, Py_ssize_t index) {
  if (index < 0 || index >= self->size) {
    PyErr_SetString(PyExc_IndexError, "ArrayShimObject index out of range");
    return NULL;
  }
  return PyLong_FromLong(self->data[index]);
}
static Py_ssize_t ArrayShimObject_length(ArrayShimObject* self) {
  return self->size;
}

// should only be called once. returns 0 on failure, 1 on success
static int createArrayShimType(void) {
  PyType_Slot slots[] = {
    {Py_tp_init, (void*) ArrayShimObject_init},
    {Py_tp_dealloc, (void*) ArrayShimObject_dealloc},
    {Py_tp_doc, (void*) PyDoc_STR("A shim to allow Chapel arrays to be used in Python")},
    {Py_tp_new, (void*) PyType_GenericNew},
    {Py_tp_str, (void*) ArrayShimObject_str},
    {Py_sq_ass_item, (void*) ArrayShimObject_setitem},
    {Py_sq_item, (void*) ArrayShimObject_getitem},
    {Py_mp_length, (void*)ArrayShimObject_length},
    {0, NULL}
  };
  PyType_Spec spec = {
    /*name*/ "ArrayI64", // TODO: qualified name
    /*basicsize*/ sizeof(ArrayShimObject),
    /*itemsize*/ 0,
    /*flags*/ Py_TPFLAGS_DEFAULT,
    /*slots*/ slots
  };
  ArrayShimType = (PyTypeObject*)PyType_FromSpec(&spec);
  if (!ArrayShimType || PyType_Ready(ArrayShimType) < 0) return 0;
  return 1;
}

static PyObject* createArrayShim(int64_t* data, Py_ssize_t size) {
  PyObject* objPy = PyObject_CallNoArgs((PyObject *) ArrayShimType);
  ArrayShimObject* obj = (ArrayShimObject*) objPy;
  obj->data = data;
  obj->size = size;
  return objPy;
}

#endif
