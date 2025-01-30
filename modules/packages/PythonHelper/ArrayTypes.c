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

#include "ArrayTypes.h"

#define chpl_MAKE_ARRAY_STRUCT(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  typedef struct { \
    PyObject_HEAD \
    DATATYPE* data; \
    Py_ssize_t size; \
  } Array##NAMESUFFIX##Object; \
  PyTypeObject* Array##NAMESUFFIX##Type = NULL;
chpl_ARRAY_TYPES(chpl_MAKE_ARRAY_STRUCT)
#undef chpl_MAKE_ARRAY_STRUCT

static int ArrayGenericObject_init(PyObject* self, PyObject* args, PyObject* kwargs) {
  return 0;
}
#define chpl_MAKE_DEALLOC(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  static void Array##NAMESUFFIX##Object_dealloc(Array##NAMESUFFIX##Object* self) { \
    void (*tp_free)(PyObject*) = (void (*)(PyObject*))(PyType_GetSlot(Array##NAMESUFFIX##Type, Py_tp_free)); \
    if (!tp_free) PyErr_SetString(PyExc_RuntimeError, "Could not free object"); \
    else          tp_free((PyObject*) self); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_DEALLOC)
#undef chpl_MAKE_DEALLOC


#define chpl_MAKE_REPR(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  static PyObject* Array##NAMESUFFIX##Object_repr(Array##NAMESUFFIX##Object* self) { \
    return PyUnicode_FromFormat("Array(eltType="CHAPELDATATYPE",size=%zd)", self->size); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_REPR)
#undef chpl_MAKE_REPR

// TODO: how can we remove the error checking here with --no-checks or -scheckExceptions=false?
#define chpl_MAKE_SETITEM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, CHECKTYPE, ASTYPE, _0) \
  static int Array##NAMESUFFIX##Object_setitem(Array##NAMESUFFIX##Object* self, Py_ssize_t index, PyObject* value) { \
    if (!CHECKTYPE(value)) { \
      PyErr_SetString(PyExc_TypeError, "can only assign "CHAPELDATATYPE" to this array"); \
      return -1; \
    } \
    DATATYPE val = ASTYPE(value); \
    if (PyErr_Occurred()) return -1; \
    if (index < 0 || index >= self->size) { \
      PyErr_SetString(PyExc_IndexError, "index out of bounds"); \
      return -1; \
    } \
    self->data[index] = val; \
    return 0; \
  }
chpl_ARRAY_TYPES(chpl_MAKE_SETITEM)
#undef chpl_MAKE_SETITEM
#define chpl_MAKE_GETITEM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, CHECKTYPE, ASTYPE, FROMTYPE) \
  static PyObject* Array##NAMESUFFIX##Object_getitem(Array##NAMESUFFIX##Object* self, Py_ssize_t index) { \
    if (index < 0 || index >= self->size) { \
      PyErr_SetString(PyExc_IndexError, "index out of bounds"); \
      return NULL; \
    } \
    return FROMTYPE(self->data[index]); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_GETITEM)
#undef chpl_MAKE_GETITEM

#define chpl_MAKE_LENGTH(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  static Py_ssize_t Array##NAMESUFFIX##Object_length(Array##NAMESUFFIX##Object* self) { \
    return self->size; \
  }
chpl_ARRAY_TYPES(chpl_MAKE_LENGTH)
#undef chpl_MAKE_LENGTH


#define chpl_MAKE_TYPE(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  static chpl_bool createArray##NAMESUFFIX##Type(void) { \
    PyType_Slot slots[] = { \
      {Py_tp_init, (void*) ArrayGenericObject_init}, \
      {Py_tp_dealloc, (void*) Array##NAMESUFFIX##Object_dealloc}, \
      {Py_tp_doc, (void*) PyDoc_STR("Array" #NAMESUFFIX "Object")}, \
      {Py_tp_new, (void*) PyType_GenericNew}, \
      {Py_tp_repr, (void*) Array##NAMESUFFIX##Object_repr}, \
      {Py_sq_ass_item, (void*) Array##NAMESUFFIX##Object_setitem}, \
      {Py_sq_item, (void*) Array##NAMESUFFIX##Object_getitem}, \
      {Py_mp_length, (void*)Array##NAMESUFFIX##Object_length}, \
      {0, NULL} \
    }; \
    PyType_Spec spec = { \
      /*name*/ "Array"#NAMESUFFIX, \
      /*basicsize*/ sizeof(Array##NAMESUFFIX##Object), \
      /*itemsize*/ 0, \
      /*flags*/ Py_TPFLAGS_DEFAULT, \
      /*slots*/ slots \
    }; \
    Array##NAMESUFFIX##Type = (PyTypeObject*)PyType_FromSpec(&spec); \
    if (!Array##NAMESUFFIX##Type || PyType_Ready(Array##NAMESUFFIX##Type) < 0) return false; \
    return true; \
  }
chpl_ARRAY_TYPES(chpl_MAKE_TYPE)
#undef chpl_MAKE_TYPE

chpl_bool createArrayTypes(void) {
#define chpl_MAKE_TYPE(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  if (!createArray##NAMESUFFIX##Type()) return false;
chpl_ARRAY_TYPES(chpl_MAKE_TYPE)
#undef chpl_MAKE_TYPE
  return true;
}

#define chpl_CREATE_ARRAY(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) \
  PyObject* createArray##NAMESUFFIX(DATATYPE* data, Py_ssize_t size) { \
    assert(Array##NAMESUFFIX##Type); \
    PyObject* objPy = PyObject_CallNoArgs((PyObject *) Array##NAMESUFFIX##Type); \
    Array##NAMESUFFIX##Object* obj = (Array##NAMESUFFIX##Object*) objPy; \
    if (!obj) return NULL; \
    obj->data = data; \
    obj->size = size; \
    return objPy; \
  }
chpl_ARRAY_TYPES(chpl_CREATE_ARRAY)
#undef chpl_CREATE_ARRAY
