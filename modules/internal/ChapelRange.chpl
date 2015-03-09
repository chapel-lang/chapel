/*
 * Copyright 2004-2015 Cray Inc.
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
  
  use Math; // for abs().
  
  // Turns on range iterator debugging.
  config param debugChapelRange = false;

  config param useOptimizedRangeIterators = true;

  enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

  proc indexToStrideType(type idxType) type  return chpl__signedType(idxType);
  
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
  
  pragma "range"
  record range
  {
    type idxType = int;                            // element type
    param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
    param stridable: bool = false;                 // range can be strided
  
    var _low: idxType = 1;                         // lower bound
    var _high: idxType = 0;                        // upper bound
    var _stride: strType = 1;                      // signed stride of range
    var _alignment: idxType = 0;                   // alignment
    var _aligned : bool = false;

    proc strType type  return indexToStrideType(idxType);
    inline proc low  return _low;
    inline proc high return _high;
    inline proc stride       where stridable  return _stride;
           proc stride param where !stridable return 1 : strType;
    inline proc alignment return _alignment;
    inline proc aligned   return _aligned;
  
    // TODO: hilde 2011/03/31
    // This should be a pragma and not a var declaration.  
    var _promotionType: idxType;                   // enables promotion

    inline proc size return this.length;
  }
  
  //################################################################################
  //# Constructors
  //#
  
  // Declare this as constructor, so we can capture range creation.
  // If it is not a constructor, then the user can still create a maximal range
  // (for example) without being warned.
  //
  proc range.range(type idxType = int,
                   param boundedType : BoundedRangeType = BoundedRangeType.bounded,
                   param stridable : bool = false,
                   _low : idxType = 1,
                   _high : idxType = 0,
                   _stride : indexToStrideType(idxType) = 1,
                   _alignment : idxType = 0,
                   _aligned : bool = false)
  {
    this._low = _low;
    this._high = _high;
    if stridable then this._stride = _stride;
    this._alignment = _alignment;

    // todo: remove the check for boundsChecking once assert is no-op upon --fast
    if !stridable && boundsChecking then
      assert(_stride == 1);

    this._aligned = _aligned;
  }

  /////////////////////////////////
  // for debugging

  proc range.displayRepresentation(msg: string = ""): void {
    writeln(msg, "(", typeToString(idxType), ",", boundedType, ",", stridable,
            " : ", low, ",", high, ",", stride, ",",
            if aligned then alignment:string else "?", ")");
  }

  //////////////////////////////////////////////////////////////////////////////////
  // Range builders:  used by the parser to create literal ranges
  //

  // Range builders for fully bounded ranges
  proc chpl_build_bounded_range(low: int(?w), high: int(w))
    return new range(idxType = int(w), _low = low, _high = high);
  proc chpl_build_bounded_range(low: uint(?w), high: uint(w))
    return new range(uint(w), _low = low, _high = high);
  proc chpl_build_bounded_range(low, high) {
    compilerError("Bounds of '..' must be integers of compatible types, when specified.");
  }

  // Range builders for partially bounded ranges
  proc chpl_build_partially_bounded_range(param bt: BoundedRangeType, bound: int(?w))
    return new range(int(w), bt, false, bound, bound);
  proc chpl_build_partially_bounded_range(param bt: BoundedRangeType, bound: uint(?w))
    return new range(uint(w), bt, false, bound, bound);
  proc chpl_build_partially_bounded_range(param bt: BoundedRangeType, bound) {
    compilerError("Bounds of '..' must be integers of compatible types, when specified.");
  }

  // Range builder for unbounded ranges
  proc chpl_build_unbounded_range(param bt: BoundedRangeType)
    return new range(int, bt);
  
  
  //################################################################################
  //# Predicates
  //#
  
  proc isRangeType(type t) param {
    proc isRangeHelp(type t: range(?)) param  return true;
    proc isRangeHelp(type t)           param  return false;
    return isRangeHelp(t);
  }
  
  proc isRangeValue(r: range(?)) param  return true;
  proc isRangeValue(r)           param  return false;
  
  // isBoundedRange(r) = true if 'r' is a (fully) bounded range
  
  proc isBoundedRange(r)           param
    return false;
  
  proc isBoundedRange(r: range(?)) param
    return isBoundedRange(r.boundedType);
  
  proc isBoundedRange(param B: BoundedRangeType) param
    return B == BoundedRangeType.bounded;
  
  // Returns true if this range has a low bound.
  proc range.hasLowBound() param
    return boundedType == BoundedRangeType.bounded ||
           boundedType == BoundedRangeType.boundedLow;
  
  // Returns true if this range has a high bound.
  proc range.hasHighBound() param
    return boundedType == BoundedRangeType.bounded ||
           boundedType == BoundedRangeType.boundedHigh;
  
  // Returns the starting index in the sequence.
  inline proc range.first {
    if ! stridable then return _low;
    else return if _stride > 0 then this.alignedLow else this.alignedHigh;
  }

  // Returns the ending index.
  inline proc range.last {
    if ! stridable then return _high;
    else return if _stride > 0 then this.alignedHigh else this.alignedLow;
  }

  // Returns the low index, properly aligned.
  // The aligned low bound may be higher than the high bound.
  // The client must check that the low bound exists
  // before calling this function.
  inline proc range.alignedLow : idxType {
    if ! stridable then return _low;
  
    // Adjust _low upward by the difference between _alignment and _low.
    return _low + chpl__diffMod(_alignment, _low, _stride);
  }

  // Returns the high index, properly aligned.
  // The aligned high bound may be lower than the low bound.
  // The client must check that the high bound exists
  // before calling this function.
  inline proc range.alignedHigh : idxType {
    if ! stridable then return _high;
  
    // Adjust _high downward by the difference between _high and _alignment.
    return _high - chpl__diffMod(_high, _alignment, _stride);
  }

  // If the represented sequence is defined, reports whether it is empty.
  // Otherwise an error is reported.
  inline proc range.isEmpty() {
    if isAmbiguous() then
      halt("isEmpty() is invoked on an ambiguously-aligned range");
    else
      return isBoundedRange(this) && this.alignedLow > this.alignedHigh;
  }
  
  // Returns the number of elements in this range, cast to the index type.
  // Note that the result will be wrong if the index is signed 
  // and the low and high bounds differ by more than max(idxType).
  proc range.length: idxType
  {
    if ! isBoundedRange(this) then
      compilerError("length is not defined on unbounded ranges");
  
    if isUintType(idxType)
    {
      // assumes alignedHigh/alignLow always work, even for an empty range
      const ah = this.alignedHigh,
            al = this.alignedLow;
      if al > ah then return 0: idxType;
      const s = abs(this.stride): idxType;
      return (ah - al) / s + 1:idxType;
    }
    else // idxType is signed
    {
      if _low > _high then return 0:idxType;
      const s = abs(this.stride): idxType;
      return (this.alignedHigh - this.alignedLow) / s + 1:idxType;
    }
  }

  proc range.hasFirst() param where !stridable && !hasHighBound()
    return hasLowBound();
  
  inline proc range.hasFirst()
    return if isAmbiguous() || isEmpty() then false else
      if stride > 0 then hasLowBound() else hasHighBound();
      
  proc range.hasLast() param where !stridable && !hasLowBound()
    return hasHighBound();
  
  inline proc range.hasLast()
    return if isAmbiguous() || isEmpty() then false else
      if stride > 0 then hasHighBound() else hasLowBound();
  
  // Returns true if this range is naturally aligned, false otherwise.
  // Note that this does not indicate if the range is ambiguously aligned.
  proc range.isNaturallyAligned()
    where this.boundedType == BoundedRangeType.bounded
  {
    // If the stride is positive, we must be aligned on the low bound.
    if stride > 0 then return this.alignedLow == _low;
    // If the stride is negative, we must be aligned on the high bound.
    if stride < 0 then return this.alignedHigh == _high;
    // stride == 0: ???
    return false;
  }
  
  inline proc range.isNaturallyAligned()
    where this.boundedType == BoundedRangeType.boundedLow
  {
    return this.alignedLow == _low;
  }
  
  inline proc range.isNaturallyAligned()
    where this.boundedType == BoundedRangeType.boundedHigh
  {
    return this.alignedHigh == _high;
  }
  
  inline proc range.isNaturallyAligned()
  {
    if _alignment == 0 then return true;
    return false;
  }
  
  // Returns true if the range is ambiguously aligned.
  proc range.isAmbiguous() param where !stridable
    return false;
  proc range.isAmbiguous()       where stridable
    return !aligned && (stride > 1 || stride < -1);
  
  // Returns true if i is in this range.
  inline proc range.member(i: idxType) 
  {
    if this.isAmbiguous() then return false;

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
      var s = abs(_stride):idxType;
      if chpl__diffMod(i, _alignment, s) != 0
        then return false;
    }
    return true;
  }
  
  // Returns true if the other range is contained within this one.
  inline proc range.member(other: range(?)) 
  {
    if this.isAmbiguous() || other.isAmbiguous() then return false;

    // Since slicing preserves the direction of the first arg, may need
    // to negate one of the strides (shouldn't matter which).
    if stridable {
      if (stride > 0 && other.stride < 0) || (stride < 0 && other.stride > 0)
        then return _memberHelp(this, other);
    } else {
      if other.stride < 0
        then return _memberHelp(this, other);
    }
    return other == this(other);
  }

  // This helper takes one arg by 'in', i.e. explicitly creating a copy,
  // so it can be modified.
  inline proc _memberHelp(arg1: range(?), in arg2: range(?)) {
    compilerAssert(arg2.stridable);
    arg2._stride = -arg2._stride;
    return arg2 == arg1(arg2);
  }
  
  // Returns true if the two ranges have the same represented sequence, or if
  // both sequences are undefined but all four primary properties are identical.
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
      const len = r1.length, l2 = r2.length;
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
  
  // This makes use of the fact that the default values for the
  // stride and alignment fields are consistent, whether the two ranges
  // being compared are stridable or not.
  proc ident(r1: range(?), r2: range(?))
    where r1.idxType == r2.idxType &&
    r1.boundedType == r2.boundedType &&
    r1.stridable == r2.stridable
  {
    if r1._low != r2._low then return false;
    if r1._high != r2._high then return false;
    if r1.stride != r2.stride then return false;
    if r1._alignment != r2._alignment then return false;
    if r1._aligned != r2._aligned then return false;
    return true;
  }
  
  // If the parameters don't match, then the two ranges cannot be identical.
  proc ident(r1: range(?), r2: range(?)) param
    return false;
  
  
  //////////////////////////////////////////////////////////////////////////////////
  // Bounds checking
  //
  // returns true if other is in bounds of this for all specified
  // bounds; these functions are used to determine if an array slice is
  // valid.  We break out the boundedNone case in order to permit
  // unbounded ranges to slice ranges of various index types -- otherwise
  // we get a compiler error in the boundsCheck function.
  //
  
  inline proc range.boundsCheck(other: range(?e,?b,?s))
    where b == BoundedRangeType.boundedNone
  {
    if this.isAmbiguous() || other.isAmbiguous()
      then return false;
  
    return true;
  }
  
  inline proc range.boundsCheck(other: range(?e,?b,?s))
  {
    if this.isAmbiguous() || other.isAmbiguous()
      then return false;
  
    var boundedOther = new range(
                          idxType, BoundedRangeType.bounded,
                          s || this.stridable,
                          if other.hasLowBound() then other._low else _low,
                          if other.hasHighBound() then other._high else _high,
                          other._stride,
                          other._alignment,
                          true);
  
    return (boundedOther.length == 0) || member(boundedOther);
  }
  
  inline proc range.boundsCheck(other: idxType)
    return member(other);
  
  
  //################################################################################
  //# Member functions
  //#
  
  // Moves the low bound of the range up to the next alignment point.
  proc range.alignLow() 
  {
    if this.isAmbiguous() then
      __primitive("chpl_error", "alignLow -- Canot be applied to a range with ambiguous alignment.");
  
    if stridable then _low = this.alignedLow;
    return this;
  }
  
  // Moves the high bound of the range down to the next alignment point.
  proc range.alignHigh()
  {
    if this.isAmbiguous() then
      __primitive("chpl_error", "alignHigh -- Canot be applied to a range with ambiguous alignment.");
  
    if stridable then _high = this.alignedHigh;
    return this;
  }
  
  // Returns the ordinal of 'i' within this range's represented sequence.
  proc range.indexOrder(i: idxType)
  {
    if this.isAmbiguous() then
      __primitive("chpl_error", "indexOrder -- Undefined on a range with ambiguous alignment.");
  
    if ! member(i) then return (-1):idxType;
    if ! stridable then return i - _low;
    else return ((i:strType - this.first:strType) / _stride):idxType;
  }
  
  // Opposite of indexOrder: returns the ord-th element of this range's
  // represented sequence.
  proc range.orderToIndex(ord: integral): idxType
  {
    if isAmbiguous() then
      halt("invoking orderToIndex on a range that is ambigously aligned");
  
    if boundsChecking {
      if ord < 0 then
        halt("invoking orderToIndex on a negative integer: ", ord);
  
      if isBoundedRange(this) && ord >= this.length then
        halt("invoking orderToIndex on an integer ", ord,
             " that is larger than the range's number of indices ", this.length);
    }
  
    return chpl__addRangeStrides(this.first, this.stride, ord);
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
  inline proc range.translate(i: integral)
    return this + i:idxType;

  inline proc range.translate(i)
  {
    compilerError("offsets must be of integral type");
  }

  // Compute the alignment of the range returned by this.interior()
  // and this.exterior(). Keep it private.
  inline proc range._effAlmt()       where stridable return _alignment;
         proc range._effAlmt() param where !stridable return 0;
  
  // Return an interior portion of this range.
  proc range.interior(i: idxType)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("interior is not supported on unbounded ranges");
  }
  
  // TODO: hilde
  // Set _aligned to true only if stridable.
  proc range.interior(i: idxType)
  {
    if i < 0 then
      return new range(idxType, boundedType, stridable,
                       _low, _low - 1 - i, stride, _effAlmt(), _aligned);
    if i > 0 then
      return new range(idxType, boundedType, stridable,
                       _high + 1 - i, _high, stride, _effAlmt(), _aligned);
    // if i == 0 then
    return new range(idxType, boundedType, stridable,
                     _low, _high, stride, _effAlmt(), _aligned);
  }
  
  // Return an exterior portion of this range.
  proc range.exterior(i: idxType)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("exterior is not supported on unbounded ranges");
  }
  
  proc range.exterior(i: idxType)
  {
    if i < 0 then
      return new range(idxType, boundedType, stridable,
                       _low + i, _low - 1, stride, _effAlmt(), _aligned);
    if i > 0 then
      return new range(idxType, boundedType, stridable,
                       _high + 1, _high + i, stride, _effAlmt(), _aligned);
    // if i == 0 then
    return new range(idxType, boundedType, stridable,
                     _low, _high, stride, _effAlmt(), _aligned);
  }
  
  // Returns an expanded range, or a contracted range if i < 0.
  // The existing absolute alignment is preserved.
  proc range.expand(i: idxType)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("expand() is not supported on unbounded ranges");
  }
  
  proc range.expand(i: idxType)
  {
    return new range(idxType, boundedType, stridable,
                     _low-i, _high+i, stride, _alignment, _aligned);
  }
  
  
  //################################################################################
  //# Syntax Functions
  //# These are special functions known to the compiler parser.
  //#
  
  // Assignment
  pragma "compiler generated"
    // The "compiler generated" flag is added so this explicit definition
    // of assignment does not disable the POD optimization.
    // Although provided explicitly, this function is effectively trivial,
    // since it performs what is effectively a bit-wise copy.
    // It effectively labels the function as trivial, even though this is not
    // precisely true.  In the case of an assignment from a stridable to non-
    // stridable range, there is a run-time check which will be missed when the
    // optimization is applied.  The rest of the routine is trivial in the
    // sense that it performs the equivalent of a bit-wise copy.
    // The POD optimization currently removes initCopy, autoCopy and destructor
    // calls whose arguments are of plain-old-data type.  Future applications
    // of this optimization may also remove assignment calls.
    // The determination of whether a type is POD or not is currently based on
    // whether a destructor or any assignment operators are defined taking that
    // that type as an operand.  In the future, the initCopy and default
    // constructor (yet to be defined) functions and possibly the autoCopy
    // function will be considered as well.
    // The purpose of considering POD-ness as the criterion for removing 
    // initCopy and autoCopy calls is that destructors are removed at the same
    // time.  So at least both functions participating in the optimization must
    // be trivial.  In the future, the optimization may also remove assignments
    // and default constructor calls, in which case the optimization should
    // only be applied when all four of these functions (or their generic
    // equivalents) are trivial.
    // See also removePODinitDestroy() in removeUnnecessaryAutoCopyCalls.cpp.
  inline proc =(ref r1: range(stridable=?s1), r2: range(stridable=?s2))
  {
    if r1.boundedType != r2.boundedType then
      compilerError("type mismatch in assignment of ranges with different boundedType parameters");
  
    if !s1 && s2 then
      if r2._stride != 1 then
        halt("non-stridable range assigned non-unit stride");
  
    r1._low = r2._low;
    r1._high = r2._high;
    r1._stride = r2._stride;
    r1._alignment = r2._alignment;
    r1._aligned = r2._aligned;
  }
  
  //////////////////////////////////////////////////////////////////////////////////
  // Range arithmetic.
  //
  // Shifts and entire range to the right or left.
  // Absolute alignment is not preserved
  // (That is, the alignment shifts along with the range.)
  //
  inline proc +(r: range(?e, ?b, ?s), i: integral)
  {
    type resultType = (r._low+i).type;
    type strType = indexToStrideType(resultType);

    return new range(resultType, b, s,
                     r._low + i, r._high + i,
           r.stride : strType, r._alignment + i : resultType, r._aligned);
  }
  
  inline proc +(i:integral, r: range(?e,?b,?s))
    return r + i;
  
  inline proc -(r: range(?e,?b,?s), i: integral)
  {
    type resultType = (r._low+i).type;
    type strType = indexToStrideType(resultType);
  
    return new range(resultType, b, s,
                     r._low - i, r._high - i,
           r._stride : strType, r._alignment - i : resultType, r._aligned);
  }
  
  
  inline proc chpl_check_step_integral(step) {
    if !isIntegral(step.type) then
      compilerError("can't apply 'by' using step of a non-integral type ",
                    typeToString(step.type));
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
    type strType = indexToStrideType(idxType);

    // At present, step must coerce to range's idxType or strType.
    if numBits(step.type) > numBits(strType) then
      compilerError("can't apply 'by' to a range with idxType ",
                    typeToString(idxType), " using a step of type ",
                    typeToString(step.type));

    if boundsChecking {
      if step == 0 then
        __primitive("chpl_error", "the step argument of the 'by' operator is zero");

      if chpl_need_to_check_step(step, strType) &&
         step > (max(strType):step.type)
      then
        __primitive("chpl_error", "the step argument of the 'by' operator is too large and cannot be represented within the range's stride type " + typeToString(strType));
    }
  }
  
  inline proc chpl_range_check_stride(param step, type idxType)  {
    chpl_check_step_integral(step);
    type strType = indexToStrideType(idxType);

    if step == 0 then
      compilerError("the step argument of the 'by' operator is zero");

    // do not check e.g. when step and strType are both int(64)
    if chpl_need_to_check_step(step, strType) &&
       step > (max(strType):step.type)
    then
      compilerError("the step argument of the 'by' operator is too large and cannot be represented within the range's stride type " + typeToString(strType));
  }
  
  
  proc chpl_by_help(r: range(?i,?b,?s), step) {
    const lw: i = r.low,
          hh: i = r.high,
          st: r.strType = r.stride * step:r.strType;
  
    const (ald, alt): (bool, i) =
      if r.isAmbiguous() then                   (false, r.alignment)
      else
        // we could talk about aligned bounds
        if      r.hasLowBound()  && st > 0 then (true, r.alignedLow)
        else if r.hasHighBound() && st < 0 then (true, r.alignedHigh)
        else                                    (r.aligned, r.alignment);
  
    return new range(i, b, true,  lw, hh, st, alt, ald);
  }

  inline proc by(r, step) {
    if !isRange(r) then
      compilerError("the first argument of the 'by' operator is not a range");
    chpl_range_check_stride(step, r.idxType);
    return chpl_by_help(r, step);
  }
  
  // We want to warn the user at compiler time if they had an invalid param
  // stride rather than waiting until runtime.
  inline proc by(r : range(?), param step) {
    chpl_range_check_stride(step, r.idxType);
    return chpl_by_help(r, step:r.strType);
  }
  
  
  // This is the syntax processing routine for the "align" keyword.
  // It produces a new range with the specified alignment.
  // By definition, alignment is relative to the low bound of the range.
  
  inline proc align(r : range(?i, ?b, ?s), algn: i)
  {
    // Note that aligning an unstrided range will set the field value,
    // but has no effect on the index set produced (a mod 1 == 0).
    return new range(i, b, s,
                     r._low, r._high, r.stride, algn, true);
  }
  
  inline proc align(r : range(?i, ?b, ?s), algn) {
    compilerError("can't align a range with idxType ", typeToString(i), 
                  " using a value of type ", typeToString(algn.type));
    return r;
  }
  
  // Set the alignment as an offset off the first element of the sequence.
  proc range.offset(offs : idxType)
  {
    if !stridable then
      compilerWarning("invoking 'offset' on an unstrided range has no effect."); 
  
    if !hasFirst() then
      halt("invoking 'offset' on a range without the first index");
  
    return new range(idxType, boundedType, stridable, low, high, stride,
                     // here's the new alignment
                     this.first + offs, true);
  }
  
  
  // Composition
  // Return the intersection of this and other.
  proc range.this(other: range(?))
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
      if g > 1 then
        __primitive("chpl_error", "Cannot slice ranges with ambiguous alignments unless their strides are relatively prime.");
  
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
    if (isUintType(idxType)) { if (lo1 < 0) then lo1 = 0; }
  
    // We inherit the sign of the stride from this.stride.
    var newStride: strType = this.stride;
    var lcm: strType = abs(this.stride);
    var (g, x): 2*strType = (lcm, 0:strType);
  
    if this.stride != other.stride && this.stride != -other.stride {
  
      (g, x) = chpl__extendedEuclid(st1, st2);
      lcm = st1 / g * st2;        // The LCM of the two strides.
    // The division must be done first to prevent overflow.
  
      newStride = if this.stride > 0 then lcm else -lcm;
    }
  
    var result = new range(idxType,
                           computeBoundedType(this, other),
                           this.stridable | other.stridable,
                           max(lo1, lo2):idxType,
                           min(hi1, hi2):idxType,
                           newStride,
                           0,
                           !ambig && (this._aligned || other._aligned));
  
    if result.stridable {
      var al1 = (this._alignment % st1:idxType):int;
      var al2 = (other._alignment % st2:other.idxType):int;
  
      if (al2 - al1) % g != 0 then
      {
        // empty intersection, return degenerate result
        if !isBoundedRange(result) then
          halt("could not represent range slice - it needs to be empty, but the slice type is not bounded");
        result._low = 1:idxType;
        result._high = 0:idxType;
        result._alignment = if this.stride > 0 then 1:idxType else 0:idxType;
        // _alignment == _low, so it won't print.
      }
      else
      { // non-empty intersection
  
        // x and/or the diff may negative, even with a uint source range.
        var offset = (al2 - al1) * x;
        // offset is in the range [-(lcm-1), lcm-1]
        if offset < 0 then offset += lcm;
  
        // Now offset can be safely cast to idxType.
        result._alignment = al1:idxType + offset:idxType * st1:idxType / g:idxType;
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
    if r.isAmbiguous() then
      __primitive("chpl_error", "count -- Cannot count off elements from a range which is ambiguously aligned.");

    type resultType = r.idxType;
    type strType = indexToStrideType(resultType);
  
    if (count == 0) then
      // Return a degenerate range.
      return new range(idxType = resultType,
                       boundedType = BoundedRangeType.bounded,
                       stridable = r.stridable,
                       _low = 1,
                       _high = 0,
                       _stride = r.stride,
                       _alignment = 0,
                       _aligned = false);
  
    if !r.hasFirst() && count > 0 then
      halt("With a positive count, the range must have a first index.");
    if !r.hasLast()  && count < 0 then
      halt("With a negative count, the range must have a last index.");
    if r.boundedType == BoundedRangeType.bounded && 
      abs(count:chpl__maxIntTypeSameSign(count.type)):uint(64) > r.length:uint(64) then {
      halt("bounded range is too small to access ", abs(count), " elements");
    }
  
    //
    // BLC: I'm not particularly proud of this, but it was the only
    // way I could figure to keep count.chpl working given that the
    // # operator no longer returns a range of idxType corresponding
    // to the sum of the idxType and count type.
    //
    proc chpl__computeTypeForCountMath(type t1, type t2) type {
      if (t1 == t2) then {
        return t1;
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
  
    return new range(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = lo,
                     _high = hi,
                     _stride = r.stride : strType,
                     _alignment = r._alignment,
                     _aligned = r._aligned);
  }
  
  proc #(r:range(?i), count:chpl__signedType(i)) {
    return chpl_count_help(r, count);
  }
  
  proc #(r:range(?i), count:chpl__unsignedType(i)) {
    return chpl_count_help(r, count);
  }
  
  proc #(r: range(?i), count) {
    compilerError("can't apply '#' to a range with idxType ", 
                  typeToString(i), " using a count of type ", 
                  typeToString(count.type));
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

    if (willOverFlow && shouldHalt) {
      halt("Iteration over a bounded range may be incorrect due to overflow.");
    }
    return willOverFlow;
  }

  proc range.checkIfIterWillOverflow(shouldHalt=true) {
    return chpl_checkIfRangeIterWillOverflow(this.idxType, this.low, this.high,
        this.stride, this.first, this.last, shouldHalt);
  }


  //################################################################################
  //# Direct range iterators that take low, high and stride as arguments. They
  //# are not iterators over ranges, but instead take the components of the range
  //# as arguments. This allows us to avoid range construction and provide
  //# optimized iterators when stride is known at compile time.
  //#

  //
  // These iterators exist so that argument coercion happens like it does for
  // chpl_build_bounded_range and the by operator. They just foward to the
  // "actual" iterators below which do not do any type checking on the
  // arguments. They are only inteneded to be used for bounded ranges. There
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


  // cases for when stride is a param int (underlying iter can figure out sign
  // of stride.) Not needed, but allows us to us "<, <=, >, >=" instead of "!="
  iter chpl_direct_range_iter(low: int(?w), high: int(w), param stride : int(w)) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), param stride: int(w)) {
    for i in chpl_direct_param_stride_range_iter(low, high, stride) do yield i;
  }


  // cases for when stride is a uint (we know the stride is must be positive)
  iter chpl_direct_range_iter(low: int(?w), high: int(w), stride: uint(w)) {
    for i in chpl_direct_uint_stride_range_iter(low, high, stride) do yield i;
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), stride: uint(w)) {
    for i in chpl_direct_uint_stride_range_iter(low, high, stride) do yield i;
  }


  // cases for when stride isn't valid
  iter chpl_direct_range_iter(low: int(?w), high: int(w), stride) {
    compilerError("can't apply 'by' to a range with idxType ",
                  typeToString(int(w)), " using a step of type ",
                  typeToString(stride.type));
    yield nil; // iters needs a yield in them
  }

  iter chpl_direct_range_iter(low: uint(?w), high: uint(w), stride) {
    compilerError("can't apply 'by' to a range with idxType ",
                  typeToString(uint(w)), " using a step of type ",
                  typeToString(stride.type));
    yield nil; // iters needs a yield in them
  }


  // case for when low and high aren't compatible types and can't be coerced
  iter chpl_direct_range_iter(low, high, stride) {
    compilerError("Bounds of '..' must be integers of compatible types, when specified.");
    yield nil; // iters needs a yield in them
  }


  // These are the "actual" direct range iterators. Note that they do not do
  // any checks on the arguments, and rely on the above functions to
  // check/coerce types (assumes args are of legal types, low/high are the same
  // same type, and stride is valid.)

  iter chpl_direct_uint_stride_range_iter(low: ?t, high, stride) {
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
  iter range.these() where boundedType != BoundedRangeType.bounded {

    if boundedType == BoundedRangeType.boundedNone then
      compilerError("iteration over a range with no bounds");

    if ! this.hasFirst() then
      halt("iteration over range that has no first index");

    if this.isAmbiguous() then
      __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");

    // This iterator could be split into different cases depending on the
    // stride like the bounded iterators. However, all that gets you is the
    // ability to use low/alignedLow over first. The additional code isn't
    // worth it just for that. In the other cases it allowed us to specialize
    // the test relational operator, which is important
    var i: idxType;
    const start = this.first;
    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      true,
                      __primitive("+=", i, stride: idxType)) {
      yield i;
    }
  }

  // A bounded and strided range iterator
  iter range.these()
    where boundedType == BoundedRangeType.bounded && stridable == true {
    if (useOptimizedRangeIterators) {
      if boundsChecking then checkIfIterWillOverflow();

      if this.isAmbiguous() then
        __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");

      // must use first/last since we have no knowledge of stride
      // must check if low > high (something like 10..1) because of the !=
      // relational operator. Such ranges are supposed to iterate 0 times
      var i: idxType;
      const start = this.first;
      const end: idxType = if this.low > this.high then start else this.last + stride: idxType;
      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("!=", i, end),
                        __primitive("+=", i, stride: idxType)) {
        yield i;
      }
    } else {
      for i in this.generalIterator() do yield i;
    }
  }

  // A bounded and non-strided (stride = 1) range iterator
  iter range.these()
    where boundedType == BoundedRangeType.bounded && stridable == false {
    if (useOptimizedRangeIterators) {
      if boundsChecking then checkIfIterWillOverflow();

      // don't need to check if isAmbigous since stride is one

      // can use low/high instead of first/last since stride is one
      var i: idxType;
      const start = this.low;
      const end = this.high;

      while __primitive("C for loop",
                        __primitive( "=", i, start),
                        __primitive("<=", i, end),
                        __primitive("+=", i, stride: idxType)) {
        yield i;
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
  // iterations that will occur (possibly using length()) and have the number
  // of iterations drive the loop and use a separate variable to track the
  // value to yield. This would mean you couldn't express maximal ranges for
  // int(64) and uint(64) but it's hard to see a case where those could ever be
  // desired.
  iter range.generalIterator() {
    if this.isAmbiguous() then
      __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");

    var i: idxType;
    const start = this.first;
    const end = if this.low > this.high then start else this.last;

    while __primitive("C for loop",
                      __primitive( "=", i, start),
                      __primitive(">=", high, low),  // execute at least once?
                      __primitive("+=", i, stride: idxType)) {
      yield i;
      if i == end then break;
    }
  }

  //################################################################################
  //# Parallel Iterators
  //#

  iter range.these(param tag: iterKind) where tag == iterKind.standalone &&
                                              !localeModelHasSublocales
  {
    if ! isBoundedRange(this) {
      compilerError("parallel iteration is not supported over unbounded ranges");
    }
    if this.isAmbiguous() {
      __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");
    }
    if debugChapelRange {
      writeln("*** In range standalone iterator:");
    }

    const v = this.length;
    const numChunks = if __primitive("task_get_serial") then
                      1 else _computeNumChunks(v);

    if debugChapelRange {
      writeln("*** RI: length=", v, " numChunks=", numChunks);
    }

    if numChunks <= 1 {
      for i in this {
        yield i;
      }
    } else {
      coforall chunk in 0..#numChunks {
        const (lo, hi) = _computeBlock(v, numChunks, chunk, v-1);
        const len = hi - (lo-1);
        var low = orderToIndex(lo);
        var high = (low:strType + stride * (len - 1):strType):idxType;
        if stride < 0 then low <=> high;
        const r = if stridable then low..high by stride else low..high;
        for i in r {
          yield i;
        }
      }
    }
  }

  iter range.these(param tag: iterKind) where tag == iterKind.leader
  {
    if ! isBoundedRange(this) then
      compilerError("parallel iteration is not supported over unbounded ranges");

    if this.isAmbiguous() then
      __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");

    if debugChapelRange then
      writeln("*** In range leader:"); // ", this);
    const numSublocs = here.getChildCount();

    if localeModelHasSublocales && numSublocs != 0 {
      const len = this.length;
      const tasksPerLocale = dataParTasksPerLocale;
      const ignoreRunning = dataParIgnoreRunningTasks;
      const minIndicesPerTask = dataParMinGranularity;
      const dptpl = if tasksPerLocale==0 then here.maxTaskPar
                    else tasksPerLocale;

      // Make sure we don't use more sublocales than the numbers of
      // tasksPerLocale requested
      const numSublocTasks = min(numSublocs, dptpl);
      // For serial tasks, we will only have a singel chunk
      const numChunks =  if __primitive("task_get_serial") then
                         1 else _computeNumChunks(numSublocTasks,
                                                  ignoreRunning,
                                                  minIndicesPerTask,
                                                  len);
      if debugDataParNuma {
        writeln("### numSublocs = ", numSublocs, "\n" +
                "### numTasksPerSubloc = ", numSublocTasks, "\n" +
                "### ignoreRunning = ", ignoreRunning, "\n" +
                "### minIndicesPerTask = ", minIndicesPerTask, "\n" +
                "### numChunks = ", numChunks);
      }
        
      if numChunks == 1 {
        yield (0..len-1,);
      } else {
        coforall chunk in 0..#numChunks {
          on here.getChild(chunk) {
            if debugDataParNuma {
              if chunk!=chpl_getSubloc() then
                writeln("*** ERROR: ON WRONG SUBLOC (should be "+chunk+
                        ", on "+chpl_getSubloc()+") ***");
            }
            const (lo,hi) = _computeBlock(len, numChunks, chunk, len-1);
            const locRange = lo..hi;
            const locLen = locRange.length;
            // Divide the locale's tasks approximately evenly
            // among the sublocales
            const numCoreTasks = dptpl/numChunks +
              if chunk==numChunks-1 then dptpl%numChunks else 0;
            const numTasks = _computeNumChunks(numCoreTasks,
                                               ignoreRunning,
                                               minIndicesPerTask,
                                               locLen);
            coforall core in 0..#numTasks {
              const (low, high) = _computeBlock(locLen, numTasks, core, hi, lo, lo);
              if debugDataParNuma {
                writeln("### chunk = ", chunk, "  core = ", core, "  " +
                        "locRange = ", locRange, "  coreRange = ", low..high);
              }
              yield (low..high,);
            }
          }
        }
      }
      
    } else {
      var v = this.length;
      const numChunks = if __primitive("task_get_serial") then
                        1 else _computeNumChunks(v);
  
      if debugChapelRange
      {
        writeln("*** RI: length=", v, " numChunks=", numChunks);
        writeln("*** RI: Using ", numChunks, " chunk(s)");
      }
  
      if numChunks == 1 then
        yield (0..v-1,);
      else
      {
        coforall chunk in 0..#numChunks
        {
          const (lo,hi) = _computeBlock(v, numChunks, chunk, v-1);
          if debugChapelRange then
            writeln("*** RI: tuple = ", (lo..hi,));
          yield (lo..hi,);
        }
      }
    }
  }
  
  iter range.these(param tag: iterKind, followThis) where tag == iterKind.follower
  {
    if this.isAmbiguous() then
      __primitive("chpl_error", "these -- Attempt to iterate over a range with ambiguous alignment.");

    if boundedType == BoundedRangeType.boundedNone then
      compilerError("iteration over a range with no bounds");
    if ! stridable && boundedType == BoundedRangeType.boundedHigh then
      compilerError("iteration over a range with no first index");
  
    if followThis.size != 1 then
      compilerError("iteration over a range with multi-dimensional iterator");
  
    if debugChapelRange then
      writeln("In range follower code: Following ", followThis);
  
    var myFollowThis = followThis(1);
  
    if debugChapelRange then
      writeln("Range = ", myFollowThis);
  
    if ! this.hasFirst() {
      if this.isEmpty() {
        if myFollowThis.isEmpty() then
          // nothing to do
          return;
        else
          halt("zippered iteration with a range has non-equal lengths");
      } else {
        halt("iteration over a range with no first index");
      }
    }
    if ! myFollowThis.hasFirst() {
      if !myFollowThis.isAmbiguous() && myFollowThis.isEmpty() then
        // nothing to do
        return;
      else
        halt("zippered iteration over a range with no first index");
    }
  
    if (isBoundedRange(myFollowThis) && !myFollowThis.stridable) ||
       myFollowThis.hasLast()
    {
      const flwlen = myFollowThis.length;
      if flwlen == 0 then
        return; // nothing to do
      if boundsChecking && this.hasLast() {
        // this check is for typechecking only
        if isBoundedRange(this) {
          if this.length < flwlen then
            halt("zippered iteration over a range with too few indices");
        } else
          assert(false, "hasFirst && hasLast do not imply isBoundedRange");
      }    
      if this.stridable || myFollowThis.stridable {
        // same as undensifyBounded(this, myFollowThis)
        const stride = this.stride * myFollowThis.stride;
        var low: idxType  = this.orderToIndex(myFollowThis.first);
        var high: idxType = ( low: strType + stride * (flwlen - 1):strType ):idxType;
        assert(high == this.orderToIndex(myFollowThis.last));

        if stride < 0 then low <=> high;

        const r = low .. high by stride:strType;
        if debugChapelRange then
          writeln("Expanded range = ",r);

        // todo: factor out this loop (and the above writeln) into a function?
        for i in r do
          yield i;
      } else {
        // same as undensifyBounded(this, myFollowThis)
        const low: idxType  = this.orderToIndex(myFollowThis.first);
        const high: idxType = ( low: strType + (flwlen - 1):strType ):idxType;
        assert(high == this.orderToIndex(myFollowThis.last));

        const r = low .. high;
        if debugChapelRange then
          writeln("Expanded range = ",r);

        // todo: factor out this loop (and the above writeln) into a function?
        for i in r do
          yield i;
      }
    }
    else // ! myFollowThis.hasLast()
    {
      // WARNING: this case has not been tested
      if boundsChecking && this.hasLast() then
        halt("zippered iteration where a bounded range follows an unbounded iterator");
  
      const first  = this.orderToIndex(myFollowThis.first);
      const stride = this.stride * myFollowThis.stride;
  
      if stride > 0
      {
        const r = first .. by stride:strType;
        if debugChapelRange then
          writeln("Expanded range = ",r);
      
        for i in r do
          yield i;
      }
      else
      {
        const r = .. first by stride:strType;
        if debugChapelRange then
          writeln("Expanded range = ",r);
      
        for i in r do
          yield i;
      }
    } // if myFollowThis.hasLast()
  }
  
  
  //################################################################################
  //# Utilities
  //#
  
  // Write implementation for ranges
  proc range.readWriteThis(f)
  {
    if f.writing && !aligned {
      // set things up so alignment does not get printed out
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

    if hasLowBound() then
      f <~> _low;
    f <~> new ioLiteral("..");
    if hasHighBound() then
      f <~> _high;
    if stride != 1 then
      f <~> new ioLiteral(" by ") <~> stride;
  
    // Write out the alignment only if it differs from natural alignment.
    // We take alignment modulo the stride for consistency.
    if f.writing {
      if ! isNaturallyAligned() then
        f <~> new ioLiteral(" align ") <~> chpl__mod(_alignment, stride);
    } else {
      // try reading an 'align'
      if !f.error() {
        f <~> new ioLiteral(" align ");
        if f.error() == EFORMAT then {
          // naturally aligned.
          f.clearError();
        } else {
          // un-naturally aligned - read the un-natural alignment
          var a: idxType;
          f <~> a;
          _alignment = a;
        }
      }
    }
  }
  
  //################################################################################
  //# Internal helper functions.
  //#
  
  inline proc range.chpl__unTranslate(i: idxType)
    return this - i;
  
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
  proc chpl__mod(dividend:integral, in modulus:integral)
  {
    type dType = dividend.type;
    modulus = abs(modulus);
    // modulus is positive, so this cast is OK unless it is very large
    // and the dividend is signed.
    var m = modulus : dType;
    if dType != modulus.type {
      if m : modulus.type != modulus then
        halt("Modulus too large.");
    }
  
    var tmp = dividend % m;
    if isIntType(dividend.type) then
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
                     in modulus : integral) : minuend.type
    where minuend.type == subtrahend.type
  {
    type minType = minuend.type;
  
    modulus = abs(modulus);
    var m = modulus : minType;
    if minType != modulus.type {
      if m : modulus.type != modulus then
        halt("Modulus too large.");
    }
  
    var minMod = chpl__mod(minuend, m);
    var subMod = chpl__mod(subtrahend, m);
  
    return if minMod < subMod
      then m  - (subMod - minMod)
      else minMod - subMod;
  }
  
  proc chpl__diffMod(minuend : integral,
                     subtrahend : integral,
                     in modulus : integral)
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
  // Use example: low + i:idxType * stride.
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
  
    while V(3) != 0 do
      (U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));
  
    return (U(3), U(1));
  }
  
  inline proc chpl__extendedEuclid(u:int(32), v:int(32))
  { return chpl__extendedEuclidHelper(u,v); }
  
  inline proc chpl__extendedEuclid(u:int(64), v:int(64))
  { return chpl__extendedEuclidHelper(u,v); }
  
}
