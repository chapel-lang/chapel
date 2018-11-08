/*
 * Copyright 2004-2018 Cray Inc.
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

#include "LoopStmt.h"

#include "codegen.h"
#include "driver.h"

void LoopStmt::fixVectorizeable()
{
  if (!this->isVectorizeable())
    return; // nothing more to do!

  // Check for DefExprs in the loop body that would present challenges
  // for vectorization.

  // DefExprs for values that we expect to be represented as registers
  // are OK.

  // Alternatively we could clone these allocations per vector
  // lane. We're not quite ready to do that yet though, and ones
  // that don't need to be cloned should be in registers anyway.

  // This runs late in the compiler because we want to do this check
  // after inlining and other late simplifications have occured.
  bool addressTaken = false;
  int ndefs = 0;

  for_alist(expr, body) {
    if (DefExpr* def = toDefExpr(expr)) {
      Symbol* sym = def->sym;
      // Decide if it could be a register
      // (Note, these rules are accurate for LLVM but
      //  serve as a reasonable guess for a C compiler)
      for_SymbolSymExprs(se, sym) {
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          // was the address taken?
          // TODO: get svec member, get member
          if (call->isPrimitive(PRIM_ADDR_OF) ||
              call->isPrimitive(PRIM_SET_REFERENCE)) {
            addressTaken = true;
          } else if (FnSymbol* fn = call->resolvedOrVirtualFunction()) {
            ArgSymbol* formal = actual_to_formal(se);
            if ((formal->intent & FLAG_REF))
              addressTaken = true;
          }

          // was the symbol set?
          // TODO: set svec member, set member
          if (call->isPrimitive(PRIM_MOVE) && call->get(1) == se)
            ndefs++;
          // Assigning to a ref doesn't change the ref itself
          if (call->isPrimitive(PRIM_ASSIGN) && !se->isRef() && call->get(1) == se)
            ndefs++;
        }
      }
    }
  }

  if (addressTaken || ndefs > 1) {
    // Turn off vectorization for this loop, we can't handle it yet
    this->vectorizeableSet(false);
  } else {
    // Enable vectorization but get the code generator to assert
    // that the inner symbols are always generated as SSA registers.
    for_alist(expr, body) {
      if (DefExpr* def = toDefExpr(expr))
        def->sym->addFlag(FLAG_LLVM_SSA_REGISTER);
    }
  }
}

// If vectorization is enabled and this loop is marked for vectorization,
// codegen CHPL_PRAGMA_IVDEP.
// This method is a no-op if vectorization is off, or the
// loop is not marked for vectorization.
void LoopStmt::codegenVectorHint()
{
  if (fNoVectorize == false && isVectorizeable())
  {
    GenInfo* info = gGenInfo;

    // Note: This *must* match the macro definitions provided in the runtime
    std:: string ivdepStr = "CHPL_PRAGMA_IVDEP";
    if (fReportOrderIndependentLoops)
    {
      ModuleSymbol *mod = toModuleSymbol(this->getModule());
      INT_ASSERT(mod);

      if (developer || mod->modTag == MOD_USER)
      {
        printf("Adding %s to %s for %s:%d\n", ivdepStr.c_str(),
            this->astTagAsString(), mod->name, this->linenum());
      }
    }

    info->cStatements.push_back(ivdepStr+'\n');
  }
}
