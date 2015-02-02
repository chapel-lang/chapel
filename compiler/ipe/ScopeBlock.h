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

#ifndef _SCOPE_BLOCK_H_
#define _SCOPE_BLOCK_H_

#include "ScopeBase.h"

class BlockStmt;

class ScopeBlock : public ScopeBase
{
public:
                        ScopeBlock(BlockStmt*       block,
                                   const ScopeBase* parent);
  virtual              ~ScopeBlock();

private:
                        ScopeBlock();

  virtual void          describeHeader(FILE* fp)                    const;

  BlockStmt*            mBlock;
};

#endif
