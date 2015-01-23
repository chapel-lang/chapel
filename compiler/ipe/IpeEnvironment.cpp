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

#include "IpeEnvironment.h"

#include "IpeSymbol.h"

#include <cstdlib>

IpeEnvironment::IpeEnvironment(IpeEnvironment*                parent,
                               const std::vector<IpeSymbol*>& symbols)
{
  mParent    = parent;

  mSlotCount = symbols.size();
  mSlotArray = (IpeSymbol**) malloc(mSlotCount * sizeof(IpeSymbol*));

  for (int i = 0; i < mSlotCount; i++)
    mSlotArray[i] = symbols[i];

  // 2015/01/14 NOAKES.  Currently assumes objects are constant size
  mData      = malloc(8 * mSlotCount);
}

IpeEnvironment::~IpeEnvironment()
{
  if (mSlotArray != 0)
    free(mSlotArray);

  if (mData     != 0)
    free(mData);
}

IpeValue IpeEnvironment::lookup(IpeSymbol* variable)
{
  Type*    type = variable->type;
  void*    env  = (((char*) mData) + variable->offset());
  IpeValue retval;

  if (strcmp(type->symbol->name, "bool") == 0)
    retval.bValue = *((bool*) env);

  else if (strcmp(type->symbol->name, "int") == 0)
    retval.iValue = *((long*) env);

  else if (strcmp(type->symbol->name, "real") == 0)
    retval.rValue = *((double*) env);

  else
  {
    retval.iValue = 0;
    INT_ASSERT(false);
  }


  return retval;
}

void IpeEnvironment::assign(IpeSymbol* variable, IpeValue value)
{
  Type* type = variable->type;
  void* env  = (((char*) mData) + variable->offset());

  if (strcmp(type->symbol->name, "bool") == 0)
    *((bool*) env)   = value.bValue;

  else if (strcmp(type->symbol->name, "int") == 0)
    *((long*) env)   = value.iValue;

  else if (strcmp(type->symbol->name, "real") == 0)
    *((double*) env) = value.rValue;

  else
    INT_ASSERT(false);
}

void IpeEnvironment::describe() const
{
  printf("Environment\n");

  for (int i = 0; i < mSlotCount; i++)
  {
    IpeSymbol* slot = mSlotArray[i];
    void*      env  = (void*) (((char*) mData) + slot->offset());

    if (Type* type = slot->type)
    {
      printf("      %-20s %-4s ", slot->name, type->symbol->name);

      if (strcmp(type->symbol->name, "bool") == 0)
      {
        if (*((long*) env) == 0)
          printf("       false\n");
        else
          printf("        true\n");
      }

      else if (strcmp(type->symbol->name, "int") == 0)
      {
        printf("%12lu\n", *((long*) env));
      }

      else if (strcmp(type->symbol->name, "real") == 0)
      {
        printf("     %12.4f\n", *((double*) env));
      }
    }
  }
}

