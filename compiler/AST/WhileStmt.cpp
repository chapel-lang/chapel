/*
 * Copyright 2004-2014 Cray Inc.
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

#include "WhileStmt.h"

WhileStmt::WhileStmt(BlockStmt* initBody) : BlockStmt(initBody)
{

}

WhileStmt::~WhileStmt()
{

}

bool WhileStmt::isLoop() const 
{
  return true;
}

bool WhileStmt::isWhileLoop() const
{
  return true;
}

void WhileStmt::copyShare(const WhileStmt& ref,
                          SymbolMap*       mapRef, 
                          bool             internal) 
{
  SymbolMap  localMap;
  SymbolMap* map       = (mapRef != 0) ? mapRef : &localMap;
  CallExpr*  blockInfo = ref.blockInfoGet();

  astloc        = ref.astloc;
  blockTag      = ref.blockTag;

  breakLabel    = ref.breakLabel;
  continueLabel = ref.continueLabel;

  if (blockInfo != 0)
    blockInfoSet(blockInfo->copy(map, true));

  if (ref.modUses  != 0)
    modUses = ref.modUses->copy(map, true);

  if (ref.byrefVars != 0)
    byrefVars = ref.byrefVars->copy(map, true);

  for_alist(expr, ref.body)
    insertAtTail(expr->copy(map, true));

  if (internal == false)
    update_symbols(this, map);
}
