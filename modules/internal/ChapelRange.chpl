/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

// ChapelRange.chpl
//
module ChapelRange {

  use ChapelBase, HaltWrappers;

  use AutoMath, DSIUtil;

  private use ChapelDebugPrint only chpl_debug_writeln;

  // Turns on range iterator debugging.
  pragma "no doc"
  config param debugChapelRange = false;

  pragma "no doc"
  config param useOptimizedRangeIterators = true;

  /* Compile with ``-snewSliceRule`` to switch to using the new slicing rule
     and to turn off the deprecation warning for using the old rule.
     When slicing with a range with a negative stride, the old rule
     preserves the direction of the original range or domain/array dimension
     whereas the new rule reverses such direction. */
  config param newSliceRule = false;

  // This enum is documented directly in the spec to fit the presentation flow.
  @chpldoc.nodoc
  enum boundKind { both, low, high, neither };

  /*
    The ``BoundedRangeType`` enum is used to specify the types of bounds a
    range is required to have.

    * ``bounded`` - The range has finite low and high bounds.
    * ``boundedLow`` - The range starts at a given low bound, but conceptually goes up to infinity.
    * ``boundedHigh`` - The range conceptually starts at negative infinity and ends at a given high bound.
    * ``boundedNone`` - The range conceptually runs from negative infinity to infinity.

    This is currently documented manually in the spec because it fit into
    the flow of the document better.
   */
  pragma "no doc"
  @deprecated("BoundedRangeType is deprecated; please use 'boundKind' instead")
  proc BoundedRangeType type do return boundKind;
//was: enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };
//deprecated by Vass in 1.31 to implement #17126
  @deprecated("'bounded' is deprecated; please use 'boundKind.both' instead")
  proc type boundKind.bounded param do return boundKind.both;
  @deprecated("'boundedLow' is deprecated; please use 'boundKind.low' instead")
  proc type boundKind.boundedLow param do return boundKind.low;
  @deprecated("'boundedHigh' is deprecated; please use 'boundKind.high' instead")
  proc type boundKind.boundedHigh param do return boundKind.high;
  @deprecated("'boundedNone' is deprecated; please use 'boundKind.neither' instead")
  proc type boundKind.boundedNone param do return boundKind.neither;

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
    param bounds: boundKind = boundKind.both;      // lower/upper bounds
    param stridable: bool = false;                 // range can be strided

    // deprecated by Vass in 1.31 to implement #17126
    @deprecated("range.boundedType is deprecated; please use '.bounds' instead")
    proc boundedType param do return bounds;

    var _low       : chpl__idxTypeToIntIdxType(idxType);  // lower bound
    var _high      : chpl__idxTypeToIntIdxType(idxType);  // upper bound
    var _stride    : if stridable then chpl__rangeStrideType(idxType) else nothing; // signed stride
    var _alignment : if stridable then chpl__idxTypeToIntIdxType(idxType) else nothing; // alignment
    var _aligned   : if stridable then bool else nothing;

    proc strType type do  return chpl__rangeStrideType(idxType);

    proc chpl__promotionType() type {
      return idxType;
    }
  }

  /* The ``idxType`` as represented by an integer type.  When
     ``idxType`` is an enum type, this evaluates to ``int``.
     Otherwise, it evaluates to ``idxType``.

     This is no-doc'd because we currently are hoping it can
     be more of an implementation detail than a user-facing
     feature. */
  pragma "no doc"
  proc range.intIdxType type {
    return chpl__idxTypeToIntIdxType(idxType);
  }

  // Helper routines for unbounded ranges of enum or bool types
  // ----------------------------------------------------------
  //
  // Is this an idxType whose values are conceptually finite?
  //
  private proc isFiniteIdxType(type t) param {
    return isBoolType(t) || isEnumType(t);
  }

  // For a finite idxType, what is its '_low' integer?
  //
  private proc finiteIdxTypeLow(type t) param {
    if isFiniteIdxType(t) then
      return 0;
    compilerError("finiteIdxTypeLow() undefined for type '" + t:string + "'");
  }

  // For a finite idxType, what is its '_high' integer?
  //
  private proc finiteIdxTypeHigh(type t) param {
    if isBoolType(t) then
      return 1;
    if isEnumType(t) then
      return t.size-1;
    compilerError("finiteIdxTypeHigh() undefined for type '" + t:string + "'");
  }

  // When iterating over this range, what is its low bound?
  // (typically, this is just its low bound, but for unbounded ranges
  // with finite idxType, it may be implied, e.g., '..true' has an
  // implied low bound of 'false')
  //
  private inline proc lowBoundForIter(r) {
    if r.hasLowBound() then
      return r.lowBound;
    else
      return chpl__intToIdx(r.idxType, finiteIdxTypeLow(r.idxType));
  }

  // When iterating over this range, what is its low bound?
  // (typically, this is just its low bound, but for unbounded ranges
  // with finite idxType, it may be implied, e.g., 'false..' has an
  // implied high bound of 'true')
  //
  private inline proc highBoundForIter(r) {
    if r.hasHighBound() then
      return r.highBound;
    else
      return chpl__intToIdx(r.idxType, finiteIdxTypeHigh(r.idxType));
  }

  private proc hasLowBoundForIter(r) param {
    return r.hasLowBound() || isFiniteIdxType(r.idxType);
  }

  private proc hasHighBoundForIter(r) param {
    return r.hasHighBound() || isFiniteIdxType(r.idxType);
  }

  //################################################################################
  //# Initializers
  //#

  // This is the initializer for a low..high bounded range
  //
  pragma "no doc"
  proc range.init(type idxType, low: idxType, high: idxType) {
    this.idxType = idxType;
    this.bounds = boundKind.both;
    this._low = chpl__idxToInt(low);
    this._high = chpl__idxToInt(high);
  }

  // This is the initializer for a low.. unbounded range
  //
  pragma "no doc"
  proc range.init(low: ?t) {
    this.idxType = t;
    this.bounds = boundKind.low;
    this._low = chpl__idxToInt(low);
    this.complete();
    if isFiniteIdxType(idxType) {
      this._high = finiteIdxTypeHigh(idxType);
    }
  }

  // This is the initializer for a ..high unbounded range
  //
  pragma "no doc"
  proc range.init(high: ?t) {
    this.idxType = t;
    this.bounds = boundKind.high;
    this._high = chpl__idxToInt(high);
    this.complete();
    if isFiniteIdxType(idxType) {
      this._low = finiteIdxTypeLow(idxType);
    }
  }

  // This is the initializer for a .. unbounded range
  //
  pragma "no doc"
  proc range.init() {
    this.idxType = int;
    this.bounds = boundKind.neither;
    this.complete();
    if isFiniteIdxType(idxType) {
      this._low = finiteIdxTypeLow(idxType);
      this._high = finiteIdxTypeHigh(idxType);
    }
  }

  // This is an initializer for defining a default range value
  //
  pragma "no doc"
  proc range.init(type idxType,
                  param bounds: boundKind,
                  param stridable: bool) {
    this.init(idxType, bounds, stridable,
              _low = chpl__defaultLowBound(idxType, bounds),
              _high = chpl__defaultHighBound(idxType, bounds),
              _stride = 1:chpl__rangeStrideType(idxType),
              _alignment = 0:chpl__idxTypeToIntIdxType(idxType),
              _aligned = true);
  }

  // This is an initializer for defining a range value in terms of its
  // internal field values
  //
  pragma "no doc"
  proc range.init(type idxType,
                  param bounds : boundKind,
                  param stridable : bool,
                  _low,
                  _high,
                  _stride,
                  _alignment,
                  _aligned = true) {
    this.idxType     = idxType;
    this.bounds      = bounds;
    this.stridable   = stridable;
    this._low        = _low;
    this._high       = _high;
    this.complete();
    if (stridable) {
      this._stride    = _stride;
      this._alignment = _alignment;
      this._aligned   = _aligned;
    }
  }

  // This is the range copy initializer
  //
  pragma "no doc"
  proc range.init=(other : range(?i,?b,?s)) {
    type idxType      = if this.type.idxType     == ? then i else this.type.idxType;
    param bounds      = if this.type.bounds      == ? then b else this.type.bounds;
    param stridable   = if this.type.stridable   == ? then s else this.type.stridable;

    if bounds != b {
      compilerError("range(bounds=" + this.type.bounds:string + ") cannot be initialized from range(bounds=" + b:string + ")");
    }

    if !stridable && s then
      compilerError("cannot initialize a non-stridable range from a stridable range");

    param isEnumBool = isFiniteIdxType(idxType);
    type bt = other.intIdxType;
    const low  = if isEnumBool && !other.hasLowBound()
                 then finiteIdxTypeLow(idxType):bt
                 else other._low;
    const high = if isEnumBool && !other.hasHighBound()
                 then finiteIdxTypeHigh(idxType):bt
                 else other._high;

    const str = if stridable && s then other.stride else 1:chpl__rangeStrideType(idxType);

    var alignment = chpl__idxToInt(other.alignment);
    if isEnumBool && !other.aligned {
      if (str > 0) {
        alignment = finiteIdxTypeLow(idxType);;
      } else {
        alignment = finiteIdxTypeHigh(idxType);
      }
    }

    this.init(idxType, bounds, stridable,
              low, high,
              str,
              alignment,
              other.aligned || isEnumBool);
  }

  /////////////////////////////////
  // for debugging
  pragma "no doc"
  proc range.displayRepresentation(msg: string = ""): void {
    chpl_debug_writeln(msg, "(", idxType:string, ",", bounds, ",", stridable,
                       " : ", low, ",", high, ",", stride, ",",
                       if aligned then alignment:string else "?", ")");
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range builders:  used by the parser to create literal ranges
  //

  private
  proc computeParamRangeIndexType(param low, param high) type {
    // if either type is int, and the int value fits in the other type,
    // return the other type
    if low.type == int &&
       min(high.type) <= low && low <= max(high.type) {
      return high.type;
    } else if high.type == int &&
              min(low.type) <= high && high <= max(low.type) {
      return low.type;
    } else {
      // otherwise, use the type that '+' would produce.
      return (low+high).type;
    }
  }
  private proc isValidRangeIdxType(type t) param {
    return isIntegralType(t) || isEnumType(t) || isBoolType(t);
  }

  // Range builders for fully bounded ranges
  proc chpl_build_bounded_range(param low: integral, param high: integral) {
    type idxType = computeParamRangeIndexType(low, high);
    return new range(idxType, low=low, high=high);
  }
  proc chpl_build_bounded_range(low: int(?w), high: int(w)) do
    return new range(int(w), low=low, high=high);
  proc chpl_build_bounded_range(low: uint(?w), high: uint(w)) do
    return new range(uint(w), low=low, high=high);
  proc chpl_build_bounded_range(low: enum, high: enum) {
    if (low.type != high.type) then
      compilerError("ranges of enums must use a single enum type");
    return new range(low.type, low=low, high=high);
  }
  proc chpl_build_bounded_range(low: bool, high: bool) do
    return new range(bool, low=low, high=high);

  proc chpl_build_bounded_range(low, high)
  where !(isValidRangeIdxType(low.type) && isValidRangeIdxType(high.type)) {
    if (low.type == high.type) then
      compilerError("Ranges defined using bounds of type '" + low.type:string + "' are not currently supported");
    else
      compilerError("Ranges defined using bounds of type '" + low.type:string + ".." + high.type:string + "' are not currently supported");
  }

  proc chpl__nudgeLowBound(low) {
    return chpl__intToIdx(low.type, chpl__idxToInt(low) + 1);
  }
  proc chpl__nudgeLowBound(param low) param {
    return chpl__intToIdx(low.type, chpl__idxToInt(low) + 1);
  }
  proc chpl__nudgeHighBound(high) {
    return chpl__intToIdx(high.type, chpl__idxToInt(high) - 1);
  }
  proc chpl__nudgeHighBound(param high) param {
    return chpl__intToIdx(high.type, chpl__idxToInt(high) - 1);
  }

  // Range builders for low bounded ranges
  proc chpl_build_low_bounded_range(low: integral) do
    return new range(low=low);
  proc chpl_build_low_bounded_range(low: enum) do
    return new range(low=low);
  proc chpl_build_low_bounded_range(low: bool) do
    return new range(low=low);
  proc chpl_build_low_bounded_range(low)
  where !isValidRangeIdxType(low.type) {
    compilerError("Ranges defined using bounds of type '" + low.type:string + "' are not currently supported");
  }

  // Range builders for high bounded ranges
  proc chpl_build_high_bounded_range(high: integral) do
    return new range(high=high);
  proc chpl_build_high_bounded_range(high: enum) do
    return new range(high=high);
  proc chpl_build_high_bounded_range(high: bool) do
    return new range(high=high);
  proc chpl_build_high_bounded_range(high)
  where !isValidRangeIdxType(high.type) {
    compilerError("Ranges defined using bounds of type '" + high.type:string + "' are not currently supported");
  }

  // Range builder for unbounded ranges
  proc chpl_build_unbounded_range() do
    return new range();

  /////////////////////////////////////////////////////////////////////
  // Helper functions for ranges in param loops (and maybe param ranges
  // later)
  //
  // Necessary for coercion support
  /////////////////////////////////////////////////////////////////////
  proc chpl_compute_low_param_loop_bound(param low: integral,
                                         param high: integral) param {
    type t = computeParamRangeIndexType(low, high);
    return low:t;
  }

  proc chpl_compute_high_param_loop_bound(param low: integral,
                                          param high: integral) param {
    type t = computeParamRangeIndexType(low, high);
    return high:t;
  }

  proc chpl_compute_low_param_loop_bound(param low: enum,
                                         param high: low.type) param {
    return low;
  }

  proc chpl_compute_high_param_loop_bound(param low: enum,
                                          param high: low.type) param {
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

  proc chpl_compute_low_param_loop_bound(param low, param high) param
  where !(isValidRangeIdxType(low.type) && isValidRangeIdxType(high.type)) {
    if (low.type == high.type) then
      compilerError("param for-loops defined using bounds of type '" + low.type:string + "' are not currently supported");
    else
      compilerError("param for-loops defined using bounds of type '" + low.type:string + ".." + high.type:string + "' are not currently supported");
  }

  proc chpl_compute_low_param_loop_bound(low, high) {
    compilerError("param for-loops must be defined over a bounded param range");
  }

  proc chpl_compute_count_param_loop(param count: integral) param {
    return count;
  }

  proc chpl_compute_count_param_loop(count) {
    compilerError("in a param for-loop, the count operator requires a param integral value");
  }

  proc chpl_low_bound_count_for_param_loop(param high: integral, param count: integral) param {
    if count > 0 {
      compilerError("count operators with positive count require the range to have a low bound");
    }
    else if count == 0 {
      return high + 1;
    }
    else {
      return high + count + 1;
    }
  }

  pragma "last resort"
  proc chpl_low_bound_count_for_param_loop(high, count) {
    chpl_build_high_bounded_range(high);  // generate normal error, if possible
    // otherwise, fall back to this one:
    compilerError("can't apply '#' to a range with idxType ",
                  high.type:string, " using a count of type ",
                  count.type:string);
  }

  proc chpl_high_bound_count_for_param_loop(param low: integral, param count: integral) param {
    if count < 0 {
      compilerError("count operators with negative count require the range to have a high bound");
    }
    else if count == 0 {
      return low - 1;
    }
    else {
      return low + count - 1;
    }
  }

  pragma "last resort"
  proc chpl_high_bound_count_for_param_loop(low, count) {
    chpl_build_low_bounded_range(low);  // generate normal error, if possible
    // otherwise, fall back to this one:
    compilerError("can't apply '#' to a range with idxType ",
                  low.type:string, " using a count of type ",
                  count.type:string);
  }

  proc chpl_bounded_count_for_param_loop_low(param low: integral, param high: integral, param count: integral) param {
    param abs_count = if count < 0 then -count else count;
    param size = high - low + 1;
    if size < abs_count {
      compilerError("Count of ", abs_count:string, " is too small for range of size ", size:string);
    }
    else if count == 0 {
      return high + 1;
    }
    else if count < 0 {
      return high + count + 1;
    }
    else {
      return low;
    }
  }

  pragma "last resort"
  proc chpl_bounded_count_for_param_loop_low(low, high, count) {
    const r = chpl_build_bounded_range(low, high);  // generate normal error, if possible
    // otherwise, fall back to this one:
    compilerError("can't apply '#' to a range with idxType ",
                  r.idxType:string, " using a count of type ",
                  count.type:string);
  }

  proc chpl_bounded_count_for_param_loop_high(param low: integral, param high: integral, param count: integral) param {
    if count == 0 {
      return low - 1;
    }
    else if count < 0 {
      return high;
    }
    else {
      return low + count - 1;
    }
  }

  pragma "last resort"
  proc chpl_bounded_count_for_param_loop_high(low, high, count) {
    const r = chpl_build_bounded_range(low, high);  // generate normal error, if possible
    // otherwise, fall back to this one:
    compilerError("can't apply '#' to a range with idxType ",
                  r.idxType:string, " using a count of type ",
                  count.type:string);
  }


  /* Returns the range's stride. */
  inline proc range.stride where stridable do  return _stride;
  pragma "no doc"
  proc range.stride param where !stridable do return 1 : strType;

  /* Returns the range's alignment. */
  inline proc range.alignment where !hasParamAlignment() do
    return chpl_intToIdx(if stridable then _alignment else 0);
  @chpldoc.nodoc proc range.alignment param where hasParamAlignment() do return
    if isEnum(idxType) then chpl__orderToEnum(0, idxType) else 0: idxType;

  @chpldoc.nodoc proc range.hasParamAlignment() param do return
    !stridable && (isIntegral(idxType) || isEnum(idxType) || isBool(idxType));

  /* Returns ``true`` if the range's alignment is unambiguous,
     ``false`` otherwise. */
  inline proc range.aligned where stridable do return _aligned;
  @chpldoc.nodoc proc range.aligned param where !stridable do return true;


  //################################################################################
  //# Predicates
  //#

  // isBoundedRange(r) = true if 'r' is a (fully) bounded range
  pragma "no doc"
  proc isBoundedRange(r)           param do
    return false;
  /* Returns ``true`` if argument ``r`` is a fully bounded range,
     ``false`` otherwise. */
  proc isBoundedRange(r: range(?)) param do
    return isBoundedRange(r.bounds);

  pragma "no doc"
  proc isBoundedRange(param B: boundKind) param do
    return B == boundKind.both;

  /* Returns ``true`` if this range is bounded, ``false`` otherwise. */
  proc range.isBounded() param do
    return bounds == boundKind.both;

  /* Returns ``true`` if this range's low bound is *not* -:math:`\infty`,
     and ``false`` otherwise. */
  proc range.hasLowBound() param do
    return bounds == boundKind.both || bounds == boundKind.low;

  /* Returns the range's low bound. If the range does not have a low
     bound (e.g., ``..10``), the behavior is undefined.  See also
     :proc:`range.hasLowBound`. */
  inline proc range.lowBound: idxType {
    if !hasLowBound() {
      compilerError("can't query the low bound of a range without one");
    }
    return chpl_intToIdx(_low);
  }

  // More cases of hasPositiveStride() will become params
  // once we switch to an enum strideKind.
  // Need design review to make this publicly available.
  /* Returns whether this range's stride is positive,
     as a `param` when possible. */
 inline proc range.chpl_hasPositiveStride() where stridable do return _stride>0;
 proc range.chpl_hasPositiveStride() param where !stridable do return true;

  // Compile-time checks
  proc range.chpl_staticHasPositiveStride() param do return !stridable;
  proc range.chpl_staticHasNegativeStride() param do return false;

  /* Returns the range's aligned low bound. If this bound is
     undefined (e.g., ``..10 by -2``), the behavior is undefined.

     Example:

     .. code-block:: chapel

       var r = 1..10 by -2;
       writeln(r.low);

     produces the output

     .. code-block:: printoutput

       2

  */
  inline proc range.low: idxType {
    if !hasLowBound() {
      compilerError("can't query the low bound of a range without one");
    }
    return chpl_intToIdx(this.alignedLowAsInt);
  }


  /* Return the range's aligned low bound.  Note that this is a
     synonym for :proc:`range.low`.
  */
  @deprecated(notes="'.alignedLow' is deprecated; please use '.low' instead")
  inline proc range.alignedLow: idxType {
    if !hasLowBound() {
      compilerError("can't query the low bound of a range without one");
    }
    return this.low;
  }

  pragma "no doc"
  inline proc range.alignedLowAsInt {
    if isAmbiguous() {
      halt("Can't query the aligned bounds of an ambiguously aligned range");
    }
    if !stridable then
      return _low;
    else
      // Adjust _low upward by the difference between _alignment and _low.
      return helpAlignLow(_low, _alignment, stride);
  }

  pragma "no doc"
  inline proc range.chpl_alignedLowAsIntForIter {
    if stridable && !hasLowBound() && isFiniteIndexType() {
      return helpAlignLow(chpl__idxToInt(lowBoundForIter(this)), _alignment, stride);
    } else {
      return alignedLowAsInt;
    }
  }

  private inline proc helpAlignLow(l, a, s) {
    // Adjust _low upward by the difference between _alignment and _low.
    return l + chpl__diffMod(a, l, s);
  }


  /* Returns ``true`` if this range's high bound is *not* :math:`\infty`,
     and ``false`` otherwise. */
  proc range.hasHighBound() param do
    return bounds == boundKind.both || bounds == boundKind.high;

  /* Return the range's high bound. If the range does not have a high
     bound (e.g., ``1..``), the behavior is undefined.  See also
     :proc:`range.hasHighBound`.
  */
  inline proc range.highBound: idxType {
    if !hasHighBound() {
      compilerError("can't query the high bound of a range without one");
    }
    if chpl__singleValIdxType(idxType) {
      if _low > _high { // avoid circularity of calling .size which calls .high
        warning("This range is empty and has a single-value idxType, so its high bound isn't trustworthy");
        return chpl_intToIdx(_low);
      }
    }
    return chpl_intToIdx(_high);
  }


  /* Returns the range's aligned high bound. If the aligned high bound is
     undefined (e.g., ``1.. by 2``), the behavior is undefined.

     Example:

     .. code-block:: chapel

       var r = 1..10 by 2;
       writeln(r.high);

     produces the output

     .. code-block:: printoutput

       9
  */
  inline proc range.high: idxType {
    if !hasHighBound() {
      compilerError("can't query the high bound of a range without one");
    }
    if chpl__singleValIdxType(idxType) {
      if _low > _high { // avoid circularity of calling .size which calls .high
        warning("This range is empty and has a single-value idxType, so its high bound isn't trustworthy");
        return this.low;
      }
    }
    if chpl__singleValIdxType(idxType) {
      if size == 0 {
        warning("This range is empty and has a single-value idxType, so its high bound isn't trustworthy");
        return chpl_intToIdx(_low);
      }
    }
    return chpl_intToIdx(this.alignedHighAsInt);
  }


  /* Return the range's aligned high bound.  Note that this is a
     synonym for :proc:`range.high`.
  */
  @deprecated(notes="'.alignedHigh' is deprecated; please use '.high' instead")
  inline proc range.alignedHigh: idxType {
    if !hasHighBound() {
      compilerError("can't query the high bound of a range without one");
    }
    return this.high;
  }

  pragma "no doc"
  inline proc range.alignedHighAsInt {
    if isAmbiguous() {
      halt("Can't query the aligned bounds of an ambiguously aligned range");
    }
    if ! stridable then
      return _high;
    else
      return helpAlignHigh(_high, _alignment, stride);
  }

  pragma "no doc"
  inline proc range.chpl_alignedHighAsIntForIter {
    if stridable && !hasHighBound() && isFiniteIdxType(idxType) {
      return helpAlignHigh(chpl__idxToInt(highBoundForIter(this)), _alignment, stride);
    } else {
      return alignedHighAsInt;
    }
  }

  private inline proc helpAlignHigh(h, a, s) {
    // Adjust _high downward by the difference between _high and _alignment.
    return h - chpl__diffMod(h, a, s);
  }

  /* Returns ``true`` if this range is naturally aligned, ``false``
     otherwise. */
  proc range.isNaturallyAligned()
    where stridable && bounds != boundKind.neither
  do if  bounds == boundKind.both {
    // If the stride is positive, we must be aligned on the low bound.
    if stride > 0 then return this.alignedLowAsInt == _low;
    // If the stride is negative, we must be aligned on the high bound.
    if stride < 0 then return this.alignedHighAsInt == _high;
    // stride == 0: ???
    return false;
  } else if bounds == boundKind.low {
    return stride > 0 && this.alignedLowAsInt == _low;
  } else if bounds == boundKind.high {
    return stride < 0 && this.alignedHighAsInt == _high;
  }

  @chpldoc.nodoc proc range.isNaturallyAligned() param
    where !stridable || bounds == boundKind.neither
    do return !stridable;

  /* Returns ``true`` if the range is ambiguously aligned, ``false``
     otherwise. */
  proc range.isAmbiguous() param where !stridable do
    return false;

  pragma "no doc"
  proc range.isAmbiguous()       where stridable do
    return !aligned && (stride > 1 || stride < -1);

  private inline proc hasAmbiguousAlignmentForIter(r) param where !r.stridable || isFiniteIdxType(r.idxType) {
    return false;
  }

  private inline proc hasAmbiguousAlignmentForIter(r) {
    return r.isAmbiguous();
  }

  /* Returns ``true`` if the sequence represented by the range is
     empty and ``false`` otherwise.  If the range is ambiguous, the
     behavior is undefined.
   */
  inline proc range.isEmpty() {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then
        return true;
    }
    if boundsChecking && isAmbiguous() then
      HaltWrappers.boundsCheckHalt("isEmpty() is invoked on an ambiguously-aligned range");
    else
      return isBoundedRange(this) && this.alignedLowAsInt > this.alignedHighAsInt;
  }

  /* Returns the number of values represented by this range as an integer.

     If the size exceeds ``max(int)``, this procedure will halt when
     bounds checks are on and have undefined behavior when they are not.

     If the represented sequence is infinite or undefined, an error is
     generated.
   */
  proc range.size: int {
    return this.sizeAs(int);
  }

  /* Returns the number of elements in this range as the specified
     integer type.

     If the size exceeds the maximal value of that type, this
     procedure will halt when bounds checks are on and have undefined
     behavior when they are not.

     If the represented sequence is infinite or undefined, an error is
     generated.
   */
  proc range.sizeAs(type t: integral): t {
    if ! isBoundedRange(this) then
      compilerError("'size' is not defined on unbounded ranges");

    if chpl__singleValIdxType(idxType) {
      if _low > _high then return 0;
    }

    return sizeAsHelp(t);
  }

  pragma "no doc"
  proc range.sizeAsHelp(type t: integral,
                        al = this.alignedLowAsInt,
                        ah = this.alignedHighAsInt): t {
    // assumes alignedHigh/alignedLow always work, even for an empty range
    const ah = this.alignedHighAsInt,
          al = this.alignedLowAsInt;
    if al > ah then return 0;
    const s = abs(this.stride): uint;
    param width = numBits(al.type);
    // Perform subtraction to compute the range's length using
    // `uint(width)` in order to get guaranteed wraparound semantics
    // in C (and arguably Chapel) before upcasting to a full uint.
    const lenAsUint = ((ah:uint(width) - al:uint(width)):uint / s + 1);
    if boundsChecking && (lenAsUint == 0 || lenAsUint > max(t)) then {
      HaltWrappers.boundsCheckHalt("range.size exceeds max("+t:string+") for: '" + this:string + "'");
    }
    return lenAsUint: t;
  }

  pragma "no doc"
  proc range.chpl_sizeAsForIter(type t: integral): t {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return 0;
    }

    if !isBoundedRange(this) && isFiniteIdxType(idxType) {
      return sizeAsHelp(t,
                        this.chpl_alignedLowAsIntForIter,
                        this.chpl_alignedHighAsIntForIter);
    } else {
      return sizeAs(t);
    }
  }

  /* Returns ``true`` if the range has a first index, ``false``
     otherwise.  Note that in the event that the range is stridable
     and at least partially bounded, the return value will not
     be a ``param``.
  */
  proc range.hasFirst() param where !stridable && !hasHighBound() do
    return hasLowBound();

  pragma "no doc"
  proc range.hasFirst() param where stridable && bounds == boundKind.neither do
  return false;

  pragma "no doc"
  inline proc range.hasFirst() do
    return if isAmbiguous() || isEmpty() then false else
      if stride > 0 then hasLowBound() else hasHighBound();

  /* Returns the first value in the sequence the range represents.  If
     the range has no first index, the behavior is undefined.  See
     also :proc:`range.hasFirst`. */
  inline proc range.first {
    return chpl_intToIdx(this.firstAsInt);
  }

  pragma "no doc"
  inline proc range.firstAsInt {
    if ! stridable then return _low;
    else return if _stride > 0 then this.alignedLowAsInt else this.alignedHighAsInt;
  }

  pragma "no doc"
  inline proc range.chpl_firstAsIntForIter {
    if this.bounds == boundKind.both {
      return this.firstAsInt;
    } else {
      if ! stridable {
        return chpl__idxToInt(lowBoundForIter(this));
      } else {
        if _stride > 0 {
          if hasLowBound() {
            return helpAlignLow(chpl__idxToInt(lowBoundForIter(this)), _alignment, _stride);
          } else {
            return chpl__idxToInt(lowBoundForIter(this));
          }
        } else {
          if hasHighBound() {
            return helpAlignHigh(chpl__idxToInt(highBoundForIter(this)), _alignment, _stride);
          } else {
            return chpl__idxToInt(highBoundForIter(this));
          }
        }
      }
    }
  }

  /* Returns ``true`` if the range has a last index, ``false`` otherwise.
     Note that in the event that the range is stridable and at least
     partially bounded, the return value will not be a ``param``.
  */
  proc range.hasLast() param where !stridable && !hasLowBound() do
    return hasHighBound();

  pragma "no doc"
  proc range.hasLast() param where stridable && bounds == boundKind.neither do
  return false;

  pragma "no doc"
  inline proc range.hasLast() do
    return if isAmbiguous() || isEmpty() then false else
      if stride > 0 then hasHighBound() else hasLowBound();

  /* Returns the last value in the sequence the range represents.  If
     the range has no last index, the behavior is undefined.  See also
     :proc:`range.hasLast`.
  */
  inline proc range.last {
    return chpl_intToIdx(this.lastAsInt);
  }

  pragma "no doc"
  inline proc range.lastAsInt {
    if ! stridable then return _high;
    else return if stride > 0 then this.alignedHighAsInt else this.alignedLowAsInt;
  }

  pragma "no doc"
  inline proc range.chpl_lastAsIntForIter {
    if bounds == boundKind.both {
      return this.lastAsInt;
    } else {
      if ! stridable {
        return chpl__idxToInt(highBoundForIter(this));
      } else if _stride > 0 {
        return helpAlignHigh(chpl__idxToInt(highBoundForIter(this)), _alignment, _stride);
      } else {
        return helpAlignLow(chpl__idxToInt(lowBoundForIter(this)), _alignment, _stride);
      }
    }
  }

  // is this type one for which a range of this type will have a change in
  // '.size' behavior?
  //
  proc chpl_idxTypeSizeChange(type t) param {
    return (isIntegralType(t) && t != int);
  }

  /* Returns ``true`` if the range's represented sequence contains
     ``ind``, ``false`` otherwise.  It is an error to invoke ``contains``
     if the represented sequence is not defined. */
  inline proc range.contains(ind: idxType)
  {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return false;
    }

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

  /* Returns ``true`` if the range ``other`` is contained within this one,
     ``false`` otherwise.
   */
  inline proc range.contains(other: range(?))
  {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return other.size == 0;
    }

    if this.isAmbiguous() || other.isAmbiguous() then return false;

    if this.isBounded() && this.sizeAs(uint) == 0 then
      return other.isBounded() && other.sizeAs(uint) == 0;

    var slice = this.chpl_slice(other, forceNewRule=true);

    // Slicing reversed the direction of 'other' if this.stride < 0.
    // Switch it back before comparing.
    if ! this.chpl_hasPositiveStride() {
      if ! slice.stridable then // o.w. need to negate differently
        compilerError("unimplemented case in range.contains()");

      slice._stride = -slice._stride;
    }

    return other == slice;
  }

  pragma "no doc"
  operator ==(r1: range(?), r2: range(?)) param
    where r1.bounds != r2.bounds do
  return false;

  pragma "no doc"
  operator ==(r1: range(?), r2: range(?)): bool
    where r1.bounds == r2.bounds
  {
    // An ambiguous ranges cannot equal an unambiguous one
    //  even if all their parameters match.
    if r1.isAmbiguous() != r2.isAmbiguous() then return false;

    // As a special case, two ambiguous ranges compare equal
    // if their representations are identical.
    if r1.isAmbiguous() then return chpl_ident(r1, r2);

    if isBoundedRange(r1) {

      // gotta have a special case for length 0 or 1
      const len = r1.sizeAs(uint), l2 = r2.sizeAs(uint);
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
        if r1.low != r2.low then return false;

      if r1.hasHighBound() then
        if r1.high != r2.high then return false;

      return true;
    }
  }

  pragma "no doc"
  operator !=(r1: range(?), r2: range(?)) do  return !(r1 == r2);

  pragma "no doc"
  operator <(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both ||r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  pragma "no doc"
  operator >(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both || r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  pragma "no doc"
  operator <=(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both || r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  pragma "no doc"
  operator >=(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both || r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  proc chpl_ident(r1: range(?), r2: range(?))
    where r1.idxType   == r2.idxType  &&
          r1.bounds    == r2.bounds   &&
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

  proc chpl_ident(r1: range(?), r2: range(?)) param {
    return false;
  }

  // If the parameters don't match, then the two ranges cannot be identical.
  pragma "no doc"
  @deprecated(notes="ident() on ranges is deprecated; please let us know if this is problematic for you")
  proc ident(r1: range(?), r2: range(?)) param {
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range Casts
  //
/* Cast a range to another range type. If the old type is stridable and the
   new type is not stridable, ensure at runtime that the old stride was 1.
 */
pragma "no doc"
proc range.safeCast(type t: range(?)) {
  var tmp: t;

  if tmp.bounds != this.bounds {
    compilerError("cannot cast range from boundKind.",
                  this.bounds:string, " to boundKind.", tmp.bounds:string);
  }

  if tmp.stridable {
    tmp._stride = this.stride.safeCast(tmp.strType);
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
operator :(r: range(?), type t: range(?)) {
  var tmp: t;

  if tmp.bounds != r.bounds {
    compilerError("cannot cast range from boundKind.",
                  r.bounds:string, " to boundKind.", tmp.bounds:string);
  }

  if tmp.stridable {
    tmp._stride = r.stride: tmp._stride.type;
    tmp._alignment = r.alignment: tmp.intIdxType;
    tmp._aligned = r.aligned;
  }

  tmp._low = (if r.hasLowBound() then r.lowBound else r._low): tmp.intIdxType;
  tmp._high = (if r.hasHighBound() then r.highBound else r._high): tmp.intIdxType;
  return tmp;
}


  //////////////////////////////////////////////////////////////////////////////////
  // Bounds checking
  //

  /* Returns ``true`` if ``other`` lies entirely within this range and
     ``false`` otherwise.  Returns ``false`` if either range is
     ambiguously aligned.
   */
  inline proc range.boundsCheck(other: range(?e,?b,?s))
    where b == boundKind.neither
  {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return false;
    }

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
                          idxType, boundKind.both,
                          s || this.stridable,
                          if other.hasLowBound() then other._low else _low,
                          if other.hasHighBound() then other._high else _high,
                          other.stride,
                          chpl__idxToInt(other.alignment),
                          true);

    return (boundedOther.sizeAs(uint) == 0) || contains(boundedOther);
  }
  /* Returns ``true`` if ``other`` is contained in this range and ``false``
     otherwise. */
  inline proc range.boundsCheck(other: idxType) do
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
     Returns an integer representing the zero-based ordinal value of
     ``ind`` within the range's sequence of values if it is a member
     of the sequence.  Otherwise, returns -1.  It is an error to
     invoke ``indexOrder`` if the represented sequence is not defined
     or the range does not have a first index.

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

      if isBoundedRange(this) && ord >= this.sizeAs(uint) then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on an integer " +
            ord:string + " that is larger than the range's number of indices " + this.sizeAs(uint):string);
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
  /* Returns a range with elements shifted from this range by ``offset``.
     Formally, the range's low bound, high bound, and alignment values
     will be shifted while the stride value will be preserved.  If the
     range's alignment is ambiguous, the behavior is undefined.

     Example:

     .. code-block:: chapel

       0..9.translate(1) == 1..10
       0..9.translate(2) == 2..11
       0..9.translate(-1) == -1..8
       0..9.translate(-2) == -2..7
   */
  inline proc range.translate(offset: integral) do
    return this + offset;

  pragma "no doc"
  pragma "last resort"
  inline proc range.translate(i)
  {
    compilerError("offsets must be of integral type");
  }

  pragma "no doc"
  inline proc range.translate(offset: integral) where chpl__singleValIdxType(idxType) {
    compilerError("can't apply '.translate()' to a range whose 'idxType' only has one value");

  }


    // Returns an expanded range, or a contracted range if offset < 0.
  // The existing absolute alignment is preserved.
  pragma "no doc"
  proc range.expand(offset: integral) where bounds != boundKind.both
  {
    compilerError("expand() is not supported on unbounded ranges");
  }
  /* Returns a range expanded by ``offset`` elements from each end.  If
     ``offset`` is negative, the range will be contracted.  The stride
     and alignment of the original range are preserved.

     Example:

     .. code-block:: chapel

       0..9.expand(1)  == -1..10
       0..9.expand(2)  == -2..11
       0..9.expand(-1) == 1..8
       0..9.expand(-2) == 2..7


     Formally, for a range represented by the tuple :math:`(l,h,s,a)`,
     the result is :math:`(l-i,h+i,s,a)`.  If the operand range is
     ambiguously aligned, then so is the resulting range.
  */
  proc range.expand(offset: integral)
  {
    const i = offset.safeCast(chpl__signedType(intIdxType));
    return new range(idxType, bounds, stridable,
                     _low-i,
                     _high+i,
                     stride, _alignment, aligned);
  }

  pragma "no doc"
  proc range.expand(offset: integral) where chpl__singleValIdxType(idxType)
  {
    compilerError("can't apply '.expand()' to a range whose 'idxType' only has one value");
  }




  // Compute the alignment of the range returned by this.interior()
  // and this.exterior(). Keep it private.
  pragma "no doc"
  inline proc range._effAlmt()       where stridable do return _alignment;

  pragma "no doc"
  proc range._effAlmt() where !stridable do return 0;

  // Return an interior portion of this range.
  pragma "no doc"
  pragma "last resort"
  proc range.interior(offset: integral) where bounds != boundKind.both
  {
    compilerError("interior is not supported on unbounded ranges");
  }

  // TODO: hilde
  // Set _aligned to true only if stridable.
  /* Returns a range with ``offset`` elements from the interior portion of this
     range. If ``offset`` is positive, take elements from the high end, and if
     ``offset`` is negative, take elements from the low end.

     Example:

     .. code-block:: chapel

       0..9.interior(1)  == 9..9
       0..9.interior(2)  == 8..9
       0..9.interior(-1) == 0..0
       0..9.interior(-2) == 0..1

     Formally, given a range denoted by the tuple :math:`(l,h,s,a)`,

     -  if :math:`i < 0`, the result is :math:`(l,l-(i-1),s,a)`,

     -  if :math:`i > 0`, the result is :math:`(h-(i-1),h,s,a)`, and

     -  if :math:`i = 0`, the result is :math:`(l,h,s,a)`.

     This differs from the behavior of the count operator, in that
     ``interior()`` preserves the alignment, and it uses the low and
     high bounds rather than ``first`` and ``last`` to establish the
     bounds of the resulting range. If the operand range is
     ambiguously aligned, then so is the resulting range.
   */
  proc range.interior(offset: integral)
  {
    if boundsChecking then
      if abs(offset):uint > this.sizeAs(uint) then
        HaltWrappers.boundsCheckHalt("can't compute the interior " + offset:string + " elements of a range with size " + this.sizeAs(uint):string);

    const i = (abs(offset)).safeCast(intIdxType);
    if offset < 0 then
      return new range(idxType, bounds, stridable,
                       _low, _low - 1 + i, stride,
                       _effAlmt(), aligned);
    if offset > 0 then
      return new range(idxType, bounds, stridable,
                       _high + 1 - i, _high, stride,
                       _effAlmt(), aligned);
    // if i == 0 then
    return new range(idxType, bounds, stridable,
                     _low, _high, stride, _effAlmt(), aligned);
  }

  pragma "no doc"
  pragma "last resort"
  proc range.exterior(offset: integral) where bounds != boundKind.both
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

     Formally, given a range denoted by the tuple :math:`(l,h,s,a)`,

     -  if :math:`i < 0`, the result is :math:`(l+i,l-1,s,a)`,

     -  if :math:`i > 0`, the result is :math:`(h+1,h+i,s,a)`, and

     -  if :math:`i = 0`, the result is :math:`(l,h,s,a)`.

     If the operand range is ambiguously aligned, then so is the resulting
     range.
   */
  proc range.exterior(offset: integral)
  {
    const i = (abs(offset)).safeCast(intIdxType);
    if offset < 0 then
      return new range(idxType, bounds, stridable,
                       _low - i,
                       _low - 1,
                       stride, _effAlmt(), aligned);
    if offset > 0 then
      return new range(idxType, bounds, stridable,
                       _high + 1,
                       _high + i,
                       stride, _effAlmt(), aligned);
    // if i == 0 then
    return new range(idxType, bounds, stridable,
                     _low, _high, stride, _effAlmt(), aligned);
  }

  pragma "no doc"
  proc range.exterior(offset: integral) where chpl__singleValIdxType(idxType)
  {
    compilerError("can't apply '.exterior()' to a range whose 'idxType' only has one value");
  }

  //################################################################################
  //# Syntax Functions
  //# These are special functions known to the compiler parser.
  //#

  // Assignment
  pragma "no doc"
  inline operator =(ref r1: range(stridable=?s1), r2: range(stridable=?s2))
  {
    if r1.bounds != r2.bounds then
      compilerError("type mismatch in assignment of ranges with different bounds parameters");

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
  pragma "no doc"
  inline operator +(r: range(?e, ?b, ?s), offset: integral)
  {
    const i = offset:r.intIdxType;
    type strType = chpl__rangeStrideType(e);

    return new range(e, b, s,
                     r._low + i,
                     r._high + i,
                     r.stride : strType,
                     chpl__idxToInt(r.alignment)+i,
                     r.aligned);
  }

  // TODO can this be removed?
  pragma "no doc"
  inline operator +=(ref r: range(?e, ?b, ?s), offset: integral)
  {
    r = r + offset;
  }

  pragma "no doc"
  inline operator +(i:integral, r: range(?e,?b,?s)) do
    return r + i;

  pragma "no doc"
  inline operator -(r: range(?e,?b,?s), i: integral)
  {
    type strType = chpl__rangeStrideType(e);

    return new range(e, b, s,
                     r._low - i,
                     r._high - i,
                     r.stride : strType,
                     chpl__idxToInt(r.alignment)-i,
                     r.aligned);
  }

  // TODO can this be removed?
  pragma "no doc"
  inline operator -=(ref r: range(?e, ?b, ?s), offset: integral)
  {
    r = r - offset;
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
    const lw = r._low,
          hh = r._high,
          st: r.strType = r.stride * step:r.strType;

    const (ald, alt) =
      if r.isAmbiguous() then
        if r.stridable then (false, r._alignment)
                       else (false, 0:r.intIdxType)
      else
        // we could talk about aligned bounds
        if      hasLowBoundForIter(r)  && st > 0 then (true, r.chpl_alignedLowAsIntForIter)
        else if hasHighBoundForIter(r) && st < 0 then (true, r.chpl_alignedHighAsIntForIter)
        else
          if r.stridable then (r.aligned, r._alignment)
                         else (false, 0:r.intIdxType);

    return new range(i, b, true,  lw, hh, st, alt, ald);
  }

  // This is the definition of the 'by' operator for ranges.
  pragma "no doc"
  inline operator by(r : range(?), step) {
    chpl_range_check_stride(step, r.idxType);
    return chpl_by_help(r, step);
  }

  // We want to warn the user at compiler time if they had an invalid param
  // stride rather than waiting until runtime.
  pragma "no doc"
  inline operator by(r : range(?), param step) {
    chpl_range_check_stride(step, r.idxType);
    return chpl_by_help(r, step:r.strType);
  }

  pragma "no doc"
  pragma "last resort"
  inline operator by(r, step) {
    compilerError("cannot apply 'by' to '", r.type:string, "'");
  }

  // This is the definition of the 'align' operator for ranges.
  // It produces a new range with the specified alignment.
  // By definition, alignment is relative to the low bound of the range.
  pragma "no doc"
  inline operator align(r : range(?i, ?b, ?s), algn: i)
  {
    // Note that aligning an unstrided range will set the field value,
    // but has no effect on the index set produced (a mod 1 == 0).
    return new range(i, b, true,
                     r._low, r._high, r.stride, chpl__idxToInt(algn), true);
  }

  pragma "no doc" pragma "last resort"
  inline operator align(r : range(?i, ?b, ?s), algn) {
    compilerError("can't align a range with idxType ", i:string,
                  " using a value of type ", algn.type:string);
  }

  pragma "no doc"
  pragma "last resort"
  inline operator align(r, algn) {
    compilerError("cannot apply 'align' to '", r.type:string, "'");
  }

  /* Returns a range whose alignment is this range's first index plus ``offset``.
     If the range has no first index, a runtime error is generated.
   */
  proc range.offset(in offset: integral)
  {
    if (chpl__singleValIdxType(idxType)) {
      compilerError("can't apply '.offset()' to a range whose 'idxType' only has one value");
    }

    var offs = offset.safeCast(intIdxType);
    if !stridable {
      if !this.hasLowBound() then
        compilerError("can't invoke 'offset' on an unstrided range with no low bound");
      else
        compilerWarning("invoking 'offset' on an unstrided range has no effect.");
      offs = 0;
    }

    if boundsChecking && !hasFirst() then
      HaltWrappers.boundsCheckHalt("invoking 'offset' on a range without a first index");

    return new range(idxType, bounds, stridable, _low, _high, stride,
                     // here's the new alignment
                     this.firstAsInt + offs, true);
  }


  // Slicing, implementing the slice semantics in #20462.
  // Return the intersection of this and other.
  pragma "no doc"
  inline proc const range.this(other: range(?))
  {
    return this.chpl_slice(other, forceNewRule=false);
  }

  proc const range.chpl_slice(other: range(?), forceNewRule: bool)
  {
    // Disallow slicing of an unaligned range, at least for now.
    if this.isAmbiguous() then
      HaltWrappers.unimplementedFeatureHalt("slicing of an unaligned range");

    // the new idxType, intIdxType, strType are inherited from `this`

    /////////// Step 1: intersect the unaligned spans ///////////

    param newBoundKind = computeBoundedType(this, other);

    // Determine the bounds of result
    proc computeBoundedType(r1, r2) param
    {
      param low = r1.hasLowBound() || r2.hasLowBound();
      param high = r1.hasHighBound() || r2.hasHighBound();
      if      low && high then return boundKind.both;
      else if low         then return boundKind.low;
      else if high        then return boundKind.high;
      else                     return boundKind.neither;
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

    //
    // These are mixed int/uint min/max functions that return a value
    // matching the first argument's type.  They are written knowing
    // that they will be called in an intersection context and set
    // this variable if the intersection will be guaranteed to be
    // empty (e.g., if all values of one range are outside of the
    // expressible range of the other.
    //
    var emptyIntersection: bool;

    proc myMin(x: int, y: uint) {
      if (y > max(int)) {
        return x;
      }

      //
      // otherwise, we can safely cast y to int and take the normal
      // min()
      //
      return min(x, y: int);
    }

    proc myMin(x: uint, y: int) {
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

    proc myMin(x: int, y: int) {
      return min(x, y);
    }
    proc myMin(x: uint, y: uint) {
      return min(x, y);
    }


    //
    // These two cases are the dual of the above
    //
    proc myMax(x: int, y: uint) {
      if (y > max(int)) {
        emptyIntersection = true;
        return x;
      }

      return max(x, y: int);
    }

    proc myMax(x: uint, y: int) {
      if (y < 0) {
        return x;
      }

      return max(x, y: uint);
    }

    proc myMax(x: int, y: int) {
      return max(x, y);
    }
    proc myMax(x: uint, y: uint) {
      return max(x, y);
    }


    emptyIntersection = false;
    var newlo = myMax(lo1, lo2):intIdxType;
    var newhi = myMin(hi1, hi2):intIdxType;

    if (emptyIntersection) {
      newlo = chpl__defaultLowBound(idxType, newBoundKind);
      newhi = chpl__defaultHighBound(idxType, newBoundKind);
    }

    /////////// Step 2: combine the strides ///////////

    // we have abs strides of args in st1, st2
    // abs(result.stride) = LCM(st1, st2)
    // sign(result.stride) = sign(this.stride) * sign(other.stride)

    param newStrideKind = this.stridable || other.stridable;
    var newStride = st1, newAbsStride = st1;
    var gcd, x: strType;

    if newStrideKind {
      if st1 == st2 {
        gcd = st1;
      } else {
        // do we need casts to  something about the types of st1, st2?
        (gcd, x) = chpl__extendedEuclid(st1, st2);
        newStride = st1 / gcd * st2;  // divide first to avoid overflow
        newAbsStride = newStride;
      }

      if newSliceRule || forceNewRule {
        // sign of resulting stride = sign of 'this' * sign of 'other'
        if  this.chpl_hasPositiveStride() && !other.chpl_hasPositiveStride() ||
           !this.chpl_hasPositiveStride() &&  other.chpl_hasPositiveStride()
        then
          newStride = -newStride;
      } else {
        // sign of resulting stride = sign of 'this'
        if ! other.chpl_hasPositiveStride() then
          warning("when slicing with a range with a negative stride, the sign of the stride of the original range or domain/array dimension is currently preserved, but will be negated in a future release; compile with -snewSliceRule to switch to this new rule and turn off this warning; while slicing ", this, " with ", other);
        if ! this.chpl_hasPositiveStride() then
          newStride = - newStride;
      }
    }

    /////////// allocate the result ///////////

    var result = new range(idxType, newBoundKind, newStrideKind,
                           newlo, newhi, newStride, 0:intIdxType, true);

    /////////// Step 3: choose the alignment ///////////

    // We require that `this` be unambiguous. The result will always be, too.

    if newStrideKind && newAbsStride > 1 {
      var al1 = (chpl__idxToInt(this.alignment) % st1:intIdxType):int;
      var al2 = (chpl__idxToInt(other.alignment) % st2:other.intIdxType):int;
      var newAlignmentIsInAl2 = false;

      if other.isAmbiguous() {
        // choice (C) in the comment from 2021-09-30 in #20462
        al2 = al1;
        if al2 < 0 then al2 += st1;

        // when st1 == st2 * (some int), there is no room for instability
        // because the resulting sequence is unique
        if st2 > gcd {
          // the rule in the comment from 2023-02-15 or -02-16 in #20462
          if result.hasLowBound() && result.chpl_hasPositiveStride() {
            if (newlo - al1) % st1 == 0 {
              al2 = ( newlo % newAbsStride ):int;
              newAlignmentIsInAl2 = true;
            }
          } else
          if result.hasHighBound() && ! result.chpl_hasPositiveStride() {
            if (newhi - al1) % st1 == 0 {
              al2 = ( newhi % newAbsStride ):int;
              newAlignmentIsInAl2 = true;
            }
          }
          if chpl_warnUnstable && ! newAlignmentIsInAl2 then
            warning("slicing '", this, "' with the unaligned range '", other,
                    "' is unstable w.r.t. the choice of alignment");
        }
      }

      const g = gcd; // the name `g` has been used in the code below
      if newAlignmentIsInAl2 then
      {
        if al2 < 0 then al2 += newAbsStride;
        result._alignment = al2: intIdxType;
      } else
      if (al2 - al1) % g != 0 then
      {
        // empty intersection, return degenerate result
        if boundsChecking && !isBoundedRange(result) then
          HaltWrappers.boundsCheckHalt("could not represent range slice - it needs to be empty, but the slice type is not bounded");
        result._low = chpl__defaultLowBound(idxType, newBoundKind);
        result._high = chpl__defaultHighBound(idxType, newBoundKind);
        result._alignment = if this.stride > 0 then 1:intIdxType else 0:intIdxType;
        // todo: what should be the alignment of an empty range?
        // _alignment == _low, so it won't print.
      }
      else
      { // non-empty intersection

        // x and/or the diff may negative, even with a uint source range.
        var offset = (al2 - al1) * x;
        // offset is in the range [-(lcm-1), lcm-1]
        // not needed: if offset < 0 then offset += lcm;

        // Now offset can be safely cast to intIdxType.
        result._alignment = al1:intIdxType + offset:intIdxType * st1:intIdxType / g:intIdxType;

        if result._alignment:int < 0 then
          result._alignment += newAbsStride;
        else if result._alignment >= newAbsStride then
          result._alignment -= newAbsStride;
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

  pragma "last resort"
  proc chpl_count_help(r:range(?), i) where r.bounds == boundKind.neither
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
                         bounds = boundKind.both,
                         stridable = r.stridable,
                         _low = r._low,
                         _high = r._low - absSameType(r.stride),
                         _stride = r.stride,
                         _alignment = r._alignment,
                         _aligned = r.aligned);
      } else if (r.hasHighBound()) {
        return new range(idxType = r.idxType,
                         bounds = boundKind.both,
                         stridable = r.stridable,
                         _low = r._high + absSameType(r.stride),
                         _high = r._high,
                         _stride = r.stride,
                         _alignment = r._alignment,
                         _aligned = r.aligned);
      } else {
        return new range(idxType = r.idxType,
                         bounds = boundKind.both,
                         stridable = r.stridable,
                         _low  = chpl__defaultLowBound(r.idxType,
                                                       boundKind.both),
                         _high = chpl__defaultHighBound(r.idxType,
                                                        boundKind.both),
                         _stride = r.stride,
                         _alignment = r._alignment,
                         _aligned = r.aligned);
      }
    }

    if boundsChecking {
      if count > 0 && !r.hasFirst() then
        boundsCheckHalt("With a positive count, the range must have a first index.");
      if count < 0 && !r.hasLast() then
        boundsCheckHalt("With a negative count, the range must have a last index.");
      if r.bounds == boundKind.both &&
        abs(count:chpl__maxIntTypeSameSign(count.type)):uint > r.sizeAs(uint) then
           boundsCheckHalt("range of size " + r.size:string +
                           " is too small for counting " + abs(count):string +
                           " elements");
    }
    //
    // BLC: I'm not particularly proud of this, but it was the only
    // way I could figure to keep count.chpl working given that the
    // # operator no longer returns a range of idxType corresponding
    // to the sum of the idxType and count type.
    //
    // MPPF: Updated it for the fact that mixed int/uint no
    // longer produce the next integer size up (but rather coerce to uint).
    //
    proc chpl__computeTypeForCountMath(type t1, type t2) type {
      type t1i = chpl__idxTypeToIntIdxType(t1);
      type t2i = chpl__idxTypeToIntIdxType(t2);
      if (t1i == t2i) then {
        return t1i;
      } else if isInt(t1i) && isInt(t2i) {
        // both int but different sizes
        return int(max(numBits(t1i), numBits(t2i)));
      } else if isUint(t1i) && isUint(t2i) {
        // both uint but different sizes
        return uint(max(numBits(t1i), numBits(t2i)));
      } else if (numBits(t1i) == 64 || numBits(t2i) == 64) then {
        // otherwise, for a mix of int/uint, use int(64) if either is 64-bit
        return int(64);
      } else if isInt(t1i) {
        // t1 is int and t2 is uint and both are smaller than 64 bit
        return int(max(numBits(t1i), 2*numBits(t2i)));
      } else {
        // t1 is int and t2 is uint and both are smaller than 64 bit
        return int(max(2*numBits(t1i), numBits(t2i)));
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
                     bounds = boundKind.both,
                     stridable = r.stridable,
                     _low = lo,
                     _high = hi,
                     _stride = if r.stridable then r.stride: strType else none,
                     _alignment = if r.stridable then r._alignment else none,
                     _aligned = r.aligned);
  }

  // TODO: Do we really want to support this?  Arkouda currently relies
  // on it, and bools don't coerce to generic 'integral' arguments so
  // dropping it broke the Arkouda build.  On one hand, it seems odd to
  // accept a boolean as a count value.  On the other hand, we permit
  // bools to coerce to ints in most cases, so this gives a similar
  // end-user experience
  pragma "no doc"
  operator #(r:range(?), count:bool) {
    return chpl_count_help(r, count:int);
  }

  pragma "no doc"
  operator #(r:range(?), count:integral) {
    return chpl_count_help(r, count);
  }

  pragma "no doc"
  pragma "last resort"
  operator #(r: range(?i), count) {
    compilerError("can't apply '#' to a range with idxType ",
                  i:string, " using a count of type ",
                  count.type:string);
  }

  pragma "no doc"
  pragma "last resort"
  operator #(r, count) {
    compilerError("cannot apply '#' to '", r.type:string, "'");
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
    if isFiniteIdxType(idxType) then
      return false;
    return chpl_checkIfRangeIterWillOverflow(this.intIdxType, this._low, this._high,
        this.stride, this.chpl_firstAsIntForIter, this.chpl_lastAsIntForIter, shouldHalt);
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

  //
  // Direct range iterators for non-strided ranges (low..high)
  // which always have stride 1.
  //

  iter chpl_direct_range_iter(param low: integral, param high: integral) {
    type idxType = computeParamRangeIndexType(low, high);
    for i in chpl_direct_param_stride_range_iter(low: idxType,
                                                 high: idxType,
                                                 1:idxType) do
      yield i;
  }

  iter chpl_direct_range_iter(low: int(?w), high: int(w)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:int(w)) do
      yield i;
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:uint(w)) do
      yield i;
  }

  iter chpl_direct_range_iter(low: enum, high: enum) {
    // Optimize for the stride == 1 case because I anticipate it'll be
    // better supported for enum ranges than the strided case (if we
    // ever support the latter)
    const r = low..high;
    for i in r do yield i;
  }

  iter chpl_direct_range_iter(low: bool, high: bool) {
    // Optimize for the stride == 1 case because I anticipate it'll be
    // better supported for enum ranges than the strided case (if we
    // ever support the latter)
    const r = low..high;
    for i in r do yield i;
  }

  // case for when low and high aren't compatible types and can't be coerced
  iter chpl_direct_range_iter(low, high)
  where !(isValidRangeIdxType(low.type) && isValidRangeIdxType(high.type)) {
    chpl_build_bounded_range(low, high);  // use general error if possible
    // otherwise, generate a more specific one (though I don't think it's
    // possible to get here)
    compilerError("Ranges defined using bounds of type '" + low.type:string + ".." + high.type:string + "' are not currently supported");
  }

  //
  // Direct range iterators for strided ranges (low..high by stride)
  //

  // cases for when stride is a non-param int (don't want to deal with finding
  // chpl__diffMod and the likes, just create a non-anonymous range to iterate
  // over.)
  iter chpl_direct_strided_range_iter(param low: integral,
                                      param high: integral,
                                      stride: integral) {
    type idxType = computeParamRangeIndexType(low, high);
    const r = low:idxType..high:idxType by stride;
    for i in r do yield i;
  }

  iter chpl_direct_strided_range_iter(low: int(?w), high: int(w),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_strided_range_iter(low: uint(?w), high: uint(w),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_strided_range_iter(low: enum, high: enum,
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_strided_range_iter(low: bool, high: bool,
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }


  // cases for when stride is a param int (underlying iter can figure out sign
  // of stride.) Not needed, but allows us to us "<, <=, >, >=" instead of "!="
  iter chpl_direct_strided_range_iter(param low: integral,
                                      param high: integral,
                                      param stride: integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_strided_range_iter(low: int(?w), high: int(w), param stride : integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_strided_range_iter(low: uint(?w), high: uint(w), param stride: integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_strided_range_iter(low: enum, high: enum,
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

  iter chpl_direct_strided_range_iter(low: bool, high: bool,
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


  // cases for when stride is a uint (we know the stride is must be positive)
  iter chpl_direct_strided_range_iter(low: int(?w), high: int(w), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(?w), high: uint(w), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }


  // cases for when stride isn't valid
  iter chpl_direct_strided_range_iter(low: int(?w), high: int(w), stride) {
    compilerError("can't apply 'by' to a range with idxType ",
                  int(w):string, " using a step of type ",
                  stride.type:string);
  }

  iter chpl_direct_strided_range_iter(low: uint(?w), high: uint(w), stride) {
    compilerError("can't apply 'by' to a range with idxType ",
                  uint(w):string, " using a step of type ",
                  stride.type:string);
  }

  // case for when low and high aren't compatible types and can't be coerced
  iter chpl_direct_strided_range_iter(low, high, stride)
  where !(isValidRangeIdxType(low.type) && isValidRangeIdxType(high.type)) {
    chpl_build_bounded_range(low, high, stride);  // use general error if possible
    // otherwise, generate a more specific one (though I don't think it's
    // possible to get here)
    if (low.type == high.type) then
      compilerError("Ranges defined using bounds of type '" + low.type:string + "' and strides of type '" + stride.type:string + "' are not currently supported");
    else
      compilerError("Ranges defined using bounds of type '" + low.type:string + ".." + high.type:string + "' and strides of type '" + stride.type:string + "' are not currently supported");
  }


  //
  // Direct range iterators for low bounded counted ranges (low..#count)
  //

  iter chpl_direct_counted_range_iter(low: int(?w), count) {
    if !isIntegral(count) && !isBool(count) {
      compilerError("can't apply '#' to a range with idxType ",
                    low.type:string, " using a count of type ",
                    count.type:string);
    }

    for i in chpl_direct_counted_range_iter_helper(low, count) do yield i;
  }

  iter chpl_direct_counted_range_iter(low: uint(?w), count) {
    if !isIntegral(count) && !isBool(count) {
      compilerError("can't apply '#' to a range with idxType ",
                    low.type:string, " using a count of type ",
                    count.type:string);
    }

    for i in chpl_direct_counted_range_iter_helper(low, count) do yield i;
  }

  iter chpl_direct_counted_range_iter(low: enum, count:integral) {
    const r = low..;
    for i in r#count do yield i;
  }

  iter chpl_direct_counted_range_iter(low: bool, count: integral) {
    const r = low..;
    for i in r#count do yield i;
  }

  iter chpl_direct_counted_range_iter(low, count)
  where !(isValidRangeIdxType(low.type) && isValidRangeIdxType(count.type)) {
    chpl_build_low_bounded_range(low);  // generate normal error, if possible
    // otherwise, fall back to this one:
    compilerError("can't apply '#' to a range with idxType ",
                  low.type:string, " using a count of type ",
                  count.type:string);
  }

  // The "actual" counted range iter. Turn the bounds of a low bounded counted
  // range into the bounds of a fully bounded non-strided range. `low..#count`
  // becomes `low..(low + (count - 1))`. Needs to check for negative counts,
  // and for zero counts iterates over a degenerate `1..0`.
  iter chpl_direct_counted_range_iter_helper(low, count) {
    if boundsChecking && isIntType(count.type) && count < 0 then
      HaltWrappers.boundsCheckHalt("With a negative count, the range must have a last index.");

    const start = low;
    // The cast to uint in the 'then' clause avoids avoids a C compile-time
    // warnings when 'low' is min(int)
    const end = if count == 0 then (low:uint - 1):low.type
                              else (low + (count:low.type - 1)):low.type;

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

  // An error overload for trying to iterate over '..'
  pragma "no doc"
  pragma "order independent yielding loops"
  iter range.these() where !hasLowBoundForIter(this) && !hasHighBoundForIter(this) {
    compilerError("iteration over a range with no bounds");
  }

  private inline proc boundsCheckUnboundedRange(r: range(?)) {
    if boundsChecking {
      if ! r.hasFirst() then
        HaltWrappers.boundsCheckHalt("iteration over range that has no first index");

      if hasAmbiguousAlignmentForIter(r) then
        HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
    }
  }

  // The serial iterator for 'lo.. [by s]' ranges
  pragma "no doc"
  pragma "order independent yielding loops"
  iter range.these() where hasLowBoundForIter(this) && !hasHighBoundForIter(this) {

    boundsCheckUnboundedRange(this);

    // This iterator could be split into different cases depending on the
    // stride like the bounded iterators. However, all that gets you is the
    // ability to use .low over .first. The additional code isn't
    // worth it just for that.
    var i: intIdxType;
    const start = chpl__idxToInt(this.first);
    const end = max(intIdxType) - stride: intIdxType;

    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive("<=", i, end),
                      __primitive("+=", i, stride: intIdxType)) {
      yield chpl_intToIdx(i);
    }
    if i > end {
      // We'd like to do the following yield, but it breaks our
      // zippering optimizations for cases like 'for i in (something,
      // 0..)', so we'll just stop an iteration early instead.  Once
      // we distinguish serial follower loops from standalone loops,
      // we could support this in the standalone case without penalty,
      // I believe.
      //
      //   yield chpl_intToIdx(i);
      if isIntegralType(idxType) then
        halt("Loop over unbounded range surpassed representable values");
    }
  }

  // The serial iterator for '..hi [by s]' ranges
  pragma "no doc"
  pragma "order independent yielding loops"
  iter range.these() where !hasLowBoundForIter(this) && hasHighBoundForIter(this) {

    boundsCheckUnboundedRange(this);

    // Apart from the computation of 'end' and the comparison used to
    // terminate the C for loop, this iterator follows the bounded-low
    // case above.  See it for additional comments.
    var i: intIdxType;
    const start = chpl__idxToInt(this.first);
    const end = min(intIdxType) - stride: intIdxType;
    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive(">=", i, end),
                      __primitive("+=", i, stride: intIdxType)) {
      yield chpl_intToIdx(i);
    }
    if i < end {
      if isIntegralType(idxType) then
        //  yield chpl_intToIdx(i);
        halt("Loop over unbounded range surpassed representable values");
    }
  }

  // A bounded and strided range iterator
  pragma "no doc"
  pragma "order independent yielding loops"
  iter range.these()
    where hasLowBoundForIter(this) && hasHighBoundForIter(this) && stridable == true {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return;
    }
    if (useOptimizedRangeIterators) {
      if boundsChecking {
        checkIfIterWillOverflow();
        if hasAmbiguousAlignmentForIter(this) then
          HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
      }

      // must use first/last since we have no knowledge of stride
      // must check if low > high (something like 10..1) because of the !=
      // relational operator. Such ranges are supposed to iterate 0 times
      var i: intIdxType;
      const start = chpl_firstAsIntForIter;
      const end: intIdxType = if this._low > this._high then start
                              else chpl_lastAsIntForIter + stride: intIdxType;
      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("!=", i, end),
                        __primitive("+=", i, stride: intIdxType)) {
        yield chpl_intToIdx(i);
      }
    } else {
      foreach i in this.generalIterator() do yield i;
    }
  }

  // A bounded and non-strided (stride = 1) range iterator
  pragma "no doc"
  pragma "order independent yielding loops"
  iter range.these()
  where hasLowBoundForIter(this) && hasHighBoundForIter(this) && stridable == false {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return;
    }
    if (useOptimizedRangeIterators) {
      if boundsChecking then checkIfIterWillOverflow();

      // don't need to check if isAmbiguous since stride is one

      // can use low/high instead of first/last since stride is one
      var i: intIdxType;
      const start = chpl__idxToInt(lowBoundForIter(this));
      const end = chpl__idxToInt(highBoundForIter(this));

      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("<=", i, end),
                        __primitive("+=", i, stride: intIdxType)) {
        yield chpl_intToIdx(i);
      }
    } else {
      foreach i in this.generalIterator() do yield i;
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
  pragma "order independent yielding loops"
  iter range.generalIterator() {
    if boundsChecking && hasAmbiguousAlignmentForIter(this) then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    var i: intIdxType;
    const start = this.first;
    const end = if this._low > this._high then start else this.last;

    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive(">=", highBoundForIter(this), lowBoundForIter(this)),  // execute at least once?
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
    !localeModelPartitionsIterationOnSublocales
  {
    if !(hasLowBoundForIter(this) && hasHighBoundForIter(this)) {
      compilerError("parallel iteration is not currently supported over ranges without bounds");
    }
    if boundsChecking && hasAmbiguousAlignmentForIter(this) {
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
    }
    if debugChapelRange {
      chpl_debug_writeln("*** In range standalone iterator:");
    }

    const len = this.sizeAs(intIdxType);
    const numChunks = if __primitive("task_get_serial") then
                      1 else _computeNumChunks(len);

    if debugChapelRange {
      chpl_debug_writeln("*** RI: length=", len, " numChunks=", numChunks);
    }

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
        foreach i in low..high by stride {
          yield i;
        }
      } else {
        const (lo, hi) = _computeBlock(len, numChunks, chunk, this._high, this._low, this._low);
        foreach i in lo..hi {
          yield chpl_intToIdx(i);
        }
      }
    }
  }

  pragma "no doc"
  iter range.these(param tag: iterKind) where tag == iterKind.leader
  {
    if !(hasLowBoundForIter(this) && hasHighBoundForIter(this)) then
      compilerError("parallel iteration is not currently supported over ranges without bounds");

    if boundsChecking && hasAmbiguousAlignmentForIter(this) then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    if debugChapelRange then
      chpl_debug_writeln("*** In range leader:"); // ", this);
    const numSublocs = here._getChildCount();

    if localeModelPartitionsIterationOnSublocales && numSublocs != 0 {
      const len = this.sizeAs(intIdxType);
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

      coforall chunk in 0..#numChunks {
        local do on here._getChild(chunk) {
          if debugDataParNuma {
            if chunk!=chpl_getSubloc() then
              chpl_debug_writeln("*** ERROR: ON WRONG SUBLOC (should be ",
                                 chunk, ", on ", chpl_getSubloc(), ") ***");
          }
          const (lo,hi) = _computeBlock(len, numChunks, chunk, len-1);
          const locRange = lo..hi;
          const locLen = locRange.sizeAs(intIdxType);
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

    } else {
      var v = this.chpl_sizeAsForIter(intIdxType);
      const numChunks = if __primitive("task_get_serial") then
                        1 else _computeNumChunks(v);

      if debugChapelRange
      {
        chpl_debug_writeln("*** RI: length=", v, " numChunks=", numChunks);
        chpl_debug_writeln("*** RI: Using ", numChunks, " chunk(s)");
      }

      coforall chunk in 0..#numChunks
      {
        const (lo,hi) = _computeBlock(v, numChunks, chunk, v-1);
        if debugChapelRange then
          chpl_debug_writeln("*** RI: tuple = ", (lo..hi,));
        yield (lo..hi,);
      }
    }
  }

  pragma "no doc"
  iter range.these(param tag: iterKind, followThis) where tag == iterKind.follower
  {
    if boundsChecking && hasAmbiguousAlignmentForIter(this) then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    if bounds == boundKind.neither then
      compilerError("iteration over a range with no bounds");
    if ! stridable && bounds == boundKind.high then
      compilerError("iteration over a range with no first index");

    if followThis.size != 1 then
      compilerError("rank mismatch in zippered iteration (can't zip a " +
                    followThis.size:string + "D expression with a range, which is 1D)");

    if debugChapelRange then
      chpl_debug_writeln("In range follower code: Following ", followThis);

    var myFollowThis = followThis(0);

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
      const flwlen = myFollowThis.sizeAs(myFollowThis.intIdxType);
      if boundsChecking {
        if this.hasLast() {
          // this check is for typechecking only
          if !isBoundedRange(this) then
            assert(false, "hasFirst && hasLast do not imply isBoundedRange");
        }
        if flwlen != 0 then
          if isBoundedRange(this) && myFollowThis.last >= this.sizeAs(uint) then
            HaltWrappers.boundsCheckHalt("size mismatch in zippered iteration");
      }

      if this.stridable || myFollowThis.stridable {
        var r: range(idxType, stridable=true);

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
        var r:range(idxType);

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

  pragma "no doc"
  operator :(x: range(?), type t: string) {
    var ret: string;

    if x.hasLowBound() then
      ret += x.lowBound:string;

    ret += "..";

    if x.hasHighBound() {
      // handle the special case of an empty range with a singleton idxType
      if (chpl__singleValIdxType(x.idxType) && x._high != x._low) {
        ret += "<" + x.lowBound:string;
      } else {
        ret += x.highBound:string;
      }
    }

    if x.stride != 1 {
      ret += " by " + x.stride:string;

      if x.stride != -1 && x.aligned && ! x.isNaturallyAligned() then
      // Write out the alignment only if it differs from natural alignment.
      // We take alignment modulo the stride for consistency.
       ret += " align " + x.chpl_intToIdx(
                  chpl__mod(chpl__idxToInt(x.alignment), x.stride)):string;
    }
    return ret;
  }


  //################################################################################
  //# Internal helper functions.
  //#

  pragma "no doc"
  inline proc range.chpl__unTranslate(i) do
    return this - i;

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
    proc convert(a,b) param do
      return ( isIntType(a.type) && isUintType(b.type) ) ||
             ( isUintType(a.type) && isIntType(b.type) );

    proc mul(a,b) do return if convert(a,b) then a:int(64) * b:int(64) else a * b;
    proc add(a,b) do return if convert(a,b) then a:int(64) + b:int(64) else a + b;

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

    while V(2) != 0 {
      // This is a workaround for a bug.
      // The previous version was:
      //(U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));
      var oldU = U;
      var q = U(2)/V(2);
      U = V;
      V = oldU - V * (q, q, q);
    }

    return (U(2), U(0));
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
    } else if isFiniteIdxType(idxType) {
      return int;
    } else {
      chpl__rangeIdxTypeError(idxType);
    }
  }

  private proc chpl__rangeUnsignedType(type idxType) type {
    if isIntegralType(idxType) {
      return chpl__unsignedType(idxType);
    } else if isFiniteIdxType(idxType) {
      return uint;
    } else {
      chpl__rangeIdxTypeError(idxType);
    }
  }

  pragma "no doc"
  proc chpl__idxTypeToIntIdxType(type idxType) type {
    if isIntegralType(idxType) {
      // integer idxTypes are their own integer idxType
      return idxType;
    } else {
      // other types (bool, enum, ...) use 'int'
      return int;
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

  inline proc chpl__intToIdx(type idxType: integral, param i: integral) param {
    if (i.type == idxType) then
      return i;
    else
      return i: idxType;
  }

  inline proc chpl__intToIdx(type idxType: enum, i: integral) {
    return chpl__orderToEnum(i, idxType);
  }

  inline proc chpl__intToIdx(type idxType: enum, param i: integral) param {
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

  inline proc chpl__idxToInt(param i: enum) param {
    return chpl__enumToOrder(i);
  }

  inline proc chpl__idxToInt(i: bool) {
    return i: int;
  }

  inline proc chpl__idxToInt(param i: bool) param {
    return i: int;
  }

  proc chpl__singleValIdxType(type t) param {
    return isEnumType(t) && t.size == 1;
  }

  // These two routines return the default values for the _low and
  // _high fields when default-initing a range
  //
  private proc chpl__defaultLowBound(type t, param bounds: boundKind) param {
    if chpl__singleValIdxType(t) {
      return 0:chpl__idxTypeToIntIdxType(t);
    } else if bounds == boundKind.high || bounds == boundKind.neither {
      return 0:chpl__idxTypeToIntIdxType(t);
    } else {
      return 1:chpl__idxTypeToIntIdxType(t);
    }
  }

  private proc chpl__defaultHighBound(type t, param bounds: boundKind) param {
    if chpl__singleValIdxType(t) {
      return -1:chpl__idxTypeToIntIdxType(t);
    } else if bounds == boundKind.low || bounds == boundKind.neither {
      if isFiniteIdxType(t) {
        return finiteIdxTypeHigh(t):chpl__idxTypeToIntIdxType(t);
      } else {
        return 0:chpl__idxTypeToIntIdxType(t);
      }
    } else {
      return 0:chpl__idxTypeToIntIdxType(t);
    }
  }
}
