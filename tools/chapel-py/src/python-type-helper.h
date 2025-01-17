
#ifndef CHAPEL_PY_PYTHON_TYPE_HELPER_H
#define CHAPEL_PY_PYTHON_TYPE_HELPER_H

#include "PythonWrapper.h"

static void call_tp_free(PyTypeObject* type, PyObject* self) {
  auto tp_free = (void (*)(PyObject*))(PyType_GetSlot(type, Py_tp_free));
  if (!tp_free) {
    PyErr_SetString(PyExc_RuntimeError, "Could not free object");
  } else {
    tp_free(self);
  }
}

#endif
