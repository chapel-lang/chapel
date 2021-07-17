/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

// This file lists the BuiltinType kinds to help with macros generating code
// for each
//
// Each line consists of one of
//   BUILTIN_TYPE(ENUM_NAME, "chapel name")

// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

// concrete builtin types
BUILTIN_TYPE(C_FILE, "_cfile")
BUILTIN_TYPE(C_FN_PTR, "c_fn_ptr")
BUILTIN_TYPE(C_VOID_PTR, "chpl__c_void_ptr")
BUILTIN_TYPE(OPAQUE, "opaque")
BUILTIN_TYPE(SYNC_AUX_T, "_sync_aux_t")
BUILTIN_TYPE(TASK_ID_T, "chpl_nullTaskID")

// generic builtin types (note ANY_BOOL must be the first;
// or else update BuiltinType::isGeneric and this comment)
BUILTIN_TYPE(ANY_BOOL, "chpl_anybool")
BUILTIN_TYPE(ANY_COMPLEX, "chpl_anycomplex")
BUILTIN_TYPE(ANY_IMAG, "chpl_anyimag")
BUILTIN_TYPE(ANY_MANAGEMENT_ANY_NILABLE, "_anyManagementAnyNilable")
BUILTIN_TYPE(ANY_MANAGEMENT_NILABLE, "_anyManagementNilable")
BUILTIN_TYPE(ANY_POD, "chpl_anyPOD")
BUILTIN_TYPE(ANY_REAL, "chpl_anyreal")
BUILTIN_TYPE(BORROWED, "borrowed")
BUILTIN_TYPE(BORROWED_NON_NILABLE, "_borrowedNonNilable")
BUILTIN_TYPE(CLASS, "class")
BUILTIN_TYPE(ENUM, "enum")
BUILTIN_TYPE(INTEGRAL, "integral")
BUILTIN_TYPE(ITERATOR_CLASS, "_iteratorClass")
BUILTIN_TYPE(ITERATOR_RECORD, "_iteratorRecord")
BUILTIN_TYPE(NUMERIC, "numeric")
BUILTIN_TYPE(RECORD, "record")
BUILTIN_TYPE(UNINSTANTIATED, "_uninstantiated")
BUILTIN_TYPE(UNION, "union")
BUILTIN_TYPE(UNMANAGED, "unmanaged")
BUILTIN_TYPE(UNMANAGED_NILABLE, "_unmanagedNilable")
BUILTIN_TYPE(UNMANAGED_NON_NILABLE, "_unmanagedNonNilable")

/// \endcond

// this comment seems to be necessary for doxygen xml output to be well-formed
