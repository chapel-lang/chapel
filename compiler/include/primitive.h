/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "chpl.h"
#include "chpl/uast/PrimOp.h"

// make the PRIM_MOVE etc values available without scoping
// as well as PrimitiveTag
using namespace chpl::uast::primtags;

class CallExpr;
class Expr;
class Type;
class VarSymbol;
class QualifiedType;
class GenRet;

// There are two questions:
// 1- is the primitive/function eligible to run in a fast block?
//    any blocking or system call disqualifies it since a fast
//    AM handler can be run in a signal handler
// 2- is the primitive/function communication free?

// Any function containing communication can't run in a fast block.

enum {
  // The primitive is ineligible for a fast (e.g. uses a lock or allocator)
  // AND it causes communication
  NOT_FAST_NOT_LOCAL,
  // Is the primitive ineligible for a fast (e.g. uses a lock or allocator)
  // but communication free?
  LOCAL_NOT_FAST,
  // Does the primitive communicate?
  // This implies NOT_FAST, unless it is in a local block
  // if it is in a local block, this means IS_FAST.
  FAST_NOT_LOCAL,
  // Is the primitive function fast (ie, could it be run in a signal handler)
  // IS_FAST implies IS_LOCAL.
  FAST_AND_LOCAL
};


class PrimitiveOp { public:
  PrimitiveTag  tag;
  const char*   name;
  QualifiedType (*returnInfo)(CallExpr*); // aka qualifiedTypeFn
  void (*codegenFn)(CallExpr*, GenRet&);
  bool          isEssential; // has effects visible outside of the function
  bool          passLineno;  // pass line number and filename to this primitive

  PrimitiveOp(PrimitiveTag  atag,
              const char*   aname,
              QualifiedType (*areturnInfo)(CallExpr*));
};

extern HashMap<const char *, StringHashFns, PrimitiveOp *> primitives_map;

extern PrimitiveOp*     primitives[NUM_KNOWN_PRIMS];

extern Vec<const char*> memDescsVec;

void       printPrimitiveCounts(const char* passName);

void       initPrimitive();

VarSymbol* newMemDesc(const char* str);

VarSymbol* newMemDesc(Type* type);

bool       getSettingPrimitiveDstSrc(CallExpr* call, Expr** dest, Expr** src);

void registerPrimitiveCodegen(PrimitiveTag tag, void (*fn)(CallExpr*, GenRet&));

#endif
