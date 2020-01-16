/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  use Sort;
  private use IO;

  class Index {
    pragma "no doc"
    proc contains(lab) {
      halt("generic Index contains no elements");
      return false;
    }

    pragma "no doc"
    proc uni(lhs: borrowed TypedSeries, rhs: borrowed TypedSeries, unifier:
        borrowed SeriesUnifier): owned Series {
      halt("generic Index cannot be unioned");
      return new owned Series();
    }

    pragma "no doc"
    proc map(s: borrowed TypedSeries, mapper: borrowed SeriesMapper): owned Series {
      halt("generic Index cannot be mapped");
      return new owned Series();
    }

    pragma "no doc"
    proc filter(s: borrowed TypedSeries, filterSeries: borrowed TypedSeries): owned Series {
      halt("generic Index cannot be filtered");
      return new owned Series();
    }

    pragma "no doc"
    proc nrows(): int {
      halt("generic Index cannot be countd");
      return 0;
    }

    pragma "no doc"
    proc writeThis(f, s: borrowed TypedSeries(?)) throws {
      halt("cannot writeThis on generic Index");
    }

    pragma "no doc"
    proc writeThis(f, d: borrowed DataFrame) throws {
      halt("cannot writeThis on generic Index");
    }

    pragma "no doc"
    proc writeThis(f) throws {
      halt("cannot writeThis on generic Index");
    }

    iter these(type idxType) {
      var _typed = this: TypedIndex(idxType)?;
      if _typed == nil then halt("Unable to cast Index to type " + idxType:string);

      for i in _typed! do yield i;
    }

    iter items(type idxType) {
      var _typed = this: TypedIndex(idxType)?;
      if _typed == nil then halt("Unable to cast Index to type " + idxType:string);

      for i in _typed!.items() do yield i;
    }

    proc this(lab : ?idxType) ref : int {
      var _typed = this: TypedIndex(idxType)?;
      if _typed == nil then halt("Unable to cast Index to type " + idxType:string);

      return (_typed!)[lab];
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

    override proc this(lab: idxType) ref : int {
      return labelToOrd[lab];
    }

    override
    proc contains(lab: idxType) {
      return labels.contains(lab);
    }

    // TODO: enforce same index type with another dispatch
    // TODO: sort Index
    override
    proc uni(lhs: borrowed TypedSeries(?lhsType), rhs: borrowed TypedSeries(?rhsType),
             unifier: borrowed SeriesUnifier(lhsType)): owned Series
             where lhsType == rhsType {
      var uni_ords = 1..(lhs.ords.size + rhs.ords.size);
      var uni_rev_idx: [uni_ords] idxType;
      var uni_data: [uni_ords] lhsType;
      var uni_valid_bits: [uni_ords] bool;

      var curr_ord = 0;
      for (lhs_v, (lhs_i, lhs_d)) in lhs._items(idxType) {
        curr_ord += 1;
        uni_rev_idx[curr_ord] = lhs_i;

        if rhs.idx!.contains(lhs_i) {
          uni_data[curr_ord] = unifier.f(lhs_d, rhs[lhs_i]);
          uni_valid_bits[curr_ord] = lhs_v && rhs.valid(lhs_i);
        } else {
          uni_data[curr_ord] = unifier.f_lhs(lhs_d);
          uni_valid_bits[curr_ord] = lhs_v;
        }
      }

      for (rhs_v, (rhs_i, rhs_d)) in rhs._items(idxType) {
        if !lhs.idx!.contains(rhs_i) {
          curr_ord += 1;
          uni_rev_idx[curr_ord] = rhs_i;
          uni_data[curr_ord] = unifier.f_rhs(rhs_d);
          uni_valid_bits[curr_ord] = rhs_v;
        }
      }

      return new owned TypedSeries(uni_data[1..curr_ord],
                             new shared TypedIndex(uni_rev_idx[1..curr_ord]),
                             uni_valid_bits[1..curr_ord]);
    }

    override
    proc map(s: borrowed TypedSeries(?T), mapper: borrowed SeriesMapper(T, ?R)): owned Series {
      var mapped: [ords] R;
      for (i, d) in s.items(idxType) do
        mapped[this[i]] = mapper.f(d);

      // TODO: We would prefer to use this commented-out pattern once we can
      // specify the this-intent as shared
      //return new owned TypedSeries(mapped, this:shared TypedIndex(idxType), s.valid_bits);

      return new owned TypedSeries(mapped, s.valid_bits);
    }

    override
    proc filter(s: borrowed TypedSeries(?T), filterSeries: borrowed TypedSeries(bool)): owned Series {
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

      return new owned TypedSeries(filter_data[1..curr_ord],
                             new shared TypedIndex(filter_rev_idx[1..curr_ord]),
                             filter_valid_bits[1..curr_ord]);
    }

    override
    proc nrows() {
      return ords.size;
    }

    pragma "no doc"
    proc writeIdxWidth() {
      var idxWidth = 0;
      for idx in this {
        // TODO: clean up to simple cast after bugfix
        var idxStr = createStringWithNewBuffer(idx: string);
        if idxStr.length > idxWidth then
          idxWidth = idxStr.length;
      }
      return idxWidth;
    }

    override
    proc writeThis(f, s: borrowed TypedSeries) throws {
      var idxWidth = writeIdxWidth() + 4;
      for (idx, (v, d)) in zip(this, s!._these()) {
        // TODO: clean up to simple cast after bugfix
        var idxStr = createStringWithNewBuffer(idx: string);
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

    override
    proc writeThis(f, d: borrowed DataFrame) throws {
      var idxWidth = writeIdxWidth() + 1;
      for space in 1..idxWidth do
        f <~> " ";
      for lab in d.labels {
        f <~> lab + "   ";
      }

      for idx in this {
        f <~> "\n";
        // TODO: clean up to simple cast after bugfix
        var idxStr = createStringWithNewBuffer(idx: string);
        f <~> idxStr;
        for space in 1..idxWidth-idxStr.length do
          f <~> " ";

        for (ser, lab) in zip(d, d.labels) {
          ser.writeElem(f, idx, lab.length);
          f <~> "   ";
        }
      }
    }

    override
    proc writeThis(f) throws {
      var idxWidth = writeIdxWidth() + 1;
      for space in 1..idxWidth do
        f <~> " ";

      for idx in this {
        f <~> "\n";
        // TODO: clean up to simple cast after bugfix
        var idxStr = createStringWithNewBuffer(idx: string);
        f <~> idxStr;
        for space in 1..idxWidth-idxStr.length do
          f <~> " ";
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
      return new owned Series();
    }

    pragma "no doc"
    proc reindex(in idx : shared Index?) {
      halt("generic Series cannot be reindexed");
    }

    pragma "no doc"
    proc reindex(type eltType, in idx : shared Index?) {
      var _typed = this: TypedSeries(eltType)?;
      if _typed == nil then halt("Unable to cast generic index with type ", eltType:string);

      _typed!.reindex(idx);
    }

    pragma "no doc"
    proc uni(lhs: borrowed TypedSeries, unifier: borrowed SeriesUnifier) {
      halt("generic Series cannot be unioned");
      return new owned Series();
    }

    pragma "no doc"
    proc map(mapper: borrowed SeriesMapper) {
      halt("generic Series cannot be unioned");
      return new owned Series();
    }

    pragma "no doc"
    proc add(rhs) {
      halt("generic Series cannot be added");
      return new owned Series();
    }

    pragma "no doc"
    proc add_scalar(n) {
      halt("generic Series cannot be added");
      return new owned Series();
    }

    pragma "no doc"
    proc subtr(rhs) {
      halt("generic Series cannot be subtracted");
      return new owned Series();
    }

    pragma "no doc"
    proc subtr_scalar(n) {
      halt("generic Series cannot be subtracted");
      return new owned Series();
    }

    pragma "no doc"
    proc mult(rhs) {
      halt("generic Series cannot be multiplied");
      return new owned Series();
    }

    pragma "no doc"
    proc mult_scalar(n) {
      halt("generic Series cannot be multiplied");
      return new owned Series();
    }

    pragma "no doc"
    proc lt_scalar(n) {
      halt("generic Series cannot be compared");
      return new owned Series();
    }

    pragma "no doc"
    proc gt_scalar(n) {
      halt("generic Series cannot be compared");
      return new owned Series();
    }

    pragma "no doc"
    proc eq_scalar(n) {
      halt("generic Series cannot be compared");
      return new owned Series();
    }

    pragma "no doc"
    proc lteq_scalar(n) {
      halt("generic Series cannot be compared");
      return new owned Series();
    }

    pragma "no doc"
    proc gteq_scalar(n) {
      halt("generic Series cannot be compared");
      return new owned Series();
    }

    pragma "no doc"
    proc nrows(): int {
      halt("generic Series cannot be counted");
      return 0;
    }

    pragma "no doc"
    proc writeElem(f, i, len: int) throws {
      halt("generic Series cannot be indexed");
    }

    pragma "no doc"
    proc writeElemNoIndex(f, i: int, len: int) throws {
      halt("generic Series cannot be accessed");
    }
  }

  class TypedSeries : Series {
    type eltType;

    // TODO: ords dmap Block
    var idx: shared Index?;
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

    proc init(data: [] ?T, in idx: shared Index?) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 1..data.size;
      this.data = data;
      this.valid_bits = true;
    }

    proc init(data: [] ?T, in idx: shared Index?, valid_bits: [] bool) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 1..data.size;
      this.data = data;
      this.valid_bits = valid_bits;
    }

    override
    proc copy() : owned Series {
      return new owned TypedSeries(this.data, this.idx, this.valid_bits);
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
        for (v, i, d) in zip(valid_bits, idx!.these(idxType), data) do
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
        for t in zip(idx!.these(idxType), data) do
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
        return data[idx![lab]];

      var default: eltType;
      return default;
    }

    // TODO: filterSeries needs to be Owned
    proc this(filterSeries: ?T) : owned Series where isSubtype(T, Series) {
      var castFilter = filterSeries: borrowed TypedSeries(bool)?;
      if idx then
        return idx!.filter(this, castFilter!);

      // TODO: needs Series with Index(int) to remove items not in range
      var filter_data: [ords] eltType;
      for (i, b) in castFilter!.items() {
        if b && i <= data.size then
          filter_data[i] = this.at(i);
      }
      return new owned TypedSeries(filter_data, this.valid_bits);
    }

    proc at(ord: int) {
      return data[ord];
    }

    proc valid(lab: ?idxType) {
      if idx then
        return valid_bits[idx![lab]];

      return false;
    }

    proc valid_at(ord: int) {
      return valid_bits[ord];
    }

    override proc reindex(in idx: shared Index?) {
      this.idx = idx;
    }

    /*
     * Functional Constructs
     */

    override
    proc uni(lhs: borrowed TypedSeries(eltType), unifier: borrowed SeriesUnifier(eltType)): owned Series {
      if lhs.idx then
        return lhs.idx!.uni(lhs, this, unifier):owned Series;

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

      return new owned TypedSeries(uni_data, uni_valid_bits);
    }

    override
    proc map(mapper: borrowed SeriesMapper): owned Series {
      if idx {
        // Workaround for lack of shared this-intent for Index.map
        var ret = idx!.map(this, mapper);
        ret.reindex(mapper.retType, idx);
        return ret;
      }

      var mapped = [d in data] mapper.f(d);
      return new owned TypedSeries(mapped, this.valid_bits);
    }

    /*
     * Operators
     */
    // TODO: "in" operator for idx.contains(lab)

    override
    proc add(rhs : borrowed Series): owned Series {
      return rhs.uni(this, new borrowed SeriesAdd(eltType));
    }

    override
    proc add_scalar(n): owned Series {
      var with_scalar = data + n;
      return new owned TypedSeries(with_scalar, idx);
    }

    override
    proc subtr(rhs): owned Series {
      return rhs.uni(this, new borrowed SeriesSubtr(eltType));
    }

    override
    proc subtr_scalar(n): owned Series {
      var with_scalar = data - n;
      return new owned TypedSeries(with_scalar, idx);
    }

    override
    proc mult(rhs): owned Series {
      return rhs.uni(this, new borrowed SeriesMult(eltType));
    }

    override
    proc mult_scalar(n): owned Series {
      var with_scalar = data * n;
      return new owned TypedSeries(with_scalar, idx);
    }

    override
    proc lt_scalar(n): owned Series {
      return this.map(new borrowed SeriesLessThan(n));
    }

    override
    proc gt_scalar(n): owned Series {
      return this.map(new borrowed SeriesGreaterThan(n));
    }

    override
    proc eq_scalar(n): owned Series {
      return this.map(new borrowed SeriesEqualTo(n));
    }

    override
    proc lteq_scalar(n): owned Series {
      return this.map(new borrowed SeriesLessThanEqualTo(n));
    }

    override
    proc gteq_scalar(n): owned Series {
      return this.map(new borrowed SeriesGreaterThanEqualTo(n));
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

    override
    proc nrows(): int {
      if idx then
        return idx!.nrows();
      else
        return ords.size;
    }

    override
    proc writeThis(f) throws {
      if idx {
        idx!.writeThis(f, _to_unmanaged(this));
      } else {
        for (v, (i, d)) in this._items() {
          f <~> i:string + "    ";
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
    override
    proc writeElem(f, i, len: int) throws {
      // TODO: clean up to simple cast after bugfix
      var output = if this.valid(i)
                   then createStringWithNewBuffer(this[i]: string)
                   else "None";

      for space in 1..len-output.length do
        f <~> " ";
      f <~> output;
    }

    pragma "no doc"
    override
    proc writeElemNoIndex(f, i: int, len: int) throws {
      // TODO: clean up to simple cast after bugfix
      var output = if this.valid_at(i)
                   then createStringWithNewBuffer(this.at(i): string)
                   else "None";

      for space in 1..len-output.length do
        f <~> " ";
      f <~> output;
    }
  }

  class DataFrame {
    var labels: domain(string);

    // TODO: array of owned Series
    //   Currently run into confusing const errors in DefaultAssociative
    var columns: [labels] unmanaged Series;

    var idx: shared Index?;

    // TODO: init with labels arg

    proc init() {
      this.complete();
    }

    proc init(columns: [?D] ?E) where isSubtype(E, Series) {
      this.labels = D;
      this.idx = nil;
      this.complete();

      for (lab, s) in zip(labels, columns) do
        this.columns[lab] = s.copy().release();
    }

    proc init(columns: [?D], in idx: shared Index) {
      this.labels = D;
      this.idx = idx;
      this.complete();

      for (lab, s) in zip(labels, columns) do
        this.insert(lab, s);
    }

    proc deinit() {
      delete columns;
    }

    iter these() {
      for s in columns do
        yield s;
    }

    proc this(lab: string) {
      return columns[lab];
    }

    proc insert(lab: string, s: borrowed Series) {
      var sCopy = s.copy().release();
      sCopy.reindex(idx);
      labels += lab;
      columns[lab] = sCopy;
    }

    proc reindex(in idx: shared Index?) {
      this.idx = idx;
      for s in columns do
        s.reindex(idx);
    }

    proc nrows() {
      var nMax = 0;
      for s in this {
        var n = s.nrows();
        if n > nMax then nMax = n;
      }
      return nMax;
    }

    override
    proc writeThis(f) throws {
      if idx {
        idx!.writeThis(f, _to_unmanaged(this));
      } else {
        var n = nrows();
        var nStr = createStringWithNewBuffer(n: string);
        var idxWidth = nStr.length + 1;

        for space in 1..idxWidth do
          f <~> " ";
        for lab in labels {
          f <~> lab + "   ";
        }

        for i in 1..n {
          f <~> "\n";
          var iStr = createStringWithNewBuffer(i: string);
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
    override
    proc f(lhs: eltType, rhs: eltType): eltType {
      if (isNumericType(eltType) || isStringType(eltType)) {
        return lhs + rhs;
      } else {
        var empty: eltType;
        return empty;
      }
    }
  }

  class SeriesSubtr : SeriesUnifier {
    override
    proc f(lhs: eltType, rhs: eltType): eltType {
      if isNumericType(eltType) {
        return lhs - rhs;
      } else {
        var empty: eltType;
        return empty;
      }
    }

    override
    proc f_rhs(rhs: eltType): eltType {
      if isNumericType(eltType) {
        return -rhs;
      } else {
        return rhs;
      }
    }
  }

  class SeriesMult : SeriesUnifier {
    override
    proc f(lhs: eltType, rhs: eltType): eltType {
      if isNumericType(eltType) {
        return lhs * rhs;
      } else {
        var empty: eltType;
        return empty;
      }
    }

    override
    proc f_lhs(lhs: eltType): eltType {
      if isNumericType(eltType) {
        return 0;
      } else {
        return lhs;
      }
    }

    override
    proc f_rhs(rhs: eltType): eltType {
      if isNumericType(eltType) {
        return 0;
      } else {
        return rhs;
      }
    }
  }

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

    override
    proc f(d: eltType): retType {
      return d < x;
    }
  }

  class SeriesGreaterThan : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d > x;
    }
  }

  class SeriesEqualTo : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d == x;
    }
  }

  class SeriesLessThanEqualTo : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d <= x;
    }
  }

  class SeriesGreaterThanEqualTo : SeriesCompareScalar {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d >= x;
    }
  }

  /*
   * ARITHMETIC AND INEQUALITY OPERATORS
   */
  proc +(lhs: borrowed Series, rhs: borrowed Series) {
    return lhs.add(rhs);
  }

  proc +(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.add_scalar(n);
  }

  proc +(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.add_scalar(n);
  }

  proc -(lhs: borrowed Series, rhs: borrowed Series) {
    return lhs.subtr(rhs);
  }

  proc -(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.subtr_scalar(n);
  }

  proc *(lhs: borrowed Series, rhs: borrowed Series) {
    return lhs.mult(rhs);
  }

  proc *(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.mult_scalar(n);
  }

  proc *(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.mult_scalar(n);
  }

  proc <(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.lt_scalar(n);
  }

  proc <(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.gt_scalar(n);
  }

  proc >(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.gt_scalar(n);
  }

  proc >(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.lt_scalar(n);
  }

  proc ==(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.eq_scalar(n);
  }

  proc ==(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.eq_scalar(n);
  }

  proc <=(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.lteq_scalar(n);
  }

  proc <=(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.gteq_scalar(n);
  }

  proc >=(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.gteq_scalar(n);
  }

  proc >=(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.lteq_scalar(n);
  }
}
