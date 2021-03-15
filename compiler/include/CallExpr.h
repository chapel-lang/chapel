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

#ifndef _CALL_EXPR_H_
#define _CALL_EXPR_H_

#include "expr.h"

enum TryTag {
  TRY_TAG_NONE,
  TRY_TAG_IN_TRY,
  TRY_TAG_IN_TRYBANG
};

class CallExpr final : public Expr {
public:
  PrimitiveOp* primitive;        // primitive expression (baseExpr == nullptr)
  Expr*        baseExpr;         // function expression

  AList        argList;          // function actuals

  bool         partialTag;
  bool         methodTag;        // Set to true if the call is a method call.
  bool         square;           // true if call made with square brackets
  TryTag       tryTag;

  CallExpr(BaseAST*     base,
           BaseAST*     arg1 = nullptr,
           BaseAST*     arg2 = nullptr,
           BaseAST*     arg3 = nullptr,
           BaseAST*     arg4 = nullptr,
           BaseAST*     arg5 = nullptr);

  CallExpr(PrimitiveOp* prim,
           BaseAST*     arg1 = nullptr,
           BaseAST*     arg2 = nullptr,
           BaseAST*     arg3 = nullptr,
           BaseAST*     arg4 = nullptr,
           BaseAST*     arg5 = nullptr);

  CallExpr(PrimitiveTag prim,
           BaseAST*     arg1 = nullptr,
           BaseAST*     arg2 = nullptr,
           BaseAST*     arg3 = nullptr,
           BaseAST*     arg4 = nullptr,
           BaseAST*     arg5 = nullptr);

  CallExpr(const char*  name,
           BaseAST*     arg1 = nullptr,
           BaseAST*     arg2 = nullptr,
           BaseAST*     arg3 = nullptr,
           BaseAST*     arg4 = nullptr,
           BaseAST*     arg5 = nullptr);

  ~CallExpr() override = default;

  void    verify() override;

  DECLARE_COPY(CallExpr);
  CallExpr* copyInner(SymbolMap* map) override;


  void    accept(AstVisitor* visitor) override;

  GenRet  codegen() override;
  void    prettyPrint(std::ostream* o) override;
  QualifiedType qualType() override;

  void    replaceChild(Expr* old_ast, Expr* new_ast) override;
  Expr*   getFirstExpr() override;
  Expr*   getNextExpr(Expr* expr) override;

  void            insertAtHead(BaseAST* ast);
  void            insertAtTail(BaseAST* ast);

  // True if the callExpr has been emptied (aka dead)
  bool            isEmpty()                                              const;

  bool            isCast();
  Expr*           castFrom();
  Expr*           castTo();

  bool            isPrimitive()                                          const;
  bool            isPrimitive(PrimitiveTag primitiveTag)                 const;
  bool            isPrimitive(const char*  primitiveName)                const;

  void            setUnresolvedFunction(const char* name);

  bool            isResolved()                                           const;
  FnSymbol*       resolvedFunction()                                     const;
  void            setResolvedFunction(FnSymbol* fn);
  FnSymbol*       resolvedOrVirtualFunction()                            const;

  FnSymbol*       theFnSymbol()                                          const;

  bool            isNamed(const char*)                                   const;
  bool            isNamedAstr(const char*)                               const;

  int             numActuals()                                           const;
  Expr*           get(int index)                                         const;
  FnSymbol*       findFnSymbol();

  void            convertToNoop();

  static void     registerPrimitivesForCodegen();

private:
  GenRet          codegenPrimitive();
  GenRet          codegenPrimMove();


  // Declare CallExpr::codegenPRIM_UNKNOWN() etc
#define PRIMITIVE_G(NAME) static void codegen ## NAME (CallExpr*, GenRet&);
#define PRIMITIVE_R(NAME)
#include "primitive_list.h"
#undef PRIMITIVE_G
#undef PRIMITIVE_R

  void            codegenInvokeOnFun();
  void            codegenInvokeTaskFun(const char* name);

  GenRet          codegenBasicPrimitiveExpr()                            const;

  bool            isRefExternStarTuple(Symbol* formal, Expr* actual)     const;
};

CallExpr* callChplHereAlloc(Type* type, VarSymbol* md = nullptr);

void      insertChplHereAlloc(Expr*      call,
                              bool       insertAfter,
                              Symbol*    sym,
                              Type*      t,
                              VarSymbol* md = nullptr);

CallExpr* callChplHereFree(BaseAST* p);

CallExpr* createCast(BaseAST* src, BaseAST* toType);

FnSymbol* resolvedToTaskFun(CallExpr* call);

static inline bool isEndOfStatementMarker(Expr* e) {
  if (CallExpr* call = toCallExpr(e))
    if (call->isPrimitive(PRIM_END_OF_STATEMENT))
      return true;

  return false;
}

inline FnSymbol* CallExpr::resolvedFunction() const {
  if (SymExpr* base = toSymExpr(baseExpr))
    return toFnSymbol(base->symbol());
  else
    return nullptr;
}

inline FnSymbol* CallExpr::theFnSymbol() const {
  return resolvedFunction();
}

inline bool CallExpr::isResolved() const {
  return resolvedFunction() != nullptr;
}

// TODO: rename these
bool isInitOrReturn(CallExpr* call, SymExpr*& lhsSe, CallExpr*& initOrCtor);
bool isRecordInitOrReturn(CallExpr* call, SymExpr*& lhsSe, CallExpr*& initOrCtor);

#endif
