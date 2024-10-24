/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

  use AutoMath, DSIUtil, Reflection;

  private use ChapelDebugPrint only chpl_debug_writeln;

  // Turns on range iterator debugging.
  @chpldoc.nodoc
  config param debugChapelRange = false;

  @chpldoc.nodoc
  config param useOptimizedRangeIterators = true;

  /* This flag, when set to `true`, used to switch to using the new slicing rule
     and to turn off the deprecation warning for using the old rule.
     Now the new rule is always enabled regardless of this flag's value.
     When slicing with a range with a negative stride, the old rule
     preserves the direction of the original range or domain/array dimension
     whereas the new rule reverses such direction. */
  @chpldoc.nodoc
  @deprecated("'newSliceRule' is deprecated and will be removed in a future release; it is now 'true' by default; slicing with a range with a negative stride now always reverses the direction of the original range or domain/array dimension, regardless of the value of 'newSliceRule'")
  config param newSliceRule = true;

  /* Compile with ``-snewRangeLiteralType`` to switch to using the new rule
     for determining the idxType of a range literal with param integral bounds
     and to turn off the deprecation warning for using the old rule.

     The new rule defines such idxType to be the type produced by adding
     the two bounds. I.e.,``(low..high).idxType`` is ``(low+high).type``
     when ``low`` and ``high`` are integral params. */
  @chpldoc.nodoc
  config param newRangeLiteralType = false;

  private param unalignedMark = -1;

  // This enum is documented directly in the spec to fit the presentation flow.
  @chpldoc.nodoc
  enum boundKind { both, low, high, neither };

  // This enum is documented directly in the spec to fit the presentation flow.
  @chpldoc.nodoc
  enum strideKind {
    one,              // stride = 1
    negOne,           // stride = -1
    positive,         // stride > 0
    negative,         // stride < 0
    any               // stride != 0
  };

  //
  // range type
  //
  //   parameterized by:
  //        1) an integral element type,
  //        2) whether low and/or high bounds exist, and
  //        3) what values the stride can take on.
  //
  // The type of the stride is a signed type with the same number of bits as the
  // index type.  This means that for an unsigned index type, a maximal range can
  // be reached in no less than two strides.
  //
  // Alignments are stored modulo abs(stride).
  // Unaligned ranges store unalignedMark as their alignment.
  //
  pragma "plain old data"
  pragma "range"
  @chpldoc.nodoc
  record _range
  {
    type idxType = int;                            // element type
    param bounds: boundKind = boundKind.both;      // lower/upper bounds
    param strides: strideKind = strideKind.one;    // what stride(s) are legal

    var _low       : chpl__idxTypeToIntIdxType(idxType);  // lower bound
    var _high      : chpl__idxTypeToIntIdxType(idxType);  // upper bound
    var _stride    : if strides.isPosNegOne() then nothing
                     else chpl__rangeStrideType(idxType); // signed stride
    var _alignment : if strides.isPosNegOne() then nothing
                     else chpl__rangeStrideType(idxType); // alignment or -1
  }

  /* Returns the type of the range's stride. */
  @unstable("range.strType is unstable and may be removed or renamed")
  proc range.strType type do return chpl__rangeStrideType(idxType);

  proc range.chpl__promotionType() type do return idxType;

  /* The ``idxType`` as represented by an integer type.  When
     ``idxType`` is an enum type, this evaluates to ``int``.
     Otherwise, it evaluates to ``idxType``.

     This is no-doc'd because we currently are hoping it can
     be more of an implementation detail than a user-facing
     feature. */
  @chpldoc.nodoc
  proc range.chpl_integralIdxType type do
    return chpl__idxTypeToIntIdxType(idxType);

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
  @chpldoc.nodoc
  proc range.init(type idxType, low: idxType, high: idxType) {
    this.idxType = idxType;
    this.bounds = boundKind.both;
    this._low = chpl__idxToInt(low);
    this._high = chpl__idxToInt(high);
  }

  // This is the initializer for a low.. unbounded range
  //
  @chpldoc.nodoc
  proc range.init(low: ?t) {
    this.idxType = t;
    this.bounds = boundKind.low;
    this._low = chpl__idxToInt(low);
    init this;
    if isFiniteIdxType(idxType) {
      this._high = finiteIdxTypeHigh(idxType);
    }
  }

  // This is the initializer for a ..high unbounded range
  //
  @chpldoc.nodoc
  proc range.init(high: ?t) {
    this.idxType = t;
    this.bounds = boundKind.high;
    this._high = chpl__idxToInt(high);
    init this;
    if isFiniteIdxType(idxType) {
      this._low = finiteIdxTypeLow(idxType);
    }
  }

  // This is the initializer for a .. unbounded range
  //
  @chpldoc.nodoc
  proc range.init() {
    this.idxType = int;
    this.bounds = boundKind.neither;
    init this;
    if isFiniteIdxType(idxType) {
      this._low = finiteIdxTypeLow(idxType);
      this._high = finiteIdxTypeHigh(idxType);
    }
  }

  // This is an initializer for defining a default range value
  //
  @chpldoc.nodoc
  proc range.init(type idxType,
                  param bounds: boundKind,
                  param strides: strideKind) {

    if chpl_warnUnstable && (bounds == boundKind.low || bounds == boundKind.high)
      then warning("Default initialization of a range with 'boundKind.low' or 'boundKind.high' is unstable");

    this.init(idxType, bounds, strides,
              _low = chpl__defaultLowBound(idxType, bounds),
              _high = chpl__defaultHighBound(idxType, bounds),
              _stride = strides.defaultStride():chpl__rangeStrideType(idxType),
              alignmentValue = 0:chpl__rangeStrideType(idxType));
  }

  // this overload can be removed when the unstable warning in the above overload is removed
  @chpldoc.nodoc
  proc range.init(type idxType,
                  param bounds: boundKind,
                  param strides: strideKind,
                  param internal: bool) {

    this.init(idxType, bounds, strides,
              _low = chpl__defaultLowBound(idxType, bounds),
              _high = chpl__defaultHighBound(idxType, bounds),
              _stride = strides.defaultStride():chpl__rangeStrideType(idxType),
              alignmentValue = 0:chpl__rangeStrideType(idxType));
  }

  // This is an initializer for defining a range value in terms of its
  // internal field values
  //
  @chpldoc.nodoc
  proc range.init(type idxType,
                  param bounds : boundKind,
                  param strides: strideKind,
                  _low, _high, _stride, _alignment, _aligned,
                  param normalizeAlignment=true) {
    type almtType = chpl__rangeStrideType(idxType);
    const alignmentValue =
      if strides.isPosNegOne() /*hasParamStrideAltvalAld()*/ then none else
        if !_aligned then unalignedMark: almtType else
          if normalizeAlignment then chpl__mod(_alignment, _stride): almtType
            else _alignment;

    this.init(idxType, bounds, strides,
              _low, _high, _stride, alignmentValue);
  }

  // This is an initializer like the above except the alignment argument
  // is expected to be "the normalized alignment or unalignedMark".
  //
  @chpldoc.nodoc
  proc range.init(type idxType,
                  param bounds : boundKind,
                  param strides: strideKind,
                  _low, _high, _stride, alignmentValue) {
    this.idxType     = idxType;
    this.bounds      = bounds;
    this.strides     = strides;
    this._low        = _low: chpl__idxTypeToIntIdxType(idxType);
    this._high       = _high: chpl__idxTypeToIntIdxType(idxType);

    if ! hasParamStrideAltvalAld() {
      this._stride    = _stride;
      this._alignment =  alignmentValue;

      if boundsChecking then verifyAppropriateStride(strides, _stride);
    }
  }

  // This is the range copy initializer
  //
  @chpldoc.nodoc
  proc range.init=(other : range(?i,?b,?s)) {
    type idxType  = if this.type.idxType == ? then i else this.type.idxType;
    param bounds  = if this.type.bounds  == ? then b else this.type.bounds;
    param strides = if this.type.strides == ? then s else this.type.strides;

    if ! assignmentIsLegal(idxType, i) then
      compilerError("initializing a range with idxType ", idxType:string,
                           " from a range with idxType ", i:string);
    if bounds != b then
      compilerError("initializing a range with boundKind.", bounds:string,
                           " from a range with boundKind.", b:string);
    if ! chpl_assignStrideIsSafe(strides, s) then
      compilerError("initializing a range with strideKind.", strides:string,
                           " from a range with strideKind.", s:string);

    param isEnumBool = isFiniteIdxType(idxType);
    type bt = other.chpl_integralIdxType;
    const low  = if isEnumBool && !other.hasLowBound()
                 then finiteIdxTypeLow(idxType):bt
                 else other._low;
    const high = if isEnumBool && !other.hasHighBound()
                 then finiteIdxTypeHigh(idxType):bt
                 else other._high;

    const str = other.stride: chpl__rangeStrideType(idxType);

    var alignment = ( if other.hasParamAlignmentField()
                      then 0 else other._alignment ): str.type;

    if isEnumBool && ! other.isAligned() {
      if isPositiveStride(strides, str) {
        alignment = finiteIdxTypeLow(idxType);;
      } else {
        alignment = finiteIdxTypeHigh(idxType);
      }
    }

    this.init(idxType, bounds, strides,
              low, high, str, alignment);
  }

  /////////////////////////////////
  // for debugging
  @chpldoc.nodoc
  proc range.displayRepresentation(msg: string = ""): void {
    operator :(arg: nothing, type t: string) param do return "none";
    chpl_debug_writeln(msg, "range(", _low:string, "..", _high:string, " by ",
          _stride:string, " align ", _alignment:string,
          " : ", idxType:string, ",", bounds:string, ",", strides:string, ")");
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range builders:  used by the parser to create literal ranges
  //

  private
  proc computeParamRangeIndexType_Old(param low, param high) type {
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
  private
  proc computeParamRangeIndexType(param low, param high) type {
    if newRangeLiteralType {
      // The idxType of 'low..high' is the type that '+' would produce.
      return (low+high).type;
    }
    type newRule = (low+high).type;
    type oldRule = computeParamRangeIndexType_Old(low, high);
    if newRule == oldRule then
      return newRule;
    compilerWarning("the idxType of this range literal ",
                    low:string, "..", high:string,
                    " with the low bound of the type ", low.type:string,
                    " and the high bound of the type ", high.type:string,
                    " is currently ", oldRule:string,
          ". In a future release it will be switched to ", newRule:string,
          ". To switch to this new typing and turn off this warning,",
          " compile with -snewRangeLiteralType.");
    return oldRule;
  }
  proc chpl_isValidRangeIdxType(type t) param {
    return isIntegralType(t) || isEnumType(t) || isBoolType(t);
  }

  // Range builders for fully bounded ranges
  proc chpl_build_bounded_range(param low: integral, param high: integral) {
    type idxType = computeParamRangeIndexType(low, high);
    return new range(idxType, low=low, high=high);
  }

  proc chpl_build_bounded_range(low: int(8), high: int(8)) do
    return new range(int(8), low = low, high = high);
  proc chpl_build_bounded_range(low: int(16), high: int(16)) do
    return new range(int(16), low = low, high = high);
  proc chpl_build_bounded_range(low: int(32), high: int(32)) do
    return new range(int(32), low = low, high = high);
  proc chpl_build_bounded_range(low: int(64), high: int(64)) do
    return new range(int(64), low = low, high = high);

  proc chpl_build_bounded_range(low: uint(8), high: uint(8)) do
    return new range(uint(8), low = low, high = high);
  proc chpl_build_bounded_range(low: uint(16), high: uint(16)) do
    return new range(uint(16), low = low, high = high);
  proc chpl_build_bounded_range(low: uint(32), high: uint(32)) do
    return new range(uint(32), low = low, high = high);
  proc chpl_build_bounded_range(low: uint(64), high: uint(64)) do
    return new range(uint(64), low = low, high = high);

  proc chpl_build_bounded_range(low: enum, high: enum) {
    if (low.type != high.type) then
      compilerError("ranges of enums must use a single enum type");
    return new range(low.type, low=low, high=high);
  }
  proc chpl_build_bounded_range(low: bool, high: bool) do
    return new range(bool, low=low, high=high);

  proc chpl_build_bounded_range(low, high)
  where !(chpl_isValidRangeIdxType(low.type) && chpl_isValidRangeIdxType(high.type)) {
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
  where !chpl_isValidRangeIdxType(low.type) {
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
  where !chpl_isValidRangeIdxType(high.type) {
    compilerError("Ranges defined using bounds of type '" + high.type:string + "' are not currently supported");
  }

  // Range builder for unbounded ranges
  proc chpl_build_unbounded_range() do
    return new range();

  // Range builder for single-element ranges
  @chpldoc.nodoc proc type range.createWithSingleElement(elm: idxType) do
     return elm..elm by strides.defaultStride();

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
  where !(chpl_isValidRangeIdxType(low.type) && chpl_isValidRangeIdxType(high.type)) {
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
  pragma "no where doc"
  inline proc range.stride where !hasParamStride() do return _stride;

  @chpldoc.nodoc proc range.stride param where hasParamStride() do
    return (if strides == strideKind.one then 1 else -1) : strType;

  /* Returns the range's alignment. */
  pragma "no where doc"
  inline proc range.alignment where !hasParamAlignment() do
    return chpl_intToIdx(if hasParamAlignmentField() then 0 else _alignment);

  @chpldoc.nodoc proc range.alignment param where hasParamAlignment() do return
    if isEnum(idxType) then chpl__orderToEnum(0, idxType) else 0: idxType;

  /* Returns ``true`` if the range's alignment is unambiguous,
     ``false`` otherwise. */
  pragma "no where doc"
  inline proc range.isAligned() where !hasParamAligned() do
    return _alignment != unalignedMark;

  @chpldoc.nodoc proc range.isAligned() param where hasParamAligned() do
    return true;

  // Does this kind of range store the stride/alignment/aligned at runtime
  //  and/or is its stride/alignment/aligned a param?

  @chpldoc.nodoc proc range.hasParamStride() param do
    return hasPosNegUnitStride();

  @chpldoc.nodoc proc range.hasParamAlignmentField() param do
    return hasPosNegUnitStride();

  @chpldoc.nodoc proc range.hasParamAlignment() param do
    return hasParamAlignmentField() &&
           (isIntegral(idxType) || isEnum(idxType) || isBool(idxType));

  @chpldoc.nodoc proc range.hasParamAligned() param do
    return hasPosNegUnitStride();

  // all three predicates return the same thing
  @chpldoc.nodoc proc range.hasParamStrideAltvalAld() param do
    return hasPosNegUnitStride();

  @chpldoc.nodoc proc type range.hasParamStrideAltvalAld() param
    { var r: this; return r.hasParamStrideAltvalAld(); }

  //################################################################################
  //# Predicates
  //#

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

  /////////// functions related to range.strides ///////////
  // todo: promote (at least some of) them to the user-facing interface

  // is this.stride always 1 ?
  @chpldoc.nodoc proc range.hasUnitStride() param
    do return strides.isOne();

  // is this.stride always 1 or always -1 ?
  @chpldoc.nodoc proc range.hasPosNegUnitStride() param
    do return strides.isPosNegOne();

  // returns 'this.stride > 0', as a param when possible
  @chpldoc.nodoc proc range.hasPositiveStride() param where strides.hasSign()
    do return strides.isPositive();

  @chpldoc.nodoc proc range.hasPositiveStride() where strides.isAny()
    do return _stride > 0;

  // returns 'this.stride < 0', as a param when possible
  // equivalent to ! hasPositiveStride()
  @chpldoc.nodoc proc range.hasNegativeStride() param where strides.hasSign()
    do return strides.isNegative();

  @chpldoc.nodoc proc range.hasNegativeStride() where strides.isAny()
    do return _stride < 0;

  /////////// helpers related to range.strides ///////////

  @chpldoc.nodoc proc param strideKind.isOne() param
    do return this == strideKind.one;

  @chpldoc.nodoc proc param strideKind.isNegOne() param
    do return this == strideKind.negOne;

  @chpldoc.nodoc proc param strideKind.isPosNegOne() param
    do return this == strideKind.one || this == strideKind.negOne;

  @chpldoc.nodoc proc param strideKind.isPositive() param
    do return this == strideKind.one || this == strideKind.positive;

  @chpldoc.nodoc proc param strideKind.isNegative() param
    do return this == strideKind.negOne || this == strideKind.negative;

  @chpldoc.nodoc proc param strideKind.hasSign() param
    do return this != strideKind.any;

  @chpldoc.nodoc proc param strideKind.isAny() param
    do return this == strideKind.any;

  @chpldoc.nodoc proc param strideKind.defaultStride() param
    do return if isNegative() then -1 else 1;

  // Counterparts to hasPositiveStride() and hasNegativeStride()
  // for use when there is no range.
  // Do we want to add assertions of consistency betw. 'strides' and 'stride'?

  private inline proc isPositiveStride(param strides:strideKind, stride)
    where strides.isAny()   do return stride > 0;

  private proc isPositiveStride(param strides:strideKind, param stride) param
    where strides.isAny()   do return stride > 0;

  private proc isPositiveStride(param strides: strideKind, stride) param
    where strides.hasSign() do return strides.isPositive();

  private inline proc isNegativeStride(param strides: strideKind, stride)
    where strides.isAny()   do return stride < 0;

  private proc isNegativeStride(param strides: strideKind, param stride) param
    where strides.isAny()   do return stride < 0;

  private proc isNegativeStride(param strides: strideKind, stride) param
    where strides.hasSign() do return strides.isNegative();

  // chpl_strideProduct(s1,s2) returns the strideKind that accommodates
  // the product of two strides that fit in 's1' and 's2'

  proc chpl_strideProduct(param s1: strideKind, param s2: strideKind) param {
    use strideKind;
    select s1 {
      when one do
        return s2;
      when positive do select s2 {
        when one      do return positive;
        when positive do return positive;
        when negOne   do return negative;
        when negative do return negative;
        when any      do return any;
      }
      when negOne do select s2 {
        when one      do return negOne;
        when positive do return negative;
        when negOne   do return one;
        when negative do return positive;
        when any      do return any;
      }
      when negative do select s2 {
        when one      do return negative;
        when positive do return negative;
        when negOne   do return positive;
        when negative do return positive;
        when any      do return any;
      }
      when any do
        return any;
    }
  }

  proc chpl_strideProduct(r1: range(?), r2: range(?)) param do
    return chpl_strideProduct(r1.strides, r2.strides);

  proc chpl_strideProduct(r1: range(?), param stride2: integral) param {
    use strideKind;
    param s2 = if stride2 == 1 then one else if stride2 == -1 then negOne
                else if stride2 > 0 then positive else negative;
    return chpl_strideProduct(r1.strides, s2);
  }

  // chpl_strideUnion() returns the most general strideKind among the arguments
  // chpl_strideUnion() replaces chpl__anyStridable()

  proc chpl_strideUnion(param s1: strideKind, param s2: strideKind) param {
    use strideKind;
    select s1 {
      when one do select s2 {
        when one      do return one;
        when positive do return positive;
        when negOne   do return any;
        when negative do return any;
        when any      do return any;
      }
      when negOne do select s2 {
        when one      do return any;
        when positive do return any;
        when negOne   do return negOne;
        when negative do return negative;
        when any      do return any;
      }
      when positive do select s2 {
        when one      do return positive;
        when positive do return positive;
        when negOne   do return any;
        when negative do return any;
        when any      do return any;
      }
      when negative do select s2 {
        when one      do return any;
        when positive do return any;
        when negOne   do return negative;
        when negative do return negative;
        when any      do return any;
      }
      when any do return any;
    }
  }

  proc chpl_strideUnion(args: _tuple) param do
    return chpl_strideUnion((...args));

  proc chpl_strideUnion(arg, args...) param do
    return chpl_strideUnion(chpl_strideUnion(arg),
                            chpl_strideUnion((...args)));

  // This single-arg overload supports the varargs ones above.
  // It works for ranges, domains, and DSI classes that provide '.strides'.
  proc chpl_strideUnion(arg) param : strideKind do return arg.strides;

  // chpl_strideUnionRC() is strideUnion that ignores non-range elements
  // chpl_strideUnionRC() replaces chpl__anyRankChangeStridable()

  proc chpl_strideUnionRC(args: _tuple) param do
    return chpl_strideUnionRC((...args));

  proc chpl_strideUnionRC(arg, args...) param do
    return chpl_strideUnion(chpl_strideUnionRC(arg),
                            chpl_strideUnionRC((...args)));

  proc chpl_strideUnionRC(arg) param do
    return if isRange(arg) then arg.strides else strideKind.one;

  // chpl_assignStrideIsSafe(lhs,rhs) returns true
  // when the rhs stride is guaranteed to fit into lhs stride

  proc chpl_assignStrideIsSafe(param lhs: strideKind,
                               param rhs: strideKind) param do
    select lhs {
      when strideKind.one      do return rhs.isOne();
      when strideKind.positive do return rhs.isPositive();
      when strideKind.negOne   do return rhs.isNegOne();
      when strideKind.negative do return rhs.isNegative();
      when strideKind.any      do return true;
    }

  proc chpl_assignStrideIsSafe(lhs: range(?), rhs: range(?)) param do
    return chpl_assignStrideIsSafe(lhs.strides, rhs.strides);

  // chpl_assignStrideIsSafe(lhs,rhs,rhsStride) returns true
  // when the given 'rhsStride' fits in lhs stride

  proc chpl_assignStrideIsSafe(param lhs: strideKind, param rhs: strideKind,
                               rhsStride) param
    where chpl_assignStrideIsSafe(lhs, rhs) ||
          (lhs == strideKind.positive && isUint(rhsStride))
    do return true;

  proc chpl_assignStrideIsSafe(param lhs: strideKind, param rhs: strideKind,
                               rhsStride) do
    select lhs {
      when strideKind.one      do return rhsStride == 1;
      when strideKind.positive do return rhsStride > 0;
      when strideKind.negOne   do return rhsStride == -1;
      when strideKind.negative do return rhsStride < 0;
      when strideKind.any      do compilerError("internal error");
    }

  // chpl_assignStrideIsUnsafe() returns true
  // when the rhs stride is guaranteed NOT to fit into lhs stride

  proc chpl_assignStrideIsUnsafe(param lhs: strideKind,
                                 param rhs: strideKind) param do
    return lhs.isPositive() && rhs.isNegative() ||
           lhs.isNegative() && rhs.isPositive();

  private proc assignmentIsLegal(type to, type from) param {
    if to == from then return true;
    var toVar: to, fromVar: from;
    return canResolve("=", toVar, fromVar);
  }

  private proc verifyAppropriateStride(param strides, stride) {
    if strides.isPositive() then assert(stride > 0);
    if strides.isNegative() then assert(stride < 0);
  }

  // chpl_setFields() modifies _low, _high, _stride fields directly,
  // to work around casting limitations in code that calculates the ordinals
  // for low/high of a pre-allocated enum range, ex., in range/domain followers

  inline proc ref range.chpl_setFields(low, high, stride) {
    this._low  = chpl__idxToInt(low):  this.chpl_integralIdxType;
    this._high = chpl__idxToInt(high): this.chpl_integralIdxType;
    if this.hasParamStrideAltvalAld() {
      if boundsChecking then verifyAppropriateStride(this.strides, stride);
    } else {
      this._stride    = stride: this.strType;
      const first     = if this.hasPositiveStride() then low else high;
      this._alignment = chpl__mod(chpl__idxToInt(first), stride): this.strType;
      if boundsChecking then assert(this.isAligned()); // not -1
    }
  }

  inline proc ref range.chpl_setFields(low, high) {
    compilerAssert(this.hasParamStride()); // otherwise stride = ?
    this._low  = chpl__idxToInt(low):  this.chpl_integralIdxType;
    this._high = chpl__idxToInt(high): this.chpl_integralIdxType;
  }

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

  @chpldoc.nodoc
  inline proc range.alignedLowAsInt {
    if ! isAligned() then
      halt("Can't query the aligned bounds of an ambiguously aligned range");

    if this.hasPosNegUnitStride() then
      return _low;
    else
      // Adjust _low upward by the difference between _alignment and _low.
      return helpAlignLow(_low, _alignment, stride);
  }

  inline proc range.chpl_alignedLowAsIntForIter {
    if !hasUnitStride() && !hasLowBound() && isFiniteIdxType(idxType) {
      return helpAlignLow(chpl__idxToInt(lowBoundForIter(this)), _alignment, stride);
    } else {
      return alignedLowAsInt;
    }
  }

  private inline proc helpAlignLow(l, a, s) {
    // Adjust _low upward by the difference between _alignment and _low.
    // If we are using the alignment value, the range should be aligned
    // therefore alignment is >= 0 and can be cast to chpl_integralIdxType.
    return l + chpl__diffMod(a: l.type, l, s);
  }


  /* Returns ``true`` if this range's high bound is *not* :math:`\infty`,
     and ``false`` otherwise. */
  proc range.hasHighBound() param do
    return bounds == boundKind.both || bounds == boundKind.high;

  /* Returns the range's high bound. If the range does not have a high
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

  @chpldoc.nodoc
  inline proc range.alignedHighAsInt {
    if ! isAligned() then
      halt("Can't query the aligned bounds of an ambiguously aligned range");

    if this.hasPosNegUnitStride() then
      return _high;
    else
      return helpAlignHigh(_high, _alignment, stride);
  }

  inline proc range.chpl_alignedHighAsIntForIter {
    if !hasUnitStride() && !hasHighBound() && isFiniteIdxType(idxType) {
      return helpAlignHigh(chpl__idxToInt(highBoundForIter(this)), _alignment, stride);
    } else {
      return alignedHighAsInt;
    }
  }

  private inline proc helpAlignHigh(h, a, s) {
    // Adjust _high downward by the difference between _high and _alignment.
    // If we are using the alignment value, the range should be aligned
    // therefore alignment is >= 0 and can be cast to chpl_integralIdxType.
    return h - chpl__diffMod(h, a: h.type, s);
  }

  // tells whether omitting the 'align' clause results in the same range
  pragma "no where doc"
  proc range.chpl_isNaturallyAligned()
    where ! hasPosNegUnitStride() && bounds != boundKind.neither
  do if bounds == boundKind.both {
    // If the stride is positive, we must be aligned on the low bound.
    if hasPositiveStride() then return this.alignedLowAsInt == _low;
    // If the stride is negative, we must be aligned on the high bound.
    if hasNegativeStride() then return this.alignedHighAsInt == _high;
    // stride == 0: ???
    return false;
  } else if bounds == boundKind.low {
    return hasPositiveStride() && this.alignedLowAsInt == _low;
  } else if bounds == boundKind.high {
    return hasNegativeStride() && this.alignedHighAsInt == _high;
  }

  proc range.chpl_isNaturallyAligned() param
    where hasPosNegUnitStride() || bounds == boundKind.neither
    do return hasPosNegUnitStride();

  private inline proc hasAmbiguousAlignmentForIter(r) param
    where r.hasPosNegUnitStride() || isFiniteIdxType(r.idxType) {
    return false;
  }

  private inline proc hasAmbiguousAlignmentForIter(r) {
    return ! r.isAligned();
  }

  /* Returns ``true`` if the sequence represented by the range is
     empty and ``false`` otherwise.  If the range is ambiguous, the
     behavior is undefined.
   */
  inline proc range.isEmpty() {
    return isEmptyHelp(this);
  }


  @unstable("range.isEmpty() is unstable for unbounded ranged over an enum or bool")
  @chpldoc.nodoc
  inline proc range.isEmpty() where isFiniteIdxType(idxType) && this.bounds !=boundKind.both {
    return isEmptyHelp(this);
  }

  private inline proc isEmptyHelp(r) {
    if chpl__singleValIdxType(r.idxType) {
      if r._low > r._high then
        return true;
    }
    if boundsChecking && ! r.isAligned() then
      HaltWrappers.boundsCheckHalt("isEmpty() is invoked on an ambiguously-aligned range");
    else
      return (r.bounds == boundKind.both || isFiniteIdxType(r.idxType)) && r.alignedLowAsInt > r.alignedHighAsInt;
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
    if this.bounds != boundKind.both then
      compilerError("'size' is not defined on unbounded ranges");

    if chpl__singleValIdxType(idxType) {
      if _low > _high then return 0;
    }

    return sizeAsHelp(t);
  }

  @chpldoc.nodoc
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

  proc range.chpl_sizeAsForIter(type t: integral): t {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return 0;
    }

    if this.bounds != boundKind.both && isFiniteIdxType(idxType) {
      return sizeAsHelp(t,
                        this.chpl_alignedLowAsIntForIter,
                        this.chpl_alignedHighAsIntForIter);
    } else {
      return sizeAs(t);
    }
  }

  private proc hasFirstLastAreParam(r) param do
    select r.bounds {
      // bounded: depends on whether r is empty at runtime
      when boundKind.both    do return false;
      // low- or high-bounded: need to know stride direction and be unambiguous
      when boundKind.low     do return r.hasPosNegUnitStride();
      when boundKind.high    do return r.hasPosNegUnitStride();
      // unbounded: never has first/last unless its a finite idx type
      when boundKind.neither do return if isFiniteIdxType(r.idxType) then
                            r.hasPosNegUnitStride() else true;
    }

  // todo: what about ranges over enums, bool?
  /* Returns ``true`` if the range has a first index, ``false`` otherwise. */
  inline proc range.hasFirst() {
    warnUnstableFirst(this, fromHasFirst = true);
    return hasFirstForIter();
  }

  // Special name to avoid unstable warnings when called by iterators
  @chpldoc.nodoc
  inline proc range.hasFirstForIter() do
    return  if ! isAligned() || isEmpty() then false else
            if isFiniteIdxType(idxType) then true else
            if hasPositiveStride() then hasLowBound() else hasHighBound();

  // hasFirst current has the same behavior as hasFirstForIter
  // However we consider hasFirst to be unstable
  @chpldoc.nodoc
  proc range.hasFirst() param where hasFirstLastAreParam(this)
  {
    warnUnstableFirst(this, fromHasFirst = true);
    return hasFirstForIter();
  }

  // Special name to avoid unstable warnings when called by iterators
  @chpldoc.nodoc
  proc range.hasFirstForIter() param where hasFirstLastAreParam(this) {
    if isFiniteIdxType(idxType) then return true;
    select bounds {
      when boundKind.low     do return strides.isPositive();
      when boundKind.high    do return strides.isNegative();
      when boundKind.neither do return false;
    }
  }


  /* Returns the first value in the sequence the range represents.  If
     the range has no first index, the behavior is undefined.  See
     also :proc:`range.hasFirst`. */
  inline proc range.first {
    warnUnstableFirst(this, fromHasFirst = false);
    return chpl_intToIdx(this.firstAsInt);
  }

  private inline proc warnUnstableFirst(r, param fromHasFirst) {
    if !chpl_warnUnstable || !isFiniteIdxType(r.idxType) then
      return; // nothing to do
    if !r.hasLowBound() {
      if r.strides.isPositive() {
        if fromHasFirst then
          compilerWarning("range.hasFirst() is unstable for a range over an enum or bool if it has a positive stride and no low bound");
        else
          compilerWarning("range.first is unstable for a range over an enum or bool if it has a positive stride and no low bound");
      } else if r.hasPositiveStride() {
        if fromHasFirst then
            warning("range.hasFirst() is unstable for a range over an enum or bool if it has a positive stride and no low bound");
        else
            warning("range.first is unstable for a range over an enum or bool if it has a positive stride and no low bound");
      }
    }
    if !r.hasHighBound() {
      if r.strides.isNegative() {
        if fromHasFirst then
          compilerWarning("range.hasFirst() is unstable for a range over an enum or bool if it has a negative stride and no high bound");
        else
          compilerWarning("range.first is unstable for a range over an enum or bool if it has a negative stride and no high bound");
      } else if r.hasNegativeStride() {
        if fromHasFirst then
          warning("range.hasFirst() is unstable for a range over an enum or bool if it has a negative stride and no high bound");
        else
          warning("range.first is unstable for a range over an enum or bool if it has a negative stride and no high bound");
      }
    }
  }

  @chpldoc.nodoc
  inline proc range.firstAsInt {
    return if hasPositiveStride() then this.alignedLowAsInt
                                  else this.alignedHighAsInt;
  }

  inline proc range.chpl_firstAsIntForIter {
    if this.bounds == boundKind.both {
      return this.firstAsInt;
    } else {
      if strides.isOne() {
        return chpl__idxToInt(lowBoundForIter(this));
      } else if strides.isNegOne() {
        return chpl__idxToInt(highBoundForIter(this));
      } else {
        if hasPositiveStride() {
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

  // todo: what about ranges over enums, bool?
  /* Returns ``true`` if the range has a last index, ``false`` otherwise. */
  inline proc range.hasLast(){
    warnUnstableLast(this, fromHasLast = true);
    return hasLastForIter();
  }

  // Special name to avoid unstable warnings when called by iterators
  @chpldoc.nodoc
  inline proc range.hasLastForIter() do
    return if ! isAligned() || isEmpty() then false else
            if isFiniteIdxType(idxType) then true else
            if hasPositiveStride() then hasHighBound() else hasLowBound();


  // hasLast current has the same behavior as hasLastForIter
  // However we consider hasLast to be unstable
  @chpldoc.nodoc
  proc range.hasLast() param where hasFirstLastAreParam(this){
    warnUnstableLast(this, fromHasLast = true);
    return hasLastForIter();
  }

  // Special name to avoid unstable warnings when called by iterators
  @chpldoc.nodoc
  proc range.hasLastForIter() param where hasFirstLastAreParam(this) {
    if isFiniteIdxType(idxType) then return true;
    select bounds {
      when boundKind.low     do return strides.isNegative();
      when boundKind.high    do return strides.isPositive();
      when boundKind.neither do return false;
    }
  }

  /* Returns the last value in the sequence the range represents.  If
     the range has no last index, the behavior is undefined.  See also
     :proc:`range.hasLast`.
  */
  inline proc range.last {
    warnUnstableLast(this, fromHasLast = false);
    return chpl_intToIdx(this.lastAsInt);
  }

  private inline proc warnUnstableLast(r, param fromHasLast) {
    if !chpl_warnUnstable || !isFiniteIdxType(r.idxType) then
      return; // nothing to do
    if !r.hasLowBound() {
      if r.strides.isNegative() then
        if fromHasLast then
          compilerWarning("range.hasLast() is unstable for a range over an enum or bool if it has a negative stride and no low bound");
        else
          compilerWarning("range.last is unstable for a range over an enum or bool if it has a negative stride and no low bound");
      else if r.hasNegativeStride() then
        if fromHasLast then
          warning("range.hasLast() is unstable for a range over an enum or bool if it has a negative stride and no low bound");
        else
          warning("range.last is unstable for a range over an enum or bool if it has a negative stride and no low bound");
    }
    if !r.hasHighBound() {
      if r.strides.isPositive() then
        if fromHasLast then
          compilerWarning("range.hasLast() is unstable for a range over an enum or bool if it has a positive stride and no high bound");
        else
          compilerWarning("range.last is unstable for a range over an enum or bool if it has a positive stride and no high bound");
      else if r.hasPositiveStride() then
        if fromHasLast then
          warning("range.hasLast() is unstable for a range over an enum or bool if it has a positive stride and no high bound");
        else
          warning("range.last is unstable for a range over an enum or bool if it has a positive stride and no high bound");
    }
  }

  @chpldoc.nodoc
  inline proc range.lastAsInt {
    return if hasPositiveStride() then this.alignedHighAsInt
                                  else this.alignedLowAsInt;
  }

  inline proc range.chpl_lastAsIntForIter {
    if bounds == boundKind.both {
      return this.lastAsInt;
    } else {
      if strides.isOne() {
        return chpl__idxToInt(highBoundForIter(this));
      } else if strides.isNegOne() {
        return chpl__idxToInt(lowBoundForIter(this));
      } else if hasPositiveStride() {
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

    if ! this.isAligned() then return false;

    const i = chpl__idxToInt(ind);

    if hasHighBound()
    {
      if i > _high then return false;
    }
    if hasLowBound()
    {
      if i < _low then return false;
    }
    if ! hasPosNegUnitStride()
    {
      if chpl__mod(i, _stride) != _alignment
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

    if ! this.isAligned() || ! other.isAligned() then return false;

    if this.bounds == boundKind.both && this.sizeAs(uint) == 0 then
      return other.bounds == boundKind.both && other.sizeAs(uint) == 0;

    var slice = this[other];

    // Slicing reversed the direction of 'other' if this.stride < 0.
    // Switch it back before comparing.
    proc adjustedSlice do return
      if this.hasPositiveStride() then slice else slice by -1;

    return other == adjustedSlice;
  }

  @chpldoc.nodoc
  operator ==(r1: range(?), r2: range(?)) param
    where r1.bounds != r2.bounds &&
          (!isFiniteIdxType(r1.idxType) ||
           !isFiniteIdxType(r2.idxType)) do
  return false;

  @chpldoc.nodoc
  @unstable("== between unbounded and bounded ranges is unstable and its behavior may change in the future")
  operator ==(r1: range(?), r2: range(?)): bool
    where r1.bounds != r2.bounds && isFiniteIdxType(r1.idxType) &&
          isFiniteIdxType(r2.idxType)
  {
    const boundedr1 = if r1.bounds == boundKind.both then r1
                       else new range(
                          r1.idxType, boundKind.both, r1.strides,
                          r1._low, r1._high, r1._stride, r1._alignment);
    const boundedr2 = if r2.bounds == boundKind.both then r2
                        else new range(
                            r2.idxType, boundKind.both, r2.strides,
                            r2._low, r2._high, r2._stride, r2._alignment);
    return boundedr1 == boundedr2;
  }

  @chpldoc.nodoc
  operator ==(r1: range(?), r2: range(?)): bool
    where r1.bounds == r2.bounds
  {
    // An ambiguous ranges cannot equal an unambiguous one
    //  even if all their parameters match.
    if r1.isAligned() != r2.isAligned() then return false;

    // As a special case, two ambiguous ranges compare equal
    // if their representations are identical.
    if ! r1.isAligned() then return chpl_ident(r1, r2);

    if r1.bounds == boundKind.both {

      // gotta have a special case for length 0 or 1
      const len = r1.sizeAs(uint), l2 = r2.sizeAs(uint);
      if len != l2 then return false;
      if len == 0 then return true;
      if r1.first != r2.first then return false;
      if len == 1 then return true; // rest doesn't matter
      if r1.stride != r2.stride then return false;
      return true;

    } else {

      // r1 is not a bounded range
      if r1.stride != r2.stride then return false;

      if r1.hasLowBound() then
        if r1.low != r2.low then return false;

      if r1.hasHighBound() then
        if r1.high != r2.high then return false;

      return true;
    }
  }

  @chpldoc.nodoc
  operator !=(r1: range(?), r2: range(?)) do  return !(r1 == r2);

  @chpldoc.nodoc
  @unstable("!= between unbounded and bounded ranges is unstable and its behavior may change in the future")
  operator !=(r1: range(?), r2: range(?)): bool
    where r1.bounds != r2.bounds && isFiniteIdxType(r1.idxType) &&
          isFiniteIdxType(r2.idxType)
    do return !(r1 == r2);

  @chpldoc.nodoc
  operator !=(r1: range(?), r2: range(?)) param
    where r1.bounds != r2.bounds &&
          (!isFiniteIdxType(r1.idxType) ||
           !isFiniteIdxType(r2.idxType)) do
  return true;

  @chpldoc.nodoc
  operator <(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both ||r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  @chpldoc.nodoc
  operator >(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both || r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  @chpldoc.nodoc
  operator <=(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both || r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  @chpldoc.nodoc
  operator >=(r1: range(?), r2: range(?))
    where r1.bounds != boundKind.both || r2.bounds != boundKind.both {
    compilerError("Unbounded ranges don't support comparisons other than '==' and '!='");
  }

  proc chpl_ident(r1: range(?), r2: range(?))
    where r1.idxType   == r2.idxType  &&
          r1.bounds    == r2.bounds   &&
          r1.strides   == r2.strides
  {
    if r1._low != r2._low then return false;
    if r1._high != r2._high then return false;
    // the following can be 'if none != none ...'
        if r1._stride != r2._stride then return false;
        if r1._alignment != r2._alignment then return false;
    return true;
  }

  proc chpl_ident(r1: range(?), r2: range(?)) param {
    return false;
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range Casts
  //

// Todo: for (1..4 by 2), return 1..4 or 1..3 ?  Currently it is the latter.
// BlockDist uses the former, see boundsBox(). The latter makes this fail:
//   test/optimizations/bulkcomm/block/blockToBlock.chpl
@chpldoc.nodoc proc range.boundingBox() {
  compilerAssert(this.bounds == boundKind.both); // otherwise need to implement
  if this.strides.isOne() then return this;
  return new range(this.idxType, this.bounds, strideKind.one,
                   this.alignedLowAsInt, this.alignedHighAsInt, none, none);
}

/* Casts a range to a new range type. Throws an IllegalArgumentError when
   the original bounds and/or stride do not fit in the new idxType
   or when the original stride is not legal for the new `strides` parameter.
 */
pragma "no where doc"
proc range.tryCast(type t: range(?)) where chpl_tryCastIsSafe(this, t) {
  const r = this;
  checkBounds(t, r);
  checkEnumBoolIdx(t, r);

  // todo: unify with the non-throwing overload of `operator :` ?
  // todo: also when 'r' is over fully-concrete enum, see chpl_idxCastIsSafe
  param useR = !( t.idxType == int && isBCPindex(r.idxType) );
  var result: t = if useR then r
                  else new range(int, r.bounds, r.strides,
                                 r._low, r._high, r._stride, r._alignment);
  return result;
}

@chpldoc.nodoc
proc range.tryCast(type t: range(?)) throws where !chpl_tryCastIsSafe(this, t){
  const r = this;
  checkBounds(t, r);
  checkEnumBoolIdx(t, r);

  var tmp: t;
  type srcType = r.idxType,
       dstType = t.idxType,
    dstIntType = tmp.chpl_integralIdxType;

  tmp._low = if !r.hasLowBound() then r._low: dstIntType
             else chpl__idxToInt( chpl_throwingCast(dstType, r.lowBound) );

  tmp._high = if !r.hasHighBound() then r._high: dstIntType
             else chpl__idxToInt( chpl_throwingCast(dstType, r.highBound) );

  var error = r.chpl_checkStrides(t);
  if error != nil then throw error;

  if ! tmp.hasParamStrideAltvalAld() {
    tmp._stride = chpl_throwingCast(tmp.strType, r.stride);
    tmp._alignment = if isNothingValue(r._alignment) then 0
                     else r._alignment: tmp.strType;
  }

  return tmp;
}

/* Cast a range to a new range type.  The overload below throws when
   the original bounds and/or stride do not fit in the new type or 'strides'.
   TODO: should we allow 't' to be generic?
 */
@chpldoc.nodoc
operator :(r: range(?), type t: range(?)) where chpl_castIsSafe(r, t) {
  // note: the implementation is duplicated with the throwing overload
  checkBounds(t, r);
  checkEnumIdx(t, r);

  var tmp = new range(t.idxType, t.bounds, t.strides, true);
  type srcType = r.idxType,
       dstType = t.idxType,
    dstIntType = tmp.chpl_integralIdxType;

  tmp._low  = ( if r.hasLowBound() then chpl__idxToInt(r.lowBound:dstType)
                else r._low ): tmp.chpl_integralIdxType;

  tmp._high = ( if r.hasHighBound() then chpl__idxToInt(r.highBound:dstType)
                else r._high ): tmp.chpl_integralIdxType;

  if boundsChecking then
    if const error = r.chpl_checkStrides(t) then
      HaltWrappers.boundsCheckHalt(error.message());

  if ! tmp.hasParamStrideAltvalAld() {
    // see github.com/chapel-lang/chapel/issues/19269#issuecomment-1683228068
    tmp._stride = r.stride.safeCast(tmp.strType);

    tmp._alignment = if isNothingValue(r._alignment) then 0
                     else r._alignment: tmp.strType;
  }

  return tmp;
}

@chpldoc.nodoc
operator :(r: range(?), type t: range(?)) throws where !chpl_castIsSafe(r, t) {
  // note: the implementation is duplicated with the non-throwing overload
  checkBounds(t, r);
  checkEnumIdx(t, r);

  var tmp = new range(t.idxType, t.bounds, t.strides, true);
  type srcType = r.idxType,
       dstType = t.idxType,
    dstIntType = tmp.chpl_integralIdxType;

  tmp._low  = ( if r.hasLowBound() then chpl__idxToInt(r.lowBound:dstType)
                else r._low ): tmp.chpl_integralIdxType;

  tmp._high = ( if r.hasHighBound() then chpl__idxToInt(r.highBound:dstType)
                else r._high ): tmp.chpl_integralIdxType;

  if boundsChecking then
    if const error = r.chpl_checkStrides(t) then
      HaltWrappers.boundsCheckHalt(error.message());

  if ! tmp.hasParamStrideAltvalAld() {
    // see github.com/chapel-lang/chapel/issues/19269#issuecomment-1683228068
    tmp._stride = r.stride.safeCast(tmp.strType);

    tmp._alignment = if isNothingValue(r._alignment) then 0
                     else r._alignment: tmp.strType;
  }

  return tmp;
}

/////////// helpers for operator : and tryCast ///////////

/* cast 'from' to 'to', throwing an error if it does not fit */
inline proc chpl_throwingCast(type toType, from) throws {
  type fromType = from.type;
  // integral casts do not perform checking
  if isIntegral(fromType) && isIntegral(toType) {
    var error = from.chpl_checkValue(toType);
    if error != nil then throw error;
  }
  if isEnum(fromType) && isIntegral(toType) && toType != int {
    const e2i = from: int;
    return chpl_throwingCast(toType, e2i);
  }

  // in the cases not checked above, `:` will throw when needed
  // currently this arises only for integral --> enum
  return from: toType;
}

// avoid the overhead of throwing if it is not needed
inline proc chpl_throwingCast(type toType, from)
  where chpl_idxCastIsSafe(toType, from.type)
do return from: toType;

private proc checkBounds(type toType, from) {
  if toType.bounds != from.bounds then
    compilerError("cannot cast range from boundKind.",
                 from.bounds:string, " to boundKind.", toType.bounds:string);
}

private proc checkEnumBoolIdx(type toType, from) {
  checkEnumIdx(toType, from);
  if toType.idxType == bool && from.idxType != bool then // see #22905
    compilerError("'tryCast' to a range type with idxType=bool is not currently supported");
}

private proc checkEnumIdx(type toType, from) {
  type srcType = from.idxType,
       dstType = toType.idxType;

  // Generate a warning when casting between ranges and one of them is an
  // enum type (and they're not both the same enum type); see #22406 for
  // more information
  if chpl_warnUnstable &&
     ((isEnumType(srcType) || isEnumType(dstType)) && srcType != dstType) then
    compilerWarning("Casts between ranges involving 'enum' indices are currently unstable (see issue #22406); consider performing the conversion manually");
}

/* Returns 'nil' if 'this.stride' fits into 'toType.strides',
   otherwise returns an IllegalArgumentError. */
proc range.chpl_checkStrides(type toType): owned IllegalArgumentError? {
  if chpl_assignStrideIsUnsafe(toType.strides, this.strides) then
    compilerError("cannot cast range from strideKind.", this.strides:string,
                                     " to strideKind.", toType.strides:string);

  var needThrow = false;
  select toType.strides {
    when strideKind.one      do needThrow = (this.stride != 1);
    when strideKind.negOne   do needThrow = (this.stride != -1);
    when strideKind.positive do needThrow = (this.stride < 0);
    when strideKind.negative do needThrow = (this.stride > 0);
    when strideKind.any      do; // any stride is OK
  }

  if needThrow then
    return new IllegalArgumentError("bad cast from stride " +
      this.stride:string + " to strideKind." + toType.strides:string);
  else
    return nil;
}

// "safe" means "does not throw"
proc chpl_tryCastIsSafe(r: range(?), type t: range(?)) param do
  return chpl_assignStrideIsSafe(t.strides, r.strides)
     &&  chpl_idxCastIsSafe(to=t.idxType, from=r.idxType);

proc chpl_castIsSafe(r: range(?), type t: range(?)) param do
  // unsuitable r.stride causes halt, not throw => no chpl_assignStrideIsSafe
  return ! chpl_idxCastThrows(to=t.idxType, from=r.idxType);

proc chpl_idxCastIsSafe(type to, type from) param do
  return ( !( isUint(to) && isInt(from) ) //'aUint=anInt' is unsafe yet allowed
           && assignmentIsLegal(to, from) )
     ||  ( to == int && isBCPindex(from) );
     // todo: allow also to==int && 'from' is a fully-concrete enum

proc chpl_idxCastThrows(type to, type from) param do
  // Todo: fine-tune when `to :from` throws.
  // Some enum-related casts in ChapelBase also need such refinement.
  // This motivates the proposals in #23006.
  return ( to != from ) && ( isEnumType(to) || isEnumType(from) );

private proc isBCPindex(type t) param do
  return t == byteIndex || t == codepointIndex;

  //////////////////////////////////////////////////////////////////////////////////
  // Bounds checking
  //

  inline proc range.chpl_boundsCheck(other: range(?e,?b,?s))
    where b == boundKind.neither
  {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return false;
    }

    if ! this.isAligned()
      then return false;

    return true;
  }

  inline proc range.chpl_boundsCheck(other: range(?e,?b,?s))
  {
    if ! this.isAligned()
      then return false;

    var boundedOther = if other.isAligned() then
                        new range(
                          idxType, boundKind.both, other.strides,
                          if other.hasLowBound() then other._low else _low,
                          if other.hasHighBound() then other._high else _high,
                          other._stride, other._alignment)
                      // If 'other' is not aligned, we want to create an
                      // aligned range to pass it to contains() below, because
                      // otherwise it will always return false. For that, we
                      // copy alignment from 'this'.
                      // If 'this' is unit-stride, the alignment is zero,
                      // otherwise this.alignment potentially needs to be
                      // normalized w.r.t. other.stride.
                      else if this.hasPosNegUnitStride() then
                        new range(
                          idxType, boundKind.both, other.strides,
                          if other.hasLowBound() then other._low else _low,
                          if other.hasHighBound() then other._high else _high,
                          other._stride, 0:chpl__rangeStrideType(idxType))
                      else
                        new range(
                          idxType, boundKind.both, other.strides,
                          if other.hasLowBound() then other._low else _low,
                          if other.hasHighBound() then other._high else _high,
                          other._stride, this._alignment, true);
                          // Note: we don't really know why the strategy of
                          // copying the alignment from 'this' is correct.
                          // But we haven't been able to produce a counterexample
                          // where it doesn't work.
                          // If someone can prove or disprove this method that
                          // would be useful to know.

    return (boundedOther.sizeAs(uint) == 0) || contains(boundedOther);
  }

  // used in checkRankChange(args) where each args(i) can be
  // either a range or an individual index
  inline proc range.chpl_boundsCheck(other: idxType) do
    return contains(other);


  //################################################################################
  //# Member functions
  //#

  // Moves the low bound of the range up to the next alignment point.
  @chpldoc.nodoc
  /* private */ proc ref range.alignLow()
  {
    if boundsChecking && ! this.isAligned() then
      HaltWrappers.boundsCheckHalt("alignLow -- Cannot be applied to a range with ambiguous alignment.");

    if ! hasPosNegUnitStride() then _low = this.alignedLowAsInt;
    return this;
  }

  // Moves the high bound of the range down to the next alignment point.
  @chpldoc.nodoc
  /* private */ proc ref range.alignHigh()
  {
    if boundsChecking && ! this.isAligned() then
      HaltWrappers.boundsCheckHalt("alignHigh -- Cannot be applied to a range with ambiguous alignment.");

    if ! hasPosNegUnitStride() then _high = this.alignedHighAsInt;
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
  @unstable("range.indexOrder() is unstable and its behavior may change in the future")
  proc range.indexOrder(ind: idxType)
  {
    if boundsChecking && ! this.isAligned() then
      HaltWrappers.boundsCheckHalt("indexOrder -- Undefined on a range with ambiguous alignment.");

    if ! contains(ind) then return (-1):chpl_integralIdxType;
    if strides.isOne() {
      if this.hasLowBound() then
        return chpl__idxToInt(ind) - _low;
    } else if strides.isNegOne() {
      if this.hasHighBound() then
        return _high - chpl__idxToInt(ind);
    } else {
      if this.hasFirst() then
        return ((chpl__idxToInt(ind):strType - chpl__idxToInt(this.first):strType) / _stride):chpl_integralIdxType;
    }
    return (-1):chpl_integralIdxType;
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
  @unstable("range.orderToIndex() is unstable and its behavior may change in the future")
  proc range.orderToIndex(ord: integral): idxType
  {
    if boundsChecking {
      if !hasFirst() then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on a range that has no first index");

      if ! isAligned() then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on a range that is ambiguously aligned");

      if ord < 0 then
        HaltWrappers.boundsCheckHalt("invoking orderToIndex on a negative integer: " + ord:string);

      if this.bounds == boundKind.both && ord >= this.sizeAs(uint) then
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
   @unstable("range.translate() is unstable and its behavior may change in the future")
  inline proc range.translate(offset: integral) do
    return this + offset;

  pragma "last resort"
  @chpldoc.nodoc
  inline proc range.translate(i)
  {
    compilerError("offsets must be of integral type");
  }

  @chpldoc.nodoc
  inline proc range.translate(offset: integral) where chpl__singleValIdxType(idxType) {
    compilerError("can't apply '.translate()' to a range whose 'idxType' only has one value");

  }


    // Returns an expanded range, or a contracted range if offset < 0.
  // The existing absolute alignment is preserved.
  @chpldoc.nodoc
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
  @unstable("range.expand() is unstable and its behavior may change in the future")
  proc range.expand(offset: integral)
  {
    const i = offset.safeCast(chpl__signedType(chpl_integralIdxType));
    return new range(idxType, bounds, strides,
                     _low-i,
                     _high+i,
                     _stride, _alignment);
  }

  @chpldoc.nodoc
  proc range.expand(offset: integral) where chpl__singleValIdxType(idxType)
  {
    compilerError("can't apply '.expand()' to a range whose 'idxType' only has one value");
  }

  // Return an interior portion of this range.
  pragma "last resort"
  @chpldoc.nodoc
  proc range.interior(offset: integral) where bounds != boundKind.both
  {
    compilerError("interior is not supported on unbounded ranges");
  }

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
   @unstable("range.interior() is unstable and its behavior may change in the future")
  proc range.interior(offset: integral)
  {
    if boundsChecking then
      if abs(offset):uint > this.sizeAs(uint) then
        HaltWrappers.boundsCheckHalt("can't compute the interior " + offset:string + " elements of a range with size " + this.sizeAs(uint):string);

    const i = (abs(offset)).safeCast(chpl_integralIdxType);
    if offset < 0 then
      return new range(idxType, bounds, strides,
                       _low, _low - 1 + i,
                       _stride, _alignment);
    if offset > 0 then
      return new range(idxType, bounds, strides,
                       _high + 1 - i, _high,
                       _stride, _alignment);
    // if i == 0 then
    return new range(idxType, bounds, strides,
                     _low, _high,
                     _stride, _alignment);
  }

  pragma "last resort"
  @chpldoc.nodoc
  proc range.exterior(offset: integral) where bounds != boundKind.both
  {
    compilerError("exterior is not supported on unbounded ranges");
  }

  /* Returns a range with ``offset`` elements from the exterior portion of this
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
  @unstable("range.exterior() is unstable and its behavior may change in the future")
  proc range.exterior(offset: integral)
  {
    const i = (abs(offset)).safeCast(chpl_integralIdxType);
    if offset < 0 then
      return new range(idxType, bounds, strides,
                       _low - i,
                       _low - 1,
                       _stride, _alignment);
    if offset > 0 then
      return new range(idxType, bounds, strides,
                       _high + 1,
                       _high + i,
                       _stride, _alignment);
    // if i == 0 then
    return new range(idxType, bounds, strides,
                     _low, _high,
                     _stride, _alignment);
  }

  @chpldoc.nodoc
  proc range.exterior(offset: integral) where chpl__singleValIdxType(idxType)
  {
    compilerError("can't apply '.exterior()' to a range whose 'idxType' only has one value");
  }

  //################################################################################
  //# Syntax Functions
  //# These are special functions known to the compiler parser.
  //#

  // Assignment
  @chpldoc.nodoc
  inline operator =(ref r1: range(?), r2: range(?))
  {
    if ! assignmentIsLegal(r1.idxType, r2.idxType) then
      compilerError("assigning to a range with idxType ", r1.idxType:string,
                           " from a range with idxType ", r2.idxType:string,
                           " without an explicit cast");
    if r1.bounds != r2.bounds then
      compilerError("assigning to a range with boundKind.", r1.bounds:string,
                           " from a range with boundKind.", r2.bounds:string,
                           " without an explicit cast");
    if ! chpl_assignStrideIsSafe(r1, r2) then
      compilerError("assigning to a range with strideKind.", r1.strides:string,
                           " from a range with strideKind.", r2.strides:string,
                           " without an explicit cast");

    r1._low = r2._low: r1.chpl_integralIdxType;
    r1._high = r2._high: r1.chpl_integralIdxType;

    if ! r1.hasParamStrideAltvalAld() {
      r1._stride = r2.stride;
      r1._alignment = if isNothingValue(r2._alignment) then 0
                      else r2._alignment;
    }
  }

  /////////// operators + and - ///////////

  @chpldoc.nodoc
  operator +(r1: range(?), r2: range(?)) do
    compilerError("range addition is currently not supported");

  @chpldoc.nodoc
  operator -(r1: range(?), r2: range(?)) do
    compilerError("range subtraction is currently not supported");

  //
  // Shifts and entire range to the right or left.
  // The alignment shifts along with the range.
  //
  @chpldoc.nodoc
  @unstable("'+' on ranges is unstable and may change in the future")
  inline operator +(r: range(?e, ?b, ?s), i: integral)
  {
    return new range(e, b, s,
                     r._low + i,
                     r._high + i,
                     r._stride,
                     chpl__idxToInt(r.alignment)+i,
                     r.isAligned());
  }

  // TODO can this be removed?
  @chpldoc.nodoc
  @unstable("'+=' on ranges is unstable and may change in the future")
  inline operator +=(ref r: range(?e, ?b, ?s), offset: integral)
  {
    r = r + offset;
  }

  @chpldoc.nodoc
  @unstable("'+' on ranges is unstable and may change in the future")
  inline operator +(i:integral, r: range(?e,?b,?s)) do
    return r + i;

  @chpldoc.nodoc
  @unstable("'-' on ranges is unstable and may change in the future")
  inline operator -(r: range(?e,?b,?s), i: integral)
  {
    return new range(e, b, s,
                     r._low - i,
                     r._high - i,
                     r._stride,
                     chpl__idxToInt(r.alignment)-i,
                     r.isAligned());
  }

  // TODO can this be removed?
  @chpldoc.nodoc
  @unstable("'-=' on ranges is unstable and may change in the future")
  inline operator -=(ref r: range(?e, ?b, ?s), offset: integral)
  {
    r = r - offset;
  }

  /////////// operators 'by', 'align', '#' ///////////

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

  // Implements the definition of 'by' in #19717 comment from 2022-10-12
  //
  proc chpl_by_help(r: range(?i,?b,?s), step, param newStrides) {
    const lw = r._low,
          hh = r._high,
          st = r.stride * step:r.strType;

    compilerAssert(! newStrides.isPosNegOne()); // handled directly in 'by'

    if ! r.isAligned() then return
      if st == 1 || st == -1 then newZeroAlmtRange() else newUnalignedRange();

    if isPositiveStride(newStrides, st) then
      // start from the low index
      return if hasLowBoundForIter(r)
             // inlined: newAlignedRange(r.chpl_alignedLowAsIntForIter)
             // because Dyno can't helper capturing nested functions.
             then new range(i, b, newStrides, lw, hh, st,
                            r.chpl_alignedLowAsIntForIter, true, true)
             else if st == 1 then newZeroAlmtRange() else newUnalignedRange();
    else
      // start from the high index
      return if hasHighBoundForIter(r)
             // inlined: newAlignedRange(r.chpl_alignedHighAsIntForIter)
             // because Dyno can't helper capturing nested functions.
             then new range(i, b, newStrides, lw, hh, st,
                            r.chpl_alignedHighAsIntForIter, true, true)
             else if st == -1 then newZeroAlmtRange() else newUnalignedRange();

    proc newAlignedRange(alignment) do
      return new range(i, b, newStrides, lw, hh, st, alignment, true, true);
    proc newZeroAlmtRange() do // if |st|==1 then the range is always aligned
      return new range(i, b, newStrides, lw, hh, st, 0: r.strType);
    inline proc newUnalignedRange() do
      return new range(i, b, newStrides, lw, hh, st, unalignedMark: r.strType);
  }

  // This is the definition of the 'by' operator for ranges.
  @chpldoc.nodoc
  inline operator by(r : range(?), step) {
    chpl_range_check_stride(step, r.idxType);
    param newStrides = if ! isUint(step) then strideKind.any
                       else chpl_strideProduct(r.strides, strideKind.positive);
    return chpl_by_help(r, step, newStrides);
  }

  // We want to warn the user at compiler time if they had an invalid param
  // stride rather than waiting until runtime.
  @chpldoc.nodoc
  inline operator by(r : range(?), param step) {
    chpl_range_check_stride(step, r.idxType);

    // streamline the simple cases
    if step == 1 then return r;

    if step == -1 then return if r.hasParamStrideAltvalAld()
      then new range(r.idxType, r.bounds, chpl_strideProduct(r, step),
                     r._low, r._high, none, none)
      else new range(r.idxType, r.bounds, chpl_strideProduct(r, step),
                     r._low, r._high, -r._stride, r._alignment);

    return chpl_by_help(r, step, chpl_strideProduct(r, step));
  }

  pragma "last resort"
  @chpldoc.nodoc
  inline operator by(r, step) {
    compilerError("cannot apply 'by' to '", r.type:string, "'");
  }

  // This is the definition of the 'align' operator for ranges.
  // It produces a new range with the specified alignment.

  @chpldoc.nodoc
  inline operator align(r : range(?i, ?b, ?s), algn: i)
  {
    return new range(r.idxType, r.bounds, r.strides, r._low, r._high,
                     r._stride, chpl__idxToInt(algn), true);
  }

  pragma "last resort"
  @chpldoc.nodoc
  inline operator align(r : range(?i, ?b, ?s), algn) {
    compilerError("can't align a range with idxType ", i:string,
                  " using a value of type ", algn.type:string);
  }

  pragma "last resort"
  @chpldoc.nodoc
  inline operator align(r, algn) {
    compilerError("cannot apply 'align' to '", r.type:string, "'");
  }

  /* Returns a range whose alignment is this range's first index plus ``offset``.
     If the range has no first index, a runtime error is generated.
   */
  @unstable("range.offset() is unstable and its behavior may change in the future")
  proc range.offset(in offset: integral)
  {
    if (chpl__singleValIdxType(idxType)) {
      compilerError("can't apply '.offset()' to a range whose 'idxType' only has one value");
    }

    var offs = offset.safeCast(chpl_integralIdxType);
    if hasUnitStride() {
      if !this.hasLowBound() then
        compilerError("can't invoke 'offset' on an unstrided range with no low bound");
      else
        compilerWarning("invoking 'offset' on an unstrided range has no effect.");
      offs = 0;
    }

    if boundsChecking && !hasFirst() then
      HaltWrappers.boundsCheckHalt("invoking 'offset' on a range without a first index");

    return new range(idxType, bounds, strides, _low, _high, stride,
                     // here's the new alignment
                     this.firstAsInt + offs, true);
  }


  // Slicing, implementing the slice semantics in #20462.
  // Return the intersection of this and other.
  @chpldoc.nodoc
  proc const range.this(other: range(?))
  {
    // Disallow slicing of an unaligned range, at least for now.
    if ! this.isAligned() then
      HaltWrappers.unimplementedFeatureHalt("slicing of an unaligned range");

    // the new idxType, chpl_integralIdxType, strType are inherited from `this`

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

    if newBoundKind != boundKind.both &&
       ! this.strides.isPosNegOne() && ! other.strides.isPosNegOne() then
      compilerWarning("slicing a ", this.type:string,
                      " with a ", other.type:string,
                      " might produce an empty range and result in a halt");

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
    if (isUintType(chpl_integralIdxType)) { if (lo1 < 0) then lo1 = 0; }

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
    var newlo = myMax(lo1, lo2):chpl_integralIdxType;
    var newhi = myMin(hi1, hi2):chpl_integralIdxType;

    if (emptyIntersection) {
      newlo = chpl__defaultLowBound(idxType, newBoundKind);
      newhi = chpl__defaultHighBound(idxType, newBoundKind);
    }

    /////////// Step 2: combine the strides ///////////

    // we have abs strides of args in st1, st2
    // abs(result.stride) = LCM(st1, st2)
    // sign(result.stride) = sign(this.stride) * sign(other.stride)

    param newStrideKind = chpl_strideProduct(this, other);

    var newStride = st1, newAbsStride = st1;
    var gcd, x: strType;

    if ! newStrideKind.isPosNegOne() {
      if st1 == st2 {
        gcd = st1;
      } else {
        // do we need casts to  something about the types of st1, st2?
        (gcd, x) = chpl__extendedEuclid(st1, st2);
        newStride = st1 / gcd * st2;  // divide first to avoid overflow
        newAbsStride = newStride;
      }

      // sign of resulting stride = sign of 'this' * sign of 'other'
      if this.hasPositiveStride() && other.hasNegativeStride() ||
        this.hasNegativeStride() && other.hasPositiveStride()
      then
        newStride = -newStride;

    } else {  // newStrideKind.isPosNegOne()
      compilerAssert(this.hasPosNegUnitStride());
      // we must have newStride == newAbsStride == 1
      if newStrideKind.isNegOne() then newStride = -1;
    }

    /////////// allocate the result ///////////

    var result = new range(idxType, newBoundKind, newStrideKind,
                           newlo, newhi, newStride, 0:strType);

    /////////// Step 3: choose the alignment ///////////

    // We require that `this` be unambiguous. The result will always be, too.

    if ! newStrideKind.isPosNegOne() && newAbsStride > 1 {
      var al1 = chpl__idxToInt(this.alignment): int;
      var al2 = chpl__idxToInt(other.alignment): int;
      var newAlignmentIsInAl2 = false;

      if ! other.isAligned() {
        // choice (C) in the comment from 2021-09-30 in #20462
        al2 = al1;
        if al2 < 0 then al2 += st1;

        // when st1 == st2 * (some int), there is no room for instability
        // because the resulting sequence is unique
        if st2 > gcd {
          // the rule in the comment from 2023-02-15 or -02-16 in #20462
          if result.hasLowBound() && result.hasPositiveStride() {
            if (newlo - al1) % st1 == 0 {
              al2 = ( newlo % newAbsStride ):int;
              newAlignmentIsInAl2 = true;
            }
          } else
          if result.hasHighBound() && result.hasNegativeStride() {
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
        result._alignment = al2: strType;
      } else
      if (al2 - al1) % g != 0 then
      {
        // empty intersection, return degenerate result
        if boundsChecking && result.bounds != boundKind.both then
          HaltWrappers.boundsCheckHalt("could not represent range slice - it needs to be empty, but the slice type is not bounded");
        result._low = chpl__defaultLowBound(idxType, newBoundKind);
        result._high = chpl__defaultHighBound(idxType, newBoundKind);
        result._alignment = ( if this.hasPositiveStride()
                              then result._low else result._high ): strType;

        // todo: what should be _low, _high, _alignment of an empty range?
        // for now, ensure that printing it does not print 'align nnn'
      }
      else
      { // non-empty intersection

        // x and/or the diff may negative, even with a uint source range.
        var offset = (al2 - al1) * x;
        // offset is in the range [-(lcm-1), lcm-1]

        result._alignment = al1:strType + offset:strType * st1 / g;

        if result._alignment < 0 {
          result._alignment += newAbsStride;
          if boundsChecking then assert(result._alignment >= 0);
        } else if result._alignment >= newAbsStride {
          result._alignment -= newAbsStride;
          if boundsChecking then assert(result._alignment < newAbsStride);
        }
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
    if boundsChecking && ! r.isAligned() then
      boundsCheckHalt("count -- Cannot count off elements from a range which is ambiguously aligned.");

    type resultType = r.chpl_integralIdxType;
    type strType = chpl__rangeStrideType(resultType);

    proc absSameType(r, type resultType) {
      if r.hasNegativeStride() {
        return (-r.stride):resultType;
      } else {
        return r.stride:resultType;
      }
    }

    if (count == 0) {
      if (r.hasLowBound()) {
        return new range(idxType = r.idxType,
                         bounds = boundKind.both,
                         strides = r.strides,
                         _low = r._low,
                         _high = r._low - absSameType(r, resultType),
                         _stride = r.stride,
                         alignmentValue = r._alignment);
      } else if (r.hasHighBound()) {
        return new range(idxType = r.idxType,
                         bounds = boundKind.both,
                         strides = r.strides,
                         _low = r._high + absSameType(r, resultType),
                         _high = r._high,
                         _stride = r.stride,
                         alignmentValue = r._alignment);
      } else {
        return new range(idxType = r.idxType,
                         bounds = boundKind.both,
                         strides = r.strides,
                         _low  = chpl__defaultLowBound(r.idxType,
                                                       boundKind.both),
                         _high = chpl__defaultHighBound(r.idxType,
                                                        boundKind.both),
                         _stride = r.stride,
                         alignmentValue = r._alignment);
      }
    }

    if boundsChecking {
      if count > 0 && !r.hasFirstForIter() then
        boundsCheckHalt("With a positive count, the range must have a first index.");
      if count < 0 && !r.hasLastForIter() then
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

    if ! r.hasPosNegUnitStride() {
      if r.hasLowBound() && lo < r._low then lo = r._low;
      if r.hasHighBound() && hi > r._high then hi = r._high;
    }

    return new range(idxType = r.idxType,
                     bounds = boundKind.both,
                     strides = r.strides,
                     _low = lo,
                     _high = hi,
                     r._stride, r._alignment);
  }

  // TODO: Do we really want to support this?  Arkouda currently relies
  // on it, and bools don't coerce to generic 'integral' arguments so
  // dropping it broke the Arkouda build.  On one hand, it seems odd to
  // accept a boolean as a count value.  On the other hand, we permit
  // bools to coerce to ints in most cases, so this gives a similar
  // end-user experience
  @chpldoc.nodoc
  operator #(r:range(?), count:bool) {
    return chpl_count_help(r, count:int);
  }

  @chpldoc.nodoc
  operator #(r:range(?), count:integral) {
    return chpl_count_help(r, count);
  }

  pragma "last resort"
  @chpldoc.nodoc
  operator #(r: range(?i), count) {
    compilerError("can't apply '#' to a range with idxType ",
                  i:string, " using a count of type ",
                  count.type:string);
  }

  pragma "last resort"
  @chpldoc.nodoc
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

  @chpldoc.nodoc
  proc range.checkIfIterWillOverflow(shouldHalt=true) {
    if isFiniteIdxType(idxType) then
      return false;
    return chpl_checkIfRangeIterWillOverflow(this.chpl_integralIdxType, this._low, this._high,
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

  iter chpl_direct_range_iter(low: int(8), high: int(8)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:int(8)) do
      yield i;
  }
  iter chpl_direct_range_iter(low: int(16), high: int(16)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:int(16)) do
      yield i;
  }
  iter chpl_direct_range_iter(low: int(32), high: int(32)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:int(32)) do
      yield i;
  }
  iter chpl_direct_range_iter(low: int(64), high: int(64)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:int(64)) do
      yield i;
  }

  iter chpl_direct_range_iter(low: uint(8), high: uint(8)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:uint(8)) do
      yield i;
  }
  iter chpl_direct_range_iter(low: uint(16), high: uint(16)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:uint(16)) do
      yield i;
  }
  iter chpl_direct_range_iter(low: uint(32), high: uint(32)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:uint(32)) do
      yield i;
  }
  iter chpl_direct_range_iter(low: uint(64), high: uint(64)) {
    for i in chpl_direct_param_stride_range_iter(low, high, 1:uint(64)) do
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
  where !(chpl_isValidRangeIdxType(low.type) && chpl_isValidRangeIdxType(high.type)) {
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

  iter chpl_direct_strided_range_iter(low: int(8), high: int(8),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(16), high: int(16),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(32), high: int(32),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(64), high: int(64),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }

  iter chpl_direct_strided_range_iter(low: uint(8), high: uint(8),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(16), high: uint(16),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(32), high: uint(32),
                                      stride: integral) {
    const r = low..high by stride;
    for i in r do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(64), high: uint(64),
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

  iter chpl_direct_strided_range_iter(low: int(8), high: int(8), param stride : integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(16), high: int(16), param stride : integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(32), high: int(32), param stride : integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(64), high: int(64), param stride : integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_strided_range_iter(low: uint(8), high: uint(8), param stride: integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(16), high: uint(16), param stride: integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(32), high: uint(32), param stride: integral) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(64), high: uint(64), param stride: integral) {
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

  iter chpl_direct_strided_range_iter(low: int(8), high: int(8), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(16), high: int(16), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(32), high: int(32), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: int(64), high: int(64), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_strided_range_iter(low: uint(8), high: uint(8), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(16), high: uint(16), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(32), high: uint(32), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }
  iter chpl_direct_strided_range_iter(low: uint(64), high: uint(64), stride: uint(?w2)) {
    for i in chpl_direct_pos_stride_range_iter(low, high, stride) do yield i;
  }


  // cases for when stride isn't valid
  iter chpl_direct_strided_range_iter(low: int(8), high: int(8), stride) do compilerError("can't apply 'by' to a range with idxType ", int(8) : string, " using a step of type ", stride.type : string);
  iter chpl_direct_strided_range_iter(low: int(16), high: int(16), stride) do compilerError("can't apply 'by' to a range with idxType ", int(16) : string, " using a step of type ", stride.type : string);
  iter chpl_direct_strided_range_iter(low: int(32), high: int(32), stride) do compilerError("can't apply 'by' to a range with idxType ", int(32) : string, " using a step of type ", stride.type : string);
  iter chpl_direct_strided_range_iter(low: int(64), high: int(64), stride) do compilerError("can't apply 'by' to a range with idxType ", int(64) : string, " using a step of type ", stride.type : string);

  iter chpl_direct_strided_range_iter(low: uint(8), high: uint(8), stride) do compilerError("can't apply 'by' to a range with idxType ", uint(8) : string, " using a step of type ", stride.type : string);
  iter chpl_direct_strided_range_iter(low: uint(16), high: uint(16), stride) do compilerError("can't apply 'by' to a range with idxType ", uint(16) : string, " using a step of type ", stride.type : string);
  iter chpl_direct_strided_range_iter(low: uint(32), high: uint(32), stride) do compilerError("can't apply 'by' to a range with idxType ", uint(32) : string, " using a step of type ", stride.type : string);
  iter chpl_direct_strided_range_iter(low: uint(64), high: uint(64), stride) do compilerError("can't apply 'by' to a range with idxType ", uint(64) : string, " using a step of type ", stride.type : string);

  // case for when low and high aren't compatible types and can't be coerced
  iter chpl_direct_strided_range_iter(low, high, stride)
  where !(chpl_isValidRangeIdxType(low.type) && chpl_isValidRangeIdxType(high.type)) {
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
  where !(chpl_isValidRangeIdxType(low.type) && chpl_isValidRangeIdxType(count.type)) {
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
  pragma "order independent yielding loops"
  @chpldoc.nodoc
  iter range.these(): nothing where !hasLowBoundForIter(this) && !hasHighBoundForIter(this) {
    compilerError("iteration over a range with no bounds");
  }

  private inline proc boundsCheckUnboundedRange(r: range(?)) {
    if boundsChecking {
      if ! r.hasFirstForIter() then
        HaltWrappers.boundsCheckHalt("iteration over range that has no first index");

      if hasAmbiguousAlignmentForIter(r) then
        HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");
    }
  }

  // The serial iterator for 'lo.. [by s]' ranges
  pragma "order independent yielding loops"
  @chpldoc.nodoc
  iter range.these() where hasLowBoundForIter(this) && !hasHighBoundForIter(this) {

    boundsCheckUnboundedRange(this);
    if strides.isNegative() then
      compilerError("iteration over a range with no first index");

    // This iterator could be split into different cases depending on the
    // stride like the bounded iterators. However, all that gets you is the
    // ability to use .low over .first. The additional code isn't
    // worth it just for that.
    var i: chpl_integralIdxType;
    const start = chpl__idxToInt(this.first);
    const end = max(chpl_integralIdxType) - stride: chpl_integralIdxType;

    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive("<=", i, end),
                      __primitive("+=", i, stride: chpl_integralIdxType)) {
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
  pragma "order independent yielding loops"
  @chpldoc.nodoc
  iter range.these() where !hasLowBoundForIter(this) && hasHighBoundForIter(this) {

    boundsCheckUnboundedRange(this);
    if strides.isPositive() then
      compilerError("iteration over a range with no first index");

    // Apart from the computation of 'end' and the comparison used to
    // terminate the C for loop, this iterator follows the bounded-low
    // case above.  See it for additional comments.
    var i: chpl_integralIdxType;
    const start = chpl__idxToInt(this.first);
    const end = min(chpl_integralIdxType) - stride: chpl_integralIdxType;
    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive(">=", i, end),
                      __primitive("+=", i, stride: chpl_integralIdxType)) {
      yield chpl_intToIdx(i);
    }
    if i < end {
      if isIntegralType(idxType) then
        //  yield chpl_intToIdx(i);
        halt("Loop over unbounded range surpassed representable values");
    }
  }

  // A bounded and strided range iterator
  pragma "order independent yielding loops"
  @chpldoc.nodoc
  iter range.these()
    where hasLowBoundForIter(this) && hasHighBoundForIter(this) && ! hasPosNegUnitStride() {
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
      var i: chpl_integralIdxType;
      const start = chpl_firstAsIntForIter;
      const end: chpl_integralIdxType = if this._low > this._high then start
                              else chpl_lastAsIntForIter + stride: chpl_integralIdxType;
      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("!=", i, end),
                        __primitive("+=", i, stride: chpl_integralIdxType)) {
        yield chpl_intToIdx(i);
      }
    } else {
      foreach i in this.generalIterator() do yield i;
    }
  }

  // A bounded and non-strided (stride = 1) range iterator
  pragma "order independent yielding loops"
  @chpldoc.nodoc
  iter range.these()
  where hasLowBoundForIter(this) && hasHighBoundForIter(this) && hasPosNegUnitStride() {
    if chpl__singleValIdxType(idxType) {
      if _low > _high then return;
    }
    if (useOptimizedRangeIterators) {
      if boundsChecking then checkIfIterWillOverflow();

      // don't need to check if !isAligned() since stride is one

      // can use low/high instead of first/last since stride is one
      var i: chpl_integralIdxType;
      const start = chpl__idxToInt(lowBoundForIter(this));
      const end = chpl__idxToInt(highBoundForIter(this));

     if stride == 1 then
      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("<=", i, end),
                        __primitive("+=", i, 1: chpl_integralIdxType)) {
        yield chpl_intToIdx(i);
      }
     else // stride == -1
      while __primitive("C for loop",
                        __primitive( "=", i, end),
                        __primitive(">=", i, start),
                        __primitive("-=", i, 1: chpl_integralIdxType)) {
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
  pragma "order independent yielding loops"
  @chpldoc.nodoc
  iter range.generalIterator() {
    if boundsChecking && hasAmbiguousAlignmentForIter(this) then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    var i: chpl_integralIdxType;
    const start = this.first;
    const end = if this._low > this._high then start else this.last;

    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive(">=", highBoundForIter(this), lowBoundForIter(this)),  // execute at least once?
                      __primitive("+=", i, stride: chpl_integralIdxType)) {
      yield i;
      if i == end then break;
    }
  }

  //################################################################################
  //# Parallel Iterators
  //#

  @chpldoc.nodoc
  iter range.these(param tag: iterKind) where tag == iterKind.standalone
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

    const len = this.sizeAs(chpl_integralIdxType);
    const numChunks = if __primitive("task_get_serial") then
                      1 else _computeNumChunks(len);

    if debugChapelRange {
      chpl_debug_writeln("*** RI: length=", len, " numChunks=", numChunks);
    }

    coforall chunk in 0..#numChunks {
      if ! hasUnitStride() {
        // TODO: find a way to avoid this densify/undensify for strided
        // ranges, perhaps by adding knowledge of alignment to _computeBlock
        // or using an aligned range
        const (lo, hi) = _computeBlock(len, numChunks, chunk, len-1);
        const mylen = hi - (lo-1);
        var low = orderToIndex(lo);
        var high = chpl_intToIdx(chpl__idxToInt(low):strType + stride * (mylen - 1):strType);
        if hasNegativeStride() then low <=> high;
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

  @chpldoc.nodoc
  iter range.these(param tag: iterKind) where tag == iterKind.leader
  {
    if !(hasLowBoundForIter(this) && hasHighBoundForIter(this)) then
      compilerError("parallel iteration is not currently supported over ranges without bounds");

    if boundsChecking && hasAmbiguousAlignmentForIter(this) then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    if debugChapelRange then
      chpl_debug_writeln("*** In range leader:"); // ", this);
    const numSublocs = here._getChildCount();

    var v = this.chpl_sizeAsForIter(chpl_integralIdxType);
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

  @chpldoc.nodoc
  iter range.these(param tag: iterKind, followThis) where tag == iterKind.follower
  {
    if boundsChecking && hasAmbiguousAlignmentForIter(this) then
      HaltWrappers.boundsCheckHalt("these -- Attempt to iterate over a range with ambiguous alignment.");

    if bounds == boundKind.neither then
      compilerError("iteration over a range with no bounds");
    if strides.isPositive() && bounds == boundKind.high then
      compilerError("iteration over a range with no first index");
    if strides.isNegative() && bounds == boundKind.low then
      compilerError("iteration over a range with no first index");

    if followThis.size != 1 then
      compilerError("rank mismatch in zippered iteration (can't zip a " +
                    followThis.size:string + "D expression with a range, which is 1D)");

    if debugChapelRange then
      chpl_debug_writeln("In range follower code: Following ", followThis);

    var myFollowThis = followThis(0);

    if debugChapelRange then
      chpl_debug_writeln("Range = ", myFollowThis);

    if boundsChecking && ! this.hasFirstForIter() {
      if this.isEmpty() {
        if ! myFollowThis.isEmpty() then
          HaltWrappers.boundsCheckHalt("size mismatch in zippered iteration");
      } else {
        HaltWrappers.boundsCheckHalt("iteration over a range with no first index");
      }
    }
    if boundsChecking && ! myFollowThis.hasFirstForIter() {
      if ! (myFollowThis.isAligned() && myFollowThis.isEmpty()) then
        HaltWrappers.boundsCheckHalt("zippered iteration over a range with no first index");
    }

    param newStrides = chpl_strideProduct(this, myFollowThis);

    if (myFollowThis.bounds == boundKind.both &&
        myFollowThis.hasPosNegUnitStride()     ) ||
       myFollowThis.hasLastForIter()
    {
      const flwlen = myFollowThis.sizeAs(myFollowThis.chpl_integralIdxType);
      if boundsChecking {
        if this.hasLastForIter() {
          // this check is for typechecking only
          if this.bounds != boundKind.both && ! isFiniteIdxType(idxType) then
            assert(false, "hasFirstForIter && hasLastForIter do not imply a range is bounded");
        }
        if flwlen != 0 then
          if this.bounds == boundKind.both && myFollowThis.highBound >= this.sizeAs(uint) then
            HaltWrappers.boundsCheckHalt("size mismatch in zippered iteration");
      }

      var r: range(idxType, strides=newStrides);

      if flwlen != 0 {
        var low = this.orderToIndex(myFollowThis.first);
        var high = this.orderToIndex(myFollowThis.last);
        if isNegativeStride(newStrides, this.stride * myFollowThis.stride)
          then low <=> high;
        r.chpl_setFields(low, high, this.stride * myFollowThis.stride);
      }

      if debugChapelRange then
        chpl_debug_writeln("Expanded range = ",r);

      for i in r do
        yield i;
    }
    else // ! myFollowThis.hasLastForIter()
    {
      // WARNING: this case has not been tested
      if boundsChecking && this.hasLastForIter() then
        HaltWrappers.zipLengthHalt("zippered iteration where a bounded range follows an unbounded iterator");

      const first  = this.orderToIndex(myFollowThis.first);
      const stride = this.stride * myFollowThis.stride;

      if isPositiveStride(newStrides, stride)
      {
        const r = first.. by if newStrides.isOne() then 1 else stride:uint;

        if debugChapelRange then
          chpl_debug_writeln("Expanded range = ",r);

        for i in r do
          yield i;
      }
      else
      {
        var r: range(idxType, boundKind.high, if newStrides.isNegOne()
                       then strideKind.negOne else strideKind.negative);
        r.chpl_setFields(/*dummy*/0:idxType, first, stride);

        if debugChapelRange then
          chpl_debug_writeln("Expanded range = ",r);

        for i in r do
          yield i;
      }
    } // if myFollowThis.hasLastForIter()
  }


  //################################################################################
  //# Utilities
  //#

  @chpldoc.nodoc
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

      if x.stride != -1 && x.isAligned() && ! x.chpl_isNaturallyAligned() then
      // Write out the alignment only if it differs from natural alignment.
      // We take alignment modulo the stride for consistency.
       ret += " align " + x.alignment:string;
    }
    return ret;
  }


  //################################################################################
  //# Internal helper functions.
  //#

  inline proc range.chpl__unTranslate(i) do
    return this - i;

  //////////////////////////////////////////////////////////////////////////////////
  // TODO: hilde
  // These functions should be migrated to a more global location.

  //
  // Return the number in the range 0 <= result < b that is congruent to a (mod b)
  //
  proc chpl__mod(dividend:integral, modulus:integral)
    where numBits(dividend.type) >= numBits(modulus.type)
  {
    type t = modulus.type;
    var m = modulus;
    // The extra check for `m != min(t)` is required to avoid an optimizer
    // (especially LLVM) determining that `-min(t)` is undefined and inserting
    // `poison`.
    if isIntType(t) && m < 0 && m != min(t) then m = -m;

    var tmp = dividend % (m: dividend.type);
    if isInt(dividend) then
      if tmp < 0 then tmp += (m: dividend.type);

    return tmp;
  }

  proc chpl__mod(dividend:integral, modulus:integral)
    where numBits(dividend.type) < numBits(modulus.type) && isInt(modulus)
  {
    type t = modulus.type;
    var m = modulus;
    // The extra check for `m != min(t)` is required to avoid an optimizer
    // (especially LLVM) determining that `-min(t)` is undefined and inserting
    // `poison`.
    if isIntType(t) && m < 0 && m != min(t) then m = -m;

    var tmp = (dividend: t) % m;
    if isInt(dividend) then
      if tmp < 0 then tmp += m;

    return tmp: dividend.type;
  }


  // Performs safe subtraction in modulo space.
  // "Safe" means that none of the calculations yields a result outside of
  // [0, abs(mod)].
  // It is assumed that abs(mod) can be represented in the type of the minuend.
  // We currently assume that the built-in modulo operator always returns an
  // integer in the range [0, mod-1].
  //
  // In general, this can return an incorrect value if modulus is a bigger
  // value than can fit into minuend.type. This function should only be used
  // in controlled circumstances.
  //
  proc chpl__diffMod(minuend : integral,
                     subtrahend : integral,
                     modulus : integral) : minuend.type
    where minuend.type == subtrahend.type
  {
    const m = abs(modulus);

    var minMod = chpl__mod(minuend, m);
    var subMod = chpl__mod(subtrahend, m);

    return if minMod < subMod
      then m: minuend.type  - (subMod - minMod)
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
  // Use example: low + i:chpl_integralIdxType * stride.
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

  proc chpl__idxTypeToIntIdxType(type idxType) type {
    if isIntegralType(idxType) {
      // integer idxTypes are their own integer idxType
      return idxType;
    } else {
      // other types (bool, enum, byteIndex, codepointIndex, ...) use 'int'
      return int;
    }
  }

  // convenience method for converting integers to index types in
  // order to make use of range.idxType.  Note that this method uses
  // a single underscore where the standalone versions use double
  // underscores.  Reason: otherwise, the calls in range.init() try
  // to call the method version, which isn't currently legal.
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
