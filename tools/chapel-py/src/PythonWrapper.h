#ifndef CHAPEL_PY_PYTHON_WRAPPER_H
#define CHAPEL_PY_PYTHON_WRAPPER_H

#ifdef CHAPEL_PY_USE_STABLE_API
// use a stable API version of python 3.10
#define Py_LIMITED_API 0x030A0000
#endif
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#endif
