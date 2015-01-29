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
                               const std::vector<IpeSymbol*>& formals)
{
  mParent    = parent;

  mDepth     = (parent == 0) ? 0 : parent->mDepth + 1;

  mSlotCount = formals.size();
  mSlotArray = (IpeSymbol**) malloc(mSlotCount * sizeof(IpeSymbol*));

  for (int i = 0; i < mSlotCount; i++)
    mSlotArray[i] = formals[i];

  // 2015/01/14 NOAKES.  Currently assumes objects are constant size
  mData      = malloc(8 * mSlotCount);
}

IpeEnvironment::IpeEnvironment(IpeEnvironment*                parent,
                               const std::vector<IpeSymbol*>& formals,
                               const std::vector<IpeValue>&   actuals)
{
  INT_ASSERT(formals.size() == actuals.size());

  mParent    = parent;

  mDepth     = (parent == 0) ? 0 : parent->mDepth + 1;

  mSlotCount = formals.size();
  mSlotArray = (IpeSymbol**) malloc(mSlotCount * sizeof(IpeSymbol*));

  for (size_t i = 0; i < formals.size(); i++)
    mSlotArray[i] = formals[i];

  // 2015/01/14 NOAKES.  Currently assumes objects are constant size
  mData      = malloc(8 * mSlotCount);

  for (size_t i = 0; i < formals.size(); i++)
    assign(formals[i], actuals[i]);
}

IpeEnvironment::IpeEnvironment(IpeEnvironment*                parent,
                               const std::vector<IpeSymbol*>& formals,
                               const std::vector<IpeValue>&   actuals,
                               int                            slotCount,
                               int                            dataSize)
{
  INT_ASSERT(actuals.size() == formals.size());
  INT_ASSERT(slotCount      >= formals.size());
  INT_ASSERT(dataSize       >= 8 * slotCount);

  mParent    = parent;

  mDepth     = (parent == 0) ? 0 : parent->mDepth + 1;

  mSlotCount = slotCount;
  mSlotArray = (IpeSymbol**) malloc(mSlotCount * sizeof(IpeSymbol*));

  for (size_t i = 0; i < formals.size(); i++)
    mSlotArray[i] = formals[i];

  for (size_t i = formals.size(); i < slotCount; i++)
    mSlotArray[i] = NULL;

  mData      = malloc(dataSize);

  for (size_t i = 0; i < formals.size(); i++)
    assign(formals[i], actuals[i]);
}

IpeEnvironment::~IpeEnvironment()
{
  if (mSlotArray != 0)
    free(mSlotArray);

  if (mData     != 0)
    free(mData);
}


#include "AstDumpToNode.h"

IpeValue IpeEnvironment::lookup(IpeSymbol* variable) const
{
  const IpeEnvironment* frame = this;

  while (frame->mDepth > variable->depth())
    frame = frame->mParent;

  Type*    type = variable->type;
  void*    env  = (((char*) frame->mData) + variable->offset());
  IpeValue retval;

  if (strcmp(type->symbol->name, "bool") == 0)
    retval.bValue = *((bool*)   env);

  else if (strcmp(type->symbol->name, "int") == 0)
    retval.iValue = *((long*)   env);

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
  IpeEnvironment* frame = this;

  while (frame->mDepth > variable->depth())
    frame = frame->mParent;

  Type* type = variable->type;
  void* env  = (((char*) frame->mData) + variable->offset());

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
  printf("Environment %3d\n", mDepth);

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

