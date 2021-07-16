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

#include "chpl/types/Type.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Function.h"
#include "chpl/util/memory.h"

#include <unordered_map>

namespace chpl {
namespace resolution {

// TODO: Should some/all of these structs be classes
// with getters etc? That would be appropriate for
// use as part of the library API.

/**
  An untyped function signature. This is really just the part of a function
  including the formals. It exists so that the process of identifying
  candidates does not need to depend on the bodies of the function
  (in terms of incremental recomputation).
 */
struct UntypedFnSignature {
  ID functionId;
  UniqueString name;
  bool isMethod; // in that case, formals[0] is the receiver
  std::vector<const uast::Formal*> formals;
  const uast::Expression* whereClause;

  UntypedFnSignature(const uast::Function* fn)
    : functionId(fn->id()),
      name(fn->name()),
      isMethod(fn->isMethod()),
      whereClause(fn->whereClause()) {
    for (auto formal : fn->formals()) {
      formals.push_back(formal);
    }
  }

  bool operator==(const UntypedFnSignature& other) const {
    return functionId != other.functionId &&
           name == other.name &&
           isMethod == other.isMethod &&
           formals == other.formals &&
           whereClause == other.whereClause;
  }
  bool operator!=(const UntypedFnSignature& other) const {
    return !(*this == other);
  }
};

struct KindParamType {
  typedef enum {
    UNKNOWN,
    VALUE,
    TYPE,
    PARAM,
    FUNCTION,
  } Kind;
  Kind kind = UNKNOWN;
  const types::Type* type = nullptr;
  // TODO: param value

  KindParamType() { }

  bool operator==(const KindParamType& other) const {
    return kind == other.kind &&
           type == other.type;
  }
  bool operator!=(const KindParamType& other) const {
    return !(*this == other);
  }
};

struct CallInfoActual {
  KindParamType type;
  UniqueString byName;
  // TODO: param and type actuals

  bool operator==(const CallInfoActual& other) const {
    return type == other.type &&
           byName == other.byName;
  }
  bool operator!=(const CallInfoActual& other) const {
    return !(*this == other);
  }
};

struct CallInfo {
  bool isMethod; // in that case, actuals[0] is the receiver
  std::vector<CallInfoActual> actuals;

  bool operator==(const CallInfo& other) const {
    return isMethod == other.isMethod &&
           actuals == other.actuals;
  }
  bool operator!=(const CallInfo& other) const {
    return !(*this == other);
  }
};

struct TypedFnSignatureFormal {
  const uast::Formal* formal;
  KindParamType type;

  bool operator==(const TypedFnSignatureFormal& other) const {
    return formal == other.formal &&
           type == other.type;
  }
  bool operator!=(const TypedFnSignatureFormal& other) const {
    return !(*this == other);
  }
};

struct TypedFnSignature {
  UntypedFnSignature* untypedSignature;
  std::vector<TypedFnSignatureFormal> formals;

  bool evaluatedWhereClause;

  // substitutions (only used for generic functions/types)
  //std::unordered_map<uast::NamedDecl*, types::Type*> typeSubs;
  // TODO: int -> Immediate
  //std::unordered_map<uast::NamedDecl*, int> paramSubs;

  // the point of instantiation
  TypedFnSignature* instantiationPointFn = nullptr;
  ID instantiationPointId;

  bool operator==(const TypedFnSignature& other) const {
    return untypedSignature == other.untypedSignature &&
           formals == other.formals &&
           evaluatedWhereClause == other.evaluatedWhereClause &&
           instantiationPointfn == other.instantiationPointFn &&
           instantiationPointId == other.instantiationPointId;
  }
  bool operator!=(const TypedFnSignature& other) const {
    return !(*this == other);
  }
};

struct ResolvedExpression {
  // the ID that is resolved
  ID id;
  // What is its type and param value?
  KindParamType type;
  // For simple (non-function) cases, the ID of a NamedDecl it refers to
  ID toId;

  // For a function call, it refers to a typed function
  // (that might be a generic instantiation)
  TypedFnSignature* function;

  // Some calls use return intent overloading. In that event,
  // all overloads are stored in this vector. They need to be
  // resolved in a different order.
  std::vector<TypedFnSignature*> overloads;

  ResolutionResult() { }
};

// postorder ID (int) -> ResolvedExpression *within* a Function etc
// an inner Function would not be covered here since it would get
// a different ResolvedSymbol entry.
using ResolutionResultByPostorderID = std::vector<ResolvedExpression>;

// A resolution result for a Function, Module, or TypeDecl (Record/Class/etc)
struct ResolvedSymbol {
  // the following are input for the resolution process but these
  // are repeated here in case they are needed in follow-on processing.

  // the NamedDecl that is resolved
  const uast::NamedDecl* decl = nullptr;
  // substitutions (only used for generic functions/types)
  std::unordered_map<uast::NamedDecl*, types::Type*> typeSubs;
  // TODO: int -> Immediate
  std::unordered_map<uast::NamedDecl*, int> paramSubs;
  // the point of instantiation
  const ResolvedSymbol* instantiationPoint = nullptr;

  // this is the output of the resolution process
  ResolutionResultByPostorderID resolutionById;
};


} // end namespace resolution
} // end namespace chpl
#endif
