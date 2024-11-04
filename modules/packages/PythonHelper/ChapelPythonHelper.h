#ifndef CHPL_PYTHON_HELPER_H_
#define CHPL_PYTHON_HELPER_H_

#include <Python.h>

static inline PyObject* chpl_PyEval_GetFrameGlobals(void) {
#if PY_VERSION_HEX >= 0x030d0000 /* Python 3.13 */
  return PyEval_GetFrameGlobals();
#else
  return PyEval_GetGlobals();
#endif
}

#endif
