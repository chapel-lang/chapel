/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/PrimitiveType.h"

#include "chpl/types/BoolType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/RealType.h"
#include "chpl/types/UintType.h"

#include "chpl/queries/global-strings.h"

namespace chpl {
namespace types {


void PrimitiveType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  ss << this->c_str();
}

const PrimitiveType*
PrimitiveType::getWithNameAndWidth(Context* context,
                                   UniqueString name,
                                   int bitwidth) {
  if (name == USTR("int"))
    if (bitwidth == 8 || bitwidth == 16 || bitwidth == 32 || bitwidth == 64)
      return IntType::get(context, bitwidth);
  if (name == USTR("uint"))
    if (bitwidth == 8 || bitwidth == 16 || bitwidth == 32 || bitwidth == 64)
      return UintType::get(context, bitwidth);
  if (name == USTR("bool"))
    if (bitwidth == 8 || bitwidth == 16 || bitwidth == 32 || bitwidth == 64)
      return BoolType::get(context, bitwidth);
  if (name == USTR("real"))
    if (bitwidth == 32 || bitwidth == 64)
      return RealType::get(context, bitwidth);
  if (name == USTR("imag"))
    if (bitwidth == 32 || bitwidth == 64)
      return ImagType::get(context, bitwidth);
  if (name == USTR("complex"))
    if (bitwidth == 64 || bitwidth == 128)
      return ComplexType::get(context, bitwidth);

  // otherwise it is an invalid name or width
  return nullptr;
}


} // end namespace types
} // end namespace chpl
