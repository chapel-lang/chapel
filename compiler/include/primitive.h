/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

class CallExpr;
class Expr;
class Type;
class VarSymbol;
class QualifiedType;
class GenRet;

#define PRIMITIVE_G(NAME) NAME,
#define PRIMITIVE_R(NAME) NAME,
enum PrimitiveTag {
#include "primitive_list.h"
  NUM_KNOWN_PRIMS
};
#undef PRIMITIVE_G
#undef PRIMITIVE_R

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
