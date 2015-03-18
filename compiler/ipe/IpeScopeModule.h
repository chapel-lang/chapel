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

#ifndef _IPE_SCOPE_MODULE_H_
#define _IPE_SCOPE_MODULE_H_

#include "IpeScope.h"

class IpeModule;

class IpeScopeModule : public IpeScope
{
public:
                           IpeScopeModule(IpeModule* module);
  virtual                 ~IpeScopeModule();

  virtual void             extend(Symbol*  sym,
                                  IpeValue defaultValue,
                                  IpeVars* vars);

  virtual const char*      name()                                     const;

  virtual void             envPush();
  virtual void             envPop();

  IpeModule*               module()                                   const;

protected:
  virtual const char*      type()                                     const;
  virtual void             describeHeader(int offset)                 const;

private:
                           IpeScopeModule();

  IpeModule*               mModule;
};

#endif
