/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include "CallExpr.h"
#include "stmt.h"
#include "symbol.h"

// interface for normalize
void arrayViewElision();

class ArrayViewElisionTransformer {
public:
  ArrayViewElisionTransformer() = delete;
  ArrayViewElisionTransformer(CallExpr* origCall);

  inline bool candidate() const { return candidate_; }

  void transform();

private:
  CallExpr* origCall_;
  CallExpr* origLhs_;
  CallExpr* origRhs_;

  bool candidate_;

  bool exprSuitableForProtoSlice(CallExpr* e, bool isLhs);
  CallExpr* genCreateProtoSlice(CallExpr* call);
};

// interface for prefold
class ArrayViewElisionPrefolder {
public:
  ArrayViewElisionPrefolder() = delete;
  ArrayViewElisionPrefolder(CallExpr* call);
  ~ArrayViewElisionPrefolder();

  inline CondStmt* condStmt() const { return condStmt_; }
  inline bool supported() const { return supported_; }
  inline BlockStmt* staticCheckBlock() const { return staticCheckBlock_; }

  CallExpr* getReplacement();
  void report();
  void updateAndFoldConditional();

private:
  CallExpr* call_;
  Symbol* lhsBase_;
  Symbol* rhsBase_;
  CallExpr* newProtoSliceLhs_;
  CallExpr* newProtoSliceRhs_;
  CondStmt* condStmt_;
  Symbol* tmpCondFlag_;
  bool supported_;
  BlockStmt* staticCheckBlock_;

  // support for report-array-view-elision
  std::string lhsBaseType_;
  std::string rhsBaseType_;

  std::vector<std::string> lhsIndexExprTypes_;
  std::vector<std::string> rhsIndexExprTypes_;

  void findCondStmt();
  void findProtoSlices();
  bool handleOneProtoSlice(bool isLhs);
  bool canAssign() const;
  CallExpr* findOneProtoSliceCall(Expr* e);
  Symbol* getFlagReplacement();
};

