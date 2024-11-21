/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include <Python.h>

const uint64_t chpl_PY_VERSION_HEX = PY_VERSION_HEX;
const char* chpl_PY_VERSION = PY_VERSION;

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

static inline int chpl_PyList_Check(PyObject* o) { return PyList_Check(o); }
static inline int chpl_PyGen_Check(PyObject* o) { return PyGen_Check(o); }

static inline PyObject* chpl_Py_None(void) { return (PyObject*)Py_None; }
static inline PyObject* chpl_Py_True(void) { return (PyObject*)Py_True; }
static inline PyObject* chpl_Py_False(void) { return (PyObject*)Py_False; }

#endif
