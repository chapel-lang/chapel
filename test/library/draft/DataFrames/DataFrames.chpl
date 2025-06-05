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

module DataFrames {
  use Sort;
  private use IO;

  class Index : writeSerializable {
    @chpldoc.nodoc
    proc contains(lab): bool {
      halt("generic Index contains no elements");
    }

    @chpldoc.nodoc
    proc uni(lhs: borrowed TypedSeries(?), rhs: borrowed TypedSeries(?), unifier:
        borrowed SeriesUnifier(?)): owned Series {
      halt("generic Index cannot be unioned");
    }

    @chpldoc.nodoc
    proc map(s: borrowed TypedSeries(?), mapper: borrowed SeriesMapper(?)): owned Series {
      halt("generic Index cannot be mapped");
    }

    @chpldoc.nodoc
    proc filter(s: borrowed TypedSeries(?), filterSeries: borrowed TypedSeries(?)): owned Series {
      halt("generic Index cannot be filtered");
    }

    @chpldoc.nodoc
    proc nrows(): int {
      halt("generic Index cannot be countd");
    }

    @chpldoc.nodoc
    proc writeThis(f, s: borrowed TypedSeries(?)) throws {
      halt("cannot writeThis on generic Index");
    }

    @chpldoc.nodoc
    proc writeThis(f, d: borrowed DataFrame) throws {
      halt("cannot writeThis on generic Index");
    }

    @chpldoc.nodoc
    override proc serialize(writer, ref serializer) throws {
      halt("cannot serialize on generic Index");
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

  class TypedIndex : Index, writeSerializable {
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

      init this;
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
      var uni_ords = 0..#(lhs.ords.size + rhs.ords.size);
      var uni_rev_idx: [uni_ords] idxType;
      var uni_data: [uni_ords] lhsType;
      var uni_valid_bits: [uni_ords] bool;

      var curr_ord = 0;
      for (lhs_v, (lhs_i, lhs_d)) in lhs._items(idxType) {
        uni_rev_idx[curr_ord] = lhs_i;

        if rhs.idx!.contains(lhs_i) {
          uni_data[curr_ord] = unifier.f(lhs_d, rhs[lhs_i]);
          uni_valid_bits[curr_ord] = lhs_v && rhs.valid(lhs_i);
        } else {
          uni_data[curr_ord] = unifier.f_lhs(lhs_d);
          uni_valid_bits[curr_ord] = lhs_v;
        }
        curr_ord += 1;
      }

      for (rhs_v, (rhs_i, rhs_d)) in rhs._items(idxType) {
        if !lhs.idx!.contains(rhs_i) {
          uni_rev_idx[curr_ord] = rhs_i;
          uni_data[curr_ord] = unifier.f_rhs(rhs_d);
          uni_valid_bits[curr_ord] = rhs_v;
          curr_ord += 1;
        }
      }

      return new owned TypedSeries(uni_data[0..#curr_ord],
                             new shared TypedIndex(uni_rev_idx[0..#curr_ord]),
                             uni_valid_bits[0..#curr_ord]);
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
          filter_rev_idx[curr_ord] = i;
          filter_data[curr_ord] = s[i];
          filter_valid_bits[curr_ord] = s.valid(i);
          curr_ord += 1;
        }
      }

      return new owned TypedSeries(filter_data[0..#curr_ord],
                             new shared TypedIndex(filter_rev_idx[0..#curr_ord]),
                             filter_valid_bits[0..#curr_ord]);
    }

    override
    proc nrows() {
      return ords.size;
    }

    @chpldoc.nodoc
    proc writeIdxWidth() {
      var idxWidth = 0;
      for idx in this {
        // TODO: clean up to simple cast after bugfix
        var idxStr = idx: string;
        if idxStr.size > idxWidth then
          idxWidth = idxStr.size;
      }
      return idxWidth;
    }

    override
    proc writeThis(f, s: borrowed TypedSeries(?)) throws {
      var idxWidth = writeIdxWidth() + 4;
      for (idx, (v, d)) in zip(this, s!._these()) {
        // TODO: clean up to simple cast after bugfix
        var idxStr = idx: string;
        f.write(idx);
        for space in 1..idxWidth-idxStr.size do
          f.write(" ");

        if v then
          f.write(d);
        else
          f.write("None");
        f.write("\n");
      }
    }

    override
    proc writeThis(f, d: borrowed DataFrame) throws {
      var idxWidth = writeIdxWidth() + 1;
      for space in 1..idxWidth do
        f.write(" ");
      const labelsSorted = sorted(d.labels);
      for lab in labelsSorted {
        f.write(lab + "   ");
      }

      for idx in this {
        f.write("\n");
        // TODO: clean up to simple cast after bugfix
        var idxStr = idx: string;
        f.write(idxStr);
        for space in 1..idxWidth-idxStr.size do
          f.write(" ");

        for lab in labelsSorted {
          const ser = d[lab];
          ser!.writeElem(f, idx, lab.size);
          f.write("   ");
        }
      }
    }

    override
    proc serialize(writer, ref serializer) throws {
      var idxWidth = writeIdxWidth() + 1;
      for space in 1..idxWidth do
        writer.write(" ");

      for idx in this {
        writer.write("\n");
        // TODO: clean up to simple cast after bugfix
        var idxStr = idx: string;
        writer.write(idxStr);
        for space in 1..idxWidth-idxStr.size do
          writer.write(" ");
      }
    }


    // TODO: label mutation (insert, drop)
    // TODO: ordinal mutation (delete)
    // TODO: label concatenation (append)
    // TODO: ordinal de-duplication (drop_duplicates)
  }

  class Series {
    @chpldoc.nodoc
    proc copy() {
      return new owned Series();
    }

    @chpldoc.nodoc
    proc reindex(in idx : shared Index?) {
      halt("generic Series cannot be reindexed");
    }

    @chpldoc.nodoc
    proc reindex(type eltType, in idx : shared Index?) {
      var _typed = this: TypedSeries(eltType)?;
      if _typed == nil then halt("Unable to cast generic index with type ", eltType:string);

      _typed!.reindex(idx);
    }

    @chpldoc.nodoc
    proc uni(lhs: borrowed TypedSeries(?), unifier: borrowed SeriesUnifier(?)): owned Series {
      halt("generic Series cannot be unioned");
    }

    @chpldoc.nodoc
    proc map(mapper: borrowed SeriesMapper(?)): owned Series {
      halt("generic Series cannot be unioned");
    }

    @chpldoc.nodoc
    proc add(rhs): owned Series {
      halt("generic Series cannot be added");
    }

    @chpldoc.nodoc
    proc add_scalar(n): owned Series {
      halt("generic Series cannot be added");
    }

    @chpldoc.nodoc
    proc subtr(rhs): owned Series {
      halt("generic Series cannot be subtracted");
    }

    @chpldoc.nodoc
    proc subtr_scalar(n): owned Series {
      halt("generic Series cannot be subtracted");
    }

    @chpldoc.nodoc
    proc mult(rhs): owned Series {
      halt("generic Series cannot be multiplied");
    }

    @chpldoc.nodoc
    proc mult_scalar(n): owned Series {
      halt("generic Series cannot be multiplied");
    }

    @chpldoc.nodoc
    proc lt_scalar(n): owned Series {
      halt("generic Series cannot be compared");
    }

    @chpldoc.nodoc
    proc gt_scalar(n): owned Series {
      halt("generic Series cannot be compared");
    }

    @chpldoc.nodoc
    proc eq_scalar(n): owned Series {
      halt("generic Series cannot be compared");
    }

    @chpldoc.nodoc
    proc lteq_scalar(n): owned Series {
      halt("generic Series cannot be compared");
    }

    @chpldoc.nodoc
    proc gteq_scalar(n): owned Series {
      halt("generic Series cannot be compared");
    }

    @chpldoc.nodoc
    proc nrows(): int {
      halt("generic Series cannot be counted");
    }

    @chpldoc.nodoc
    proc writeElem(f, i, len: int) throws {
      halt("generic Series cannot be indexed");
    }

    @chpldoc.nodoc
    proc writeElemNoIndex(f, i: int, len: int) throws {
      halt("generic Series cannot be accessed");
    }

    @chpldoc.nodoc
    operator :(val: Series, type t: string) {
      import IO.FormattedIO;
      return try! "%?".format(val);
    }
  }

  class TypedSeries : Series, writeSerializable {
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

      this.ords = 0..#data.size;
      this.data = data;
      this.valid_bits = true;
    }

    proc init(data: [] ?T, valid_bits: [] bool) {
      super.init();
      eltType = T;

      this.ords = 0..#data.size;
      this.data = data;
      this.valid_bits = valid_bits;
    }

    proc init(data: [] ?T, in idx: shared Index?) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 0..#data.size;
      this.data = data;
      this.valid_bits = true;
    }

    proc init(data: [] ?T, in idx: shared Index?, valid_bits: [] bool) {
      super.init();
      eltType = T;

      this.idx = idx;
      this.ords = 0..#data.size;
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
      for (o,d) in zip(ords, data) do
        yield (o,d);
    }

    iter items_fast(type idxType) {
      if idx {
        for (i,d) in zip(idx!.these(idxType), data) do
          yield (i,d);
      }
    }

    // yields tuples where the first value is the valid bit
    @chpldoc.nodoc iter _these() {
      for (v,d) in zip(valid_bits, data) do
        yield (v,d);
    }

    @chpldoc.nodoc
    iter _items() {
      for (v,d) in zip(valid_bits, this.items_fast()) do
        yield (v,d);
    }

    @chpldoc.nodoc
    iter _items(type idxType) {
      for (v,d) in zip(valid_bits, this.items_fast(idxType)) do
        yield (v,d);
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
        if b && i < data.size then
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
                     then 0..#lhs.ords.size
                     else 0..#this.ords.size;
      var uni_data: [uni_ords] eltType;
      var uni_valid_bits: [uni_ords] bool;

      for i in uni_ords {
        var inLhs = i < lhs.ords.size;
        var inThis = i < this.ords.size;
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
    proc map(mapper: borrowed SeriesMapper(?)): owned Series {
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
      return rhs.uni(this, (new owned SeriesAdd(eltType)).borrow());
    }

    override
    proc add_scalar(n): owned Series {
      var with_scalar = data + n;
      return new owned TypedSeries(with_scalar, idx);
    }

    override
    proc subtr(rhs): owned Series {
      return rhs.uni(this, (new owned SeriesSubtr(eltType)).borrow());
    }

    override
    proc subtr_scalar(n): owned Series {
      var with_scalar = data - n;
      return new owned TypedSeries(with_scalar, idx);
    }

    override
    proc mult(rhs): owned Series {
      return rhs.uni(this, (new owned SeriesMult(eltType)).borrow());
    }

    override
    proc mult_scalar(n): owned Series {
      var with_scalar = data * n;
      return new owned TypedSeries(with_scalar, idx);
    }

    override
    proc lt_scalar(n): owned Series {
      return this.map((new owned SeriesLessThan(n)).borrow());
    }

    override
    proc gt_scalar(n): owned Series {
      return this.map((new owned SeriesGreaterThan(n)).borrow());
    }

    override
    proc eq_scalar(n): owned Series {
      return this.map((new owned SeriesEqualTo(n)).borrow());
    }

    override
    proc lteq_scalar(n): owned Series {
      return this.map((new owned SeriesLessThanEqualTo(n)).borrow());
    }

    override
    proc gteq_scalar(n): owned Series {
      return this.map((new owned SeriesGreaterThanEqualTo(n)).borrow());
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
    proc serialize(writer, ref serializer) throws {
      if idx {
        idx!.writeThis(writer, _to_unmanaged(this));
      } else {
        for (v, (i, d)) in this._items() {
          writer.write(i:string + "    ");
          if v then
            writer.write(d);
          else
            writer.write("None");
          writer.write("\n");
        }
      }
      writer.write("dtype: " + eltType:string);
    }

    @chpldoc.nodoc
    override
    proc writeElem(f, i, len: int) throws {
      // TODO: clean up to simple cast after bugfix
      var output = if this.valid(i)
                   then this[i]: string
                   else "None";

      for space in 1..len-output.size do
        f.write(" ");
      f.write(output);
    }

    @chpldoc.nodoc
    override
    proc writeElemNoIndex(f, i: int, len: int) throws {
      // TODO: clean up to simple cast after bugfix
      var output = if this.valid_at(i)
                   then this.at(i): string
                   else "None";

      for space in 1..len-output.size do
        f.write(" ");
      f.write(output);
    }
  }

  class DataFrame : writeSerializable {
    var labels: domain(string);

    // TODO: array of owned Series
    //   Currently run into confusing const errors in DefaultAssociative
    var columns: [labels] unmanaged Series?;

    var idx: shared Index?;

    // TODO: init with labels arg

    proc init() {
      init this;
    }

    proc init(columns: [?D] ?E) where isSubtype(E, Series) {
      this.labels = D;
      this.idx = nil;
      init this;

      for lab in labels do
        this.columns[lab] = owned.release(columns[lab].copy());
    }

    @chpldoc.nodoc
    proc init(columns: [?D] ?E) where isSubtype(E, Series?) {
      this.labels = D;
      this.idx = nil;
      init this;

      for lab in labels do
        this.columns[lab] = owned.release(columns[lab]!.copy());
    }

    proc init(columns: [?D], in idx: shared Index) {
      this.labels = D;
      this.idx = idx;
      init this;

      for lab in labels do
        this.insert(lab, columns[lab]!);
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
      var sCopy = owned.release(s.copy());
      sCopy.reindex(idx);
      labels += lab;
      columns[lab] = sCopy;
    }

    proc reindex(in idx: shared Index?) {
      this.idx = idx;
      for s in columns do
        s!.reindex(idx);
    }

    proc nrows() {
      var nMax = 0;
      for s in this {
        var n = s!.nrows();
        if n > nMax then nMax = n;
      }
      return nMax;
    }

    override
    proc serialize(writer, ref serializer) throws {
      if idx {
        idx!.writeThis(writer, _to_unmanaged(this));
      } else {
        var n = nrows();
        var nStr = n: string;
        var idxWidth = nStr.size + 1;
        const labelsSorted = sorted(labels);

        for space in 1..idxWidth do
          writer.write(" ");
        for lab in labelsSorted {
          writer.write(lab + "   ");
        }

        for i in 0..#n {
          writer.write("\n");
          var iStr = i: string;
          writer.write(iStr);
          for space in 1..idxWidth-iStr.size do
            writer.write(" ");

          for lab in labelsSorted {
            const ser = columns[lab];
            ser!.writeElemNoIndex(writer, i, lab.size);
            writer.write("   ");
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
  class SeriesAdd : SeriesUnifier(?) {
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

  class SeriesSubtr : SeriesUnifier(?) {
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

  class SeriesMult : SeriesUnifier(?) {
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

  class SeriesCompareScalar : SeriesMapper(?) {
    var x: eltType;

    proc init(x) {
      super.init(x.type, bool);
      this.x = x;
    }
  }

  class SeriesLessThan : SeriesCompareScalar(?) {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d < x;
    }
  }

  class SeriesGreaterThan : SeriesCompareScalar(?) {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d > x;
    }
  }

  class SeriesEqualTo : SeriesCompareScalar(?) {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d == x;
    }
  }

  class SeriesLessThanEqualTo : SeriesCompareScalar(?) {
    proc init(x) {
      super.init(x);
    }

    override
    proc f(d: eltType): retType {
      return d <= x;
    }
  }

  class SeriesGreaterThanEqualTo : SeriesCompareScalar(?) {
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
  operator Series.+(lhs: borrowed Series, rhs: borrowed Series) {
    return lhs.add(rhs);
  }

  operator Series.+(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.add_scalar(n);
  }

  operator Series.+(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.add_scalar(n);
  }

  operator Series.-(lhs: borrowed Series, rhs: borrowed Series) {
    return lhs.subtr(rhs);
  }

  operator Series.-(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.subtr_scalar(n);
  }

  operator Series.*(lhs: borrowed Series, rhs: borrowed Series) {
    return lhs.mult(rhs);
  }

  operator Series.*(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.mult_scalar(n);
  }

  operator Series.*(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.mult_scalar(n);
  }

  operator Series.<(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.lt_scalar(n);
  }

  operator Series.<(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.gt_scalar(n);
  }

  operator Series.>(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.gt_scalar(n);
  }

  operator Series.>(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.lt_scalar(n);
  }

  operator Series.==(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.eq_scalar(n);
  }

  operator Series.==(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.eq_scalar(n);
  }

  operator Series.<=(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.lteq_scalar(n);
  }

  operator Series.<=(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.gteq_scalar(n);
  }

  operator Series.>=(lhs: borrowed Series, n: ?N) where isNumericType(N) {
    return lhs.gteq_scalar(n);
  }

  operator Series.>=(n: ?N, rhs: borrowed Series) where isNumericType(N) {
    return rhs.lteq_scalar(n);
  }
}
