/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _FOR_LOOP_H_
#define _FOR_LOOP_H_

#include "LoopStmt.h"

// A ForLoop represents the language construct:
//  for <idx> in <iterator> do <body>
// Its semantics are to let <idx> assume the value of each of the "indices"
// yielded by the iterator and execute the body once for each of these.
//
// Structurally, a ForLoop implies that there must be an underlying iterator.
// A reference to the underlying iterator is stored in the mIterator field.
// If the ForLoop is built with an <iterator> expression that is not an
// iterator, code must added to promote that index set to an iterator.  In the
// current implementation, this promotion is performed by _getIterator in
// ChapelIteratorSupport.chpl.
//
// If the <idx> symbol is NULL, that means that the value of each
// yielded element is not used in the body of the loop: the ForLoop effectively
// just "counts" the elements in the underlying iterator -- executing the body
// of the loop once for each.  A reference to the index symbol is stored in the
// mIndex field.
//
class ForLoop : public LoopStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildForLoop (Expr*      indices,
                                       Expr*      iteratorExpr,
                                       BlockStmt* body,
                                       bool       coforall,
                                       bool       zippered);

  //
  // Instance Interface
  //
public:
                         ForLoop(VarSymbol* index,
                                 VarSymbol* iterator,
                                 BlockStmt* initBody);
  virtual               ~ForLoop();

  virtual ForLoop*       copy(SymbolMap* map      = NULL,
                              bool       internal = false);

  virtual GenRet         codegen();
  virtual void           verify();
  virtual void           accept(AstVisitor* visitor);

  // Interface to Expr
  virtual void        replaceChild(Expr* oldAst, Expr* newAst);
  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

  virtual bool           isForLoop()                                  const;
  virtual bool           isCoforallLoop()                             const;

  virtual bool           deadBlockCleanup();

  BlockStmt*             copyBody();
  BlockStmt*             copyBody(SymbolMap* map);

  SymExpr*               indexGet()                                   const;
  SymExpr*               iteratorGet()                                const;

  virtual CallExpr*      blockInfoGet()                               const;
  virtual CallExpr*      blockInfoSet(CallExpr* expr);

private:
                         ForLoop();

  SymExpr*               mIndex;
  SymExpr*               mIterator;
};

#endif

