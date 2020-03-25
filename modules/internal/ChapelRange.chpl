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
/*
  A ``range`` is a first-class, constant-space representation of a
  regular sequence of values.  These values are typically integers,
  though ranges over enum types are also supported.  Ranges
  support iteration over the sequences they represent as well as
  operations such as counting, striding, intersection, shifting, and
  comparisons.

  Range Values
  ------------
  In their simplest form, ranges are represented by their low and high
  bounds:


  .. code-block:: chapel

    1..3    // 1, 2, 3
    0..n    // 0, 1, 2, 3, ..., n
    lo..hi  // lo, lo+1, lo+2, ..., hi

  Ranges may also be `unbounded`, in which case, the lower and/or upper
  bounds may be omitted:

  .. code-block:: chapel

    1..   // 1, 2, 3, ...
    ..10  // .., 8, 9, 10
    ..    // ..., -2, -1, 0, 1, 2, ...

  Ranges over enum types respect the declaration order of its values:

  .. code-block:: chapel

    enum color {red=4, orange=2, yellow=1, green=3, blue=6, indigo=7, violet=5};
    color.orange..color.green;   // orange, yellow, green

  Range Types
  -----------
  Range types are generic with respect to three fields:

  * ``idxType``: The type of the range's values—must an integral or enum type (defaults to ``int``)
  * ``boundedType``: A :enum:`BoundedRangeType` value indicating which bounds the range stores (defaults to ``bounded``)
  * ``stridable``: A boolean indicating whether or not the range can be strided (defaults to ``false``)

  The following code shows range variables declared with specified
  type signatures:

  .. code-block:: chapel

    var r1: range = 1..10;
    var r2: range(int(8)) = 1..myInt8;
    var r3: range(color) = color.green..color.blue;
    var r4: range(stridable=true) = 1..10 by 2;
    var r5: range(boundedType=BoundedRangeType.boundedNone) = ..;

  Like other variables, these types can be inferred by the compiler
  from the initializing expressions for simplicity:

  .. code-block:: chapel

    var r1 = 1..10;
    var r2 = 1..myInt8;
    var r3 = color.green..color.blue;
    var r4 = 1..10 by 2;
    var r5 = ..;



  Range Operators
  ---------------
  New ranges can be constructed from existing ones using the counting,
  striding, and/or alignment operators, ``#``, ``by``, and ``align``:

  .. code-block:: chapel

    0..#10              // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    0..10 by 2          // 0, 2, 4, 6, 8, 10
    0..10 by 2 align 1  // 1, 3, 5, 7, 9
    0.. by 2 # 10       // 0, 2, 4, 6, 8, 10, 12, 14, 16, 18

  Iteration over ranges
  ---------------------
  Ranges can be used as the iterable expression in ``for``, ``forall``, and ``coforall`` loops.

  .. code-block:: chapel

    for i in 1..10 { ... f(i) ... }
    forall i in 1..1000 { ... f(i) ... }
    coforall i in 0..#numTasks { ... f(i) ... }

  When ranges that are not fully bounded are zipped with another iterator,
  the other iterator is used to determine an ending point.

  .. code-block:: chapel

    // (i, j) will take the values: (1, 7), (2, 8), (3, 9), (4, 10)
    for (i, j) in zip(1..4, 7..) { ... }

    // (i, j) will take the values: (1, 10), (2, 9), (3, 8), (4, 7)
    for (i,j) in zip(1..4, ..10 by -1) { ... }

  Range Intersection
  ------------------
  A range can be intersected with another range to form a new range representing the intersection of the two ranges by `slicing` one range with the other.

  .. code-block:: chapel

    (1..10)[3..8] // 3..8
    (0..20)[1..20 by 2] // 1..20 by 2
    (1..10)[5..] // 5..10
    (1..10)[..5] // 1..5

  Range Shifting
  --------------
  A range can be shifted by an integer using the ``+`` and ``-`` operators.

  .. code-block:: chapel

    (1..10) + 5 // 6..15
    (1..10) - 3 // -2..7
    (1..) + 1 // 2..
    (..10) + 1 // ..11

  Range Comparisons
  -----------------
  Ranges can be compared for equality using the ``==`` and ``!=`` operators.

  .. code-block:: chapel

    1..10 == 1..10 // true
    1.. == 1.. // true
    1..10 != (1..10 by 2) // true

 */
module ChapelRange {

  private use ChapelBase, SysBasic, HaltWrappers;

  use Math;

  // Turns on range iterator debugging.
  pragma "no doc"
  config param debugChapelRange = false;

  pragma "no doc"
  config param useOptimizedRangeIterators = true;

  /*
    The ``BoundedRangeType`` enum is used to specify the types of bounds a
    range is required to have.

    * ``bounded`` - The range has finite low and high bounds.
    * ``boundedLow`` - The range starts at a given low bound, but conceptually goes up to infinity.
    * ``boundedHigh`` - The range conceptually starts at negative infinity and ends at a given high bound.
    * ``boundedNone`` - The range conceptually runs from negative infinity to infinity.
   */
  enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

  //
  // range type
  //
  //   parameterized by:
  //        1) an integral element type,
  //        2) whether low and/or high bounds exist, and
  //        3) whether the stride is one or not.
  //
  // The type of the stride is a signed type with the same number of bits as the
  // index type.  This means that for an unsigned index type, a maximal range can
  // be reached in no less than two strides.
  //
  // Alignments are taken modulo the stride, so if the stride is one,
  // even a nonzero alignment has no effect.  The alignment of a range
  // is relative to the low bound if it exists, otherwise relative to
  // the high bound if it exists, otherwise absolute.
  //
  // The _aligned flag is used to track whether a range has a non-ambiguous alignment.
  // All range literals have ambiguous alignment, but since they have a stride of 1
  // their isAmbiguous() is false.
  // The _aligned bit can also be set through the application of an explicit alignment.
  //

  // I think the record itself should not be documented, but the above comment
  // should be moved to the top-level module documentation.
  pragma "no doc"
  pragma "plain old data"
  pragma "range"
  record range
  {
    type idxType = int;                            // element type
    param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
    param stridable: bool = false;                 // range can be strided

    var _low       : chpl__idxTypeToIntIdxType(idxType);  // lower bound
    var _high      : chpl__idxTypeToIntIdxType(idxType);  // upper bound
    var _stride    : if stridable then chpl__rangeStrideType(idxType) else nothing; // signed stride
    var _alignment : if stridable then chpl__idxTypeToIntIdxType(idxType) else nothing; // alignment
    var _aligned   : if stridable then bool else nothing;

    proc strType type  return chpl__rangeStrideType(idxType);

    proc chpl__promotionType() type {
      return idxType;
    }
  }

  /* The ``idxType`` as represented by an integer type.  When
     ``idxType`` is an enum type, this evaluates to ``int``.
     Otherwise, it evaluates to ``idxType``. */
  proc range.intIdxType type {
    return chpl__idxTypeToIntIdxType(idxType);
  }


  //################################################################################
  //# Initializers
  //#

  // Declare this as an initializer, so we can capture range creation.
  // If it is not an initializer, then the user can still create a maximal
  // range (for example) without being warned.
  //
  pragma "no doc"
  proc range.init(type idxType = int,
                  param boundedType : BoundedRangeType = BoundedRangeType.bounded,
                  param stridable : bool = false,
                  _low : idxType = chpl__intToIdx(idxType, 1),
                  _high : idxType = chpl__intToIdx(idxType, 0),
                  _stride : chpl__rangeStrideType(idxType) = 1,
                  _alignment : idxType = chpl__intToIdx(idxType, 0),
                  _aligned : bool = false) {
    this.idxType     = idxType;
    this.boundedType = boundedType;
    this.stridable   = stridable;
    this._low = chpl__idxToInt(_low);
    this._high = chpl__idxToInt(_high);
    this.complete();
    if stridable {
      this._stride    = _stride;
      this._alignment = chpl__idxToInt(_alignment);
      this._aligned   = _aligned;
    }

    if !stridable && boundsChecking then
      assert(_stride == 1);
  }

  private proc _isAnyNothing(args...) param : bool {
    for param i in 1..args.size {
      if isNothingType(args(i).type) then return true;
    }
    return false;
  }

  pragma "no doc"
  proc range.init(type idxType = int,
                  param boundedType : BoundedRangeType = BoundedRangeType.bounded,
                  param stridable : bool = false,
                  _low : idxType = chpl__intToIdx(idxType, 1),
                  _high : idxType = chpl__intToIdx(idxType, 0),
                  _stride,
                  _alignment,
                  _aligned)
    where _isAnyNothing(_stride, _alignment, _aligned) {

    this.idxType     = idxType;
    this.boundedType = boundedType;
    this.stridable   = stridable;
    this._low        = chpl__idxToInt(_low);
    this._high       = chpl__idxToInt(_high);
    this.complete();
    if stridable then
      compilerError("non-stridable range initializer called with stridable=true");
  }

  pragma "no doc"
  proc range.init=(other : range(?i,?b,?s)) {
    type idxType      = if this.type.idxType     == ? then i else this.type.idxType;
    param boundedType = if this.type.boundedType == ? then b else this.type.boundedType;
    param stridable   = if this.type.stridable   == ? then s else this.type.stridable;

    if boundedType != b {
      compilerError("range(boundedType=" + this.type.boundedType:string + ") cannot be initialized from range(boundedType=" + b:string + ")");
    }

    if !stridable && s then
      compilerError("cannot initialize a non-stridable range from a stridable range");

    const str = if stridable && s then other.stride else 1:chpl__rangeStrideType(idxType);

    this.init(idxType, boundedType, stridable,
              chpl__intToIdx(idxType, other._low), chpl__intToIdx(idxType, other._high),
              str,
              chpl__intToIdx(idxType, chpl__idxToInt(other.alignment)),
              other.aligned);
  }

  /////////////////////////////////
  // for debugging
  pragma "no doc"
  proc range.displayRepresentation(msg: string = ""): void {
    chpl_debug_writeln(msg, "(", idxType:string, ",", boundedType, ",", stridable,
                       " : ", low, ",", high, ",", stride, ",",
                       if aligned then alignment:string else "?", ")");
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range builders:  used by the parser to create literal ranges
  //

  // Range builders for fully bounded ranges
  proc chpl_build_bounded_range(low: int(?w), high: int(w))
    return new range(int(w), _low=low, _high=high);
  proc chpl_build_bounded_range(low: uint(?w), high: uint(w))
    return new range(uint(w), _low=low, _high=high);
  proc chpl_build_bounded_range(low: enum, high: enum) {
    if (low.type != high.type) then
      compilerError("ranges of enums must use a single enum type");
    return new range(low.type, _low=low, _high=high);
  }
  proc chpl_build_bounded_range(low: bool, high: bool)
    return new range(bool, _low=low, _high=high);
  proc chpl_build_bounded_range(low, high) {
    compilerError("Bounds of 'low..high' must be integers of compatible types.");
  }

  // Range builders for low bounded ranges
  proc chpl_build_low_bounded_range(low: integral)
    return new range(low.type, BoundedRangeType.boundedLow, _low=low);
  proc chpl_build_low_bounded_range(low: enum)
    return new range(low.type, BoundedRangeType.boundedLow, _low=low);
  proc chpl_build_low_bounded_range(low: bool)
    return new range(low.type, BoundedRangeType.boundedLow, _low=low);
  proc chpl_build_low_bounded_range(low) {
    compilerError("Bound of 'low..' must be an integer");
  }

  // Range builders for high bounded ranges
  proc chpl_build_high_bounded_range(high: integral)
    return new range(high.type, BoundedRangeType.boundedHigh, _high=high);
  proc chpl_build_high_bounded_range(high: enum)
    return new range(high.type, BoundedRangeType.boundedHigh, _high=high);
  proc chpl_build_high_bounded_range(high: bool)
    return new range(high.type, BoundedRangeType.boundedHigh, _high=high);
  proc chpl_build_high_bounded_range(high) {
    compilerError("Bound of '..high' must be an integer.");
  }

  // Range builder for unbounded ranges
  proc chpl_build_unbounded_range()
    return new range(int, BoundedRangeType.boundedNone);

  /////////////////////////////////////////////////////////////////////
  // Helper functions for ranges in param loops (and maybe param ranges
  // later)
  //
  // Necessary for coercion support
  /////////////////////////////////////////////////////////////////////
  proc chpl_compute_low_param_loop_bound(param low: int(?w),
                                         param high: int(w)) param {
    return low;
  }

  proc chpl_compute_high_param_loop_bound(param low: int(?w),
                                          param high: int(w)) param {
    return high;
  }

  proc chpl_compute_low_param_loop_bound(param low: uint(?w),
                                         param high: uint(w)) param {
    return low;
  }

  proc chpl_compute_high_param_loop_bound(param low: uint(?w),
                                          param high: uint(w)) param {
    return high;
  }

  proc chpl_compute_low_param_loop_bound(param low: bool,
                                         param high: bool) param {
    return low;
  }

  proc chpl_compute_high_param_loop_bound(param low: bool,
                                          param high: bool) param {
    return high;
  }

  pragma "last resort"
  proc chpl_compute_low_param_loop_bound(param low, param high) param {
    compilerError("Range bounds must be integers of compatible types");
  }

  pragma "last resort"
  proc chpl_compute_low_param_loop_bound(low, high) {
    compilerError("param for loop must be defined over a bounded param range");
  }

  //################################################################################
  //# Predicates
  //#
  /* Return true if argument ``t`` is a range type, false otherwise */
  proc isRangeType(type t) param {
    proc isRangeHelp(type t: range(?)) param  return true;
    proc isRangeHelp(type t)           param  return false;
    return isRangeHelp(t);
  }
  pragma "no doc"
  proc isRangeValue(r: range(?)) param  return true;
  /* Return true if argument ``r`` is a range, false otherwise */
  pragma "no doc"
  proc isRangeValue(r)           param  return false;

  // isBoundedRange(r) = true if 'r' is a (fully) bounded range
  pragma "no doc"
  proc isBoundedRange(r)           param
    return false;
  /* Return true if argument ``r`` is a fully bounded range, false otherwise */
  proc isBoundedRange(r: range(?)) param
    return isBoundedRange(r.boundedType);

  pragma "no doc"
  proc isBoundedRange(param B: BoundedRangeType) param
    return B == BoundedRangeType.bounded;

  /* Return true if this range has a low bound, false otherwise */
  proc range.hasLowBound() param
    return boundedType == BoundedRangeType.bounded ||
           boundedType == BoundedRangeType.boundedLow;

  /* Returns true if this range has a high bound, false otherwise */
  proc range.hasHighBound() param
    return boundedType == BoundedRangeType.bounded ||
           boundedType == BoundedRangeType.boundedHigh;

  /* Returns the stride of the range */
  inline proc range.stride where stridable  return _stride;
  pragma "no doc"
  proc range.stride param where !stridable return 1 : strType;

  /* Returns the alignment of the range */
  inline proc range.alignment where stridable return chpl_intToIdx(_alignment);
  pragma "no doc"
  proc range.alignment where !stridable && hasLowBound() return low;
  pragma "no doc"
  proc range.alignment return chpl_intToIdx(0);

  /* Returns true if the range is aligned */
  inline proc range.aligned where stridable return _aligned;

  pragma "no doc"
  proc range.aligned param where !stridable &&
                                 (boundedType == BoundedRangeType.bounded ||
                                  boundedType == BoundedRangeType.boundedLow)
    return true;
  pragma "no doc"
  proc range.aligned param /* !stridable && (boundedHigh || boundedNone) */
    return false;

  /* Return the first element in the sequence the range represents */
  inline proc range.first {
    return chpl_intToIdx(this.firstAsInt);
  }

  pragma "no doc"
  inline proc range.firstAsInt {
    if ! stridable then return _low;
    else return if _stride > 0 then this.alignedLowAsInt else this.alignedHighAsInt;
  }

  /* Return the last element in the sequence the range represents */
  inline proc range.last {
    return chpl_intToIdx(this.lastAsInt);
  }

  pragma "no doc"
  inline proc range.lastAsInt {
    if ! stridable then return _high;
    else return if stride > 0 then this.alignedHighAsInt else this.alignedLowAsInt;
  }

  /* Return the range's low bound. If the range does not have a low
     bound the behavior is undefined. */
  inline proc range.low  return chpl_intToIdx(_low);


  /* Return the range's high bound. If the range does not have a high
     bound the behavior is undefined. */
  inline proc range.high return chpl_intToIdx(_high);


  /* Returns the range's aligned low bound. If the aligned low bound is
     undefined (does not exist), the behavior is undefined.
   */
  inline proc range.alignedLow : idxType {
    return chpl_intToIdx(this.alignedLowAsInt);
  }

  pragma "no doc"
  inline proc range.alignedLowAsInt {
    if !stridable then
      return _low;
    else
      // Adjust _low upward by the difference between _alignment and _low.
      return _low + chpl__diffMod(_alignment, _low, stride);
  }

  /* Returns the range's aligned high bound. If the aligned high bound is
     undefined, the behavior is undefined.
   */
  // TODO: Add back example?
  inline proc range.alignedHigh : idxType {
    return chpl_intToIdx(this.alignedHighAsInt);
  }

  pragma "no doc"
  inline proc range.alignedHighAsInt {
    if ! stridable then
      return _high;
    else
      // Adjust _high downward by the difference between _high and _alignment.
      return _high - chpl__diffMod(_high, _alignment, stride);
  }

  /* If the sequence represented by the range is empty, return true.  An
     error is reported if the range is ambiguous.
   */
  inline proc range.isEmpty() {
    if boundsChecking && isAmbiguous() then
      HaltWrappers.boundsCheckHalt("isEmpty() is invoked on an ambiguously-aligned range");
    else
      return isBoundedRange(this) && this.alignedLow > this.alignedHigh;
  }


  /* Deprecated - please use :proc:`range.size`. */
  inline proc range.length: intIdxType {
    compilerWarning("'range.length' is deprecated - " +
                    "please use 'range.size' instead");
    return this.size;
  }

  /* Returns the number of elements in this range, cast to the index type.

     Note: The result is undefined if the index is signed
     and the low and high bounds differ by more than ``max(``:proc:`range.intIdxType` ``)``.
   */
  proc range.size: intIdxType {
    if ! isBoundedRange(this) then
      compilerError("'size' is not defined on unbounded ranges");

    // assumes alignedHigh/alignLow always work, even for an empty range
    const ah = this.alignedHighAsInt,
          al = this.alignedLowAsInt;
    if al > ah then return 0: intIdxType;
    const s = abs(this.stride): intIdxType;
    return (ah - al) / s + 1:intIdxType;
  }

  /* Return true if the range has a first index, false otherwise */
  proc range.hasFirst() param where !stridable && !hasHighBound()
    return hasLowBound();

  pragma "no doc"
  inline proc range.hasFirst()
    return if isAmbiguous() || isEmpty() then false else
      if stride > 0 then hasLowBound() else hasHighBound();

  /* Return true if the range has a last index, false otherwise */
  proc range.hasLast() param where !stridable && !hasLowBound()
    return hasHighBound();

  pragma "no doc"
  inline proc range.hasLast()
    return if isAmbiguous() || isEmpty() then false else
      if stride > 0 then hasHighBound() else hasLowBound();

  /* Returns true if this range is naturally aligned, false otherwise */
  proc range.isNaturallyAligned()
    where stridable && this.boundedType == BoundedRangeType.bounded
  {
    // If the stride is positive, we must be aligned on the low bound.
    if stride > 0 then return this.alignedLowAsInt == _low;
    // If the stride is negative, we must be aligned on the high bound.
    if stride < 0 then return this.alignedHighAsInt == _high;
    // stride == 0: ???
    return false;
  }

  pragma "no doc"
  inline proc range.isNaturallyAligned() param
    where !stridable && this.boundedType == BoundedRangeType.bounded
  {
    return true;
  }

  pragma "no doc"
  inline proc range.isNaturallyAligned()
    where !stridable && this.boundedType == BoundedRangeType.boundedLow
  {
    return this.alignedLowAsInt == _low;
  }

  pragma "no doc"
  inline proc range.isNaturallyAligned()
    where stridable && this.boundedType == BoundedRangeType.boundedLow
  {
    return stride > 0 && this.alignedLowAsInt == _low;
  }

  pragma "no doc"
  inline proc range.isNaturallyAligned() param
    where this.boundedType == BoundedRangeType.boundedNone ||
          !stridable && this.boundedType == BoundedRangeType.boundedHigh
  {
    return false;
  }

  pragma "no doc"
  inline proc range.isNaturallyAligned()
    where stridable && this.boundedType == BoundedRangeType.boundedHigh
  {
    return stride < 0 && this.alignedHighAsInt == _high;
  }

  /* Returns true if the range is ambiguously aligned, false otherwise */
  proc range.isAmbiguous() param where !stridable
    return false;

  pragma "no doc"
  proc range.isAmbiguous()       where stridable
    return !aligned && (stride > 1 || stride < -1);

  /* Returns true if ``ind`` is in this range, false otherwise */
  inline proc range.contains(ind: idxType)
  {
    if this.isAmbiguous() then return false;

    const i = chpl__idxToInt(ind);

    if hasHighBound()
    {
      if i > _high then return false;
    }
    if hasLowBound()
    {
      if i < _low then return false;
    }
    if stridable
    {
      var s = abs(_stride):intIdxType;
      if chpl__diffMod(i, _alignment, s) != 0
        then return false;
    }
    return true;
  }

  /* Returns true if the range ``other`` is contained within this one,
     false otherwise
   */
  inline proc range.contains(other: range(?))
  {
    if this.isAmbiguous() || other.isAmbiguous() then return false;

    // Since slicing preserves the direction of the first arg, may need
    // to negate one of the strides (shouldn't matter which).
    if stridable {
      if (stride > 0 && other.stride < 0) || (stride < 0 && other.stride > 0)
        then return _containsHelp(this, other);
    } else {
      if other.stride < 0
        then return _containsHelp(this, other);
    }
    return other == this(other);
  }

  /* Deprecated - please use :proc:`range.contains`. */
  inline proc range.member(ind: idxType) {
    compilerWarning("range.member is deprecated - " +
                    "please use range.contains instead");
    return this.contains(ind);
  }

  /* Deprecated - please use :proc:`range.contains`. */
  inline proc range.member(other: range(?)) {
    compilerWarning("range.member is deprecated - " +
                    "please use range.contains instead");
    return this.contains(other);
  }

  // Negate one of the two args' strides before comparison.
  private inline proc _containsHelp(in arg1: range(?), in arg2: range(?)) {
    if arg2.stridable then
      arg2._stride = -arg2._stride;
    else
      arg1._stride = -arg1._stride;
    return arg2 == arg1(arg2);
  }

  proc ==(r1: range(?), r2: range(?)) param
    where r1.boundedType != r2.boundedType
  return false;

  proc ==(r1: range(?), r2: range(?)): bool
    where r1.boundedType == r2.boundedType
  {
    // An ambiguous ranges cannot equal an unambiguous one
    //  even if all their parameters match.
    if r1.isAmbiguous() != r2.isAmbiguous() then return false;

    // As a special case, two ambiguous ranges compare equal
    // if their representations are identical.
    if r1.isAmbiguous() then return ident(r1, r2);

    if isBoundedRange(r1) {

      // gotta have a special case for length 0 or 1
      const len = r1.size, l2 = r2.size;
      if len != l2 then return false;
      if len == 0 then return true;
      if r1.first != r2.first then return false;
      if len == 1 then return true; // rest doesn't matter
      if r1.stride != r2.stride then return false;
      return true;

    } else {

      // ! isBoundedRange(r1)
      if r1.stride != r2.stride then return false;

      if r1.hasLowBound() then
        if r1.alignedLow != r2.alignedLow then return false;

      if r1.hasHighBound() then
        if r1.alignedHigh != r2.alignedHigh then return false;

      return true;
    }
  }

  proc !=(r1: range(?), r2: range(?))  return !(r1 == r2);

  /* Returns true if the two ranges are the same in every respect: i.e. the
     two ranges have the same ``idxType``, ``boundedType``, ``stridable``,
     ``low``, ``high``, ``stride`` and ``alignment`` values.
   */
  proc ident(r1: range(?), r2: range(?))
    where r1.idxType == r2.idxType &&
    r1.boundedType == r2.boundedType &&
    r1.stridable == r2.stridable
  {
    if r1._low != r2._low then return false;
    if r1._high != r2._high then return false;
    if (r1.stridable) {
      if (r2.stridable) {
        if r1._stride != r2._stride then return false;
        if r1._alignment != r2._alignment then return false;
        if r1._aligned != r2._aligned then return false;
      } else {
        return false;
      }
    }
    return true;
  }

  // If the parameters don't match, then the two ranges cannot be identical.
  pragma "no doc"
  proc ident(r1: range(?), r2: range(?)) param
    return false;

  //////////////////////////////////////////////////////////////////////////////////
  // Range Casts
  //
/* Cast a range to another range type. If the old type is stridable and the
   new type is not stridable, ensure at runtime that the old stride was 1.
 */
pragma "no doc"
proc range.safeCast(type t) where isRangeType(t) {
  var tmp: t;

  if tmp.boundedType != this.boundedType {
    compilerError("cannot cast range from ",
                  this.boundedType:string, " to ", tmp.boundedType:string);
  }

  if tmp.stridable {
    tmp._stride = this.stride;
    tmp._alignment = chpl__idxToInt(this.alignment).safeCast(tmp.intIdxType);
    tmp._aligned = this.aligned;
  } else if this.stride != 1 {
    HaltWrappers.safeCastCheckHalt("illegal safeCast from non-unit stride range to unstridable range");
  }

  tmp._low = this._low.safeCast(tmp.intIdxType);
  tmp._high = this._high.safeCast(tmp.intIdxType);

  return tmp;
}

/* Cast a range to a new range type.  If the old type was stridable and the
   new type is not stridable, then force the new stride to be 1.
 */
pragma "no doc"
proc _cast(type t, r: range(?)) where isRangeType(t) {
  var tmp: t;

  if tmp.boundedType != r.boundedType {
    compilerError("cannot cast range from ",
                  r.boundedType:string, " to ", tmp.boundedType:string);
  }

  if tmp.stridable {
    tmp._stride = r._stride;
    tmp._alignment = r._alignment: tmp.intIdxType;
    tmp._aligned = r._aligned;
  }

  tmp._low = r.low: tmp.intIdxType;
  tmp._high = r.high: tmp.intIdxType;
  return tmp;
}


  //////////////////////////////////////////////////////////////////////////////////
  // Bounds checking
  //
  /* Returns true if ``other`` lies entirely within this range and false
     otherwise.  Returns false if either range is ambiguously aligned.
   */
  inline proc range.boundsCheck(other: range(?e,?b,?s))
    where b == BoundedRangeType.boundedNone
  {
    if this.isAmbiguous() || other.isAmbiguous()
      then return false;

    return true;
  }

  pragma "no doc"
  inline proc range.boundsCheck(other: range(?e,?b,?s))
  {
    if this.isAmbiguous() || other.isAmbiguous()
      then return false;

    var boundedOther = new range(
                          idxType, BoundedRangeType.bounded,
                          s || this.stridable,
                          if other.hasLowBound() then other.low else low,
                          if other.hasHighBound() then other.high else high,
                          other.stride,
                          other.alignment,
                          true);

    return (boundedOther.size == 0) || contains(boundedOther);
  }
  /* Return true if ``other`` is contained in this range and false otherwise */
  inline proc range.boundsCheck(other: idxType)
    return contains(other);


  //################################################################################
  //# Member functions
  //#

  // Moves the low bound of the range up to the next alignment point.
  pragma "no doc"
  /* private */ proc ref range.alignLow()
  {
    if boundsChecking && this.isAmbiguous() then
      HaltWrappers.boundsCheckHalt("alignLow -- Cannot be applied to a range with ambiguous alignment.");

    if stridable then _low = this.alignedLowAsInt;
    return this;
  }

  // Moves the high bound of the range down to the next alignment point.
  pragma "no doc"
  /* private */ proc ref range.alignHigh()
  {
    if boundsChecking && this.isAmbiguous() then
      HaltWrappers.boundsCheckHalt("alignHigh -- Cannot be applied to a range with ambiguous alignment.");

    if stridable then _high = this.alignedHighAsInt;
    return this;
  }

  /*
     If ``ind`` is a member of the range's represented sequence, returns
     an integer giving the ordinal index of ind within the sequence
     using zero-based indexing. Otherwise, returns
     ``(-1):``:proc:`range.intIdxType`. It is an error to invoke
     ``indexOrder`` if the represented sequence is not defined or the
     range does not have a first index.

     The following calls show the order of index 4 in each of the given ranges:

.. code-block:: chapel

       (0..10).indexOrder(4) == 4
       (1..10).indexOrder(4) == 3
       (3..5).indexOrder(4) == 1
       (0..10 by 2).indexOrder(4) == 2
       (3..5 by 2).indexOrder(4) == -1
   */
  proc range.indexOrder(ind: idxType)
  {
    if boundsChecking && this.isAmbiguous() then
      HaltWrappers.boundsCheckHalt("indexOrder -- Undefined on a range with ambiguous alignment.");

    if ! contains(ind) then return (-1):intIdxType;
    if ! stridable {
      if this.hasLowBound() then
        return chpl__idxToInt(ind) - _low;
    } else {
      if this.hasFirst() then
        return ((chpl__idxToInt(ind):strType - chpl__idxToInt(this.first):strType) / _stride):intIdxType;
    }
    return (-1):intIdxType;
  }

  /* Returns the zero-based ``ord``-th element of this range's represented
     sequence. It is an error to invoke ``orderToIndex`` if the range is not
     defined, or if ``ord`` is negative or greater than the range's size.
     The ``orderToIndex`` procedure is the reverse of ``indexOrder``.

     Example:

     .. code-block:: chapel

       0..10.orderToIndex(4) == 4
       1..10.orderToIndex(3) == 4
       3..5.orderToIndex(1)  == 4
       0..10 by 2.orderToIndex(2) == 4
   */
  proc range.orderToIndex(ord: integral): idxType
  {
    if boundsChecking {
      if !hasFirst() then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on a range that has no first index");

      if isAmbiguous() then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on a range that is ambiguously aligned");

      if ord < 0 then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on a negative integer: " + ord:string);

      if isBoundedRange(this) && ord >= this.size then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on an integer " +
            ord:string + " that is larger than the range's number of indices " + this.size:string);
    }

    return chpl_intToIdx(chpl__addRangeStrides(this.firstAsInt, this.stride,
                                                ord));
  }

  //////////////////////////////////////////////////////////////////////////////////
  // translate the indices in this range by i
  // A range can be translated even if its alignment is ambiguous.
  //
  // REVIEW: hilde
  // Should member functions normally return new objects?
  //
  // NOTE: The casts below assume that the value will fit into
  // idxType.  Handling errors when (down) casting is something
  // we need to handle more generally in the future, so for
  // consistency, we are not handling it here at all :-P
  //
  /* Return a range with elements shifted from this range by ``offset``.

     Example:

     .. code-block:: chapel

       0..9.translate(1) == 1..10
       0..9.translate(2) == 2..11
       0..9.translate(-1) == -1..8
       0..9.translate(-2) == -2..7
   */
  inline proc range.translate(offset: integral)
    return this + offset;

  pragma "no doc"
  inline proc range.translate(i)
  {
    compilerError("offsets must be of integral type");
  }

  // Compute the alignment of the range returned by this.interior()
  // and this.exterior(). Keep it private.
  pragma "no doc"
  inline proc range._effAlmt()       where stridable return alignment;

  pragma "no doc"
  proc range._effAlmt() where !stridable return chpl_intToIdx(0);

  // Return an interior portion of this range.
  pragma "no doc"
  proc range.interior(offset: integral)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("interior is not supported on unbounded ranges");
  }

  // TODO: hilde
  // Set _aligned to true only if stridable.
  /* Return a range with ``offset`` elements from the interior portion of this
     range. If ``offset`` is positive, take elements from the high end, and if
     ``offset`` is negative, take elements from the low end.

     Example:

     .. code-block:: chapel

       0..9.interior(1)  == 9..9
       0..9.interior(2)  == 8..9
       0..9.interior(-1) == 0..0
       0..9.interior(-2) == 0..1
   */
  proc range.interior(offset: integral)
  {
    const i = offset.safeCast(intIdxType);
    if i < 0 then
      return new range(idxType, boundedType, stridable,
                       low, chpl_intToIdx(_low - 1 - i), stride,
                       _effAlmt(), aligned);
    if i > 0 then
      return new range(idxType, boundedType, stridable,
                       chpl_intToIdx(_high + 1 - i), high, stride,
                       _effAlmt(), aligned);
    // if i == 0 then
    return new range(idxType, boundedType, stridable,
                     low, high, stride, _effAlmt(), aligned);
  }

  pragma "no doc"
  proc range.exterior(offset: integral)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("exterior is not supported on unbounded ranges");
  }

  /* Return a range with ``offset`` elements from the exterior portion of this
     range. If ``offset`` is positive, take elements from the high end, and if
     ``offset`` is negative, take elements from the low end.

     Example:

     .. code-block:: chapel

       0..9.exterior(1)  = 10..10
       0..9.exterior(2)  = 10..11
       0..9.exterior(-1) = -1..-1
       0..9.exterior(-2) = -2..-1
   */
  proc range.exterior(offset: integral)
  {
    const i = offset.safeCast(intIdxType);
    if i < 0 then
      return new range(idxType, boundedType, stridable,
                       chpl_intToIdx(_low + i),
                       chpl_intToIdx(_low - 1),
                       stride, _effAlmt(), aligned);
    if i > 0 then
      return new range(idxType, boundedType, stridable,
                       chpl_intToIdx(_high + 1),
                       chpl_intToIdx(_high + i),
                       stride, _effAlmt(), aligned);
    // if i == 0 then
    return new range(idxType, boundedType, stridable,
                     low, high, stride, _effAlmt(), aligned);
  }

  // Returns an expanded range, or a contracted range if offset < 0.
  // The existing absolute alignment is preserved.
  pragma "no doc"
  proc range.expand(offset: integral)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("expand() is not supported on unbounded ranges");
  }
  /* Return a range expanded by ``offset`` elements from each end.  If ``offset`` is
     negative, the range will be contracted.

     Example:

     .. code-block:: chapel

       0..9.expand(1)  == -1..10
       0..9.expand(2)  == -2..11
       0..9.expand(-1) == 1..8
       0..9.expand(-2) == 2..7
   */
  proc range.expand(offset: integral)
  {
    const i = offset.safeCast(intIdxType);
    return new range(idxType, boundedType, stridable,
                     chpl_intToIdx(_low-i),
                     chpl_intToIdx(_high+i),
                     stride, alignment, _aligned);
  }


  //################################################################################
  //# Syntax Functions
  //# These are special functions known to the compiler parser.
  //#

  // Assignment
  inline proc =(ref r1: range(stridable=?s1), r2: range(stridable=?s2))
  {
    if r1.boundedType != r2.boundedType then
      compilerError("type mismatch in assignment of ranges with different boundedType parameters");

    if !s1 && s2 then
      compilerError("type mismatch in assignment of ranges with different stridable parameters");
    else if s1 && s2 then
      r1._stride = r2.stride;
    else if s1 then
      r1._stride = 1;

    r1._low = r2._low;
    r1._high = r2._high;

    if s1 {
      r1._alignment = chpl__idxToInt(r2.alignment);
      r1._aligned = r2.aligned;
    }
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range arithmetic.
  //
  // Shifts and entire range to the right or left.
  // Absolute alignment is not preserved
  // (That is, the alignment shifts along with the range.)
  //
  inline proc +(r: range(?e, ?b, ?s), offset: integral)
  {
    const i = offset:r.intIdxType;
    type strType = chpl__rangeStrideType(e);

    return new range(e, b, s,
                     r.chpl_intToIdx(r._low + i),
                     r.chpl_intToIdx(r._high + i),
                     r.stride : strType,
                     r.chpl_intToIdx(chpl__idxToInt(r.alignment)+i),
                     r.aligned);
  }

  inline proc +(i:integral, r: range(?e,?b,?s))
    return r + i;

  inline proc -(r: range(?e,?b,?s), i: integral)
  {
    type strType = chpl__rangeStrideType(e);

    return new range(e, b, s,
                     r.chpl_intToIdx(r._low - i),
                     r.chpl_intToIdx(r._high - i),
                     r.stride : strType,
                     r.chpl_intToIdx(chpl__idxToInt(r.alignment) - i),
                     r.aligned);
  }

  inline proc chpl_check_step_integral(step) {
    if !isIntegral(step.type) then
      compilerError("can't apply 'by' using step of a non-integral type ",
                    step.type:string);
  }

  proc chpl_need_to_check_step(step, type strType) param {
    compilerAssert(isInt(strType)); // we assume strType is signed
    // 'step' is either same-sized unsigned, or any larger size
    return step.type != strType && numBits(step.type) >= numBits(strType);
  }

  // Helpers to check if the stride of a range is invalid. Error (either at
  // runtime or compile time) if it's invalid.

  inline proc chpl_range_check_stride(step, type idxType) {
    chpl_check_step_integral(step);
    type strType = chpl__rangeStrideType(idxType);

    // At present, step must coerce to range's intIdxType or strType.
    if numBits(step.type) > numBits(strType) then
      compilerError("can't apply 'by' to a range with idxType ",
                    idxType:string, " using a step of type ",
                    step.type:string);

    if boundsChecking {
      if step == 0 then
        HaltWrappers.boundsCheckHalt("the step argument of the 'by' operator is zero");

      if chpl_need_to_check_step(step, strType) &&
         step > (max(strType):step.type)
      then
        HaltWrappers.boundsCheckHalt("the step argument of the 'by' operator is too large and cannot be represented within the range's stride type " + strType:string);
    }
  }

  inline proc chpl_range_check_stride(param step, type idxType)  {
    chpl_check_step_integral(step);
    type strType = chpl__rangeStrideType(idxType);

    if step == 0 then
      compilerError("the step argument of the 'by' operator is zero");

    // do not check e.g. when step and strType are both int(64)
    if chpl_need_to_check_step(step, strType) &&
       step > (max(strType):step.type)
    then
      compilerError("the step argument of the 'by' operator is too large and cannot be represented within the range's stride type " + strType:string);
  }

  proc chpl_by_help(r: range(?i,?b,?s), step) {
    const lw: i = r.low,
          hh: i = r.high,
          st: r.strType = r.stride * step:r.strType;

    const (ald, alt): (bool, i) =
      if r.isAmbiguous() then
        if r.stridable then (false, r.alignment)
                       else (false, r.chpl_intToIdx(0))
      else
        // we could talk about aligned bounds
        if      r.hasLowBound()  && st > 0 then (true, r.alignedLow)
        else if r.hasHighBound() && st < 0 then (true, r.alignedHigh)
        else
          if r.stridable then (r.aligned, r.alignment)
                         else (false, r.chpl_intToIdx(0));

    return new range(i, b, true,  lw, hh, st, alt, ald);
  }

  /*
   * The following procedure is effectively equivalent to:
   *
  inline proc chpl_by(r, step) { ... }
   *
   * because the parser renames the routine since 'by' is a keyword.
   */
  pragma "no doc"
  inline proc by(r, step) {
    if !isRange(r) then
      compilerError("the first argument of the 'by' operator is not a range");
    chpl_range_check_stride(step, r.idxType);
    return chpl_by_help(r, step);
  }

  /*
   * The following procedure is effectively equivalent to:
   *
  inline proc chpl_by(r: range(?), param step) { ... }
   *
   * because the parser renames the routine since 'by' is a keyword.
   */
  // We want to warn the user at compiler time if they had an invalid param
  // stride rather than waiting until runtime.
  pragma "no doc"
  inline proc by(r : range(?), param step) {
    chpl_range_check_stride(step, r.idxType);
    return chpl_by_help(r, step:r.strType);
  }


  /*
   * The following procedure is effectively equivalent to:
   *
  inline proc chpl_align(r: range(?i, ?b, ?s), algn: i) { ... }
   *
   * because the parser renames the routine since 'align' is a keyword.
   */
  // This is the definition of the 'align' operator for ranges.
  // It produces a new range with the specified alignment.
  // By definition, alignment is relative to the low bound of the range.
  pragma "no doc"
  inline proc align(r : range(?i, ?b, ?s), algn: i)
  {
    // Note that aligning an unstrided range will set the field value,
    // but has no effect on the index set produced (a mod 1 == 0).
    return new range(i, b, true,
                     r.chpl_intToIdx(r._low), r.chpl_intToIdx(r._high), r.stride, algn, true);
  }


  /*
   * The following procedure is effectively equivalent to:
   *
  inline proc chpl_align(r: range(?i, ?b, ?s), algn) { ... }
   *
   * because the parser renames the routine since 'align' is a keyword.
   */
  pragma "no doc"
  inline proc align(r : range(?i, ?b, ?s), algn) {
    compilerError("can't align a range with idxType ", i:string,
                  " using a value of type ", algn.type:string);
    return r;
  }

  /* Returns a range whose alignment is this range's first index plus ``offset``.
     If the range has no first index, a runtime error is generated.
   */
  proc range.offset(in offset: integral)
  {
    var offs = offset.safeCast(intIdxType);
    if !stridable {
      compilerWarning("invoking 'offset' on an unstrided range has no effect.");
      offs = 0;
    }

    if boundsChecking && !hasFirst() then
      HaltWrappers.boundsCheckHalt("invoking 'offset' on a range without the first index");

    return new range(idxType, boundedType, stridable, low, high, stride,
                     // here's the new alignment
                     chpl_intToIdx(this.firstAsInt + offs), true);
  }


  // Composition
  // Return the intersection of this and other.
  pragma "no doc"
  proc const range.this(other: range(?))
  {
    // Two cases to consider:
    //  1) Both ranges unambiguously aligned
    //  2) One or both ranges ambiguously aligned.
    // In the latter case, we can obtain a result modulo alignment
    // as long as the two strides are relatively prime.
    // Otherwise, we can't know whether the two ranges intersect or not.
    var ambig = false;
    if this.isAmbiguous() || other.isAmbiguous()
    {
      var st1 = abs(this.stride);
      var st2 = abs(other.stride);
      var (g,x) = chpl__extendedEuclid(st1, st2);
      if boundsChecking && g > 1 then
        HaltWrappers.boundsCheckHalt("Cannot slice ranges with ambiguous alignments unless their strides are relatively prime.");

      // OK, we can combine these two ranges, but the result is marked as ambiguous.
      ambig = true;
    }

    // Determine the boundedType of result
    proc computeBoundedType(r1, r2) param
    {
      param low = r1.hasLowBound() || r2.hasLowBound();
      param high = r1.hasHighBound() || r2.hasHighBound();
      if low && high then
        return BoundedRangeType.bounded;
      else if low then
        return BoundedRangeType.boundedLow;
      else if high then
        return BoundedRangeType.boundedHigh;
      else
        return BoundedRangeType.boundedNone;
    }

    // If this range is unbounded below, we use low from the other range,
    // so that max(lo1, lo2) == lo2.  etc.
    var lo1 = if hasLowBound() then this._low else other._low;
    var hi1 = if hasHighBound() then this._high else other._high;
    var st1 = abs(this.stride);

    var lo2 = if other.hasLowBound() then other._low else this._low;
    var hi2 = if other.hasHighBound() then other._high else this._high;
    var st2 = abs(other.stride);

    // If the result type is unsigned, don't let the low bound go negative.
    // This is a kludge.  We should really obey type coercion rules. (hilde)
    if (isUintType(intIdxType)) { if (lo1 < 0) then lo1 = 0; }

    // We inherit the sign of the stride from this.stride.
    var newStride: strType = this.stride;
    var lcm: strType = abs(this.stride);
    var (g, x): 2*strType = (lcm, 0:strType);

    if this.stride != other.stride && this.stride != -other.stride {

      const (tg, tx) = chpl__extendedEuclid(st1, st2);
      (g, x) = (tg.safeCast(strType), tx.safeCast(strType));
      lcm = st1 / g * st2;        // The LCM of the two strides.
    // The division must be done first to prevent overflow.

      newStride = if this.stride > 0 then lcm else -lcm;
    }

    //
    // These are mixed int/uint min/max functions that return a value
    // matching the first argument's type.  They are written knowing
    // that they will be called in an intersection context and set
    // this variable if the intersection will be guaranteed to be
    // empty (e.g., if all values of one range are outside of the
    // expressible range of the other.
    //
    var emptyIntersection: bool;

    proc min(x: int, y: uint) {
      if (y > max(int)) {
        return x;
      }

      //
      // otherwise, we can safely cast y to int and take the normal
      // min()
      //
      return min(x, y: int);
    }

    proc min(x: uint, y: int) {
      //
      // if the high uint bound is bigger than int can represent,
      // this slice is guaranteed to be empty.
      //
      //
      // if the smallest value is negative, the low bound of this
      // slice will be the uint value.
      //
      if (y < 0) {
        emptyIntersection = true;
        return x;
      }

      //
      // otherwise, we can safely cast y to uint and take the normal
      // min()
      //
      return min(x, y: uint);
    }

    //
    // These two cases are the dual of the above
    //
    proc max(x: int, y: uint) {
      if (y > max(int)) {
        emptyIntersection = true;
        return x;
      }

      return max(x, y: int);
    }

    proc max(x: uint, y: int) {
      if (y < 0) {
        return x;
      }

      return max(x, y: uint);
    }

    emptyIntersection = false;
    var newlo = max(lo1, lo2):intIdxType;
    var newhi = min(hi1, hi2):intIdxType;
    if (emptyIntersection) {
      newlo = 1;
      newhi = 0;
    }

    var result = new range(idxType,
                           computeBoundedType(this, other),
                           this.stridable | other.stridable,
                           chpl_intToIdx(newlo),
                           chpl_intToIdx(newhi),
                           newStride,
                           chpl_intToIdx(0),
                           !ambig && (this.aligned || other.aligned));

    if result.stridable {
      var al1 = (chpl__idxToInt(this.alignment) % st1:intIdxType):int;
      var al2 = (chpl__idxToInt(other.alignment) % st2:other.intIdxType):int;

      if (al2 - al1) % g != 0 then
      {
        // empty intersection, return degenerate result
        if boundsChecking && !isBoundedRange(result) then
          HaltWrappers.boundsCheckHalt("could not represent range slice - it needs to be empty, but the slice type is not bounded");
        result._low = 1:intIdxType;
        result._high = 0:intIdxType;
        result._alignment = if this.stride > 0 then 1:intIdxType else 0:intIdxType;
        // _alignment == _low, so it won't print.
      }
      else
      { // non-empty intersection

        // x and/or the diff may negative, even with a uint source range.
        var offset = (al2 - al1) * x;
        // offset is in the range [-(lcm-1), lcm-1]
        if offset < 0 then offset += lcm;

        // Now offset can be safely cast to intIdxType.
        result._alignment = al1:intIdxType + offset:intIdxType * st1:intIdxType / g:intIdxType;
      }
    }

    return result;
  }


  //////////////////////////////////////////////////////////////////////////////////
  // syntax functions for counted ranges
  //

  // Creates a new range from an existing range, but with the specified number of
  // elements.
  // The new range has the same stride and alignment as the existing range.
  // If the argument n is positive, the new range contains
  // the first n elements in the existing range.
  // If the argument n is negative, the new range contains
  // the last abs(n) elements in the existing range.

  proc chpl_count_help(r:range(?), i)
    where r.boundedType == BoundedRangeType.boundedNone
  {
    compilerError("count operator is not defined for unbounded ranges");
  }

  proc chpl_count_help(r, count: integral) {
    if boundsChecking && r.isAmbiguous() then
      boundsCheckHalt("count -- Cannot count off elements from a range which is ambiguously aligned.");

    type resultType = r.intIdxType;
    type strType = chpl__rangeStrideType(resultType);

    proc absSameType(str) {
      if (r.stride < 0) {
        return (-r.stride):resultType;
      } else {
        return r.stride:resultType;
      }
    }

    if (count == 0) {
      if (r.hasLowBound()) {
        return new range(idxType = r.idxType,
                         boundedType = BoundedRangeType.bounded,
                         stridable = r.stridable,
                         _low = chpl__intToIdx(r.idxType, r._low),
                         _high = chpl__intToIdx(r.idxType, r._low - absSameType(r.stride)),
                         _stride = r.stride,
                         _alignment = chpl__intToIdx(r.idxType, r._alignment),
                         _aligned = r.aligned);
      } else if (r.hasHighBound()) {
        return new range(idxType = r.idxType,
                         boundedType = BoundedRangeType.bounded,
                         stridable = r.stridable,
                         _low = chpl__intToIdx(r.idxType, r._high + absSameType(r.stride)),
                         _high = chpl__intToIdx(r.idxType, r._high),
                         _stride = r.stride,
                         _alignment = chpl__intToIdx(r.idxType, r._alignment),
                         _aligned = r.aligned);
      } else {
        halt("Internal error: Unexpected case in chpl_count_help");
      }
    }

    if boundsChecking && !r.hasFirst() && count > 0 then
      boundsCheckHalt("With a positive count, the range must have a first index.");
    if boundsChecking && !r.hasLast()  && count < 0 then
      boundsCheckHalt("With a negative count, the range must have a last index.");
    if boundsChecking && r.boundedType == BoundedRangeType.bounded &&
      abs(count:chpl__maxIntTypeSameSign(count.type)):uint(64) > r.size:uint(64) then {
      boundsCheckHalt("bounded range is too small to access " + abs(count):string + " elements");
    }

    //
    // BLC: I'm not particularly proud of this, but it was the only
    // way I could figure to keep count.chpl working given that the
    // # operator no longer returns a range of idxType corresponding
    // to the sum of the idxType and count type.
    //
    proc chpl__computeTypeForCountMath(type t1, type t2) type {
      if (t1 == t2) then {
        return chpl__idxTypeToIntIdxType(t1);
      } else if (numBits(t1) == 64 || numBits(t2) == 64) then {
        return int(64);
      } else {
        var x1: t1; var x2: t2;
        return (x1+x2).type;
      }
    }

    type computeType = chpl__computeTypeForCountMath(resultType, count.type);
    type signedComputeType = chpl__signedType(computeType);

    // The distance between the first and last indices.
    var diff = count : signedComputeType * r.stride : signedComputeType;

    var lo : resultType =
      if diff > 0 then r._low
      else chpl__add(r._high : computeType, (diff + 1): computeType, resultType);
    var hi : resultType =
      if diff < 0 then r._high
      else chpl__add(r._low : computeType, diff : computeType - 1, resultType);

    if r.stridable {
      if r.hasLowBound() && lo < r._low then lo = r._low;
      if r.hasHighBound() && hi > r._high then hi = r._high;
    }

    return new range(idxType = r.idxType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = r.chpl_intToIdx(lo),
                     _high = r.chpl_intToIdx(hi),
                     _stride = if r.stridable then r.stride: strType else none,
                     _alignment = if r.stridable then r.alignment else none,
                     _aligned = if r.stridable then r.aligned else none);
  }

  proc #(r:range(?i), count:chpl__rangeStrideType(i)) {
    return chpl_count_help(r, count);
  }

  proc #(r:range(?i), count:chpl__rangeUnsignedType(i)) {
    return chpl_count_help(r, count);
  }

  proc #(r: range(?i), count) {
    compilerError("can't apply '#' to a range with idxType ",
                  i:string, " using a count of type ",
                  count.type:string);
    return r;
  }

  // This function checks if a bounded iterator will overflow. This is basic
  // signed/unsigned overflow checking for the last index + stride. Either
  // returns whether overflow will occur or not, or halts with an error
  // message.
  proc chpl_checkIfRangeIterWillOverflow(type idxType, low, high, stride, first=low,
      last=high, shouldHalt=true) {
    // iterator won't execute at all so it can't overflow
    if (low > high) {
      return false;
    }

    var willOverFlow = false;
    if (isIntType(idxType)) {
      if (last > 0 && stride > 0) {
        if (stride > (max(idxType) - last)) {
          willOverFlow = true;
        }
      } else if (last < 0 && stride < 0) {
        if (stride < (min(idxType) - last)) {
          willOverFlow = true;
        }
      }
    }
    else if (isUintType(idxType)) {
      if (stride > 0) {
          if (last + stride:idxType < last) {
            willOverFlow = true;
          }
        } else if (stride < 0) {
          if (last + stride:idxType > last) {
            willOverFlow = true;
          }
        }
    }
    else {
      compilerError("Iterator overflow checking is only supported ",
                    "for integral types");
    }

    if willOverFlow && shouldHalt {
      HaltWrappers.boundsCheckHalt("Iteration over a bounded range may be incorrect due to overflow.");
    }
    return willOverFlow;
  }

  pragma "no doc"
  proc range.checkIfIterWillOverflow(shouldHalt=true) {
    return chpl_checkIfRangeIterWillOverflow(this.intIdxType, this._low, this._high,
        this.stride, this.firstAsInt, this.lastAsInt, shouldHalt);
  }


  //################################################################################
  //# Direct range iterators that take low, high and stride as arguments. They
  //# are not iterators over ranges, but instead take the components of the range
  //# as arguments. This allows us to avoid range construction and provide
  //# optimized iterators when stride is known at compile time.
  //#

  //
  // These iterators exist so that argument coercion happens like it does for
  // chpl_build_bounded_range and the by operator. They just forward to the
  // "actual" iterators below which do not do any type checking on the
  // arguments. They are only intended to be used for bounded ranges. There
  // must be versions for the cross product of 'chpl_build_bounded_range' and
  // the 'by' operator. The low and high types must be the same, and the stride
  // can be the same sized signed or unsigned version of low/high
  //


  // cases for when stride is a non-param int (don't want to deal with finding
  // chpl__diffMod and the likes, just create a non-anonymous range to iterate
  // over.)
  iter chpl_direct_range_iter(low: int(?w), high: int(w), stride: int(w)) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), stride: int(w)) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_range_iter(low: enum, high: enum,
                              stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_range_iter(low: bool, high: bool, stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }



  // cases for when stride is a param int (underlying iter can figure out sign
  // of stride.) Not needed, but allows us to us "<, <=, >, >=" instead of "!="
  iter chpl_direct_range_iter(low: int(?w), high: int(w), param stride : int(w)) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), param stride: int(w)) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_range_iter(low: enum, high: enum,
                              param stride: integral) {
    if (stride == 1) {
        // Optimize for the stride == 1 case because I anticipate it'll be
        // better supported for enum ranges than the strided case (if we
        // ever support the latter)
      const r = low..high;
      for i in r do yield i;
    } else {
      // I'm guessing we won't be able to optimize the param stride case
      // for (general) enums in the short-term, so call to the non-param
      // stride case
      const r = low..high by stride;
      for i in r do yield i;
    }
  }

  iter chpl_direct_range_iter(low: bool, high: bool, param stride: integral) {
    if (stride == 1) {
        // Optimize for the stride == 1 case because I anticipate it'll be
        // better supported for enum ranges than the strided case (if we
        // ever support the latter)
      const r = low..high;
      for i in r do yield i;
    } else {
      // I'm guessing we won't be able to optimize the param stride case
      // for (general) enums in the short-term, so call to the non-param
      // stride case
      const r = low..high by stride;
      for i in r do yield i;
    }
  }


  // cases for when stride is a uint (we know the stride is must be positive)
  iter chpl_direct_range_iter(low: int(?w), high: int(w), stride: uint(w)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), stride: uint(w)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }


  // cases for when stride isn't valid
  iter chpl_direct_range_iter(low: int(?w), high: int(w), stride) {
    compilerError("can't apply 'by' to a range with idxType ",
                  int(w):string, " using a step of type ",
                  stride.type:string);
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), stride) {
    compilerError("can't apply 'by' to a range with idxType ",
                  uint(w):string, " using a step of type ",
                  stride.type:string);
  }

  // case for when low and high aren't compatible types and can't be coerced
  iter chpl_direct_range_iter(low, high, stride) {
    compilerError("Bounds of 'low..high' must be integers of compatible types.");
  }


  //
  // Direct range iterators for low bounded counted ranges (low..#count)
  //

  iter chpl_direct_counted_range_iter(low: int(?w), count: int(w)) {
    for i in chpl_direct_counted_range_iter_helper(low, count) do yield i;
  }

  iter chpl_direct_counted_range_iter(low: int(?w), count: uint(w)) {
    for i in chpl_direct_counted_range_iter_helper(low, count) do yield i;
  }

  iter chpl_direct_counted_range_iter(low: uint(?w), count: int(w)) {
    for i in chpl_direct_counted_range_iter_helper(low, count) do yield i;
  }

  iter chpl_direct_counted_range_iter(low: uint(?w), count: uint(w)) {
    for i in chpl_direct_counted_range_iter_helper(low, count) do yield i;
  }

  iter chpl_direct_counted_range_iter(low: enum, count:int(?w)) {
    const r = low..;
    for i in r#count do yield i;
  }

  iter chpl_direct_counted_range_iter(low: enum, count:uint(?w)) {
    const r = low..;
    for i in r#count do yield i;
  }

  iter chpl_direct_counted_range_iter(low: bool, count: int(?w)) {
    const r = low..;
    for i in r#count do yield i;
  }

  iter chpl_direct_counted_range_iter(low: bool, count: uint(?w)) {
    const r = low..;
    for i in r#count do yield i;
  }

  iter chpl_direct_counted_range_iter(low: integral, count) {
    compilerError("can't apply '#' to a range with idxType ",
                  low.type:string, " using a count of type ",
                  count.type:string);
  }

  iter chpl_direct_counted_range_iter(low, count) {
    compilerError("Bound of 'low..' must be an integer");
  }

  // The "actual" counted range iter. Turn the bounds of a low bounded counted
  // range into the bounds of a fully bounded non-strided range. `low..#count`
  // becomes `low..(low + (count - 1))`. Needs to check for negative counts,
  // and for zero counts iterates over a degenerate `1..0`.
  iter chpl_direct_counted_range_iter_helper(low, count) {
    if boundsChecking && isIntType(count.type) && count < 0 then
      HaltWrappers.boundsCheckHalt("With a negative count, the range must have a last index.");

    // The casts in the 'then' clause are seemingly unnecessary, but
    // avoid C compile-time warnings when 'low' is min(int)
    const (start, end) = if count == 0 then (low, (low:uint - 1):low.type)
                                       else (low, low + (count:low.type - 1));

    for i in chpl_direct_param_stride_range_iter(start, end, 1) do yield i;
  }


  // These are the "actual" direct range iterators. Note that they do not do
  // any checks on the arguments, and rely on the above functions/expert user
  // to check/coerce types (i.e. they assume args are of legal types, low/high
  // are the same same type, stride is valid, etc.)
  iter chpl_direct_pos_stride_range_iter(low: ?t, high, stride) {
    if (useOptimizedRangeIterators) {
      chpl_range_check_stride(stride, t);

      if boundsChecking then
        chpl_checkIfRangeIterWillOverflow(t, low, high, stride);

      var i: t;
      while __primitive("C for loop",
                        __primitive( "=", i, low),
                        __primitive("<=", i, high),
                        __primitive("+=", i, stride:t)) {
        yield i;
      }
    } else {
      for i in (low..high by stride).generalIterator() do yield i;
    }
  }

  iter chpl_direct_param_stride_range_iter(low: ?t, high, param stride) {
    if (useOptimizedRangeIterators) {
      chpl_range_check_stride(stride, t);

      var i: t;
      if (stride > 0) {
        if boundsChecking then
          chpl_checkIfRangeIterWillOverflow(t, low, high, stride);

        while __primitive("C for loop",
                          __primitive( "=", i, low),
                          __primitive("<=", i, high),
                          __primitive("+=", i, stride:t)) {
          yield i;
        }
      } else if (stride < 0) {
        if boundsChecking then
          chpl_checkIfRangeIterWillOverflow(t, low, high, stride, high, low);

        while __primitive("C for loop",
                          __primitive( "=", i, high),
                          __primitive(">=", i, low),
                          __primitive("+=", i, stride:t)) {
          yield i;
        }
      }
    } else {
      for i in (low..high by stride).generalIterator() do yield i;
    }
  }


  //################################################################################
  //# Serial Iterators
  //#

  // An unbounded range iterator (for all strides)
  pragma "no doc"
  iter range.these() where boundedType != BoundedRangeType.bounded {

    if boundedType == BoundedRangeType.boundedNone then
      compilerError("iteration over a range with no bounds");

    if boundsChecking {
      if ! this.hasFirst() then
        HaltWrappers.boundsCheckHalt("iteration over range that has no first index");

      if this.isAmbiguous() then
        HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
    }

    // This iterator could be split into different cases depending on the
    // stride like the bounded iterators. However, all that gets you is the
    // ability to use low/alignedLow over first. The additional code isn't
    // worth it just for that. In the other cases it allowed us to specialize
    // the test relational operator, which is important
    var i: intIdxType;
    const start = chpl__idxToInt(this.first);
    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      true,
                      __primitive("+=", i, stride: intIdxType)) {
      yield chpl_intToIdx(i);
    }
  }

  // A bounded and strided range iterator
  pragma "no doc"
  iter range.these()
    where boundedType == BoundedRangeType.bounded && stridable == true {
    if (useOptimizedRangeIterators) {
      if boundsChecking {
        checkIfIterWillOverflow();
        if this.isAmbiguous() then
          HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
      }

      // must use first/last since we have no knowledge of stride
      // must check if low > high (something like 10..1) because of the !=
      // relational operator. Such ranges are supposed to iterate 0 times
      var i: intIdxType;
      const start = this.firstAsInt;
      const end: intIdxType = if this.low > this.high then start else this.lastAsInt + stride: intIdxType;
      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("!=", i, end),
                        __primitive("+=", i, stride: intIdxType)) {
        yield chpl_intToIdx(i);
      }
    } else {
      for i in this.generalIterator() do yield i;
    }
  }

  // A bounded and non-strided (stride = 1) range iterator
  pragma "no doc"
  iter range.these()
    where boundedType == BoundedRangeType.bounded && stridable == false {
    if (useOptimizedRangeIterators) {
      if boundsChecking then checkIfIterWillOverflow();

      // don't need to check if isAmbiguous since stride is one

      // can use low/high instead of first/last since stride is one
      var i: intIdxType;
      const start = this._low;
      const end = this._high;

      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("<=", i, end),
                        __primitive("+=", i, stride: intIdxType)) {
        yield chpl_intToIdx(i);
      }
    } else {
      for i in this.generalIterator() do yield i;
    }
  }

  //################################################################################
  //# General, Unoptimized Serial Iterator
  //#

  // The bounded iterators are all optimized versions that can't handle
  // iterating over ranges like max(int)-10..max(int). This iterator is
  // designed to be able to iterate over any range, though it will be much
  // slower. It will be slower because it breaks the singleLoopIterator
  // optimization and because it has additional control flow in it. In this
  // form it can still be inlined, but that only helps for non-zippered
  // iterators. An alternate method would be to calculate the number of
  // iterations that will occur (possibly using size()) and have the number
  // of iterations drive the loop and use a separate variable to track the
  // value to yield. This would mean you couldn't express maximal ranges for
  // int(64) and uint(64) but it's hard to see a case where those could ever be
  // desired.
  pragma "no doc"
  iter range.generalIterator() {
    if boundsChecking && this.isAmbiguous() then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    var i: intIdxType;
    const start = this.first;
    const end = if this.low > this.high then start else this.last;

    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive(">=", high, low),  // execute at least once?
                      __primitive("+=", i, stride: intIdxType)) {
      yield i;
      if i == end then break;
    }
  }

  //################################################################################
  //# Parallel Iterators
  //#

  pragma "no doc"
  iter range.these(param tag: iterKind) where tag == iterKind.standalone &&
                                              !localeModelHasSublocales
  {
    if ! isBoundedRange(this) {
      compilerError("parallel iteration is not supported over unbounded ranges");
    }
    if boundsChecking && this.isAmbiguous() {
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
    }
    if debugChapelRange {
      chpl_debug_writeln("*** In range standalone iterator:");
    }

    const len = this.size;
    const numChunks = if __primitive("task_get_serial") then
                      1 else _computeNumChunks(len);

    if debugChapelRange {
      chpl_debug_writeln("*** RI: length=", len, " numChunks=", numChunks);
    }

    if numChunks <= 1 {
      for i in this {
        yield i;
      }
    } else {
      coforall chunk in 0..#numChunks {
        if stridable {
          // TODO: find a way to avoid this densify/undensify for strided
          // ranges, perhaps by adding knowledge of alignment to _computeBlock
          // or using an aligned range
          const (lo, hi) = _computeBlock(len, numChunks, chunk, len-1);
          const mylen = hi - (lo-1);
          var low = orderToIndex(lo);
          var high = chpl_intToIdx(chpl__idxToInt(low):strType + stride * (mylen - 1):strType);
          if stride < 0 then low <=> high;
          for i in low..high by stride {
            yield i;
          }
        } else {
          const (lo, hi) = _computeBlock(len, numChunks, chunk, this._high, this._low, this._low);
          for i in lo..hi {
            yield chpl_intToIdx(i);
          }
        }
      }
    }
  }

  pragma "no doc"
  iter range.these(param tag: iterKind) where tag == iterKind.leader
  {
    if ! isBoundedRange(this) then
      compilerError("parallel iteration is not supported over unbounded ranges");

    if boundsChecking && this.isAmbiguous() then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    if debugChapelRange then
      chpl_debug_writeln("*** In range leader:"); // ", this);
    const numSublocs = here.getChildCount();

    if localeModelHasSublocales && numSublocs != 0 {
      const len = this.size;
      const tasksPerLocale = dataParTasksPerLocale;
      const ignoreRunning = dataParIgnoreRunningTasks;
      const minIndicesPerTask = dataParMinGranularity;
      var dptpl = if tasksPerLocale==0 then here.maxTaskPar
                  else tasksPerLocale;
      if !ignoreRunning {
        const otherTasks = here.runningTasks() - 1; // don't include self
        dptpl = if otherTasks < dptpl then (dptpl-otherTasks):int else 1;
      }

      // Make sure we don't use more sublocales than the numbers of
      // tasksPerLocale requested
      const numSublocTasks = min(numSublocs, dptpl);
      // For serial tasks, we will only have a single chunk
      const numChunks =  if __primitive("task_get_serial") then
                         1 else _computeNumChunks(numSublocTasks,
                                                  ignoreRunning=true,
                                                  minIndicesPerTask,
                                                  len);
      if debugDataParNuma {
        chpl_debug_writeln("### numSublocs = ", numSublocs, "\n",
                           "### numTasksPerSubloc = ", numSublocTasks, "\n",
                           "### ignoreRunning = ", ignoreRunning, "\n",
                           "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                           "### numChunks = ", numChunks);
      }

      if numChunks == 1 {
        yield (0..len-1,);
      } else {
        coforall chunk in 0..#numChunks {
          local do on here.getChild(chunk) {
            if debugDataParNuma {
              if chunk!=chpl_getSubloc() then
                chpl_debug_writeln("*** ERROR: ON WRONG SUBLOC (should be ",
                                   chunk, ", on ", chpl_getSubloc(), ") ***");
            }
            const (lo,hi) = _computeBlock(len, numChunks, chunk, len-1);
            const locRange = lo..hi;
            const locLen = locRange.size;
            // Divide the locale's tasks approximately evenly
            // among the sublocales
            const numSublocTasks = (if chunk < dptpl % numChunks
                                    then dptpl / numChunks + 1
                                    else dptpl / numChunks);
            const numTasks = _computeNumChunks(numSublocTasks,
                                               ignoreRunning=true,
                                               minIndicesPerTask,
                                               locLen);
            coforall core in 0..#numTasks {
              const (low, high) = _computeBlock(locLen, numTasks, core, hi, lo, lo);
              if debugDataParNuma {
                chpl_debug_writeln("### chunk = ", chunk, "  core = ", core, "  ",
                                   "locRange = ", locRange, "  coreRange = ", low..high);
              }
              yield (low..high,);
            }
          }
        }
      }

    } else {
      var v = this.size;
      const numChunks = if __primitive("task_get_serial") then
                        1 else _computeNumChunks(v);

      if debugChapelRange
      {
        chpl_debug_writeln("*** RI: length=", v, " numChunks=", numChunks);
        chpl_debug_writeln("*** RI: Using ", numChunks, " chunk(s)");
      }

      if numChunks == 1 then
        yield (0..v-1,);
      else
      {
        coforall chunk in 0..#numChunks
        {
          const (lo,hi) = _computeBlock(v, numChunks, chunk, v-1);
          if debugChapelRange then
            chpl_debug_writeln("*** RI: tuple = ", (lo..hi,));
          yield (lo..hi,);
        }
      }
    }
  }

  pragma "no doc"
  iter range.these(param tag: iterKind, followThis) where tag == iterKind.follower
  {
    if boundsChecking && this.isAmbiguous() then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    if boundedType == BoundedRangeType.boundedNone then
      compilerError("iteration over a range with no bounds");
    if ! stridable && boundedType == BoundedRangeType.boundedHigh then
      compilerError("iteration over a range with no first index");

    if followThis.size != 1 then
      compilerError("iteration over a range with multi-dimensional iterator");

    if debugChapelRange then
      chpl_debug_writeln("In range follower code: Following ", followThis);

    var myFollowThis = followThis(1);

    if debugChapelRange then
      chpl_debug_writeln("Range = ", myFollowThis);

    if boundsChecking && ! this.hasFirst() {
      if this.isEmpty() {
        if ! myFollowThis.isEmpty() then
          HaltWrappers.boundsCheckHalt("size mismatch in zippered iteration");
      } else {
        HaltWrappers.boundsCheckHalt("iteration over a range with no first index");
      }
    }
    if boundsChecking && ! myFollowThis.hasFirst() {
      if ! (!myFollowThis.isAmbiguous() && myFollowThis.isEmpty()) then
        HaltWrappers.boundsCheckHalt("zippered iteration over a range with no first index");
    }

    if (isBoundedRange(myFollowThis) && !myFollowThis.stridable) ||
       myFollowThis.hasLast()
    {
      const flwlen = myFollowThis.size;
      if boundsChecking && this.hasLast() {
        // this check is for typechecking only
        if isBoundedRange(this) {
          if this.size < flwlen then
            HaltWrappers.boundsCheckHalt("zippered iteration over a range with too few indices");
        } else
          assert(false, "hasFirst && hasLast do not imply isBoundedRange");
      }
      if this.stridable || myFollowThis.stridable {
        var r = chpl_intToIdx(1)..chpl_intToIdx(0) by 1:chpl__rangeStrideType(intIdxType);

        if flwlen != 0 {
          const stride = this.stride * myFollowThis.stride;
          var low = this.orderToIndex(myFollowThis.first);
          var high = chpl_intToIdx(chpl__idxToInt(low):strType + stride * (flwlen - 1):strType);
          assert(high == this.orderToIndex(myFollowThis.last));

          if stride < 0 then low <=> high;
          r = low .. high by stride:strType;
        }

        if debugChapelRange then
          chpl_debug_writeln("Expanded range = ",r);

        for i in r do
          yield i;

      } else {
        var r = chpl__intToIdx(idxType,1)..chpl__intToIdx(idxType,0);

        if flwlen != 0 {
          const low = this.orderToIndex(myFollowThis.first);
          const high = chpl_intToIdx(chpl__idxToInt(low): strType + (flwlen - 1):strType);
          assert(high == this.orderToIndex(myFollowThis.last));
          r = low .. high;
        }

        if debugChapelRange then
          chpl_debug_writeln("Expanded range = ",r);

        for i in r do
          yield i;
      }
    }
    else // ! myFollowThis.hasLast()
    {
      // WARNING: this case has not been tested
      if boundsChecking && this.hasLast() then
        HaltWrappers.zipLengthHalt("zippered iteration where a bounded range follows an unbounded iterator");

      const first  = this.orderToIndex(myFollowThis.first);
      const stride = this.stride * myFollowThis.stride;

      if stride > 0
      {
        const r = first .. by stride:strType;
        if debugChapelRange then
          chpl_debug_writeln("Expanded range = ",r);

        for i in r do
          yield i;
      }
      else
      {
        const r = .. first by stride:strType;
        if debugChapelRange then
          chpl_debug_writeln("Expanded range = ",r);

        for i in r do
          yield i;
      }
    } // if myFollowThis.hasLast()
  }


  //################################################################################
  //# Utilities
  //#

  proc _cast(type t, x: range(?)) where t == string {
    var ret: string;

    if x.hasLowBound() then
      ret += x.low:string;
    ret += "..";
    if x.hasHighBound() then
      ret += x.high:string;
    if x.stride != 1 then
      ret += " by " + x.stride:string;

    var alignCheckRange = x;
    alignCheckRange.normalizeAlignment();

    // Write out the alignment only if it differs from natural alignment.
    // We take alignment modulo the stride for consistency.
    if !(alignCheckRange.isNaturallyAligned()) then
      ret += " align " + chpl__mod(chpl__idxToInt(x.alignment), x.stride):string;
    return ret;
  }

  pragma "no doc"
  proc ref range.normalizeAlignment()
  {
    if stridable && !aligned {
      _alignment =
        if isBoundedRange(this) then
          (if stride > 0 then _low else _high)
        else if this.boundedType == BoundedRangeType.boundedLow then
          _low
        else if this.boundedType == BoundedRangeType.boundedHigh then
          _high
        else
          0;
      // could verify that we succeeded:
      //assert(isNaturallyAligned());
    }
  }


  //################################################################################
  //# Internal helper functions.
  //#
  pragma "no doc"
  inline proc range.chpl__unTranslate(i: intIdxType)
    return this - i;

  pragma "no doc"
  inline proc range.chpl__unTranslate(i)
  {
    if isIntType(i.type) then
      return this - i;
    else
      return this + abs(i);
  }

  // Determine if a strided range has a definite alignment.
  proc chpl__hasAlignment(r : range(?))
  {
    if r.hasLowBound() && r.stride >= 2 then return true;
    if r.hasHighBound() && r.stride <= -2 then return true;
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////
  // TODO: hilde
  // These functions should be migrated to a more global location.

  //
  // Return the number in the range 0 <= result < b that is congruent to a (mod b)
  //
  proc chpl__mod(dividend:integral, modulus:integral)
  {
    const m = abs(modulus).safeCast(dividend.type);

    var tmp = dividend % m;
    if isInt(dividend) then
      if tmp < 0 then tmp += m;

    return tmp;
  }


  // Performs safe subtraction in modulo space.
  // "Safe" means that none of the calculations yields a result outside of
  // [0, abs(mod)].
  // It is assumed that abs(mod) can be represented in the type of the minuend.
  // We currently assume that the built-in modulo operator always returns an
  // integer in the range [0, mod-1].
  //
  proc chpl__diffMod(minuend : integral,
                     subtrahend : integral,
                     modulus : integral) : minuend.type
    where minuend.type == subtrahend.type
  {
    const m = abs(modulus).safeCast(minuend.type);

    var minMod = chpl__mod(minuend, m);
    var subMod = chpl__mod(subtrahend, m);

    return if minMod < subMod
      then m  - (subMod - minMod)
      else minMod - subMod;
  }

  proc chpl__diffMod(minuend : integral,
                     subtrahend : integral,
                     modulus : integral)
  {
    compilerError("chpl__diffMod -- Operand types must match.");
  }

  // Add two numbers together, and peg them to the min or max representable value
  // if there is overflow.
  // We might wish to add dialable run-time warning messages.
  proc chpl__add(a: ?t, b: t, type resultType)
  {
    if !isIntegralType(t) then
      compilerError("Values must be of integral type.");

    if a > 0 && b > 0 && b > max(t) - a then return max(resultType);
    if a < 0 && b < 0 && b < min(t) - a then return min(resultType);

    // If the result is unsigned, check for a negative result and peg
    // the result to 0 if the sum is going to be negative.
    if isUintType(resultType) {
      if ((a < 0 && b > 0 && (a == min(t) || abs(a) > abs(b))) ||
          (a > 0 && b < 0 && (b == min(t) || abs(b) > abs(a)))) then
        return 0:resultType;
    }

    return (a + b):resultType;
  }

  // Get the simple expression 'start + stride*count' to typecheck.
  // Use example: low + i:intIdxType * stride.
  // Use explicit conversions, no other additional runtime work.
  proc chpl__addRangeStrides(start, stride, count): start.type {
    proc convert(a,b) param
      return ( a.type == int(64) && b.type == uint(64) ) ||
             ( a.type == uint(64) && b.type == int(64) );

    proc mul(a,b) return if convert(a,b) then a:int(64) * b:int(64) else a * b;
    proc add(a,b) return if convert(a,b) then a:int(64) + b:int(64) else a + b;

    return add(start, mul(stride, count)) :start.type;
  }

  // Returns (gcd(u, v), x) where x is set such that
  // u*x + v*y = gcd(u, v) assuming u and v are non-negative.
  //
  // source: Knuth Volume 2 --- Section 4.5.2
  //
  proc chpl__extendedEuclidHelper(u, v)
  {
    var zero: u.type = 0;
    var one: u.type = 1;

    var U = (one, zero, u);
    var V = (zero, one, v);

    while V(3) != 0 {
      // This is a workaround for a bug.
      // The previous version was:
      //(U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));
      var oldU = U;
      var q = U(3)/V(3);
      U = V;
      V = oldU - V * (q, q, q);
    }

    return (U(3), U(1));
  }

  inline proc chpl__extendedEuclid(u:int(32), v:int(32))
  { return chpl__extendedEuclidHelper(u,v); }

  inline proc chpl__extendedEuclid(u:int(64), v:int(64))
  { return chpl__extendedEuclidHelper(u,v); }

  private proc chpl__rangeIdxTypeError(type idxType) {
    compilerError("ranges don't support '", idxType:string, "' as their idxType");
  }

  private proc chpl__rangeStrideType(type idxType) type {
    if isIntegralType(idxType) {
      return chpl__signedType(idxType);
    } else if isEnumType(idxType) || isBoolType(idxType) {
      return int;
    } else {
      chpl__rangeIdxTypeError(idxType);
    }
  }

  private proc chpl__rangeUnsignedType(type idxType) type {
    if isIntegralType(idxType) {
      return chpl__unsignedType(idxType);
    } else if isEnumType(idxType) || isBoolType(idxType) {
      return uint;
    } else {
      chpl__rangeIdxTypeError(idxType);
    }
  }

  pragma "no doc"
  proc chpl__idxTypeToIntIdxType(type idxType) type {
    if isBoolType(idxType) {
      return int;
    } else if isEnumType(idxType) {
      // Most range/array code currently relies on being able to store
      // empty ranges like 1..0.  If an enum only defines a single
      // symbol, we can't create such a range, so print the following
      // error message to avoid going off the rails.
      if idxType.size < 2 then
        compilerError("ranges are not currently supported for enums with fewer than two values");
      return int;
    } else {
      return idxType;
    }
  }

  // convenience method for converting integers to index types in
  // order to make use of range.idxType.  Note that this method uses
  // a single underscore where the standalone versions use double
  // underscores.  Reason: otherwise, the calls in range.init() try
  // to call the method version, which isn't currently legal.
  pragma "no doc"
  inline proc range.chpl_intToIdx(i) {
    return chpl__intToIdx(this.idxType, i);
  }

  // helper routines for converting integers to indices and back again
  inline proc chpl__intToIdx(type idxType: integral, i: integral) {
    if (i.type == idxType) then
      return i;
    else
      return i: idxType;
  }

  inline proc chpl__intToIdx(type idxType: integral, param i: integral) {
    if (i.type == idxType) then
      return i;
    else
      return i: idxType;
  }

  inline proc chpl__intToIdx(type idxType: enum, i: integral) {
    return chpl__orderToEnum(i, idxType);
  }

  inline proc chpl__intToIdx(type idxType, i: integral) where isBoolType(idxType) {
    return i: bool;
  }

  inline proc chpl__intToIdx(type idxType, param i: integral) param where isBoolType(idxType) {
    return i: bool;
  }

  inline proc chpl__intToIdx(type idxType, i: nothing) {
    return none;
  }

  inline proc chpl__idxToInt(i: integral) {
    return i;
  }

  inline proc chpl__idxToInt(param i: integral) param {
    return i;
  }

  inline proc chpl__idxToInt(i: enum) {
    return chpl__enumToOrder(i);
  }

  inline proc chpl__idxToInt(i: bool) {
    return i: int;
  }

  inline proc chpl__idxToInt(param i: bool) param {
    return i: int;
  }
}
