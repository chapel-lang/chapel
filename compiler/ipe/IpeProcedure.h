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

#ifndef _IPE_PROCEDURE_H_
#define _IPE_PROCEDURE_H_

#include <vector>

class Expr;
class IpeCallExpr;
class IpeMethod;
class SymExpr;

class IpeProcedure
{
public:
                            IpeProcedure(const char* identifierName);
                           ~IpeProcedure();

  const char*               name()                                                   const;

  bool                      isValid(int generationId)                                const;

  int                       methodCount()                                            const;
  void                      methodAdd(IpeMethod* method);
  IpeMethod*                methodGet(int index)                                     const;

  IpeCallExpr*              resolve(SymExpr* procExpr, std::vector<Expr*>& actuals)  const;

  void                      describe(int offset)                                     const;

private:
                            IpeProcedure();

  bool                      isActualRef(Expr* actual)                                const;

  const char*               mIdentifierName;
  int                       mVersion;
  std::vector<IpeMethod*>   mMethods;
};

#endif
