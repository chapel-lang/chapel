/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _LOOP_DETAILS_H_
#define _LOOP_DETAILS_H_

#include <cstddef>
#include <vector>

class Expr;
class FnSymbol;
class ForallStmt;
class ForLoop;
class Symbol;
class Type;

struct IteratorDetails {
  Expr*     iterable;
  int       iterableTupleElement; // if != 0, iterable(idx) is the iterable
  Symbol*   index;
  int       indexTupleElement; // if != 0, index(idx) is the index
  Type*     iteratorClass;
  FnSymbol* iterator;

  IteratorDetails()
    : iterable(NULL), iterableTupleElement(0),
      index(NULL), indexTupleElement(0),
      iteratorClass(NULL), iterator(NULL)
  {
  }
};

/* Gather important information about a loop.

leaderDetails is only relevant for leader/follower iterators
              and in that case refers to details of the leader loop.
followerForLoop is the follower ForLoop in leader/follower iteration.

When considering zippered iteration, detailsVector contains details of
the individually zippered iterations.

When considering non-zippered iteration, detailsVector contains exactly
one element. It stores information
about the loop. In leader/follower loops, it contains information about
the follower loop.

Always uses the non-fast-follower version of the follower loop.
 */
void gatherLoopDetails(ForLoop*  forLoop,
                       bool&     isForall,
                       IteratorDetails& leaderDetails,
                       ForLoop*& followerForLoop,
                       std::vector<IteratorDetails>& detailsVector);

void gatherLoopDetails(ForallStmt*  fs,
                       bool&     isForall,
                       IteratorDetails& leaderDetails,
                       ForLoop*& followerForLoop,
                       std::vector<IteratorDetails>& detailsVector);

bool isChplIterOrLoopIterator(Symbol* sym, ForLoop*& loop);

#endif
