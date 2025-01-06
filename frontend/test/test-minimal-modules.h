/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef TEST_MINIMAL_MODULES_H
#define TEST_MINIMAL_MODULES_H

#include <string>

static std::string ArrayModule =
R"""(
module ChapelArray {
  public use ChapelDomain;

  record _array {
    var _pid:int;
    var _instance;
    var _unowned:bool;
  }

  iter _array.these() : this.eltType {
    var i : this.eltType;
    yield i;
  }

  proc _array.size : int {
    var i : int;
    return i;
  }

  // TODO: 'this' accessor can't work because accessing 'this.domain' in the
  // formal's type-expr is resolved as an ErroneousType

  proc chpl__buildIndexType(param rank: int, type idxType) type where rank == 1 {
    return idxType;
  }

  proc chpl__buildIndexType(param rank: int, type idxType) type where __primitive(">", rank, 1) {
    return rank*idxType;
  }

  proc chpl__buildIndexType(param rank: int) type do
    return chpl__buildIndexType(rank, int);

  proc chpl__buildIndexType(d: domain) type do
    return chpl__buildIndexType(d.rank, d.idxType);

  param nullPid = -1;

  proc _isPrivatized(value) param do return false;

  record dmap { }
}
)""";

#endif
