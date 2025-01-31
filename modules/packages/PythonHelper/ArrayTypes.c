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
#include "chpl-mem.h"

// required for Python <12
#if PY_VERSION_HEX < 0x030c0000
#include "structmember.h"
#ifndef Py_T_PYSSIZET
#define Py_T_PYSSIZET T_PYSSIZET
#endif
#ifndef Py_T_OBJECT_EX
#define Py_T_OBJECT_EX T_OBJECT_EX
#endif
#ifndef Py_READONLY
#define Py_READONLY READONLY
#endif
#endif


static PyObject* ArrayTypeEnum = NULL;
chpl_bool registerArrayTypeEnum(void) {

  PyObject* elements = PyDict_New();
  if (!elements) return false;
  {
    int i = 0;
#define chpl_MAKE_ENUM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  PyDict_SetItemString(elements, #NAMESUFFIX, PyLong_FromLong(i++));
chpl_ARRAY_TYPES(chpl_MAKE_ENUM)
#undef chpl_MAKE_ENUM
  }

  PyObject* enumModule = PyImport_ImportModule("enum");
  if (!enumModule) {
    Py_CLEAR(elements);
    return false;
  }

  ArrayTypeEnum = PyObject_CallMethod(enumModule, "Enum", "sO", "ArrayType", elements);
  if (!ArrayTypeEnum) {
    Py_CLEAR(elements);
    Py_CLEAR(enumModule);
    return false;
  }

  Py_CLEAR(elements);
  Py_CLEAR(enumModule);
  return true;
}


#define chpl_MAKE_ARRAY_STRUCT(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  typedef struct { \
    PyObject_HEAD \
    DATATYPE* data; \
    Py_ssize_t size; \
    chpl_bool isOwned; \
    PyObject* eltType; \
    Py_ssize_t ndim; \
  } Array##NAMESUFFIX##Object; \
  PyTypeObject* Array##NAMESUFFIX##Type = NULL;
chpl_ARRAY_TYPES(chpl_MAKE_ARRAY_STRUCT)
#undef chpl_MAKE_ARRAY_STRUCT

static int ArrayGenericObject_init(PyObject* self, PyObject* args, PyObject* kwargs) {
  return 0;
}
#define chpl_MAKE_DEALLOC(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  static void Array##NAMESUFFIX##Object_dealloc(Array##NAMESUFFIX##Object* self) { \
    if (self->isOwned) chpl_mem_free(self->data, 0, 0); \
    Py_CLEAR(self->eltType); \
    void (*tp_free)(PyObject*) = (void (*)(PyObject*))(PyType_GetSlot(Array##NAMESUFFIX##Type, Py_tp_free)); \
    if (!tp_free) PyErr_SetString(PyExc_RuntimeError, "Could not free object"); \
    else          tp_free((PyObject*) self); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_DEALLOC)
#undef chpl_MAKE_DEALLOC


#define chpl_MAKE_REPR(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  static PyObject* Array##NAMESUFFIX##Object_repr(Array##NAMESUFFIX##Object* self) { \
    return PyUnicode_FromFormat("Array(eltType="CHAPELDATATYPE",size=%zd)", self->size); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_REPR)
#undef chpl_MAKE_REPR

// TODO: how can we remove the error checking here with --no-checks or -scheckExceptions=false?
#define chpl_MAKE_SETITEM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, CHECKTYPE, ASTYPE, ...) \
  static int Array##NAMESUFFIX##Object_sq_setitem(Array##NAMESUFFIX##Object* self, Py_ssize_t index, PyObject* value) { \
    if (!value) { \
      PyErr_SetString(PyExc_TypeError, "cannot delete items from this array"); \
      return -1; \
    } \
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

#define chpl_MAKE_SETITEM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, CHECKTYPE, ASTYPE, ...) \
  static int Array##NAMESUFFIX##Object_mp_setitem(Array##NAMESUFFIX##Object* self, PyObject* indexObj, PyObject* value) { \
    if (!value) { \
      PyErr_SetString(PyExc_TypeError, "cannot delete items from this array"); \
      return -1; \
    } \
    if (!CHECKTYPE(value)) { \
      PyErr_SetString(PyExc_TypeError, "can only assign "CHAPELDATATYPE" to this array"); \
      return -1; \
    } \
    DATATYPE val = ASTYPE(value); \
    if (PyErr_Occurred()) return -1; \
    if (!PyLong_Check(indexObj)) { \
      /* TODO: support slices */ \
      PyErr_SetString(PyExc_TypeError, "index must be an integer"); \
      return -1; \
    } \
    Py_ssize_t index = PyLong_AsSsize_t(indexObj); \
    if (index < 0 || index >= self->size) { \
      PyErr_SetString(PyExc_IndexError, "index out of bounds"); \
      return -1; \
    } \
    self->data[index] = val; \
    return 0; \
  }
chpl_ARRAY_TYPES(chpl_MAKE_SETITEM)
#undef chpl_MAKE_SETITEM

#define chpl_MAKE_GETITEM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, CHECKTYPE, ASTYPE, FROMTYPE, ...) \
  static PyObject* Array##NAMESUFFIX##Object_sq_getitem(Array##NAMESUFFIX##Object* self, Py_ssize_t index) { \
    if (index < 0 || index >= self->size) { \
      PyErr_SetString(PyExc_IndexError, "index out of bounds"); \
      return NULL; \
    } \
    return FROMTYPE(self->data[index]); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_GETITEM)
#undef chpl_MAKE_GETITEM
#define chpl_MAKE_GETITEM(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, CHECKTYPE, ASTYPE, FROMTYPE, ...) \
  static PyObject* Array##NAMESUFFIX##Object_mp_getitem(Array##NAMESUFFIX##Object* self, PyObject* indexObj) { \
    if (!PyLong_Check(indexObj)) { \
      /* TODO: support slices */ \
      PyErr_SetString(PyExc_TypeError, "index must be an integer"); \
      return NULL; \
    } \
    Py_ssize_t index = PyLong_AsSsize_t(indexObj); \
    if (index < 0 || index >= self->size) { \
      PyErr_SetString(PyExc_IndexError, "index out of bounds"); \
      return NULL; \
    } \
    return FROMTYPE(self->data[index]); \
  }
chpl_ARRAY_TYPES(chpl_MAKE_GETITEM)
#undef chpl_MAKE_GETITEM

#define chpl_MAKE_LENGTH(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  static Py_ssize_t Array##NAMESUFFIX##Object_length(Array##NAMESUFFIX##Object* self) { \
    return self->size; \
  }
chpl_ARRAY_TYPES(chpl_MAKE_LENGTH)
#undef chpl_MAKE_LENGTH


// https://docs.python.org/3/c-api/typeobj.html#c.PyBufferProcs.bf_getbuffer
// 1. Check if the request can be met. If not, raise BufferError, set view->obj to NULL and return -1.
// 2. Fill in the requested fields.
// 3. Increment an internal counter for the number of exports.
// 4. Set view->obj to exporter and increment view->obj.
// 5. Return 0.
#define chpl_MAKE_GET_BUFFER(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2, SUPPORTSBUFFERS) \
static int Array##NAMESUFFIX##Object_bf_getbuffer(Array##NAMESUFFIX##Object* arr, Py_buffer* view, int flags) { \
  if (!SUPPORTSBUFFERS) { \
    PyErr_SetString(PyExc_BufferError, "This array does not support the buffer protocol"); \
    view->obj = NULL; \
    return -1; \
  } \
  /* other error checking for invalid request in flags?! */ \
  view->buf = arr->data; \
  view->obj = (PyObject*) arr; \
  view->len = arr->size * sizeof(DATATYPE); \
  view->itemsize = sizeof(DATATYPE); \
  view->readonly = 0; \
  view->ndim = arr->ndim; \
  view->format = NULL; \
  if (flags & PyBUF_FORMAT) { \
    view->format = "q"; /*compute format string based on datatype */ \
  } \
  /*TODO: support nd arrays*/ \
  view->shape = NULL; \
  view->strides = NULL; \
  view->suboffsets = NULL; \
  Py_INCREF(view->obj); \
  return 0; \
}
chpl_ARRAY_TYPES(chpl_MAKE_GET_BUFFER)
#undef chpl_MAKE_GET_BUFFER


#define chpl_MAKE_TYPE(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  static chpl_bool createArray##NAMESUFFIX##Type(void) { \
    PyMethodDef methods[] = { \
      {NULL, NULL, 0, NULL}  /* Sentinel */ \
    }; \
    /* Do not expose data as a member!!!! */ \
    PyMemberDef members[] = { \
      {"size", Py_T_PYSSIZET, offsetof(Array##NAMESUFFIX##Object, size), Py_READONLY, PyDoc_STR("size of the array")}, \
      {"eltType", Py_T_OBJECT_EX, offsetof(Array##NAMESUFFIX##Object, eltType), Py_READONLY, PyDoc_STR("type of elements in the array")}, \
      {"ndim", Py_T_PYSSIZET, offsetof(Array##NAMESUFFIX##Object, ndim), Py_READONLY, PyDoc_STR("number of dimensions in the array")}, \
      {NULL} /* Sentinel */\
    }; \
    PyType_Slot slots[] = { \
      {Py_tp_init, (void*) ArrayGenericObject_init}, \
      {Py_tp_dealloc, (void*) Array##NAMESUFFIX##Object_dealloc}, \
      {Py_tp_doc, (void*) PyDoc_STR("Array" #NAMESUFFIX "Object")}, \
      {Py_tp_repr, (void*) Array##NAMESUFFIX##Object_repr}, \
      {Py_sq_ass_item, (void*) Array##NAMESUFFIX##Object_sq_setitem}, \
      {Py_sq_item, (void*) Array##NAMESUFFIX##Object_sq_getitem}, \
      {Py_sq_length, (void*) Array##NAMESUFFIX##Object_length}, \
      {Py_mp_ass_subscript, (void*) Array##NAMESUFFIX##Object_mp_setitem}, \
      {Py_mp_subscript, (void*) Array##NAMESUFFIX##Object_mp_getitem}, \
      {Py_mp_length, (void*) Array##NAMESUFFIX##Object_length}, \
      {Py_tp_methods, (void*) methods}, \
      {Py_tp_members, (void*) members}, \
      {Py_bf_getbuffer, (void*) Array##NAMESUFFIX##Object_bf_getbuffer}, \
      {0, NULL} \
    }; \
    PyType_Spec spec = { \
      /*name*/ "Array"#NAMESUFFIX, \
      /*basicsize*/ sizeof(Array##NAMESUFFIX##Object), \
      /*itemsize*/ 0, \
      /*flags*/ Py_TPFLAGS_DEFAULT | Py_TPFLAGS_SEQUENCE, \
      /*slots*/ slots \
    }; \
    Array##NAMESUFFIX##Type = (PyTypeObject*)PyType_FromSpec(&spec); \
    if (!Array##NAMESUFFIX##Type || PyType_Ready(Array##NAMESUFFIX##Type) < 0) return false; \
    return true; \
  }
chpl_ARRAY_TYPES(chpl_MAKE_TYPE)
#undef chpl_MAKE_TYPE

chpl_bool createArrayTypes(void) {
#define chpl_MAKE_TYPE(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  if (!createArray##NAMESUFFIX##Type()) return false;
chpl_ARRAY_TYPES(chpl_MAKE_TYPE)
#undef chpl_MAKE_TYPE
  return true;
}

#define chpl_CREATE_ARRAY(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, ...) \
  PyObject* createArray##NAMESUFFIX(DATATYPE* data, Py_ssize_t size, chpl_bool isOwned) { \
    assert(Array##NAMESUFFIX##Type); \
    assert(ArrayTypeEnum); \
    PyObject* objPy = PyObject_CallNoArgs((PyObject *) Array##NAMESUFFIX##Type); \
    Array##NAMESUFFIX##Object* obj = (Array##NAMESUFFIX##Object*) objPy; \
    if (!obj) return NULL; \
    obj->data = data; \
    obj->size = size; \
    obj->isOwned = isOwned; \
    obj->eltType = PyObject_GetAttrString(ArrayTypeEnum, #NAMESUFFIX); \
    obj->ndim = 1; /*TODO: when we support proper ND chapel arrays, set dynamicly */\
    return objPy; \
  }
chpl_ARRAY_TYPES(chpl_CREATE_ARRAY)
#undef chpl_CREATE_ARRAY
