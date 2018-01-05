/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _IPE_MODULE_ROOT_H_
#define _IPE_MODULE_ROOT_H_

#include "IpeModule.h"
#include "symbol.h"

#include <vector>

class ModuleSymbol;

class IpeModuleRoot : public IpeModule
{
  //
  // The Class interface
  //
public:
  static void              setHighWaterMark(int value);
  static IpeModuleRoot*    allocate();
  static IpeModule*        create(ModuleSymbol* sym);

private:
  static ModuleSymbol*     createDeclaration();

  static IpeModuleRoot*    sRootModule;
  static int               sHighWater;




  //
  // The Instance interface
  //
public:
  bool                     loadSystemModules();

  virtual                 ~IpeModuleRoot();

  void                     describeAllModules(int offset)               const;

protected:
  virtual const char*      moduleTypeAsString()                         const;

private:
                           IpeModuleRoot(ModuleSymbol* modSym);
                           IpeModuleRoot();

  void                     init();

  bool                     loadFile(ModTag      moduleType,
                                    const char* fileName);

  const char*              pathNameForFile(ModTag      moduleType,
                                           const char* fileName);
};

#endif
