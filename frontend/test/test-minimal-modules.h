/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

static std::string DomainModule =
R"""(
module ChapelDomain {
  record _domain {
    var _pid: int;
    var _instance;
    var _unowned:bool;
  }

  proc type _domain.static_type(param rank : int, type idxType=int, param stridable: bool = false) type {
    return __primitive("static domain type", rank, idxType, stridable);
  }

  proc type _domain.static_type(type idxType, param parSafe: bool = true) type {
    return __primitive("static domain type", idxType, parSafe);
  }

  proc computeIndexType(arg: domain) type {
    if arg.isRectangular() {
      if arg.rank == 1 then return arg.idxType;
      else return arg.rank*arg.idxType;
    } else {
      return arg.idxType;
    }
  }

  iter _domain.these() {
    var ret : computeIndexType(this);
    yield ret;
  }

  // Prove that fields and methods on '_domain' work
  proc _domain.pid() {
    return _pid;
  }
}
)""";


static std::string ArrayModule =
R"""(
module ChapelArray {
  public use ChapelDomain;

  record _array {
    var _pid:int;
    var _instance;
    var _unowned:bool;
  }

  proc _array.these() : this.eltType {
    var i : this.eltType;
    yield i;
  }

  proc _array.size : int {
    var i : int;
    return i;
  }

  // TODO: 'this' accessor can't work because accessing 'this.domain' in the
  // formal's type-expr is resolved as an ErroneousType
}
)""";

#endif
