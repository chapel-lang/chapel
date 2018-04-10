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
    // TODO:
    // how do I get these generic things to work?
    iter these() {
      return;
    }

    iter items() {
      return;
    }

    proc this(lab) ref {
      return nil;
    }
  }

  class TypedIndex : Index {
    type idxType;

    // TODO: implement as binary tree
    // - sorted on ords
    // - allow duplicates
    // - in order traversal
    var ords: domain(1);
    var ordToLabel: [ords] idxType;

    // TODO: implement as hash table
    // - keys: labels
    // - values: ords
    var labels: domain(idxType);
    var labelToOrd: [labels] int;

    // TODO: verify that D is rectangular domain(1)
    proc init(rev_idx: [?D] ?I) {
      super.init();
      idxType = I;
      ords = D;
      ordToLabel = rev_idx;

      this.complete();
      for (ord, lab) in zip(ords, ordToLabel) {
        labels.add(lab);
        labelToOrd[lab] = ord;
      }
    }

    iter these() {
      for lab in ordToLabel do
        yield lab;
    }

    iter items() {
      for tup in zip(ordToLabel, ords) do
        yield tup;
    }

    proc this(lab: idxType) ref {
      return labelToOrd[lab];
    }

    // TODO: label mutation (insert, drop)
    // TODO: ordinal mutation (delete)
    // TODO: label concatenation (append)
    // TODO: ordinal de-duplication (drop_duplicates)
  }

  class Series {
    // TODO: dynamic dispatch on arithmetic
    // i.e. proc +(lhs, rhs), return left.add(right);
  }

  class TypedSeries : Series {
    type eltType;

    // TODO: ords dmap Block
    var idx: Index;
    var ords: domain(1);
    var data: [ords] eltType;

    proc init(data: [] ?T) {
      super.init();
      eltType = T;

      ords = 1..data.size;
      this.data = data;
    }

    // TODO: verify |idx| = data.size
    // TODO: verify that data is rectangular domain(1)
    proc init(rev_idx: [], data: [] ?T) {
      super.init();
      eltType = T;

      this.idx = new TypedIndex(rev_idx);
      ords = 1..data.size;
      this.data = data;
    }

    iter these() {
      for d in data do
        yield d;
    }

    iter items() {
      for tup in zip(idx, data) do
        yield tup;
    }

    proc this(lab) {
      return data[idx[lab]];
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

