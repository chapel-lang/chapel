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

/************************************ | *************************************
*                                                                           *
* Maintain lexical scoping information for AST definitions to support name  *
* resolution for the IPE.                                                   *
*                                                                           *
* It is important to note that these scopes are similar to but distinct     *
* from the dynamic environments that will be maintained by the IPE's        *
* runtime.                                                                  *
*                                                                           *
************************************* | ************************************/

#ifndef _DEF_SCOPE_H_
#define _DEF_SCOPE_H_

#include <vector>

class DefExpr;
class ModuleSymbol;
class UnresolvedSymExpr;
class VisibleSymbol;

class DefScope
{
  // Class state
public:
  static DefScope*                createRoot();
  static DefScope*                extend(const DefScope* parent);

private:
  static std::vector<DefScope*>   mScopes;

  // Instance state
public:
                                 ~DefScope();

  int                             depthGet()                                            const;
  void                            depthSet(int depth);

  void                            addDefinition (DefExpr*      expr);

  void                            extendByModule(ModuleSymbol* module);

  void                            visibleSymbols(UnresolvedSymExpr*          unres,
                                                 std::vector<VisibleSymbol>& functions) const;

private:
                                  DefScope();
                                  DefScope(const DefScope* parent);

  const DefScope*                 mParent;
  int                             mDepth;

  std::vector<DefExpr*>           mDefinitions;
};

#endif
