/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "signature-checks.h"

#include "chpl/framework/query-impl.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

static const bool& checkSignatureQuery(Context* context,
                                       const TypedFnSignature* sig) {
  QUERY_BEGIN(checkSignatureQuery, context, sig);

  bool unusedResult = false;

  const UntypedFnSignature* u = sig->untyped();
  UniqueString name = u->name();
  ID errId = u->id();

  int nFormals = u->numFormals();
  for (int i = 0; i < nFormals; i++) {
    if (u->formalIsVarArgs(i)) {
      auto varArgIntent = sig->formalType(i).kind();
      if (varArgIntent == Qualifier::OUT) {
        context->error(errId, "cannot have out intent varargs");
      }
    }
  }

  if (name == USTR("init=")) {
    // check that it is a method
    if (!u->isMethod()) {
      context->error(errId, "init= must be a method");
    }
    // check that there is just one argument in addition to the 'this' argument
    if (u->numFormals() != 2) {
      context->error(errId, "Too many formals for init=");
    }
    auto thisIntent = sig->formalType(0).kind();
    auto rhsIntent = sig->formalType(1).kind();
    // check the intent of the 'this' argument
    if (!(isGenericQualifier(thisIntent) || isRefQualifier(thisIntent))) {
      context->error(errId, "Bad 'this' intent for init=");
    }
    bool rhsIntentGenericOrRef = isGenericQualifier(rhsIntent) ||
                                 isRefQualifier(rhsIntent);
    // check the intent of the rhs argument
    if (sig->formalType(0).type() == sig->formalType(1).type()) {
      // same-type case: only const/default/ref/const ref RHS is allowed
      if (!rhsIntentGenericOrRef) {
        context->error(errId, "Bad intent for same-type init= other argument");
      }
    } else {
      // different-type case: RHS can be 'in' intent in addition
      if (!(rhsIntentGenericOrRef || isInQualifier(rhsIntent))) {
        context->error(errId, "Bad intent for cross-type init= other argument");
      }
    }
  }
  if (name == USTR("=")) {
    // check that there is just one argument in addition to the 'this' argument
    int numThisArgs = u->isMethod() ? 1 : 0;
    if (u->numFormals() - numThisArgs != 2) {
      context->error(errId, "Too many formals for operator =");
    }
    auto lhsIntent = sig->formalType(numThisArgs).kind();
    auto rhsIntent = sig->formalType(numThisArgs+1).kind();
    if (!(isGenericQualifier(lhsIntent) || isRefQualifier(lhsIntent))) {
      context->error(errId, "Bad intent for = LHS formal");
    }
    if (!(isGenericQualifier(rhsIntent) || isRefQualifier(rhsIntent) ||
          isInQualifier(rhsIntent))) {
      context->error(errId, "Bad intent for = RHS formal");
    }
  }

  return QUERY_END(unusedResult);
}


void checkSignature(Context* context, const TypedFnSignature* sig) {
  checkSignatureQuery(context, sig);
}


} // end namespace resolution
} // end namespace chpl
