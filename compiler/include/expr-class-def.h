/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#ifndef _EXPR_CLASS_DEF_H_
#define _EXPR_CLASS_DEF_H_

class Expr : public BaseAST {
public:
   Expr(AstTag astTag);
  ~Expr() override = default;

  // Interface for BaseAST
          bool    inTree()                                        override;
  virtual bool    isStmt()                                           const;
          QualifiedType qualType()                                override;
          void    verify()                                        override;

  void verify(AstTag expectedTag); // ensure tag is as expected, then verify()
  void verifyParent(const Expr* child); // verify proper child->parentExpr

  // New interface
  virtual Expr*   copy(SymbolMap* map = NULL, bool internal = false)   = 0;
  virtual void    replaceChild(Expr* old_ast, Expr* new_ast)           = 0;

  virtual Expr*   getFirstExpr()                                       = 0;
  virtual Expr*   getNextExpr(Expr* expr);

  virtual bool    isNoInitExpr()                                     const;

  virtual void    prettyPrint(std::ostream* o);


  bool            isRef();
  bool            isWideRef();
  bool            isRefOrWideRef();

  /* Returns true if the given expression is contained by this one. */
  bool            contains(const Expr* expr)                         const;

  bool            isModuleDefinition();

  void            insertBefore(Expr* new_ast);
  void            insertAfter(Expr* new_ast);
  void            replace(Expr* new_ast);

  // Insert multiple ASTs in the order of the arguments.
  // Todo: replace with a single varargs version.
  void            insertAfter(Expr* e1, Expr* e2);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4,
                              Expr* e5);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4,
                              Expr* e5, Expr* e6);

  void            insertBefore(AList exprs);
  void            insertAfter(AList exprs);

  void            insertBefore(const char* format, ...);
  void            insertAfter(const char* format, ...);
  void            replace(const char* format, ...);

  Expr*           remove();

  bool            isStmtExpr()                                       const;
  Expr*           getStmtExpr();

  BlockStmt*      getScopeBlock();

  Symbol*         parentSymbol;
  Expr*           parentExpr;

  AList*          list;           // alist pointer
  Expr*           prev;           // alist previous pointer
  Expr*           next;           // alist next     pointer

private:
  virtual Expr*   copyInner(SymbolMap* map) = 0;
};

#endif
