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

#ifndef _IPE_MODULE_H_
#define _IPE_MODULE_H_

class IpeEnv;
class IpeModuleRoot;
class IpeScopeModule;
class IpeSequence;

class Expr;
class DefExpr;
class ModuleSymbol;

class IpeModule
{
public:
  virtual                 ~IpeModule();

  const char*              name()                                             const;
  IpeEnv*                  environment()                                      const;
  IpeScopeModule*          scopeGet()                                         const;

  void                     moduleAdd(IpeModule* module);
  IpeModule*               moduleByName(const char* name)                     const;

  void                     moduleEvaluate();
  void                     moduleResolve();
  void                     initialize();

  void                     describe(int offset)                               const;

protected:
  enum State
  {
    kLoaded,

    kResolving,
    kResolved,

    kInitializing,
    kInitialized
  };

                           IpeModule(IpeModule*    parent,
                                     ModuleSymbol* modSym);

  virtual const char*      moduleTypeAsString()                               const = 0;

  const char*              stateAsString()                                    const;

  void                     ensureInitialized();

  State                    mState;

  IpeEnv*                  mEnv;

  ModuleSymbol*            mModSym;

  IpeSequence*             mBodyResolved;

private:
                           IpeModule();
};

#endif
