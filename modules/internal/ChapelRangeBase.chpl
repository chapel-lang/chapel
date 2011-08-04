// ChapelRangeBase.chpl
//

use DSIUtil;

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
  pragma "inline" proc low return _low;       // public getter for low bound
  pragma "inline" proc high return _high;     // public getter for high bound
  pragma "inline" proc stride return if stridable then _stride else 1:_stride.type;
  pragma "inline" proc alignment return _alignment;        // public getter for alignment

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
      if _low <= _high && this.last + _stride : idxType == this.first then
        writeln("Maximal range declared.  ",
        "A for loop on this range will execute zero times.  ",
        "Try using a wider index type.");
    }
  }
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
pragma "inline" 
proc rangeBase.first
{
  if ! stridable then return _low;
  else return if _stride > 0 then this.alignedLow else this.alignedHigh;
}

// Returns the ending index (with minimal checks).
pragma "inline" 
proc rangeBase.last
{
  if ! stridable then return _high;
  else return if _stride > 0 then this.alignedHigh else this.alignedLow;
}

// Returns the low index, properly aligned.
// The aligned low bound may be higher than the high bound.
// The client must check that the low bound exists before calling this function.
pragma "inline"
proc rangeBase.alignedLow : idxType
{
  if ! stridable then return _low;

  // Adjust _low upward by the difference between _alignment and _low.
  return _low + chpl__diffMod(_alignment, _low, _stride);
}

// Returns the high index, properly aligned.
// The aligned high bound may be lower than the low bound.
// The client must check that the high bound exists before calling this function.
pragma "inline"
proc rangeBase.alignedHigh : idxType
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

//################################################################################
//# Predicates
//#

// Returns true if this range has a low bound.
proc rangeBase.hasLowBound() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedLow;

// Returns true if this range has a high bound.
proc rangeBase.hasHighBound() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedHigh;

pragma "inline"
proc rangeBase.isEmpty()
where boundedType == BoundedRangeType.bounded
  return this.alignedLow > this.alignedHigh;

pragma "inline"
proc rangeBase.isEmpty()
  return false;

pragma "inline" 
proc rangeBase.hasFirst()
{
  if this.isEmpty() then return false;
  return if stride > 0 then hasLowBound() else hasHighBound();
}
    
pragma "inline" 
proc rangeBase.hasLast()
{
  if this.isEmpty() then return false;
  return if stride > 0 then hasHighBound() else hasLowBound();
}

// Returns true if this range is naturally aligned, false otherwise.
proc rangeBase.isNaturallyAligned()
  where this.boundedType == BoundedRangeType.bounded
{
  // If the stride is positive, we must be aligned on the low bound.
  if _stride > 0 then return this.alignedLow == _low;
  // If the stride is negative, we must be aligned on the high bound.
  if _stride < 0 then return this.alignedHigh == _high;
  // _stride == 0: ???
  return false;
}

pragma "inline"
proc rangeBase.isNaturallyAligned()
  where this.boundedType == BoundedRangeType.boundedLow
{
  return this.alignedLow == _low;
}

pragma "inline"
proc rangeBase.isNaturallyAligned()
  where this.boundedType == BoundedRangeType.boundedHigh
{
  return this.alignedHigh == _high;
}

pragma "inline"
proc rangeBase.isNaturallyAligned()
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
pragma "inline"
proc rangeBase.member(other: rangeBase(?))
{
  return other == this(other);
}

// ==(r1,r2) returns true if the two ranges produce the same index sequence.

proc ==(r1: rangeBase(?), r2: rangeBase(?)) where
  r1.boundedType != r2.boundedType
{
  return false;
}

proc ==(r1: rangeBase(?), r2: rangeBase(?)) where
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

proc ==(r1: rangeBase(?), r2: rangeBase(?)) where
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

pragma "inline"
proc rangeBase.boundsCheck(other: rangeBase(?e,?b,?s)) where b == BoundedRangeType.boundedNone
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

pragma "inline"
proc rangeBase.boundsCheck(other: idxType)
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
  type strType = chpl__signedType(idxType);
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
                   _low-i, _high+i, _stride, _alignment);
}


//################################################################################
//# Syntax Functions
//#

// Assignment
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
		   r._stride : strType, r._alignment + i : resultType);
}

pragma "inline"
proc +(i:integral, r: rangeBase(?e,?b,?s))
  return r + i;

proc -(r: rangeBase(?e,?b,?s), i: integral)
{
  type resultType = (r._low+i).type;
  type strType = chpl__signedType(resultType);

  return new rangeBase(resultType, b, s,
                   r._low - i, r._high - i,
		   r._stride : strType, r._alignment - i : resultType);
}

proc chpl__byHelp(r : rangeBase(?e, ?b, ?s), str : chpl__signedType(e))
{
  if str == 0 then
    halt("range cannot be strided by zero");

  var s = r._stride * str;
  var al : e = 0;
  if b == BoundedRangeType.boundedLow then al = r.alignedLow;
  if b == BoundedRangeType.boundedHigh then al = r.alignedHigh;
  if b == BoundedRangeType.bounded then
    al = if s > 0 then r.alignedLow else r.alignedHigh;

  return new rangeBase(e, b, true,
                   r._low, r._high, s, al);
}

proc chpl__by(r : rangeBase(?), str)
{
  type idxType = r.idxType;
  type strType = r.strType;
  type argType = str.type;
  
  if (argType == strType) then return chpl__byHelp(r, str);
  else if (argType == idxType || chpl__legalIntCoerce(argType, strType)) then
    return chpl__byHelp(r, str:strType);
  else
    compilerError("type mismatch applying 'by' to range(", typeToString(idxType),
                  ") with ", typeToString(argType));
}

proc chpl__align(r : rangeBase(?e, ?b, ?s), algn)
{
  type idxType = r.idxType;
  type argType = algn.type;

  if argType == idxType || chpl__legalIntCoerce(argType, idxType) then
    // Note that aligning an unstrided range will set the field value,
    // but has no effect on the index set produced (a mod 1 == 0).
    return new rangeBase(e, b, s,
                     r._low, r._high, r._stride, algn);
  else
    compilerError("type mismatch applying 'align' to range(", typeToString(idxType),
                  ") with ", typeToString(argType));
}

// Apply a natural alignment to an existing range.
proc rangeBase.offset(offs : integral)
{
  type argType = offs.type;

  if argType == idxType || chpl__legalIntCoerce(argType, idxType)
  {
    if this.hasLowBound() then
      this._alignment = _low + offs;
    else if this.boundedType == BoundedRangeType.boundedHigh then
      this._alignment = _high + offs;
    else
      this._alignment = offs;
  }
  else
    compilerError("type mismatch applying 'absAlign' to range(",
              typeToString(idxType), ") with ", typeToString(argType));

  if ! stridable then
  {
    compilerWarning("Applying an alignment to an unstrided range has no effect."); 
    this._alignment = 0; // Maintain the invariant.
  }
  return this;
}

// Composition
// Return the intersection of this and other.
proc rangeBase.this(other: rangeBase(?idxType2, ?boundedType, ?stridable))
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
  var st1 = abs(this._stride);

  var lo2 = if other.hasLowBound() then other._low else this._low;
  var hi2 = if other.hasHighBound() then other._high else this._high;
  var st2 = abs(other._stride);

  // If the result type is unsigned, don't let the low bound go negative.
  // This is a kludge.  We should really obey type coercion rules. (hilde)
  if (_isUnsignedType(idxType)) { if (lo1 < 0) then lo1 = 0; }

  var (g, x) = chpl__extendedEuclid(st1, st2);
  var lcm = st1 / g * st2;        // The LCM of the two strides.
  // The division must be done first to prevent overflow.

  var result = new rangeBase(idxType,
                         computeBoundedType(this, other),
                         this.stridable | other.stridable,
                         max(lo1, lo2):idxType,
                         min(hi1, hi2):idxType,
                         lcm:chpl__signedType(idxType));

  var al1 = (this._alignment % st1:idxType):int;
  var al2 = (other._alignment % st2:other.idxType):int;

  if (al2 - al1) % g != 0 then
    // empty intersection, return degenerate result
    (result._low, result._high, result._alignment) =
    (1:idxType, 0:idxType, 1:idxType);
    // _alignment == _low, so it won't print.
  else
  { // non-empty intersection

    // x and/or the diff may negative, even with a uint source range.
    var offset = (al2 - al1) * x;
    // offset is in the range [-(lcm-1), lcm-1]
    if offset < 0 then offset += lcm;

    // Now offset can be safely cast to idxType.
    result._alignment = al1:idxType + offset:idxType * st1:idxType / g:idxType;

    // We inherit the sign of the stride from the operand.
    if other._stride < 0 then 
      result._stride = -result._stride;
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

proc chpl__count(r:rangeBase(?), i:integral)
{
  type resultType = (r._low+i).type;
  type strType = chpl__signedType(resultType);

  if (i == 0) then
    // Return a degenerate range.
    return new rangeBase(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = 1,
                     _high = 0,
                     _stride = r._stride,
                     _alignment = 1);

  if i > 0 && !r.hasFirst() then
    halt("With a positive count, the range must have a first index.");
  if i < 0 && !r.hasLast() then
    halt("With a negative count, the range must have a last index.");
  if r.boundedType == BoundedRangeType.bounded && abs(i) > r.length then
    halt("bounded range is too small to access ", abs(i), " elements");

  // The distance between the first and last indices.
  var s = r.stride : strType;
  var diff = i : strType * s;

  var lo : resultType =
    if diff > 0 then r._low
    else chpl__add(r._high : resultType, (diff + 1): resultType);
  var hi : resultType =
    if diff < 0 then r._high
    else chpl__add(r._low : resultType, diff : resultType - 1);

  if r.hasLowBound() && lo < r._low then lo = r._low;
  if r.hasHighBound() && hi > r._high then hi = r._high;

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

iter rangeBase.these(param tag: iterator) where tag == iterator.leader
{
  if ! isBoundedRangeB(this) then
    compilerError("parallel iteration is not supported over unbounded ranges");

  if debugChapelRange then
    writeln("*** In range leader:"); // ", this);

  var v = this.length;
  var numChunks = _computeNumChunks(v);

  if debugChapelRange
  {
    writeln("*** RI: length=", v, " numChunks=", numChunks);
    writeln("*** RI: Using ", numChunks, " chunk(s)");
  }

  if (CHPL_TARGET_PLATFORM == "xmt")
  {
    var per_stream_i: uint(64) = 0;
    var total_streams_n: uint(64) = 0;

    __primitive_loop("xmt pragma forall i in n", per_stream_i,
                     total_streams_n) {
      const (lo,hi) = _computeBlock(v, total_streams_n, per_stream_i, v-1);
      yield tuple(lo..hi);
    }
  }
  else
  {
    if numChunks == 1 then
      yield tuple(0..v-1);
    else
    {
      coforall chunk in 0..#numChunks
      {
        const (lo,hi) = _computeBlock(v, numChunks, chunk, v-1);
        if debugChapelRange then
          writeln("*** RI: tuple = ", tuple(lo..hi));
        yield tuple(lo..hi);
      }
    }
  }
}

iter rangeBase.these(param tag: iterator, follower) where tag == iterator.follower
{
  if boundedType == BoundedRangeType.boundedNone then
    compilerError("iteration over a range with no bounds");
  if ! stridable && boundedType == BoundedRangeType.boundedHigh then
    compilerError("iteration over a range with no first index");

  if follower.size != 1 then
    compilerError("iteration over a range with multi-dimensional iterator");

  if debugChapelRange then
    writeln("In range follower code: Following ", follower);

  var followThis = follower(1);

  if debugChapelRange then
    writeln("Range = ", followThis);

  if ! this.hasFirst() then
    halt("iteration over a range with no first index");
  if ! followThis.hasFirst() then
    halt("zippered iteration over a range with no first index");

  if (isBoundedRangeB(followThis) && !followThis.stridable) ||
     followThis.hasLast()
  {
    const flwlen = followThis.length;
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

    // same as undensifyBounded(this, followThis), but on a rangeBase
    var low: idxType  = this.orderToIndex(followThis.first);
    if flwlen == 1
    {
      if debugChapelRange then
        writeln("Expanded range = ", low..low);
      yield low;
      return;
    }

    const stride = this.stride * followThis.stride;
    var high: idxType = ( low + stride * (flwlen - 1) ):idxType;
    assert(high == this.orderToIndex(followThis.last));
    if stride < 0 then low <=> high;
    assert(low <= high);

    const r = low .. high by stride:strType;
    if debugChapelRange then
      writeln("Expanded range = ",r);

    // todo: factor out this loop (and the above writeln) into a function?
    for i in r
    {
      __primitive("noalias pragma");
      yield i;
    }
  }
  else // ! followThis.hasLast()
  {
    // WARNING: this case has not been tested
    if boundsChecking && this.hasLast() then
      halt("zippered iteration where a bounded range follows an unbounded iterator");

    const first  = this.orderToIndex(followThis.first);
    const stride = this.stride * followThis.stride;

    if stride > 0
    {
      const r = first .. by stride:strType;
      if debugChapelRange then
        writeln("Expanded range = ",r);
    
      for i in r
        {
          __primitive("noalias pragma");
          yield i;
        }
    }
    else
    {
      const r = .. first by stride:strType;
      if debugChapelRange then
        writeln("Expanded range = ",r);
    
      for i in r
        {
          __primitive("noalias pragma");
          yield i;
        }
    }
  } // if followThis.hasLast()
}


//################################################################################
//# Utilities
//#

// Write implementation for ranges
proc rangeBase.writeThis(f: Writer)
{
  if hasLowBound() then
    f.write(_low);
  f.write("..");
  if hasHighBound() then
    f.write(_high);
  if _stride != 1 then
    f.write(" by ", _stride);

  // Write out the alignment only if it differs from natural alignment.
  // We take alignment modulo the stride for consistency.
  if ! isNaturallyAligned() then
    f.write(" align ", chpl__mod(_alignment, _stride));
}

// Return a substring of a string with a range of indices.
pragma "inline" proc string.substring(r: rangeBase(?))
{
  if r.boundedType != BoundedRangeType.bounded then
    compilerError("substring indexing undefined on unbounded ranges");

  if r._stride != 1 then
    return __primitive("string_strided_select", this, r.alignedLow, r.alignedHigh, r._stride);
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
  if m : modulus.type != modulus then
    halt("Modulus too large.");

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
  if m : modulus.type != modulus then
    halt("Modulus too large.");

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
proc chpl__add(a: ?t, b: t)
{
  if !_isIntegralType(t) then
    compilerError("Values must be of integral type.");

  if a > 0 && b > 0 && b > max(t) - a then return max(t);
  if a < 0 && b < 0 && b < min(t) - a then return min(t);
  return a + b;
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

pragma "inline" proc chpl__extendedEuclid(u:int, v:int)
{ return chpl__extendedEuclidHelper(u,v); }

pragma "inline" proc chpl__extendedEuclid(u:int(64), v:int(64))
{ return chpl__extendedEuclidHelper(u,v); }

