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

#ifndef CHPL_RESOLUTION_RESOLUTION_TYPES_H
#define CHPL_RESOLUTION_RESOLUTION_TYPES_H

#include "chpl/uast/TypeDecl.h"

namespace chpl {
namespace resolution {


class Type {
 public:
  enum TypeTag {
    FROM_DECL,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    BOOL,
    BOOL8,
    BOOL16,
    BOOL32,
    BOOL64,
    REAL32,
    REAL64,
    IMAG32,
    IMAG64,
    COMPLEX64,
    COMPLEX128,
  };
 private:
  TypeTag tag_;
  const uast::TypeDecl* typeDecl_;
 public:
  TypeTag tag() { return tag_; } 
  const uast::TypeDecl* typeDecl() { return typeDecl_; }
  UniqueString name(Context* context);
};


} // end namespace resolution
} // end namespace chpl
#endif
