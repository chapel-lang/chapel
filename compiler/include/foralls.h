/*
 * Copyright 2004-2017 Cray Inc.
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

#ifndef _FORALLS_H_
#define _FORALLS_H_

//// foralls.h, foralls.cpp - support for forall loops ////

//
// TFITag: a task or forall intent
//
enum TFITag {
  TFI_DEFAULT, // aka TFI_BLANK
  TFI_CONST,
  TFI_IN,
  TFI_CONST_IN,
  TFI_REF,
  TFI_CONST_REF,
  TFI_REDUCE,
};

const char* tfiTagDescrString(TFITag tfiTag);

//
// ForallIntents: with clause/forall intents
//
class ForallIntents {
public:
  std::vector<Expr*>   fiVars;   // affected variables
  std::vector<TFITag>  fIntents; // associated intents
  std::vector<Expr*>   riSpecs;  // reduce intent info

  // used in implementForallIntents1()
  SymExpr* iterRec;
  SymExpr* leadIdx;
  SymExpr* leadIdxCopy;

  ForallIntents();    // constructor
  ~ForallIntents() {} // destructor: just deallocate the vectors

  int   numVars() { return fiVars.size(); }
  bool  isReduce(int idx) { return fIntents[idx] == TFI_REDUCE; }

  // Support standard AST operations on the enclosing BlockStmt.
  ForallIntents* copy(SymbolMap* map, bool internal);
  bool replaceChildFI(Expr* oldAst, Expr* newAst);
  void removeFI(Expr* parentB);
  void verifyFI(BlockStmt* parentB);
  void acceptFI(AstVisitor* visitor);
};

bool astUnderFI(const Expr* ast, ForallIntents* fi);

#define for_riSpecs_vector(VAL, FI) \
  for_vector_allowing_0s(Expr, VAL, (FI)->riSpecs)

#endif
