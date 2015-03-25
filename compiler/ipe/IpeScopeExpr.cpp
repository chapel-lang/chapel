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

#include "IpeScopeExpr.h"

#include "IpeValue.h"

IpeScopeExpr::IpeScopeExpr(IpeScope* scope) : IpeScope(scope)
{

}

IpeScopeExpr::~IpeScopeExpr()
{

}

const char* IpeScopeExpr::type() const
{
  return "Expr";
}

const char* IpeScopeExpr::name() const
{
  return "";
}

void IpeScopeExpr::describeHeader(int offset) const
{

}

void IpeScopeExpr::extend(Symbol* sym, IpeValue defaultValue, IpeVars* vars)
{
  IpeScope::symbolPush(sym);
}

void IpeScopeExpr::envPush()
{

}

void IpeScopeExpr::envPop()
{

}
