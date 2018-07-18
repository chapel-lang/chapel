/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _ITERATOR_H_
#define _ITERATOR_H_
#include "symbol.h"
#include "vec.h"

class BaseAST;
class AggregateType;
class FnSymbol;
class CallExpr;

enum IteratorTag { 
  it_iterator, 
  it_leader,
  it_follower 
};

class IteratorInfo {
public:
  IteratorInfo();

  IteratorTag    tag;

  FnSymbol*      iterator;
  FnSymbol*      getIterator;
  AggregateType* iclass;
  AggregateType* irecord;
  FnSymbol*      advance;
  FnSymbol*      zip1;
  FnSymbol*      zip2;
  FnSymbol*      zip3;
  FnSymbol*      zip4;
  FnSymbol*      hasMore;
  FnSymbol*      getValue;
  FnSymbol*      init;
  FnSymbol*      incr;

  Type*          yieldedType;
  RetTag         iteratorRetTag;
};

CallExpr* isSingleLoopIterator(FnSymbol* fn, Vec<BaseAST*>& asts);
void lowerIterator(FnSymbol* fn);

#endif
