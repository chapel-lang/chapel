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
// The arguments are (name of field and tag, type of field, type expr)
//
// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

PARAM_NODE(Complex128Param, Complex128Storage, ComplexType::get(context, 128))
PARAM_NODE(Complex64Param, Complex64Storage, ComplexType::get(context, 64))
PARAM_NODE(Real64Param, double, RealType::get(context, 64))
PARAM_NODE(Real32Param, float, RealType::get(context,32))
PARAM_NODE(Imag64Param, double, ImagType::get(context, 64))
PARAM_NODE(Imag32Param, float, ImagType::get(context, 32))
PARAM_NODE(Int64Param, int64_t, IntType::get(context, 64))
PARAM_NODE(Int32Param, int32_t, IntType::get(context, 32))
PARAM_NODE(Int16Param, int16_t, IntType::get(context, 16))
PARAM_NODE(Int8Param, int8_t, IntType::get(context, 8))
PARAM_NODE(Uint64Param, uint64_t, UintType::get(context, 64))
PARAM_NODE(Uint32Param, uint32_t, UintType::get(context, 32))
PARAM_NODE(Uint16Param, uint16_t, UintType::get(context, 16))
PARAM_NODE(Uint8Param, uint8_t, UintType::get(context, 8))
PARAM_NODE(BoolSysParam, uint64_t, BoolType::get(context, 0))
PARAM_NODE(Bool64Param, uint64_t, BoolType::get(context, 64))
PARAM_NODE(Bool32Param, uint32_t, BoolType::get(context, 32))
PARAM_NODE(Bool16Param, uint16_t, BoolType::get(context, 16))
PARAM_NODE(Bool8Param, uint8_t, BoolType::get(context, 8))
PARAM_NODE(NoneParam, uint8_t, NothingType::get(context))

// update Param::mark if any others storing string are added
PARAM_NODE(BytesParam, UniqueString, BytesType::get(context))
PARAM_NODE(CStringParam, UniqueString, CStringType::get(context))
PARAM_NODE(StringParam, UniqueString, StringType::get(context))

/// \endcond

// this comment seems to be necessary for doxygen xml output to be well-formed
