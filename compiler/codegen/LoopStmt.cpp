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

void LoopStmt::reportVectorizable()
{
  if (!this->isVectorizable())
    return; // nothing more to do!

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
