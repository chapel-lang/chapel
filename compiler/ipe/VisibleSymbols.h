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

#ifndef _VISIBILE_SYMBOLS_H_
#define _VISIBILE_SYMBOLS_H_

#include <vector>

class Symbol;

class VisibleSymbols
{
public:
                         VisibleSymbols();
                        ~VisibleSymbols();

  int                    count()                                      const;

  void                   pushBack(Symbol* symbol, int distance);

  Symbol*                symbol  (int index)                          const;
  int                    distance(int index)                          const;

private:
  std::vector<Symbol*>   mSymbols;
  std::vector<int>       mDistances;
};

#endif
