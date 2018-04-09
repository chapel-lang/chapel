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

module Dataframes {
  use Sort;

  class Index {
    type idxType;
    var set: domain(idxType);
    var map: [set] int;

    var rev_set: domain(1);
    var rev_map: [rev_set] idxType;

    // TODO: domain of rev_idx must be domain(1)
    proc init(rev_idx: [?D] ?I) {
      super.init();
      idxType = I;
      this.rev_set = D;
      this.rev_map = rev_idx;
      this.complete();

      for (ord, i) in zip(1.., rev_idx) {
        set.add(i);
        map[i] = ord;
      }
    }

    iter these() {
      for i in rev_map do
        yield i;
    }

    iter items() {
      for tup in zip(rev_map, rev_set) do
        yield tup;
    }

    proc this(i: idxType) {
      return map[i];
    }

    proc set(i: idxType, o: int) throws {
      if o > set.size then
        throw new IllegalArgumentError("o", "ordinal [%d] > Index size [%d]".format(o, set.size));

      if set.member(i) {
        map[i] = o;
        rev_map[o] = i;
      } else {
        this._add(i, o);
      }
    }

    proc add(i: idxType) {
      var new_size: int = set.size + 1;
      this._add(i, new_size);
    }

    pragma "no doc"
    proc _add(i: idxType, o: int) {
      set += i;
      map[i] = o;

      rev_set += o;
      rev_map[o] = i;
    }

    proc add(other: Index(idxType)) {
      for i in other do
        this.add(i);
    }

    // TODO: adding items needs to be well-managed
  }

  class Series {
  }

  class TypedSeries : Series {
    type idxType;
    type eltType;

    var idx: Index(idxType);
    // TODO: ord_set dmap Block
    var ord_set: domain(1);
    var data: [ord_set] eltType;

    proc init(data: [] ?T) {
      super.init();
      idxType = int;
      eltType = T;

      ord_set = 1..data.size;
      this.data = data;
    }

    proc init(idx: [] ?I, data: [] ?T) {
      // TODO: verify |idx| = |D|
      // TODO: verify (i in idx) iff (i in D)
      super.init();
      idxType = I;
      eltType = T;

      this.idx = new Index(idx);
      ord_set = 1..data.size;

      // TODO: throw error/write message if data is not rectangular 1D
      this.data = data;
    }

    iter these() {
      for t in data do
        yield t;
    }

    iter items() {
      for tup in zip(idx, data) do
        yield tup;
    }

    proc this(i: idxType) {
      return data[idx[i]];
    }

    proc at(ord: int) {
      return data[ord];
    }

    proc writeThis(f) {
      if idx {
        f <~> "index: ";
        idx.writeThis(f);
        f <~> " | ";
      }
      f <~> "data: ";
      data.writeThis(f);
    }
  }
}

