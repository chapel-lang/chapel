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
    pragma "no doc"
    proc contains(lab) {
      halt("generic Index contains no elements");
      return false;
    }

    pragma "no doc"
    proc uni(lhs: TypedSeries, rhs: TypedSeries, unifier: SeriesUnifier): Series {
      halt("generic Index cannot be unioned");
      return lhs;
    }

    pragma "no doc"
    proc map(s: TypedSeries, mapper: SeriesMapper): Series {
      halt("generic Index cannot be mapped");
      return s;
    }

    pragma "no doc"
    proc filter(s: TypedSeries, filterSeries: TypedSeries): Series {
      halt("generic Index cannot be filtered");
      return s;
    }

    pragma "no doc"
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

    // TODO: enforce same index type with another dispatch
    // TODO: sort Index
    proc uni(lhs: TypedSeries(?lhsType), rhs: TypedSeries(?rhsType), unifier: SeriesUnifier(lhsType)): TypedSeries(lhsType) where lhsType == rhsType {
      var uni_ords = 1..(lhs.ords.size + rhs.ords.size);
      var uni_rev_idx: [uni_ords] idxType;
      var uni_data: [uni_ords] lhsType;

      var curr_ord = 0;
      for (lhs_i, lhs_d) in lhs.items(idxType) {
        curr_ord += 1;
        uni_rev_idx[curr_ord] = lhs_i;

        if rhs.idx.contains(lhs_i) {
          uni_data[curr_ord] = unifier.f(lhs_d, rhs[lhs_i]);
        } else {
          uni_data[curr_ord] = unifier.f_lhs(lhs_d);
        }
      }

      for (rhs_i, rhs_d) in rhs.items(idxType) {
        if !lhs.idx.contains(rhs_i) {
          curr_ord += 1;
          uni_rev_idx[curr_ord] = rhs_i;
          uni_data[curr_ord] = unifier.f_rhs(rhs_d);
        }
      }

      delete unifier;
      return new TypedSeries(uni_data[1..curr_ord],
                             new TypedIndex(uni_rev_idx[1..curr_ord]));
    }

    proc map(s: TypedSeries(?T), mapper: SeriesMapper(T, ?R)): TypedSeries(R) {
      var mapped: [ords] R;
      for (i, d) in s.items(idxType) do
        mapped[this[i]] = mapper.f(d);

      delete mapper;
      return new TypedSeries(mapped, this);
    }

    proc filter(s: TypedSeries(?T), filterSeries: TypedSeries(bool)): TypedSeries(T) {
      var filter_rev_idx: [ords] idxType;
      var filter_data: [ords] T;

      var curr_ord = 0;
      for (i, b) in filterSeries.items(idxType) {
        if b && this.contains(i) {
          curr_ord += 1;
          filter_rev_idx[curr_ord] = i;
          filter_data[curr_ord] = s[i];
        }
      }

      return new TypedSeries(filter_data[1..curr_ord],
                             new TypedIndex(filter_rev_idx[1..curr_ord]));
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
    pragma "no doc"
    proc uni(lhs: TypedSeries, unifier: SeriesUnifier) {
      halt("generic Series cannot be unioned");
      return this;
    }

    pragma "no doc"
    proc map(mapper: SeriesMapper) {
      halt("generic Series cannot be unioned");
      return this;
    }

    pragma "no doc"
    proc add(rhs) {
      halt("generic Series cannot be added");
      return this;
    }

    pragma "no doc"
    proc add_scalar(n) {
      halt("generic Series cannot be added");
      return this;
    }

    pragma "no doc"
    proc subtr(rhs) {
      halt("generic Series cannot be subtracted");
      return this;
    }

    pragma "no doc"
    proc subtr_scalar(n) {
      halt("generic Series cannot be subtracted");
      return this;
    }

    pragma "no doc"
    proc mult(rhs) {
      halt("generic Series cannot be multiplied");
      return this;
    }

    pragma "no doc"
    proc mult_scalar(n) {
      halt("generic Series cannot be multiplied");
      return this;
    }

    pragma "no doc"
    proc lt_scalar(n) {
      halt("generic Series cannot be compared");
      return this;
    }

    pragma "no doc"
    proc gt_scalar(n) {
      halt("generic Series cannot be compared");
      return this;
    }

    pragma "no doc"
    proc eq_scalar(n) {
      halt("generic Series cannot be compared");
      return this;
    }

    pragma "no doc"
    proc lteq_scalar(n) {
      halt("generic Series cannot be compared");
      return this;
    }

    pragma "no doc"
    proc gteq_scalar(n) {
      halt("generic Series cannot be compared");
      return this;
    }
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
    proc init(data: [] ?T, idx: Index) {
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
      if idx then
        return data[(idx:TypedIndex(idxType))[lab]];

      var default: eltType;
      return default;
    }

    // TODO: filterSeries needs to be Owned
    proc this(filterSeries: ?T) where T: Series {
      var castFilter = filterSeries: TypedSeries(bool);
      if idx then
        return idx.filter(this, castFilter);

      // TODO: needs notion of None to remove items not in range
      var filter_data: [ords] eltType;
      for (i, b) in castFilter.items() {
        if b && i <= data.size then
          filter_data[i] = this.at(i);
      }
      return new TypedSeries(filter_data);
    }

    // TODO: "in" operator for idx.contains(lab)

    proc uni(lhs: TypedSeries(eltType), unifier: SeriesUnifier(eltType)): TypedSeries(eltType) {
      if lhs.idx then
        return lhs.idx.uni(lhs, this, unifier);

      var uni_ords = if lhs.ords.size > this.ords.size
                     then 1..lhs.ords.size
                     else 1..this.ords.size;
      var uni_data: [uni_ords] eltType;

      for i in uni_ords {
        var inLhs = i <= lhs.ords.size;
        var inThis = i <= this.ords.size;
        if inLhs && inThis {
          uni_data[i] = unifier.f(lhs.at(i), this.at(i));
        } else if inLhs {
          uni_data[i] = unifier.f_lhs(lhs.at(i));
        } else if inThis {
          uni_data[i] = unifier.f_rhs(this.at(i));
        }
      }

      delete unifier;
      return new TypedSeries(uni_data);
    }

    proc map(mapper: SeriesMapper): Series {
      if idx then
        return idx.map(this, mapper);

      var mapped = [d in data] mapper.f(d);
      delete mapper;
      return new TypedSeries(mapped);
    }

    proc add(rhs): Series {
      return rhs.uni(this, new SeriesAdd(eltType));
    }

    proc add_scalar(n): Series {
      var with_scalar = data + n;
      return new TypedSeries(with_scalar, idx);
    }

    proc subtr(rhs): Series {
      return rhs.uni(this, new SeriesSubtr(eltType));
    }

    proc subtr_scalar(n): Series {
      var with_scalar = data - n;
      return new TypedSeries(with_scalar, idx);
    }

    proc mult(rhs): Series {
      return rhs.uni(this, new SeriesMult(eltType));
    }

    proc mult_scalar(n): Series {
      var with_scalar = data * n;
      return new TypedSeries(with_scalar, idx);
    }

    proc lt_scalar(n): Series {
      return this.map(new SeriesLessThan(n));
    }

    proc gt_scalar(n): Series {
      return this.map(new SeriesGreaterThan(n));
    }

    proc eq_scalar(n): Series {
      return this.map(new SeriesEqualTo(n));
    }

    proc lteq_scalar(n): Series {
      return this.map(new SeriesLessThanEqualTo(n));
    }

    proc gteq_scalar(n): Series {
      return this.map(new SeriesGreaterThanEqualTo(n));
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

  /*
   * NOTE: PPS
   * THERE BE FUNCTION OBJECT DRAGONS HERE.
   * FIRST CLASS FUNCTION SERENITY NOW.
   */

  // TODO: return tuple versions where first element is "None"
  class SeriesUnifier {
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

  class SeriesAdd : SeriesUnifier {
    proc f(lhs: eltType, rhs: eltType): eltType {
      return lhs + rhs;
    }
  }

  class SeriesSubtr : SeriesUnifier {
    proc f(lhs: eltType, rhs: eltType): eltType {
      return lhs - rhs;
    }

    proc f_rhs(rhs: eltType): eltType {
      return -rhs;
    }
  }

  class SeriesMult : SeriesUnifier {
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

  pragma "use default init"
  class SeriesMapper {
    type eltType;
    type retType;

    proc f(d: eltType): retType {
      return d;
    }
  }

  class SeriesCompareScalar : SeriesMapper {
    var x: eltType;

    proc init(x) {
      super.init(x.type, bool);
      this.x = x;
    }
  }

  class SeriesLessThan : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    proc f(d: eltType): retType {
      return d < x;
    }
  }

  class SeriesGreaterThan : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    proc f(d: eltType): retType {
      return d > x;
    }
  }

  class SeriesEqualTo : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    proc f(d: eltType): retType {
      return d == x;
    }
  }

  class SeriesLessThanEqualTo : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    proc f(d: eltType): retType {
      return d <= x;
    }
  }

  class SeriesGreaterThanEqualTo : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    proc f(d: eltType): retType {
      return d >= x;
    }
  }

  /*
   * ARITHMETIC AND INEQUALITY OPERATORS
   */
  proc +(lhs: Series, rhs: Series) {
    return lhs.add(rhs);
  }

  proc +(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.add_scalar(n);
  }

  proc +(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.add_scalar(n);
  }

  proc -(lhs: Series, rhs: Series) {
    return lhs.subtr(rhs);
  }

  proc -(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.subtr_scalar(n);
  }

  proc *(lhs: Series, rhs: Series) {
    return lhs.mult(rhs);
  }

  proc *(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.mult_scalar(n);
  }

  proc *(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.mult_scalar(n);
  }

  proc <(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.lt_scalar(n);
  }

  proc <(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.gt_scalar(n);
  }

  proc >(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.gt_scalar(n);
  }

  proc >(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.lt_scalar(n);
  }

  proc ==(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.eq_scalar(n);
  }

  proc ==(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.eq_scalar(n);
  }

  proc <=(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.lteq_scalar(n);
  }

  proc <=(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.gteq_scalar(n);
  }

  proc >=(lhs: Series, n: ?N) where isNumericType(N) {
    return lhs.gteq_scalar(n);
  }

  proc >=(n: ?N, rhs: Series) where isNumericType(N) {
    return rhs.lteq_scalar(n);
  }
}
