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

#ifndef _IPE_ENVIRONMENT_H_
#define _IPE_ENVIRONMENT_H_

#include "IpeValue.h"

#include <vector>

class IpeSymbol;
class VarSymbol;

class IpeEnvironment
{
public:
                     IpeEnvironment(IpeEnvironment*                parent,
                                    const std::vector<IpeSymbol*>& symbols);

                    ~IpeEnvironment();

  IpeValue           lookup(IpeSymbol* variable);
  void               assign(IpeSymbol* variable, IpeValue value);

  void               describe()                                       const;

private:
                     IpeEnvironment();

  IpeEnvironment*    mParent;

  int                mSlotCount;
  IpeSymbol**        mSlotArray;

  void*              mData;
};

#endif
