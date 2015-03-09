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

#include "IpeValue.h"

IpeValue::IpeValue()
{
  mValue.iValue       = 0;
}

IpeValue::IpeValue(bool value)
{
  mValue.iValue       = (value == true) ? 1 : 0;
}

IpeValue::IpeValue(IpeModule* value)
{
  mValue.modulePtr    = value;
}

IpeValue::IpeValue(IpeProcedure* value)
{
  mValue.procedurePtr = value;
}


bool IpeValue::boolGet() const
{
  return (mValue.iValue == 1) ? true : false;
}

void IpeValue::boolSet(bool value)
{
  mValue.iValue = (value == true) ? 1 : 0;
}

long IpeValue::integerGet() const
{
  return mValue.iValue;
}

void IpeValue::integerSet(long value)
{
  mValue.iValue = value;
}

double IpeValue::realGet() const
{
  return mValue.rValue;
}

void IpeValue::realSet(double value)
{
  mValue.rValue = value;
}

const char* IpeValue::cstringGet() const
{
  return mValue.sValue;
}

void IpeValue::cstringSet(const char* value)
{
  mValue.sValue = value;
}

IpeModule* IpeValue::moduleGet() const
{
  return mValue.modulePtr;
}

IpeProcedure* IpeValue::procedureGet() const
{
  return mValue.procedurePtr;
}

IpeValue* IpeValue::refGet() const
{
  return mValue.valuePtr;
}

void IpeValue::refSet(IpeValue* value)
{
  mValue.valuePtr = value;
}
