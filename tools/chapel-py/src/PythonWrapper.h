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

#ifndef CHAPEL_PY_PYTHON_WRAPPER_H
#define CHAPEL_PY_PYTHON_WRAPPER_H

//
// Python.h should only be included in this header file, other files should
// include this one.
// This header guarantees a specific stable version of Python.h is used.
//

// use a stable API version of python 3.8
#define Py_LIMITED_API 0x03080000
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#endif
