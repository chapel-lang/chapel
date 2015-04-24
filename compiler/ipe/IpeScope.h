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

class ArgSymbol;
class IpeModule;
class LcnSymbol;
class UnresolvedSymExpr;
class VarSymbol;

#include <vector>

class IpeScope
{
public:
  virtual                 ~IpeScope();

  virtual const char*      type()                                             const = 0;
  virtual const char*      name()                                             const = 0;

  virtual int              locationSet(ArgSymbol* arg)                        const = 0;
  virtual int              locationSet(VarSymbol* var)                        const = 0;

  virtual int              frameSize()                                        const = 0;

  virtual bool             isScopeBlockStmt()                                 const;
  virtual bool             isScopeModule()                                    const;

  bool                     hasParent()                                        const;
  void                     parentSummarize(char* pad)                         const;

  void                     useAdd(IpeModule* use);
  int                      useCount()                                         const;
  IpeModule*               useGet(int index)                                  const;

  void                     varAdd(LcnSymbol* variable);
  int                      varCount()                                         const;
  LcnSymbol*               varGet(int index)                                  const;

  bool                     isDefinedLocally(const char* name)                 const;

  LcnSymbol*               findVariable(UnresolvedSymExpr* expr,
                                        bool               localOnly = false) const;

  LcnSymbol*               findVariable(const char*        name,
                                        bool               localOnly = false) const;

protected:
                           IpeScope(IpeScope* parent);

  IpeScope*                mParent;
  std::vector<LcnSymbol*>  mVariables;
  std::vector<IpeModule*>  mUsedModules;

private:
                           IpeScope();
};

#endif
