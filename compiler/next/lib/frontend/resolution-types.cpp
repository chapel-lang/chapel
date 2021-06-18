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

#include "chpl/frontend/resolution-types.h"

namespace chpl {
namespace frontend {


using namespace uast;

UniqueString Type::name(Context* context) {
  switch (tag()) {
    case FROM_DECL:
      assert(typeDecl());
      return typeDecl()->name();
    case INT8:
      return UniqueString::build(context, "int(8)");
    case INT16:
      return UniqueString::build(context, "int(16)");
    case INT32:
      return UniqueString::build(context, "int(32)");
    case INT64:
      return UniqueString::build(context, "int(64)");
    case UINT8:
      return UniqueString::build(context, "uint(8)");
    case UINT16:
      return UniqueString::build(context, "uint(16)");
    case UINT32:
      return UniqueString::build(context, "uint(32)");
    case UINT64:
      return UniqueString::build(context, "uint(64)");
    case BOOL:
      return UniqueString::build(context, "bool");
    case BOOL8:
      return UniqueString::build(context, "bool(8)");
    case BOOL16:
      return UniqueString::build(context, "bool(16)");
    case BOOL32:
      return UniqueString::build(context, "bool(32)");
    case BOOL64:
      return UniqueString::build(context, "bool(64)");
    case REAL32:
      return UniqueString::build(context, "real(32)");
    case REAL64:
      return UniqueString::build(context, "real(64)");
    case IMAG32:
      return UniqueString::build(context, "imag(32)");
    case IMAG64:
      return UniqueString::build(context, "imag(64)");
    case COMPLEX64:
      return UniqueString::build(context, "complex(64)");
    case COMPLEX128:
      return UniqueString::build(context, "complex(128)");
  }
  assert(false && "case not handled");
  return UniqueString::build(context, "<unknown>");
}


} // end namespace frontend
} // end namespace chpl
