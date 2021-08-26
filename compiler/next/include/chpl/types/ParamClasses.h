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

#ifndef CHPL_TYPES_PARAMCLASSES_H
#define CHPL_TYPES_PARAMCLASSES_H

namespace chpl {
namespace types {


// forward declare the various Param subclasses
// using macros and ParamClassesList.h
/// \cond DO_NOT_DOCUMENT
#define PARAM_NODE(NAME, VALTYPE) class NAME;
/// \endcond
// Apply the above macros to ParamClassesList.h
#include "chpl/types/ParamClassesList.h"
// clear the macros
#undef PARAM_NODE

// forward declare other classes
class Param;


} // end namespace types
} // end namespace chpl

#endif
