// ChapelRangeBase.chpl
//
pragma "no use ChapelStandard"
module ChapelRangeBase {
  
  use Math; // for abs().
  //use DSIUtil;
  
  // Turns on range iterator debugging.
  config param debugChapelRange = false;
  
  //
  // range type
  //
  //   parameterized by:
  //        1) an integral element type,
  //        2) whether low and/or high bounds exist, and
  //        3) whether the stride is one or not.
  //
  // A range can only be aligned if it is strided.  Since alignments are taken
  // modulo the stride, if the stride is one, even a nonzero alignment has no effect.
  // The alignment of a range is relative to the low bound if it exists, otherwise
  // relative to the high bound if it exists, otherwise absolute.
  //
  // The type of the stride is a signed type with the same number of bits as the
  // index type.  This means that for an unsigned index type, a maximal range can
  // be reached in no less than two strides.
  //
  enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };
  
  record rangeBase
  {
    type idxType = int;                            // element type
    param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
    param stridable: bool = false;                 // range can be strided
    
    var _low: idxType = 1;                         // lower bound
    var _high: idxType = 0;                        // upper bound
    var _stride: strType = 1;                      // signed stride of range
    var _alignment: idxType = 0;                   // alignment
  
    proc strType type return chpl__signedType(idxType);
    inline proc low return _low;       // public getter for low bound
    inline proc high return _high;     // public getter for high bound
    inline proc stride where stridable return _stride;
    proc stride param where !stridable return 1 : strType;
    inline proc alignment return _alignment;        // public getter for alignment
  
  }
  
  //################################################################################
  //# Constructors
  //#
  
  // Declare this as constructor, so we can capture range creation.
  // If it is not a constructor, then the user can still create a maximal range
  // (for example) without begin warned.
  //
  proc rangeBase.rangeBase(type idxType = int,
                  param boundedType : BoundedRangeType = BoundedRangeType.bounded,
             param stridable : bool = false,
             _low : idxType = 1,
             _high : idxType = 0,
             _stride : chpl__signedType(idxType) = 1,
             _alignment: idxType = 0)
  {
    this._low = _low;
    this._high = _high;
    if stridable then this._stride = _stride;
    this._alignment = _alignment;
  
    if warnMaximalRange
    {
      if boundedType == BoundedRangeType.bounded
      {
        if _low <= _high && this.last + stride : idxType == this.first then
          warning("Maximal range declared.  " +
          "A for loop on this range will execute zero times.  " +
          "Try using a wider index type.");
      }
    }
  
    // todo: remove the check for boundsChecking once assert is no-op upon --fast
    if !stridable && boundsChecking then
      assert(_stride == 1);
  }
  
  
  //################################################################################
  //# Accessors
  //#
  
  // true if 'r' is a rangeBase(?,bounded,?)
  proc isBoundedRangeB(r)               param
    return false;
  proc isBoundedRangeB(r: rangeBase(?)) param
    return r.boundedType == BoundedRangeType.bounded;
  
  // Returns the starting index (with minimal checks).
  inline proc rangeBase.first
  {
    if ! stridable then return _low;
    else return if _stride > 0 then this.alignedLow else this.alignedHigh;
  }
  
  // Returns the ending index (with minimal checks).
  inline proc rangeBase.last
  {
    if ! stridable then return _high;
    else return if _stride > 0 then this.alignedHigh else this.alignedLow;
  }
  
  // Returns the low index, properly aligned.
  // The aligned low bound may be higher than the high bound.
  // The client must check that the low bound exists before calling this function.
  inline proc rangeBase.alignedLow : idxType
  {
    if ! stridable then return _low;
  
    // Adjust _low upward by the difference between _alignment and _low.
    return _low + chpl__diffMod(_alignment, _low, _stride);
  }
  
  // Returns the high index, properly aligned.
  // The aligned high bound may be lower than the low bound.
  // The client must check that the high bound exists before calling this function.
  inline proc rangeBase.alignedHigh : idxType
  {
    if ! stridable then return _high;
  
    // Adjust _high downward by the difference between _high and _alignment.
    return _high - chpl__diffMod(_high, _alignment, _stride);
  }
  
  // Returns the number of elements in this range, cast to the index type.
  // Note that the result will be wrong if the index is signed 
  // and the low and high bounds differ by more than max(idxType).
  proc rangeBase.length: idxType
  {
    if ! isBoundedRangeB(this) then
      compilerError("length is not defined on unbounded ranges");
  
    if _isUnsignedType(idxType)
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
  
  proc rangeBase.size: idxType
  {
      return this.length;
  }
  
  //################################################################################
  //# Predicates
  //#
  
  // Returns true if this range has a low bound.
  proc rangeBase.hasLowBound() param
    return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedLow;
  
  // Returns true if this range has a high bound.
  proc rangeBase.hasHighBound() param
    return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedHigh;
  
  inline proc rangeBase.isEmpty()       where isBoundedRangeB(this)
    return this.alignedLow > this.alignedHigh;
  
  proc rangeBase.isEmpty() param where !isBoundedRangeB(this)
    return false;
  
  proc rangeBase.hasFirst() param where !stridable && !hasHighBound()
    return hasLowBound();
  
  inline proc rangeBase.hasFirst()
  {
    if this.isEmpty() then return false;
    return if stride > 0 then hasLowBound() else hasHighBound();
  }
      
  proc rangeBase.hasLast() param where !stridable && !hasLowBound()
    return hasHighBound();
  
  inline proc rangeBase.hasLast()
  {
    if this.isEmpty() then return false;
    return if stride > 0 then hasHighBound() else hasLowBound();
  }
  
  // Returns true if this range is naturally aligned, false otherwise.
  proc rangeBase.isNaturallyAligned()
    where this.boundedType == BoundedRangeType.bounded
  {
    // If the stride is positive, we must be aligned on the low bound.
    if stride > 0 then return this.alignedLow == _low;
    // If the stride is negative, we must be aligned on the high bound.
    if stride < 0 then return this.alignedHigh == _high;
    // stride == 0: ???
    return false;
  }
  
  inline proc rangeBase.isNaturallyAligned()
    where this.boundedType == BoundedRangeType.boundedLow
  {
    return this.alignedLow == _low;
  }
  
  inline proc rangeBase.isNaturallyAligned()
    where this.boundedType == BoundedRangeType.boundedHigh
  {
    return this.alignedHigh == _high;
  }
  
  inline proc rangeBase.isNaturallyAligned()
  {
    if _alignment == 0 then return true;
    return false;
  }
      
  // Returns true if i is in this range.
  proc rangeBase.member(i: idxType)
  {
    var al : idxType = _alignment;
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
      if chpl__diffMod(i, al, s) != 0
        then return false;
    }
    return true;
  }
  
  // Returns true if the other range is contained within this one.
  inline proc rangeBase.member(other: rangeBase(?))
  {
    // Since slicing preserves the direction of the first arg, may need
    // to negate one of the strides (shouldn't matter which).
    if stridable {
      if (stride > 0 && other.stride < 0) || (stride < 0 && other.stride > 0)
        then  _stride = -_stride;
    } else {
      if other.stride < 0
        then other._stride = -other._stride;
    }
    return other == this(other);
  }
  
  // ==(r1,r2) returns true if the two ranges produce the same index sequence.
  
  proc ==(r1: rangeBase(?), r2: rangeBase(?)) param where
    r1.boundedType != r2.boundedType
  {
    return false;
  }
  
  inline proc ==(r1: rangeBase(?), r2: rangeBase(?)) where
    r1.boundedType == r2.boundedType && isBoundedRangeB(r1)
  {
    // gotta have a special case for length 0 or 1
    const len = r1.length, l2 = r2.length;
    if len != l2 then return false;
    if len == 0 then return true;
    if r1.first != r2.first then return false;
    if len == 1 then return true; // rest doesn't matter
    if r1.stride != r2.stride then return false;
    return true;
  }
  
  inline proc ==(r1: rangeBase(?), r2: rangeBase(?)) where
    r1.boundedType == r2.boundedType && ! isBoundedRangeB(r1)
  {
    if r1.stride != r2.stride then return false;
  
    if r1.hasLowBound() then
      if r1.alignedLow != r2.alignedLow then return false;
  
    if r1.hasHighBound() then
      if r1.alignedHigh != r2.alignedHigh then return false;
  
    return true;
  }
  
  // This makes use of the fact that the default values for the
  // stride and alignment fields are consistent, whether the two ranges
  // being compared are stridable or not.
  proc ident(r1: rangeBase(?), r2: rangeBase(?))
    where r1.idxType == r2.idxType &&
    r1.boundedType == r2.boundedType &&
    r1.stridable == r2.stridable
  {
    if r1._low != r2._low then return false;
    if r1._high != r2._high then return false;
    if r1.stride != r2.stride then return false;
    if r1._alignment != r2._alignment then return false;
    return true;
  }
  
  proc ident(r1: rangeBase(?), r2: rangeBase(?))
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
  
  inline proc rangeBase.boundsCheck(other: rangeBase(?e,?b,?s)) where b == BoundedRangeType.boundedNone
    return true;
  
  proc rangeBase.boundsCheck(other: rangeBase(?e,?b,?s))
  {
    var boundedOther: rangeBase(idxType, BoundedRangeType.bounded, s || this.stridable);
  
    if other.hasLowBound() then
      boundedOther._low = other._low;
    else
      boundedOther._low = _low;
  
    if other.hasHighBound() then
      boundedOther._high = other._high;
    else
      boundedOther._high = _high;
  
    boundedOther._stride = other._stride;
    boundedOther._alignment = other._alignment;
  
    return (boundedOther.length == 0) || member(boundedOther);
  }
  
  inline proc rangeBase.boundsCheck(other: idxType)
    return member(other);
  
  
  //################################################################################
  //# Member functions
  //#
  
  // Moves the low bound of the range up to the next alignment point.
  proc rangeBase.alignLow()
  {
    if stridable then _low = this.alignedLow;
    return this;
  }
  
  // Moves the high bound of the range down to the next alignment point.
  proc rangeBase.alignHigh()
  {
    if stridable then _high = this.alignedHigh;
    return this;
  }
  
  proc rangeBase.indexOrder(i: idxType): idxType
  {
    if ! member(i) then return (-1):idxType;
    if ! stridable then return i - _low;
    else return ((i:strType - this.first:strType) / _stride):idxType;
  }
  
  proc rangeBase.orderToIndex(i: integral): idxType
  {
    if boundsChecking {
      if i < 0 then
        halt("invoking orderToIndex on a negative integer: ", i);
  
      if isBoundedRangeB(this) && i >= this.length then
        halt("invoking orderToIndex on an integer ", i,
             " that is larger than the range's number of indices ", this.length);
    }
  
    return chpl__addRangeStrides(this.first, this.stride, i);
  }
  
  //////////////////////////////////////////////////////////////////////////////////
  // translate the indices in this range by i
  //
  // REVIEW: hilde
  // Should member functions normally return new objects?
  //
  // NOTE: The casts below assume that the value will fit into
  // idxType.  Handling errors when (down) casting is something
  // we need to handle more generally in the future, so for
  // consistency, we are not handling it here at all :-P
  //
  proc rangeBase.translate(i: integral)
  {
    if _isSignedType(i.type) then
      return this + i:idxType;
    else if i >= 0 then return this + i:idxType;
    else return this - abs(i):idxType;
  }
  
  proc rangeBase.translate(i)
  {
    compilerError("offsets must be of integral type");
  }
  
  // Return an interior portion of this range.
  // TODO: hilde
  // Alignment is set to true only if stridable is already set to true.
  proc rangeBase.interior(i: idxType)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("interior is not supported on unbounded ranges");
  }
  
  proc rangeBase.interior(i: idxType)
    where stridable == false
  {
    if i < 0 then
      return new rangeBase(idxType, boundedType, false,
                       _low, _low- 1 - i);
    if i > 0 then
      return new rangeBase(idxType, boundedType, false,
                       _high + 1 - i, _high);
    // if i == 0 then
    return new rangeBase(idxType, boundedType, false,
                     _low, _high);
  }
  
  proc rangeBase.interior(i: idxType)
  {
    if i < 0 then
      return new rangeBase(idxType, boundedType, true,
                       _low, _low-1-i, _stride, _alignment);
    if i > 0 then
      return new rangeBase(idxType, boundedType, true,
                       _high + 1 - i, _high, _stride, _alignment);
    // if i == 0 then
    return new rangeBase(idxType, boundedType, true,
                    _low, _high, _stride, _alignment);
  }
  
  // Return an exterior portion of this range.
  proc rangeBase.exterior(i: idxType)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("exterior is not supported on unbounded ranges");
  }
  
  proc rangeBase.exterior(i: idxType)
    where stridable == false
  {
    if i < 0 then
      return new rangeBase(idxType, boundedType, false,
                       _low+i, _low-1);
    if i > 0 then
      return new rangeBase(idxType, boundedType, false,
                       _high+1, _high+i);
    // if i == 0 then
    return new rangeBase(idxType, boundedType, false,
                     _low, _high);
  }
  
  proc rangeBase.exterior(i: idxType)
  {
    if i < 0 then
      return new rangeBase(idxType, boundedType, true,
                       _low - i, _low - 1, _stride, _alignment);
    if i > 0 then
      return new rangeBase(idxType, boundedType, true,
                       _high + 1, _high + i, _stride, _alignment);
    // if i == 0 then
    return new rangeBase(idxType, boundedType, true,
                     _low, _high, _stride, _alignment);
  }
  
  proc rangeBase.expand(i: idxType)
    where boundedType != BoundedRangeType.bounded
  {
    compilerError("expand() is not supported on unbounded ranges");
  }
  
  // Returns an expanded range, or a contracted range if i < 0.
  // The existing absolute alignment is preserved.
  proc rangeBase.expand(i: idxType)
  {
    return new rangeBase(idxType, boundedType, stridable,
                     _low-i, _high+i, stride, _alignment);
  }
  
  
  //################################################################################
  //# Syntax Functions
  //#
  
  // Assignment
  pragma "compiler generated"
    // The "compiler generated" flag is added so this explicit definition of
    // assignment does not disable the POD optimization.
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
    // and default constructor calls, in which case the optimization should only
    // be applied when all four of these functions (or their generic equivalents) 
    // are trivial.

    // See also removePODinitDestroy() in removeUnnecessaryAutoCopyCalls.cpp.
  proc =(r1: rangeBase(stridable=?s1), r2: rangeBase(stridable=?s2))
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
  
    return r1;
  }
  
  //////////////////////////////////////////////////////////////////////////////////
  // Range arithmetic.
  //
  // Shifts and entire range to the right or left.
  // Absolute alignment is not preserved
  // (That is, the alignment shifts along with the range.)
  //
  proc +(r: rangeBase(?e,?b,?s), i: integral)
  {
    type resultType = (r._low+i).type;
    type strType = chpl__signedType(resultType);
  
    return new rangeBase(resultType, b, s,
                     r._low + i, r._high + i,
           r.stride : strType, r._alignment + i : resultType);
  }
  
  inline proc +(i:integral, r: rangeBase(?e,?b,?s))
    return r + i;
  
  proc -(r: rangeBase(?e,?b,?s), i: integral)
  {
    type resultType = (r._low+i).type;
    type strType = chpl__signedType(resultType);
  
    return new rangeBase(resultType, b, s,
                     r._low - i, r._high - i,
           r._stride : strType, r._alignment - i : resultType);
  }
  
  proc chpl__align(r : rangeBase(?e, ?b, ?s), algn) {
    // Note that aligning an unstrided range will set the field value,
    // but has no effect on the index set produced (a mod 1 == 0).
    return new rangeBase(e, b, s,
                         r._low, r._high, r.stride, algn);
  }
  
  // Composition
  // Return the intersection of this and other.
  proc rangeBase.this(other: rangeBase(?))
  {
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
    if (_isUnsignedType(idxType)) { if (lo1 < 0) then lo1 = 0; }
  
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
  
    var result = new rangeBase(idxType,
                           computeBoundedType(this, other),
                           this.stridable | other.stridable,
                           max(lo1, lo2):idxType,
                           min(hi1, hi2):idxType,
                           newStride);
  
   if result.stridable {
    var al1 = (this._alignment % st1:idxType):int;
    var al2 = (other._alignment % st2:other.idxType):int;
  
    if (al2 - al1) % g != 0 then
    {
      // empty intersection, return degenerate result
      if !isBoundedRangeB(result) then
        halt("could not represent range slice - it needs to be empty, but the slice type is not bounded");
      (result._low, result._high, result._alignment) =
      (1:idxType, 0:idxType, if this.stride > 0 then 1:idxType else 0:idxType);
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
   } else {
      // !(result.stridable)
      result._alignment = 0;
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
  
  proc chpl__count(r:rangeBase(?), i:integral)
    where r.boundedType == BoundedRangeType.boundedNone
  {
    compilerError("count operator is not defined for unbounded ranges");
  }
  
  proc chpl__count(r:rangeBase(?), count:integral)
  {
    type resultType = r.idxType;
    type strType = chpl__signedType(resultType);
  
    if (count == 0) then
      // Return a degenerate range.
      return new rangeBase(idxType = resultType,
                       boundedType = BoundedRangeType.bounded,
                       stridable = r.stridable,
                       _low = 1,
                       _high = 0,
                       _stride = r.stride,
                       _alignment = 1);
  
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
  
    return new rangeBase(idxType = resultType,
                         boundedType = BoundedRangeType.bounded,
                         stridable = r.stridable,
                         _low = lo,
                         _high = hi,
                         _stride = r.stride : strType,
                         _alignment = r._alignment);
  }
  
  
  //################################################################################
  //# Iterators
  //#
  
  // Default iterator optimized for unit stride
  iter rangeBase.these()
  {
    if boundedType != BoundedRangeType.bounded
    {
      if boundedType == BoundedRangeType.boundedNone then
        compilerError("iteration over a range with no bounds");
  
      if ! hasFirst() then
        halt("iteration over range that has no first index");
  
      var i = this.first;
      while true
      {
        yield i;
        i = i + stride:idxType;
      }
    }
    else
    {
      // a bounded range ...
  
      // This case is written so that the only control is the loop test.
      // Zippered iterator inlining currently requires this.
  
      var i = this.first;
      var end : idxType = if _low > _high then i else this.last + stride:idxType;
      while i != end
      {
        yield i;
        i = i + stride:idxType;
      }
    }
  }
  
  iter rangeBase.these(param tag: iterKind) where tag == iterKind.leader
  {
    if ! isBoundedRangeB(this) then
      compilerError("parallel iteration is not supported over unbounded ranges");
  
    if debugChapelRange then
      writeln("*** In range leader:"); // ", this);
    const ignoreRunning = dataParIgnoreRunningTasks;
    const minIndicesPerTask = dataParMinGranularity;
    const numSublocs = here.getChildCount();

    if localeModelHasSublocales && numSublocs != 0 {
      const len = this.length;
      const numChunks = _computeNumChunks(numSublocs,
                                          ignoreRunning,
                                          minIndicesPerTask,
                                          len);
      coforall chunk in 0..#numChunks {
        on here.getChild(chunk) {
          if debugDataParNuma {
            extern proc chpl_task_getSubloc(): chpl_sublocID_t;
            if chunk!=chpl_task_getSubloc() then
              writeln("*** ERROR: ON WRONG SUBLOC (should be "+chunk+
                      ", on "+chpl_task_getSubloc()+") ***");
          }
          const (lo,hi) = _computeBlock(len, numChunks, chunk, len-1);
          const locRange = lo..hi;
          const locLen = locRange.length;
          const numTasks = _computeNumChunks(here.numCores,
                                             ignoreRunning,
                                             minIndicesPerTask,
                                             locLen);
          coforall core in 0..#numTasks {
            const (low, high) = _computeBlock(locLen, numTasks, core, hi, lo, lo);
            if debugDataParNuma then
              writeln("(chunk, core, locRange, coreRange)",
                       (chunk, core, locRange, low..high));
            yield (low..high,);
          }
        }
      }
      
    } else {
      var v = this.length;
      var numChunks = _computeNumChunks(v);
  
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
  
  iter rangeBase.these(param tag: iterKind, followThis) where tag == iterKind.follower
  {
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
  
    if (isBoundedRangeB(myFollowThis) && !myFollowThis.stridable) ||
       myFollowThis.hasLast()
    {
      const flwlen = myFollowThis.length;
      if flwlen == 0 then
        return; // nothing to do
      if boundsChecking && this.hasLast() {
        // this check is for typechecking only
        if isBoundedRangeB(this) {
          if this.length < flwlen then
            halt("zippered iteration over a range with too few indices");
        } else
          assert(false, "hasFirst && hasLast do not imply isBoundedRangeB");
      }    
  
      // same as undensifyBounded(this, myFollowThis), but on a rangeBase
      var low: idxType  = this.orderToIndex(myFollowThis.first);
      if flwlen == 1
      {
        if debugChapelRange then
          writeln("Expanded range = ", low..low);
        yield low;
        return;
      }
  
      const stride = this.stride * myFollowThis.stride;
  
      var high: idxType = ( low: strType + stride * (flwlen - 1):strType ):idxType;
      assert(high == this.orderToIndex(myFollowThis.last));
      if stride < 0 then low <=> high;
      assert(low <= high);
  
      const r = low .. high by stride:strType;
      if debugChapelRange then
        writeln("Expanded range = ",r);
  
      // todo: factor out this loop (and the above writeln) into a function?
      for i in r do
        yield i;
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
  proc rangeBase.readWriteThis(f)
  {
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
  
  // Return a substring of a string with a range of indices.
  inline proc string.substring(r: rangeBase(?))
  {
    if r.boundedType != BoundedRangeType.bounded then
      compilerError("substring indexing undefined on unbounded ranges");
  
    if r.stride != 1 then
      return __primitive("string_strided_select", this, r.alignedLow, r.alignedHigh, r.stride);
    else
      return __primitive("string_select", this, r.low, r.high);
  }
  
  
  //################################################################################
  //# Internal helper functions.
  //#
  
  proc rangeBase.chpl__unTranslate(i: idxType)
  {
    return this - i;
  }
  
  proc rangeBase.chpl__unTranslate(i)
  {
    if _isSignedType(i.type) then
      return this - i;
    else
      return this + abs(i);
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
    if _isSignedType(dividend.type) then
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
    if !_isIntegralType(t) then
      compilerError("Values must be of integral type.");
  
    if a > 0 && b > 0 && b > max(t) - a then return max(resultType);
    if a < 0 && b < 0 && b < min(t) - a then return min(resultType);
  
    // If the result is unsigned, check for a negative result and peg
    // the result to 0 if the sum is going to be negative.
    if _isUnsignedType(resultType) {
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
