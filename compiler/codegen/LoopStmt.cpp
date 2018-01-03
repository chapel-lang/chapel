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

// If vectorization is enabled and this loop is order independent, codegen
// CHPL_PRAGMA_IVDEP. This method is a no-op if vectorization is off, or the
// loop is not order independent.
void LoopStmt::codegenOrderIndependence()
{
  if (fNoVectorize == false && isOrderIndependent())
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
