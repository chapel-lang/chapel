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

#ifndef CHPL_PYTHON_HELPER_ARRAY_TYPES_H_
#define CHPL_PYTHON_HELPER_ARRAY_TYPES_H_

#include "ChapelPythonHelper.h"
#include <stdint.h>
#include "chpltypes.h"

// TODO: for some of the smaller types, when comsuming a PyObject we should check
// if it will fit in the C type, and if not raise an error.
//
// args:
// C datatype
// Chapel datatype
// array suffix
// python check function
// consuming PyObject function
// producing PyObject function
//
#define chpl_ARRAY_TYPES(V) \
  V(int64_t, "int(64)", I64, PyLong_Check, PyLong_AsLongLong, PyLong_FromLongLong) \
  V(uint64_t, "uint(64)", U64, PyLong_Check, PyLong_AsUnsignedLongLong, PyLong_FromUnsignedLongLong) \
  V(int32_t, "int(32)", I32, PyLong_Check, PyLong_AsLong, PyLong_FromLong) \
  V(uint32_t, "uint(32)", U32, PyLong_Check, PyLong_AsUnsignedLong, PyLong_FromUnsignedLong) \
  V(int16_t, "int(16)", I16, PyLong_Check, PyLong_AsLong, PyLong_FromLong) \
  V(uint16_t, "uint(16)", U16, PyLong_Check, PyLong_AsUnsignedLong, PyLong_FromUnsignedLong) \
  V(int8_t, "int(8)", I8, PyLong_Check, PyLong_AsLong, PyLong_FromLong) \
  V(uint8_t, "uint(8)", U8, PyLong_Check, PyLong_AsUnsignedLong, PyLong_FromUnsignedLong) \
  V(_real64, "real(64)", R64, PyFloat_Check, PyFloat_AsDouble, PyFloat_FromDouble) \
  V(_real32, "real(32)", R32, PyFloat_Check, PyFloat_AsDouble, PyFloat_FromDouble) \
  V(chpl_bool, "bool", Bool, PyBool_Check, PyObject_IsTrue, PyBool_FromLong)


#define chpl_MAKE_ARRAY_TYPES(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) extern PyTypeObject* Array##NAMESUFFIX##Type;
chpl_ARRAY_TYPES(chpl_MAKE_ARRAY_TYPES)
#undef chpl_MAKE_ARRAY_TYPES

chpl_bool createArrayTypes(void);

#define chpl_CREATE_ARRAY(DATATYPE, CHAPELDATATYPE, NAMESUFFIX, _0, _1, _2) PyObject* createArray##NAMESUFFIX(DATATYPE* data, Py_ssize_t size);
chpl_ARRAY_TYPES(chpl_CREATE_ARRAY)
#undef chpl_CREATE_ARRAY

#endif
