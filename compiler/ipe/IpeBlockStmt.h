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

#ifndef _IPE_BLOCK_STMT_H_
#define _IPE_BLOCK_STMT_H_

#include "IpeSequence.h"

#include <vector>

class Expr;
class IpeEnv;

class IpeBlockStmt : public IpeSequence
{
public:
                           IpeBlockStmt(const std::vector<Expr*>& stmts,
                                        IpeEnv*                   env);

  virtual                 ~IpeBlockStmt();

  virtual bool             isScopeless()                            const;
  virtual void             describe(int offset);

  IpeEnv*                  envGet()                                 const;

private:
                           IpeBlockStmt();

  IpeEnv*                  mEnv;
};

#endif
