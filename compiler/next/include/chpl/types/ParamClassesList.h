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

// This file lists the Param subclasses to help with macros generating code
// for each.
//
// The arguments are (name of field and tag, type of field, type expr)
//
// the following comment disables doxygen for these
/// \cond DO_NOT_DOCUMENT

PARAM_NODE(BoolParam, bool)
PARAM_NODE(ComplexParam, ComplexDouble)
PARAM_NODE(IntParam, int64_t)
PARAM_NODE(NoneParam, NoneValue)
PARAM_NODE(RealParam, double)
PARAM_NODE(StringParam, UniqueString)
PARAM_NODE(UintParam, uint64_t)

/// \endcond

// this comment seems to be necessary for doxygen xml output to be well-formed
