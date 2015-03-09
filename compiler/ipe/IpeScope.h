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

#ifndef _IPE_SCOPE_H_
#define _IPE_SCOPE_H_

class Symbol;
class IpeModule;
class IpeValue;
class IpeVars;
class UnresolvedSymExpr;
class VarSymbol;
class VisibleSymbols;

#include <vector>

class IpeScope
{
public:
  //
  // Class interface
  //

  static IpeScope*         rootAllocate();
  static IpeScope*         rootScope;

  //
  // Instance interface
  //

public:
  virtual                 ~IpeScope();

  virtual void             envPush();
  virtual void             envPop();

  virtual void             extend(Symbol*  sym,
                                  IpeValue defaultValue,
                                  IpeVars* vars)                            = 0;

  virtual const char*      name()                                     const = 0;

  void                     useAdd(IpeModule* use);

  VisibleSymbols           visibleSymbols(UnresolvedSymExpr* expr)    const;
  VisibleSymbols           visibleSymbols(Symbol*            sym)     const;
  VisibleSymbols           visibleSymbols(const char*        name)    const;

  void                     describe(int  offset     = 0,
                                    bool recursiveP = false)          const;

  int                      describeVariables(int offset, int index)   const;

protected:
                           IpeScope(IpeScope* parent);

  virtual const char*      type()                                     const = 0;
  virtual void             describeHeader(int offset)                 const = 0;

  void                     symbolPush(Symbol* sym);

private:
                           IpeScope();

  void                     findSymbols(const char*     name,
                                       int             distance,
                                       VisibleSymbols& symbols)       const;

  IpeScope*                mParent;

  std::vector<Symbol*>     mSymbols;
  std::vector<IpeModule*>  mUsedModules;
};

#endif
