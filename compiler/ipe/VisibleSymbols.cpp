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

#include "VisibleSymbols.h"

VisibleSymbols::VisibleSymbols()
{

}

VisibleSymbols::~VisibleSymbols()
{

}

int VisibleSymbols::count() const
{
  return mSymbols.size();
}

void VisibleSymbols::pushBack(Symbol* symbol, int distance)
{
  mSymbols.push_back(symbol);
  mDistances.push_back(distance);
}

Symbol* VisibleSymbols::symbol(int index) const
{
  Symbol* retval = 0;

  if (index >= 0 && index < count())
    retval = mSymbols[index];

  return retval;
}

int VisibleSymbols::distance(int index) const
{
  int retval = -1;

  if (index >= 0 && index < count())
    retval = mDistances[index];

  return retval;
}

