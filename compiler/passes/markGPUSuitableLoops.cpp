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

#include "primitive.h"
#include "ForLoop.h"
#include "ForallStmt.h"

extern int classifyPrimitive(CallExpr *call, bool inLocal);
extern bool inLocalBlock(CallExpr *call);

static bool blockLooksLikeStreamForGPU(BlockStmt* blk) {
  if (blk->getModule()->modTag != MOD_USER)
    return false;
  std::vector<CallExpr*> calls;
  collectCallExprs(blk, calls);
  for_vector(CallExpr, call, calls) {
    if (call->primitive) {
      // only primitives that are fast and local are allowed for now
      bool inLocal = inLocalBlock(call);
      int is = classifyPrimitive(call, inLocal);
      if ((is != FAST_AND_LOCAL)) {
        return false;
      }
    }
  }
  return true;
}


void markGPUSuitableLoops() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* forLoop = toForLoop(block)) {
      if (forLoop->isOrderIndependent()) {
        if (blockLooksLikeStreamForGPU(forLoop)) {
          printf("found suitable GPU loop at %s:%d\n", forLoop->fname(), forLoop->linenum());
          forLoop->setIsGPUSuitable(true);
        }
      }
    }
  }

  forv_Vec(ForallStmt, fs, gForallStmts) {
    if (blockLooksLikeStreamForGPU(fs->loopBody())) {
      printf("found suitable GPU loop at %s:%d\n", fs->fname(), fs->linenum());
      fs->setIsGPUSuitable(true);
    }
  }
}

