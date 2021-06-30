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

#include "chpl/types/BuiltinType.h"

namespace chpl {
namespace types {


void BuiltinType::canonicalizeBitWidth() {
  switch (kind()) {
    case BOOL:
    case INT:
    case UINT:
      if (bitwidth_ == 0) {
        if (kind() == BOOL) {
          // OK, 'bool' is separate type from other bool(num),
          // represent that with bit width 0.
        } else {
          bitwidth_ = 64; // e.g. `int` means `int(64)`.
        }
      } else {
        assert(bitwidth_ == 8 || bitwidth_ == 16 ||
               bitwidth_ == 32 || bitwidth_ == 64);
      }
      break;

    case IMAG:
    case REAL:
      if (bitwidth_ == 0) {
        bitwidth_ = 64;
      } else {
        assert(bitwidth_ == 32 || bitwidth_ == 64);
      }
      break;

    case COMPLEX:
      if (bitwidth_ == 0) {
        bitwidth_ = 128;
      } else {
        assert(bitwidth_ == 64 || bitwidth_ == 128);
      }
      break;

    default:
      // always use 0 bitwidth
      bitwidth_ = 0;
  }
}

const char* BuiltinType::c_str() const {
  switch (kind_) {
    case INT:
      switch (bitwidth_) {
        case 8:
          return "int(8)";
        case 16:
          return "int(16)";
        case 32:
          return "int(32)";
        case 64:
          return "int(64)";
        default:
          assert(false && "int bit width case not handled");
          return "int(<unknown>)";
      }
    case UINT:
      switch (bitwidth_) {
        case 8:
          return "uint(8)";
        case 16:
          return "uint(16)";
        case 32:
          return "uint(32)";
        case 64:
          return "uint(64)";
        default:
          assert(false && "uint bit width case not handled");
          return "uint(<unknown>)";
      }
    case BOOL:
      switch (bitwidth_) {
        case 0:
          return "bool";
        case 8:
          return "bool(8)";
        case 16:
          return "bool(16)";
        case 32:
          return "bool(32)";
        case 64:
          return "bool(64)";
        default:
          assert(false && "bool bit width case not handled");
          return "bool(<unknown>)";
      }
    case REAL:
      switch (bitwidth_) {
        case 32:
          return "real(32)";
        case 64:
            return "real(64)";
        default:
          assert(false && "real bit width case not handled");
          return "real(<unknown>)";
      }
    case IMAG:
      switch (bitwidth_) {
        case 32:
          return "imag(32)";
        case 64:
            return "imag(64)";
        default:
          assert(false && "imag bit width case not handled");
          return "imag(<unknown>)";
      }
    case COMPLEX:
      switch (bitwidth_) {
        case 64:
          return "complex(64)";
        case 128:
          return "complex(128)";
        default:
          assert(false && "complex bit width case not handled");
          return "complex(<unknown>)";
      }

    case NUMERIC:
      return "numeric";
  }

  assert(false && "case not handled");
  return "<unknown>";
}

owned<BuiltinType> BuiltinType::build(BuiltinType::Kind kind, int bitwidth) {
  return toOwned(new BuiltinType(kind, bitwidth));
}


} // end namespace types
} // end namespace chpl
