/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _LOOP_WITH_SHADOW_VARS_INTERFACE_STMT_H_
#define _LOOP_WITH_SHADOW_VARS_INTERFACE_STMT_H_

#include "stmt.h"

// Both 'forall' and 'foreach' loops have shadow variables. We have code that we want to be able
// to process either but the nearest common ancestor to these two classes is 'Expr'.
// In the long run it might be beneficial to rearrange our class hierarchy so these two share
// a closer ancestor but for the time being I use the following interface for the shared
// behavior between the two that's used when processing shadow variables.
struct LoopWithShadowVarsInterface {
  virtual AList& shadowVariables() = 0;
  virtual bool needToHandleOuterVars() const = 0;
  virtual BlockStmt* loopBody() const = 0;
  virtual bool needsInitialAccumulate() const = 0;
  virtual Expr* asExpr() = 0;
  virtual bool isInductionVar(Symbol* sym) = 0;

  virtual bool isForallStmt() = 0;
  virtual ForallStmt *forallStmt() = 0;
};

#endif
