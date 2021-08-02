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

// This file lists the Param union types to help with macros generating code
// for each.
//
// The arguments are (name of field and tag, type of field)
//
// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

PARAM_NODE(Complex128, Complex128Storage)
PARAM_NODE(Complex64, Complex64Storage)
PARAM_NODE(Real64, double)
PARAM_NODE(Real32, float)
PARAM_NODE(Imag64, double)
PARAM_NODE(Imag32, float)
PARAM_NODE(Int64, int64_t)
PARAM_NODE(Int32, int32_t)
PARAM_NODE(Int16, int16_t)
PARAM_NODE(Int8, int8_t)
PARAM_NODE(Uint64, uint64_t)
PARAM_NODE(Uint32, uint32_t)
PARAM_NODE(Uint16, uint16_t)
PARAM_NODE(Uint8, uint8_t)
PARAM_NODE(BoolSys, uint64_t)
PARAM_NODE(Bool64, uint64_t)
PARAM_NODE(Bool32, uint32_t)
PARAM_NODE(Bool16, uint16_t)
PARAM_NODE(Bool8, uint8_t)
PARAM_NODE(None, uint8_t)

// update Param::mark if any others storing string are added
PARAM_NODE(Bytes, UniqueStringAndLength)
PARAM_NODE(CString, UniqueStringAndLength)
PARAM_NODE(String, UniqueStringAndLength)

/// \endcond

// this comment seems to be necessary for doxygen xml output to be well-formed
