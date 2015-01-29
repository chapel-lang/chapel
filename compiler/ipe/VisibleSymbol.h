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
* Function resolution requires a scan through a chain of DefScopes to find  *
* all visible functions with a given name followed by a complex algorithm   *
* that attempts to select a preferred function based on matching the list   *
* of formals to the list of actuals.                                        *
*                                                                           *
* This data structure is used to record an instance of a match. The         *
* resolution algorithm is depdendent on the distance, in terms of lexical   *
* scope, between the caller and the definition and so we need both the      *
* value and the scope in which the value was found.                         *
*                                                                           *
************************************* | ************************************/

#ifndef _VISIBLE_SYMBOL_H_
#define _VISIBLE_SYMBOL_H_

class ScopeBase;
class Symbol;

class VisibleSymbol
{
public:
                               VisibleSymbol(Symbol*          sym,
                                             const ScopeBase* scope,
                                             int              distance);
                              ~VisibleSymbol();

  Symbol*                      symbol()                           const;
  const ScopeBase*             scope()                            const;
  int                          distance()                         const;

private:
                               VisibleSymbol();

  Symbol*                      mSymbol;
  const ScopeBase*             mScope;
  int                          mDistance;
};

#endif
