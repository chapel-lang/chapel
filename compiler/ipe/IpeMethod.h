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

#ifndef _IPE_METHOD_H_
#define _IPE_METHOD_H_

#include <vector>

class IpeCallExpr;
class IpeEnv;
class IpeScope;
class IpeScopeMethod;
class IpeValue;

class Expr;
class CallExpr;
class DefExpr;
class SymExpr;
class UnresolvedSymExpr;

class BlockStmt;
class CondStmt;
class WhileDoStmt;

class ArgSymbol;
class FnSymbol;
class VarSymbol;

class Type;

class IpeMethod
{
public:
                          IpeMethod(FnSymbol* sym, IpeEnv* parentEnv);
                         ~IpeMethod();

  const char*             name()                                                     const;
  Type*                   typeGet()                                                  const;
  FnSymbol*               fnSymbol()                                                 const;

  // Zero based
  ArgSymbol*              formalGet(int index)                                       const;

  bool                    resolveFormals();
  bool                    resolveReturnType();

  bool                    isExactMatch(std::vector<Expr*>& actuals)                  const;

  IpeValue                apply(IpeCallExpr* expr, IpeEnv* env);

  void                    describe(int offset, bool fullP = false)                   const;

private:
  enum State
  {
    kUnresolved,
    kResolvedFormals,
    kResolvedReturnType,  // Implies resolved formals

    kResolved,
    kUnresolvable
  };
                          IpeMethod();

  const char*             stateAsString()                                            const;

  bool                    prepareBody();
  void                    resolveBody();
  int                     assignLocations()                                          const;

  bool                    isExternFunction(FnSymbol* fn)                             const;
  IpeValue                externFunctionInvoke(IpeEnv* env)                          const;

  bool                    isWriteln()                                                const;
  void                    writeln1(              IpeValue value, Type* type)         const;
  void                    writeln2(IpeValue msg, IpeValue value, Type* type)         const;

  bool                    isQuit()                                                   const;

  State                   mState;
  IpeEnv*                 mEnvParent;
  IpeScope*               mScope;
  int                     mFrameSize;

  FnSymbol*               mFnDecl;
  Expr*                   mBody;

  int                     mInvokeCount;
};

#endif
