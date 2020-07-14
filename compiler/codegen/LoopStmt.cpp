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

#include "LoopStmt.h"

#include "codegen.h"
#include "driver.h"

static void markVectorizationHazards(LoopStmt* loop) {
  bool parallelLoopAccessHazard = false;
  bool vectorizationHazard = false;

  bool report = false;
  if (fReportVectorizedLoops) {
    ModuleSymbol *mod = toModuleSymbol(loop->getModule());
    INT_ASSERT(mod);

    report = (developer || mod->modTag == MOD_USER);
  }

  if (loop->id == 1851472)
    gdbShouldBreakHere();

  std::vector<DefExpr*> defExprs;
  collectDefExprs(loop, defExprs);

  for_vector(DefExpr, def, defExprs) {
    Symbol* sym = def->sym;

    // Decide if it could be a register
    // (Note, these rules are accurate for LLVM but
    //  serve as a reasonable guess for a C compiler)
    bool addressTaken = false;
    bool needsGets = false;
    int ndefs = 0;

    // Decide if it is used outside of the loop
    // (allocas only used within the loop are OK for RV)
    bool defVarUsedOutsideOfLoop = false;

    for_SymbolSymExprs(se, sym) {
      if (!loop->contains(se)) {
        defVarUsedOutsideOfLoop = true;
      }
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

    if (parallelLoopAccessHazard == false) {
      if (addressTaken || needsGets || ndefs > 1) {
        parallelLoopAccessHazard = true;

        if (report) {
          if (addressTaken)
            USR_PRINT(sym, "parallel access disabled -- address taken");
          else if (ndefs > 1)
            USR_PRINT(sym, "parallel access disabled -- multiple defs");
          else if (needsGets)
            USR_PRINT(sym, "parallel access disabled -- could GET");
        }
      }
    }

    if (vectorizationHazard == false) {
      if (defVarUsedOutsideOfLoop) {
        vectorizationHazard = true;
        parallelLoopAccessHazard = true;

        if (report) {
          USR_PRINT(sym, "vectorization disabled -- def used outside loop");
        }
      }
    }
  }

  // save vectorization hazard info in the loop
  if (parallelLoopAccessHazard)
    loop->setHasParallelAccessVectorizationHazard(true);

  if (vectorizationHazard)
    loop->setHasVectorizationHazard(true);
}

void LoopStmt::fixVectorizable()
{
  if (!this->isVectorizable())
    return; // nothing more to do!

  // Note: this routine could also return early if fNoVectorize==true.
  // However letting it run in that case gives better testing coverage.

  // Check for DefExprs in the loop body that would present challenges
  // for vectorization.

  // This runs late in the compiler because we want to do this check
  // after inlining and other late simplifications have occurred.

  // Look for hazards if we're not told to ignore them.
  if (fForceVectorize == false) {
    markVectorizationHazards(this);
  }

  if (fReportVectorizedLoops) {
    ModuleSymbol *mod = toModuleSymbol(this->getModule());
    INT_ASSERT(mod);

    if (developer || mod->modTag == MOD_USER)
    {
      if (this->isVectorizable()) {
        const char* kind = NULL;

        // if there is a vectorization hazard, we shouldn't
        // have considered it vectorizable at all.
        INT_ASSERT(!this->hasVectorizationHazard());

        if (this->hasParallelAccessVectorizationHazard())
          kind = "loop vectorization (no parallel access)";
        else
          kind = "loop vectorization (with parallel access)";

        if (developer)
          USR_PRINT(this, "%s hinted for %s [%i]", kind,
                    this->astTagAsString(), this->id);
        else
          USR_PRINT(this, "%s hinted for %s", kind,
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
  if (fNoVectorize == false && isParallelAccessVectorizable())
  {
    GenInfo* info = gGenInfo;

    // Note: This *must* match the macro definitions provided in the runtime
    info->cStatements.push_back("CHPL_PRAGMA_IVDEP\n");
  }
}
