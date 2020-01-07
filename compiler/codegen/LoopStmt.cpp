/*
 * Copyright 2004-2020 Cray Inc.
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

void LoopStmt::fixVectorizable()
{
  if (!this->isVectorizable())
    return; // nothing more to do!

  // Note: this routine could also return early if fNoVectorize==true.
  // However letting it run in that case gives better testing coverage.

  bool report = false;
  if (fReportVectorizedLoops) {
    ModuleSymbol *mod = toModuleSymbol(this->getModule());
    INT_ASSERT(mod);

    report = (developer || mod->modTag == MOD_USER);
  }

  // Check for DefExprs in the loop body that would present challenges
  // for vectorization.

  // DefExprs for values that we expect to be represented as registers
  // are OK.

  // Alternatively we could clone these allocations per vector
  // lane. We're not quite ready to do that yet though, and ones
  // that don't need to be cloned should be in registers anyway.

  // This runs late in the compiler because we want to do this check
  // after inlining and other late simplifications have occurred.

  bool hazard = false;

  // Look for hazards if we're not told to ignore them.
  if (fForceVectorize == false) {

    for_alist(expr, body) {
      if (DefExpr* def = toDefExpr(expr)) {
        Symbol* sym = def->sym;

        // Decide if it could be a register
        // (Note, these rules are accurate for LLVM but
        //  serve as a reasonable guess for a C compiler)
        bool addressTaken = false;
        bool needsGets = false;
        int ndefs = 0;

        for_SymbolSymExprs(se, sym) {
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            // was the address taken?
            if (call->isPrimitive(PRIM_ADDR_OF) ||
                call->isPrimitive(PRIM_SET_REFERENCE) ||
                call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                call->isPrimitive(PRIM_GET_MEMBER)) {
              addressTaken = true;
            } else if (call->isPrimitive(PRIM_SET_MEMBER) ||
                       call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
              // These don't technically necessarily use a ref
              // but they probably do and they could potentially do a GET
              addressTaken = true;
              needsGets = true;
            } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
              // Ignore method name and id arguments
              if (se != call->get(1) && se != call->get(2)) {
                ArgSymbol* formal = actual_to_formal(se);
                if ((formal->intent & FLAG_REF))
                  addressTaken = true;
              }
            } else if (call->resolvedFunction()) {
              ArgSymbol* formal = actual_to_formal(se);
              if ((formal->intent & FLAG_REF))
                addressTaken = true;
            }

            // was the symbol set?
            // Also, would setting the symbol require us to generate a GET?
            if (call->isPrimitive(PRIM_MOVE) &&
                call->get(1) == se) {
              ndefs++;
            }
            // Assigning to a ref doesn't change the ref itself
            if (call->isPrimitive(PRIM_ASSIGN) &&
                !se->isRef() &&
                call->get(1) == se) {
              ndefs++;
              if (call->get(2)->isWideRef())
                needsGets = true;
            }

            // Would setting the symbol require a GET?
            if (call->isPrimitive(PRIM_MOVE) && call->isPrimitive(PRIM_ASSIGN)) {
              if (CallExpr* rhsCall = toCallExpr(call->get(2)))
                if (rhsCall->isPrimitive(PRIM_DEREF) ||
                    rhsCall->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                    rhsCall->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE))
                  if (rhsCall->get(1)->isWideRef())
                    needsGets = true;
            }
          }
        }

        // with --llvm-wide-opt, GETs are just regular loads, so there are
        // no concerns about needing a temporary for a GET.
        if (fLLVMWideOpt)
          needsGets = false;

        if (hazard == false) {
          if (addressTaken || needsGets || ndefs > 1) {
            hazard = true;

            if (report) {
              if (addressTaken)
                USR_PRINT(sym, "Vectorization disabled -- address taken");
              else if (ndefs > 1)
                USR_PRINT(sym, "Vectorization disabled -- multiple defs");
              else if (needsGets)
                USR_PRINT(sym, "Vectorization disabled -- could GET");
            }
          }
        }
      }
    }
  }

  if (hazard) {
    // Turn off vectorization for this loop, we can't handle it yet
    this->setHasVectorizationHazard(true);
  } else {
    if (fReportVectorizedLoops) {
      ModuleSymbol *mod = toModuleSymbol(this->getModule());
      INT_ASSERT(mod);

      if (developer || mod->modTag == MOD_USER)
      {
        if (developer)
          USR_PRINT(this, "Vectorization hinted for %s [%i]",
                    this->astTagAsString(), this->id);
        else
          USR_PRINT(this, "Vectorization hinted for %s",
                    this->astTagAsString());
      }
    }
  }
}

// If vectorization is enabled and this loop is marked for vectorization,
// codegen CHPL_PRAGMA_IVDEP.
// This method is a no-op if vectorization is off, or the
// loop is not marked for vectorization.
void LoopStmt::codegenVectorHint()
{
  if (fNoVectorize == false && isVectorizable())
  {
    GenInfo* info = gGenInfo;

    // Note: This *must* match the macro definitions provided in the runtime
    info->cStatements.push_back("CHPL_PRAGMA_IVDEP\n");
  }
}
