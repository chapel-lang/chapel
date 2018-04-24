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
    proc contains(lab) {
      halt("generic Index contains no elements");
      return false;
    }

    proc writeThis(f, s: TypedSeries(?) = nil) {
      halt("cannot writeThis on generic Index");
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

    proc contains(lab: idxType) {
      return labels.member(lab);
    }

    proc writeThis(f, s: TypedSeries(?) = nil) {
      for (i, d) in zip(this, s) {
        f <~> i;
        f <~> "\t";
        f <~> d;
        f <~> "\n";
      }
    }

    // TODO: label mutation (insert, drop)
    // TODO: ordinal mutation (delete)
    // TODO: label concatenation (append)
    // TODO: ordinal de-duplication (drop_duplicates)
  }

  class Series {
  }

  class TypedSeries : Series {
    type eltType;

    // TODO: reconsider nil idx = rectangular 1D
    // TODO: ords dmap Block
    var idx: Index;
    var ords: domain(1);
    var data: [ords] eltType;

    proc init(data: [] ?T) {
      super.init();
      eltType = T;

      this.ords = 1..data.size;
      this.data = data;
    }

    // TODO: verify that data is rectangular domain(1)
    proc init(data: [] ?T, rev_idx: []) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 1..data.size;
      this.data = data;
    }

    iter these() {
      for d in data do
        yield d;
    }

    iter items() {
      for tup in zip(ords, data) do
        yield tup;
    }

    iter items(type idxType) {
      if idx {
        for tup in zip(idx:TypedIndex(idxType), data) do
          yield tup;
      }
    }

    proc at(ord: int) {
      return data[ord];
    }

    proc this(lab: ?idxType) {
      return data[(idx:TypedIndex(idxType))[lab]];
    }

    // TODO: "in" operator for idx.contains(lab)

    proc join(other: TypedSeries(eltType), joiner: SeriesJoiner(eltType)): TypedSeries(eltType) {
      var join_ords = if this.ords.size > other.ords.size
                      then 1..this.ords.size
                      else 1..other.ords.size;

      var join_data: [join_ords] eltType;
      for i in join_ords {
        var inThis = i <= this.ords.size;
        var inOther = i <= other.ords.size;
        if inThis && inOther {
          join_data[i] = joiner.f(this.at(i), other.at(i));
        } else if inThis {
          join_data[i] = joiner.f_lhs(this.at(i));
        } else if inOther {
          join_data[i] = joiner.f_rhs(other.at(i));
        }
      }
      delete joiner;
      return new TypedSeries(join_data);
    }

    proc join(other: TypedSeries(eltType), type idxType, joiner: SeriesJoiner(eltType)): TypedSeries(eltType) {
      // TODO: check if the index types are the same, throw if not
      var join_ords = 1..(this.ords.size + other.ords.size);
      var join_rev_idx: [join_ords] idxType;
      var join_data: [join_ords] eltType;

      var curr_ord = 0;
      for (i, d) in this.items(idxType) {
        curr_ord += 1;
        join_rev_idx[curr_ord] = i;

        if other.idx.contains(i) {
          join_data[curr_ord] = joiner.f(d, other[i]);
        } else {
          join_data[curr_ord] = joiner.f_lhs(d);
        }
      }

      for (other_i, other_d) in other.items(idxType) {
        if !this.idx.contains(other_i) {
          curr_ord += 1;
          join_rev_idx[curr_ord] = other_i;
          join_data[curr_ord] = joiner.f_rhs(other_d);
        }
      }

      delete joiner;
      return new TypedSeries(join_data[1..curr_ord], join_rev_idx[1..curr_ord]);
    }

    proc add(other: TypedSeries(eltType)): TypedSeries(eltType) {
      return join(other, new SeriesAdd(eltType));
    }

    proc add(other: TypedSeries(eltType), type idxType): TypedSeries(eltType) {
      return join(other, idxType, new SeriesAdd(eltType));
    }

    proc subtr(other: TypedSeries(eltType)): TypedSeries(eltType)
                                             where isNumericType(eltType) {
      return join(other, new SeriesSubtr(eltType));
    }

    proc subtr(other: TypedSeries(eltType), type idxType): TypedSeries(eltType)
                                                           where isNumericType(eltType) {
      return join(other, idxType, new SeriesSubtr(eltType));
    }

    proc mult(other: TypedSeries(eltType)): TypedSeries(eltType)
                                            where isNumericType(eltType) {
      return join(other, new SeriesMult(eltType));
    }

    proc mult(other: TypedSeries(eltType), type idxType): TypedSeries(eltType)
                                                          where isNumericType(eltType) {
      return join(other, idxType, new SeriesMult(eltType));
    }

    proc writeThis(f) {
      if idx {
        idx.writeThis(f, this);
      } else {
        for (i, d) in this.items() do
          f <~> i + "\t" + d + "\n";
      }
      f <~> "dtype: " + eltType:string;
    }
  }

  class SeriesJoiner {
    type eltType;

    proc f(lhs: eltType, rhs: eltType): eltType {
      var empty: eltType;
      return empty;
    }

    proc f_lhs(lhs: eltType): eltType {
      return lhs;
    }

    proc f_rhs(rhs: eltType): eltType {
      return rhs;
    }
  }

  class SeriesAdd : SeriesJoiner {
    proc f(lhs: eltType, rhs: eltType): eltType {
      return lhs + rhs;
    }
  }

  class SeriesSubtr : SeriesJoiner {
    proc f(lhs: eltType, rhs: eltType): eltType {
      return lhs - rhs;
    }

    proc f_rhs(rhs: eltType): eltType {
      return -rhs;
    }
  }

  class SeriesMult : SeriesJoiner {
    proc f(lhs: eltType, rhs: eltType): eltType {
      return lhs * rhs;
    }

    proc f_lhs(lhs: eltType): eltType {
      return 0;
    }

    proc f_rhs(rhs: eltType): eltType {
      return 0;
    }
  }


 // TODO: overload + on a class for dynamic dispatch?
  /*
  proc +(ref lhs: TypedSeries(?T), ref rhs: TypedSeries(T)) {
    return lhs.add(rhs);
  }
   */
}
