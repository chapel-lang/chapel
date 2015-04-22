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

#ifndef _IPE_MODULE_ROOT_H_
#define _IPE_MODULE_ROOT_H_

#include "IpeModule.h"

class ModuleSymbol;

class IpeModuleRoot : public IpeModule
{
  //
  // The Class interface
  //
public:
  static void              setHighWaterMark(int value);
  static IpeModuleRoot*    allocate();

private:
  static ModuleSymbol*     createDeclaration();

  static IpeModuleRoot*    sRootModule;
  static int               sHighWater;

  //
  // The Instance interface
  //
protected:
  virtual const char*      moduleTypeAsString()                         const;

private:
                           IpeModuleRoot(ModuleSymbol* sym);
                           IpeModuleRoot();

  virtual                 ~IpeModuleRoot();

  void                     init();
};

#endif
