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

module DataFrames {
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
    proc nrows(): int {
      halt("generic Index cannot be countd");
      return 0;
    }

    pragma "no doc"
    proc writeThis(f, s: TypedSeries(?) = nil) {
      halt("cannot writeThis on generic Index");
    }

    pragma "no doc"
    proc writeThis(f, d: DataFrame = nil) {
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
    proc uni(lhs: TypedSeries(?lhsType), rhs: TypedSeries(?rhsType),
             unifier: SeriesUnifier(lhsType)): TypedSeries(lhsType)
             where lhsType == rhsType {
      var uni_ords = 1..(lhs.ords.size + rhs.ords.size);
      var uni_rev_idx: [uni_ords] idxType;
      var uni_data: [uni_ords] lhsType;
      var uni_valid_bits: [uni_ords] bool;

      var curr_ord = 0;
      for (lhs_v, (lhs_i, lhs_d)) in lhs._items(idxType) {
        curr_ord += 1;
        uni_rev_idx[curr_ord] = lhs_i;

        if rhs.idx.contains(lhs_i) {
          uni_data[curr_ord] = unifier.f(lhs_d, rhs[lhs_i]);
          uni_valid_bits[curr_ord] = lhs_v && rhs.valid(lhs_i);
        } else {
          uni_data[curr_ord] = unifier.f_lhs(lhs_d);
          uni_valid_bits[curr_ord] = lhs_v;
        }
      }

      for (rhs_v, (rhs_i, rhs_d)) in rhs._items(idxType) {
        if !lhs.idx.contains(rhs_i) {
          curr_ord += 1;
          uni_rev_idx[curr_ord] = rhs_i;
          uni_data[curr_ord] = unifier.f_rhs(rhs_d);
          uni_valid_bits[curr_ord] = rhs_v;
        }
      }

      delete unifier;
      return new TypedSeries(uni_data[1..curr_ord],
                             new TypedIndex(uni_rev_idx[1..curr_ord]),
                             uni_valid_bits[1..curr_ord]);
    }

    proc map(s: TypedSeries(?T), mapper: SeriesMapper(T, ?R)): TypedSeries(R) {
      var mapped: [ords] R;
      for (i, d) in s.items(idxType) do
        mapped[this[i]] = mapper.f(d);

      delete mapper;
      return new TypedSeries(mapped, this, s.valid_bits);
    }

    proc filter(s: TypedSeries(?T), filterSeries: TypedSeries(bool)):
                TypedSeries(T) {
      var filter_rev_idx: [ords] idxType;
      var filter_data: [ords] T;
      var filter_valid_bits: [ords] bool;

      var curr_ord = 0;
      for (i, b) in filterSeries.items(idxType) {
        if b && this.contains(i) {
          curr_ord += 1;
          filter_rev_idx[curr_ord] = i;
          filter_data[curr_ord] = s[i];
          filter_valid_bits[curr_ord] = s.valid(i);
        }
      }

      return new TypedSeries(filter_data[1..curr_ord],
                             new TypedIndex(filter_rev_idx[1..curr_ord]),
                             filter_valid_bits[1..curr_ord]);
    }

    proc nrows() {
      return ords.size;
    }

    pragma "no doc"
    proc writeIdxWidth() {
      var idxWidth = 0;
      for idx in this {
        // TODO: clean up to simple cast after bugfix
        var idxStr = new string(idx: string);
        if idxStr.length > idxWidth then
          idxWidth = idxStr.length;
      }
      return idxWidth;
    }

    proc writeThis(f, s: TypedSeries(?) = nil) {
      var idxWidth = writeIdxWidth() + 4;
      for (idx, (v, d)) in zip(this, s._these()) {
        // TODO: clean up to simple cast after bugfix
        var idxStr = new string(idx: string);
        f <~> idx;
        for space in 1..idxWidth-idxStr.length do
          f <~> " ";

        if v then
          f <~> d;
        else
          f <~> "None";
        f <~> "\n";
      }
    }

    proc writeThis(f, d: DataFrame = nil) {
      var idxWidth = writeIdxWidth() + 1;
      for space in 1..idxWidth do
        f <~> " ";
      for lab in d.labels {
        f <~> lab + "   ";
      }

      for idx in this {
        f <~> "\n";
        // TODO: clean up to simple cast after bugfix
        var idxStr = new string(idx: string);
        f <~> idxStr;
        for space in 1..idxWidth-idxStr.length do
          f <~> " ";

        for (ser, lab) in zip(d, d.labels) {
          ser.writeElem(f, idx, lab.length);
          f <~> "   ";
        }
      }
    }

    // TODO: label mutation (insert, drop)
    // TODO: ordinal mutation (delete)
    // TODO: label concatenation (append)
    // TODO: ordinal de-duplication (drop_duplicates)
  }

  class Series {
    pragma "no doc"
    proc copy() {
      return this;
    }

    pragma "no doc"
    proc reindex(idx) {
      halt("generic Series cannot be reindexed");
    }

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

    pragma "no doc"
    proc nrows(): int {
      halt("generic Series cannot be counted");
      return 0;
    }

    pragma "no doc"
    proc writeElem(f, i, len: int) {
      halt("generic Series cannot be indexed");
    }

    pragma "no doc"
    proc writeElemNoIndex(f, i: int, len: int) {
      halt("generic Series cannot be accessed");
    }
  }

  class TypedSeries : Series {
    type eltType;

    // TODO: ords dmap Block
    var idx: Index;
    var ords: domain(1);
    var data: [ords] eltType;
    var valid_bits: [ords] bool;

    /*
     * Initializers
     */
    // TODO: verify that data is rectangular domain(1)

    proc init(data: [] ?T) {
      super.init();
      eltType = T;

      this.ords = 1..data.size;
      this.data = data;
      this.valid_bits = true;
    }

    proc init(data: [] ?T, valid_bits: [] bool) {
      super.init();
      eltType = T;

      this.ords = 1..data.size;
      this.data = data;
      this.valid_bits = valid_bits;
    }

    proc init(data: [] ?T, idx: Index) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 1..data.size;
      this.data = data;
      this.valid_bits = true;
    }

    proc init(data: [] ?T, idx: Index, valid_bits: [] bool) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 1..data.size;
      this.data = data;
      this.valid_bits = valid_bits;
    }

    proc copy() {
      return new TypedSeries(this.data, this.idx, this.valid_bits);
    }

    /*
     * Iterators
     */

    // only yields valid items
    iter these() {
      for (v, d) in zip(valid_bits, data) do
        if v then yield d;
    }

    iter items() {
      for (v, o, d) in zip(valid_bits, ords, data) do
        if v then yield (o, d);
    }

    iter items(type idxType) {
      if idx {
        for (v, i, d) in zip(valid_bits, idx:TypedIndex(idxType), data) do
          if v then yield (i, d);
      }
    }

    // also yields invalid items
    iter fast() {
      for d in data do
        yield d;
    }

    iter items_fast() {
      for t in zip(ords, data) do
        yield t;
    }

    iter items_fast(type idxType) {
      if idx {
        for t in zip(idx:TypedIndex(idxType), data) do
          yield t;
      }
    }

    // yields tuples where the first value is the valid bit
    pragma "no doc" iter _these() {
      for t in zip(valid_bits, data) do
        yield t;
    }

    pragma "no doc"
    iter _items() {
      for t in zip(valid_bits, this.items_fast()) do
        yield t;
    }

    pragma "no doc"
    iter _items(type idxType) {
      for t in zip(valid_bits, this.items_fast(idxType)) do
        yield t;
    }

    /*
     * Accessors
     */
    // TODO: throw if out of bounds
    // TODO: throw if None

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

      // TODO: needs Series with Index(int) to remove items not in range
      var filter_data: [ords] eltType;
      for (i, b) in castFilter.items() {
        if b && i <= data.size then
          filter_data[i] = this.at(i);
      }
      return new TypedSeries(filter_data, this.valid_bits);
    }

    proc at(ord: int) {
      return data[ord];
    }

    proc valid(lab: ?idxType) {
      if idx then
        return valid_bits[(idx:TypedIndex(idxType))[lab]];

      return false;
    }

    proc valid_at(ord: int) {
      return valid_bits[ord];
    }

    proc reindex(idx: Index) {
      this.idx = idx;
    }

    /*
     * Functional Constructs
     */

    proc uni(lhs: TypedSeries(eltType), unifier: SeriesUnifier(eltType)): TypedSeries(eltType) {
      if lhs.idx then
        return lhs.idx.uni(lhs, this, unifier):TypedSeries(eltType);

      var uni_ords = if lhs.ords.size > this.ords.size
                     then 1..lhs.ords.size
                     else 1..this.ords.size;
      var uni_data: [uni_ords] eltType;
      var uni_valid_bits: [uni_ords] bool;

      for i in uni_ords {
        var inLhs = i <= lhs.ords.size;
        var inThis = i <= this.ords.size;
        if inLhs && inThis {
          uni_data[i] = unifier.f(lhs.at(i), this.at(i));
          uni_valid_bits[i] = lhs.valid_at(i) && this.valid_at(i);
        } else if inLhs {
          uni_data[i] = unifier.f_lhs(lhs.at(i));
          uni_valid_bits[i] = lhs.valid_at(i);
        } else if inThis {
          uni_data[i] = unifier.f_rhs(this.at(i));
          uni_valid_bits[i] = this.valid_at(i);
        }
      }

      delete unifier;
      return new TypedSeries(uni_data, uni_valid_bits);
    }

    proc map(mapper: SeriesMapper): Series {
      if idx then
        return idx.map(this, mapper);

      var mapped = [d in data] mapper.f(d);
      delete mapper;
      return new TypedSeries(mapped, this.valid_bits);
    }

    /*
     * Operators
     */
    // TODO: "in" operator for idx.contains(lab)

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

    /*
     * Reductions
     */

    proc sum(): eltType {
      return + reduce this.these();
    }

    proc min(): eltType {
      return min reduce this.these();
    }

    proc max(): eltType {
      return max reduce this.these();
    }

    proc and() where eltType == bool {
      return && reduce this.these();
    }

    proc or() where eltType == bool {
      return || reduce this.these();
    }

    proc nrows(): int {
      if idx then
        return idx.nrows();
      else
        return ords.size;
    }

    proc writeThis(f) {
      if idx {
        idx.writeThis(f, this);
      } else {
        for (v, (i, d)) in this._items() {
          f <~> i + "    ";
          if v then
            f <~> d;
          else
            f <~> "None";
          f <~> "\n";
        }
      }
      f <~> "dtype: " + eltType:string;
    }

    pragma "no doc"
    proc writeElem(f, i, len: int) {
      // TODO: clean up to simple cast after bugfix
      var output = if this.valid(i)
                   then new string(this[i]: string)
                   else "None";

      for space in 1..len-output.length do
        f <~> " ";
      f <~> output;
    }

    pragma "no doc"
    proc writeElemNoIndex(f, i: int, len: int) {
      // TODO: clean up to simple cast after bugfix
      var output = if this.valid_at(i)
                   then new string(this.at(i): string)
                   else "None";

      for space in 1..len-output.length do
        f <~> " ";
      f <~> output;
    }
  }

  class DataFrame {
    var labels: domain(string);
    var columns: [labels] Series;
    var idx: Index;

    // TODO: init with labels arg

    proc init() {
      this.complete();
    }

    proc init(columns: [?D] Series) {
      this.labels = D;
      this.idx = nil;
      this.complete();

      for (lab, s) in zip(labels, columns) do
        this.columns[lab] = s.copy();
    }

    proc init(idx: Index) {
      this.idx = idx;
      this.complete();
    }

    proc init(columns: [?D], idx: Index) {
      this.labels = D;
      this.idx = idx;
      this.complete();

      for (lab, s) in zip(labels, columns) do
        this.insert(lab, s);
    }

    proc copy() {
      return new DataFrame(this.columns, this.idx);
    }

    iter these() {
      for s in columns do
        yield s;
    }

    proc this(lab: string) {
      return columns[lab];
    }

    // TODO: have this function take an owned Series
    proc insert(lab: string, s: Series) {
      var sCopy = s.copy();
      sCopy.reindex(idx);
      columns[lab] = sCopy;
    }

    proc reindex(idx: Index) {
      this.idx = idx;
      for s in columns do
        s.reindex(idx);
    }

    proc set_index(type eltType, lab: string) {
      var df = this.copy();
      df.labels.remove(lab);

      var ser = this[lab];
      var idx = new TypedIndex((ser:TypedSeries(eltType)).data);
      df.reindex(idx);

      return df;
    }

    proc nrows() {
      var nMax = 0;
      for s in this {
        var n = s.nrows();
        if n > nMax then nMax = n;
      }
      return nMax;
    }

    proc writeThis(f) {
      if idx {
        idx.writeThis(f, this);
      } else {
        var n = nrows();
        var nStr = new string(n: string);
        var idxWidth = nStr.length + 1;

        for space in 1..idxWidth do
          f <~> " ";
        for lab in labels {
          f <~> lab + "   ";
        }

        for i in 1..n {
          f <~> "\n";
          var iStr = new string(i: string);
          f <~> iStr;
          for space in 1..idxWidth-iStr.length do
            f <~> " ";

          for (ser, lab) in zip(this, labels) {
            ser.writeElemNoIndex(f, i, lab.length);
            f <~> "   ";
          }
        }
      }
    }
  }

  /*
   * NOTE: PPS
   * THERE BE FUNCTION OBJECT DRAGONS HERE.
   * FIRST CLASS FUNCTION SERENITY NOW.
   */

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

  // TODO: isNumericType prevents instantiation with bools
  // would prefer "is summable" type here
  class SeriesAdd : SeriesUnifier {
    proc f(lhs: eltType, rhs: eltType): eltType where isNumericType(eltType) ||
                                                      isStringType(eltType) {
      return lhs + rhs;
    }
  }

  class SeriesSubtr : SeriesUnifier {
    proc f(lhs: eltType, rhs: eltType): eltType where isNumericType(eltType) {
      return lhs - rhs;
    }

    proc f_rhs(rhs: eltType): eltType where isNumericType(eltType) {
      return -rhs;
    }
  }

  class SeriesMult : SeriesUnifier {
    proc f(lhs: eltType, rhs: eltType): eltType where isNumericType(eltType) {
      return lhs * rhs;
    }

    proc f_lhs(lhs: eltType): eltType where isNumericType(eltType) {
      return 0;
    }

    proc f_rhs(rhs: eltType): eltType where isNumericType(eltType) {
      return 0;
    }
  }

  pragma "use default init"
  class SeriesMapper {
    type eltType;
    type retType;

    proc f(d: eltType): retType {
      return d:retType;
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
