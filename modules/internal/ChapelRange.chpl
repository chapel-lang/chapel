// ChapelRange.chpl
//

use DSIUtil;

config param debugChapelRange = false;        // Turns on range iterator debugging.

//
// range type
//
//   parameterized by:
//        1) an integral element type,
//        2) whether low and/or high bounds exist, and
//        3) whether the stride is one or not.
//
// We may add parameterization later on whether the alignment (offset) is zero,
// but at present, no significant performance advantage is available.
// A range can only be aligned if it is strided.  Since alignments are taken
// modulo the stride, if the stride is one, even a nonzero alignment has no effect.
//
// The type of the stride is a signed type with the same number of bits as the
// index type.  This means that for an unsigned index type, a maximal range can
// be reached in no less than two strides.
//
enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

pragma "range"
record range
{
  type idxType = int;                            // element type
  param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
  param stridable: bool = false;                 // range can be strided
  param aligned: bool = false;
  
  var _low: idxType = 1;                         // lower bound
  var _high: idxType = 0;                        // upper bound
  var _stride: chpl__idxTypeToStrType(idxType) = 1;    // signed stride of range
  var _alignment: idxType = 0;                         // alignment relative to _low
  
  var _promotionType: idxType;                  // enables promotion
  
  pragma "inline" proc low return _low;       // public getter for low bound
  pragma "inline" proc high return _high;     // public getter for high bound
  pragma "inline" proc stride return _stride; // public getter for stride
  pragma "inline" proc alignment return _alignment;        // public getter for alignment

}

//################################################################################
//# Constructors
//#

// Declare this as constructor, so we can capture range creation.
// If it is not a constructor, then the user can still create a maximal range
// (for example) without begin warned.
//
proc range.range(type idxType = int,
                param boundedType : BoundedRangeType = BoundedRangeType.bounded,
           param stridable : bool = false,
           param aligned : bool = false,
           _low : idxType = 1,
           _high : idxType = 0,
           _stride : chpl__idxTypeToStrType(idxType) = 1,
           _alignment: idxType = 0)
{
  this._low = _low;
  this._high = _high;
  if stridable then
    this._stride = _stride;
  if aligned then
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


//////////////////////////////////////////////////////////////////////////////////
// Range builders for bounded ranges
//
proc _build_range(low: int, high: int)
  return new range(idxType = int, _low = low, _high = high);
proc _build_range(low: uint, high: uint)
  return new range(uint, _low = low, _high = high);
proc _build_range(low: int(64), high: int(64))
  return new range(int(64), _low = low, _high = high);
proc _build_range(low: uint(64), high: uint(64))
  return new range(uint(64), _low = low, _high = high);


//////////////////////////////////////////////////////////////////////////////////
// Range builders for unbounded ranges
//
proc _build_range(param bt: BoundedRangeType, bound: int)
  return new range(int, bt, false, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: uint)
  return new range(uint, bt, false, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: int(64))
  return new range(int(64), bt, false, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: uint(64))
  return new range(uint(64), bt, false, false, bound, bound);
proc _build_range(param bt: BoundedRangeType)
  return new range(int, bt);


//################################################################################
//# Accessors
//#

// Returns the starting index (with minimal checks).
proc range.first
{
  if ! stridable then return _low;
  else return if _stride > 0 then this.alignedLow else this.alignedHigh;
}

// Returns the ending index (with minimal checks).
proc range.last
{
  if ! stridable then return _high;
  else return if _stride > 0 then this.alignedHigh else this.alignedLow;
}

// Returns the low index, properly aligned.
proc range.alignedLow : idxType
{
  if ! stridable then return _low;
  else
  {
    if _low > _high then return _low;
    return _low + chpl__mod(_alignment, _stride);
  }
}

// Returns the high index, properly aligned.
proc range.alignedHigh : idxType
{
  if ! stridable then return _high;
  else
  {
    if _low > _high then return _high;
    return _high - chpl__diffMod(_high, _low + _alignment, _stride);
  }
}

// Returns the number of elements in this range.
proc range.length
{
  if boundedType != BoundedRangeType.bounded then
    compilerError("Unbounded range has infinite length.");

  if _low > _high then
    return 0:idxType;

  var s = abs(_stride): idxType;
  return (_high - this.alignedLow) / s + 1;
}

//################################################################################
//# Predicates
//#

// Returns true if this range has a low bound.
proc range.hasLowBound() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedLow;

// Returns true if this range has a high bound.
proc range.hasHighBound() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedHigh;

// Returns true if i is in this range.
proc range.member(i: idxType)
{
  if (hasLowBound() && i < _low) then return false;
  if (hasHighBound() && i > _high) then return false;
  if stridable
  {
    var s = abs(_stride):idxType;
    if (i % s - this.alignedLow % s) % s != 0
      then return false;
  }
  return true;
}

// Returns true the other range is contained within this one.
proc range.member(other: range(?))
{
  return other.equiv(this(other));
}

// Returns true this range is equivalent to other.
// Equivalent ranges produce the same index set.
//
// This routine relies on the assumption that
// the stride of an unstrided range is set to 1.
proc range.equiv(other: range(?))
{
  // Cheapest test first!
  if this._stride != other._stride then return false;
  if this.first != other.first then return false;
  if this.last != other.last then return false;
  return true;
}

// This makes use of the fact that the default values for the
// stride and alignment fields are consistent, whether the two ranges
// being compared are stridable or not, aligned or not.
proc ==(r1: range(?), r2: range(?))
{
  if r1._low != r2._low then return false;
  if r1._high != r2._high then return false;
  if r1._stride != r2._stride then return false;
  if r1._alignment != r2._alignment then return false;
  return true;
}

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
proc range.boundsCheck(other: range(?e,?b,?s)) where b == BoundedRangeType.boundedNone
  return true;

proc range.boundsCheck(other: range(?e,?b,?s))
{
  var boundedOther: range(idxType, BoundedRangeType.bounded, s || this.stridable);

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
proc range.boundsCheck(other: idxType)
  return member(other);


//################################################################################
//# Member functions
//#

// Align the low bound of this range to its alignment.
//
// Moves the low bound of the range up to the next alignment point,
// and sets the alignment to zero.
// This preserves the invariant that alignment is relative to _low.
proc range.alignLow()
{
  if ! stridable then return;
  _low = this.alignedLow;
  _alignment = 0;
}

// Align the high bound of this range to its alignment.
//
// Moves the high bound of the range down to the next alignment point.
proc range.alignHigh()
{
  if ! stridable then return;
  _high = this.alignedHigh;
}

proc range.indexOrder(i: idxType)
{
  if ! member(i) then return (-1):idxType;
  if ! stridable then return i - _low;

  var s = abs(_stride):idxType;
  return (i - this.alignedLow) / s;
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
proc range.translate(i: integral)
{
  if _isSignedType(i.type) then
    return this + i:idxType;
  else if i >= 0 then return this + i:idxType;
  else return this - abs(i):idxType;
}

proc range.translate(i)
{
  compilerError("offsets must be of integral type");
}

// Return an interior portion of this range.
// TODO: hilde
// Alignment is set to true only if stridable is already set to true.
proc range.interior(i: idxType)
  where boundedType != BoundedRangeType.bounded
{
  compilerError("interior is not supported on unbounded ranges");
}

proc range.interior(i: idxType)
  where stridable == false
{
  if i == 0 then return new range(idxType, boundedType, false, false,
				  _low, _high);
  if i < 0
    then return new range(idxType, boundedType, false, false,
                          _low, _low- 1 - i);
    else return new range(idxType, boundedType, false, false,
                          _high + 1 - i, _high);
}

proc range.interior(i: idxType)
{
  if i == 0 then return new range(idxType, boundedType, true, true,
  				  _low, _high, _stride, _alignment);
  if i < 0
    then return new range(idxType, boundedType, true, true,
                          _low, _low-1-i, _stride, _alignment);
    else return new range(idxType, boundedType, true, true,
                          _high + 1 - i, _high, _stride,
		          chpl__diffMod(_low + _alignment, _high + 1 - i, _stride));
}

// Return an exterior portion of this range.
proc range.exterior(i: idxType)
  where boundedType != BoundedRangeType.bounded
{
  compilerError("exterior is not supported on unbounded ranges");
}

proc range.exterior(i: idxType)
  where stridable == false
{
  if i == 0 then return new range(idxType, boundedType, false, false,
                                  _low, _high);
  if i < 0
    then return new range(idxType, boundedType, false, false,
                          _low+i, _low-1);
    else return new range(idxType, boundedType, false, false,
                          _high+1, _high+i);
}

proc range.exterior(i: idxType)
{
  if i == 0 then return new range(idxType, boundedType, true, true,
       	    	 	          _low, _high, _stride, _alignment);
  if i < 0
    then return new range(idxType, boundedType, true, true,
          	          _low - i, _low - 1, _stride, _alignment - i);
    else return new range(idxType, boundedType, true, true,
                          _high + 1, _high + i, _stride,
			  chpl__diffMod(_low + _alignment, _high + 1, _stride));
}

// Returns an expanded range, or a contracted range if i < 0.
// The existing absolute alignment is preserved.
proc range.expand(i: idxType)
{
  return new range(idxType, boundedType, stridable, true,
                   _low-i, _high+i, _stride, _alignment+i);
}


//################################################################################
//# Syntax Functions
//#

// Assignment
proc =(r1: range(stridable=?s1), r2: range(stridable=?s2))
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
proc +(r: range(?e,?b,?s, ?a), i: integral)
  return new range((r._low+i).type, b, s, a,
  		    r._low+i, r._high+i, r._stride, r._alignment);

proc +(i:integral, r: range(?e,?b,?s,?a))
  return new range((i+r._low).type, b, s, a,
                    i+r._low, i+r._high, r._stride, r._alignment);

proc -(r: range(?e,?b,?s,?a), i: integral)
  return new range((r._low-i).type, b, s, a,
                    r._low-i, r._high-i, r._stride, r._alignment);

// This is the syntax processing routine for the "by" keyword.
proc by(r : range(?), str)
{
  proc byHelp(r : range(?e, ?b, ?s, ?a), str : chpl__idxTypeToStrType(e))
  {
    return new range(e, b, true, a,
                     r._low, r._high, r._stride*str, r._alignment);
  }
  
  if str == 0 then
    halt("range cannot be strided by zero");

  type idxType = r.idxType;
  type strType = chpl__idxTypeToStrType(idxType);
  type argType = str.type;
  
  if (argType == strType) then return byHelp(r, str);
  else if (argType == idxType || chpl__legalIntCoerce(argType, strType)) then
    return byHelp(r, str:strType);
  else
    compilerError("type mismatch applying 'by' to range(", typeToString(idxType),
                  ") with ", typeToString(argType));
}

// This is the syntax processing routine for the "align" keyword.
// It produces a new range with the specified alignment.
// By definition, alignment is relative to the low bound of the range.
proc align(r : range(?e, ?b, ?s, ?a), algn)
{
  type idxType = r.idxType;
  type argType = algn.type;

  if argType == idxType || chpl__legalIntCoerce(argType, idxType) then
    return new range(e, b, s, true,
                     r._low, r._high, r._stride, algn);
  else
    compilerError("type mismatch applying 'align' to range(", typeToString(idxType),
                  ") with ", typeToString(argType));
}

// Apply an absolute alignment to an existing range.
proc range.absAlign(algn)
{
  type argType = algn.type;

  if argType == idxType || chpl__legalIntCoerce(argType, idxType)
  {
    this._alignment = algn - _low;
  }
  else
    compilerError("type mismatch applying 'absAlign' to range(",
              typeToString(idxType), ") with ", typeToString(argType));

  if ! aligned then
  {
    compilerWarning("Applying an alignment to an unaligned range has no effect."); 
    this._alignment = 0; // Maintain the invariant.
  }
}

// Composition
// Return the intersection of this and other.
proc range.this(other: range(?idxType2, ?boundedType, ?stridable))
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

  // Returns (gcd(u, v), x) where x is set such that
  // u*x + v*y = gcd(u, v) assuming u and v are non-negative.
  //
  // source: Knuth Volume 2 --- Section 4.5.2
  //
  proc extendedEuclidHelp(u, v)
  {
    var zero: u.type = 0;
    var one: u.type = 1;

    var U = (one, zero, u);
    var V = (zero, one, v);

    while V(3) != 0 do
      (U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));

    return (U(3), U(1));
  }
  
  pragma "inline" proc extendedEuclid(u:int, v:int)
  { return extendedEuclidHelp(u,v); }

  pragma "inline" proc extendedEuclid(u:int(64), v:int(64))
  { return extendedEuclidHelp(u,v); }

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

  var (g, x) = extendedEuclid(st1, st2);
  var lcm = st1 / g * st2;        // The LCM of the two strides.
  // The division must be done first to prevent overflow.

  var result = new range(idxType,
                         computeBoundedType(this, other),
                         this.stridable | other.stridable,
                         this.stridable | other.stridable,
                         max(lo1, lo2):idxType,
                         min(hi1, hi2):idxType,
                         lcm:chpl__idxTypeToStrType(idxType));

  // hilde: We may be able to eliminate this test.
  if result.boundedType == BoundedRangeType.bounded &&
     result._low > result._high then
    // empty intersection
    return result;

  var al1 = (this.first % st1:idxType):int;
  var al2 = (other.first % st2:other.idxType):int;

  if (al2 - al1) % g != 0 then
    // empty intersection, return degenerate result
    (result._low, result._high) = (1:idxType, 0:idxType);
  else
  {
    // non-empty intersection

    // x and/or the diff may negative, even with a uint source range.
    var offset = (al2 - al1) * x;
    // offset is in the range [-(lcm-1), lcm-1]
    if offset < 0 then offset += lcm;
    // Now offset can be safely cast to idxType.
    var al = al1:idxType + offset:idxType * st1:idxType / g:idxType;
    result._alignment = chpl__diffMod(al, result._low, lcm);

    // We inherit the sign of the stride from the operand.
    if other._stride < 0 then 
      result._stride = -result._stride;
  }

  return result;
}

//////////////////////////////////////////////////////////////////////////////////
// syntax functions for counted ranges
//

// Creates a new range from an existing range, but with the specified number of elements.
// The new range has the same low bound, stride and alignment as the existing range.
proc #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedLow
{
  if i < 0 then halt("range cannot have a negative number of elements");

  type resultType = (r._low + i).type;

    // TODO: hilde
    // We need to check for overflow in the _high expression.
    // The way to do this is to make the default bound for an "unbounded"
  // range equal the max or min number representable in its index type.
  if i == 0 then
    return new range(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     aligned = r.aligned,
                     _low = 1,
                     _high = 0,
                     _stride = r._stride,
                     _alignment = r._alignment);

  // If the range is not already aligned, we align on the low bound by default.
  return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   aligned = r.aligned,
                   _low = r._low,
                   _high = r._low + (i-1)*abs(r._stride):resultType,
                   _stride = r._stride,
                   _alignment = r._alignment);
}

proc #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedHigh
{
  if i < 0 then halt("range cannot have a negative number of elements");

  type resultType = (r._low + i).type;

  if i == 0
    then return new range(resultType, BoundedRangeType.bounded, r.stridable, true,
                          1, 0, r._stride, r._alignment);

  // If the range is not already aligned, align on the high bound.
  var lo : resultType = r._high - (i-1)*abs(r._stride):resultType;
  var algn : resultType;
  if ! r.aligned then
    // This is the difference between _high and _low modulo the stride.
    algn = chpl__diffMod(r._high, lo, r._stride);
  else
    algn = chpl__diffMod(r._alignment, r._low, r._stride);

  // TODO: hilde
  // We need to check for underflow in the _low expression.
  return new range(resultType, BoundedRangeType.bounded, r.stridable, true,
                   lo, r._high, r._stride, algn);
}

proc #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.bounded
{
  if i < 0 then halt("range cannot have a negative number of elements");
  if i > r.length then
    halt("bounded range is too small to access ", i, " elements");

  type resultType = (r._low + i).type;

  if i == 0 then
    return new range(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     aligned = r.aligned,
                     _low = 1,
                     _high = 0,
                     _stride = r._stride,
                     _alignment = r._alignment);

  return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   aligned = r.aligned,
                   _low = if r._stride < 0 then r._high + (i-1)*r._stride:resultType else r._low,
                   _high = if r._stride < 0 then r._high else r._low + (i-1)*r._stride:resultType,
                   _stride = r._stride,
                   _alignment = r._alignment);
}

proc #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedNone
{
  compilerError("count operator is not defined for unbounded ranges");
}


//################################################################################
//# Iterators
//#

// Default iterator optimized for unit stride
iter range.these()
{
  if boundedType != BoundedRangeType.bounded
  {
    if boundedType == BoundedRangeType.boundedNone then
      halt("iteration over a range with no bounds");
    if stridable
    {
      if boundedType == BoundedRangeType.boundedLow then
        if _stride < 0 then
          halt("iteration over range with negative stride but no high bound");
      if boundedType == BoundedRangeType.boundedHigh then
        if _stride > 0 then
          halt("iteration over range with positive stride but no low bound");
      var i = this.first;
      while true
      {
        yield i;
        i = i + _stride:idxType;
      }
    }
    else
    {
      if boundedType == BoundedRangeType.boundedHigh then
        halt("iteration over range with positive stride but no low bound");
      var i = _low;
      while true
      {
        yield i;
        i = i + 1;
      }
    }
  }
  else
  {
    // a bounded range ...

    // This case is written so that the only control is the loop test.
    // Zippered iterator inlining currently requires this.

    var i = this.first;
    var end : idxType;

    if stridable
    {
      end = if _low > _high then i else this.last + _stride:idxType;
      while i != end
      {
        yield i;
        i = i + _stride:idxType;
      }
    }
    else
    {
      end = if _low > _high then i else this.last + 1;
      while i != end
      {
        yield i;
        i = i + 1;
      }
    }
  }
}

iter range.these(param tag: iterator) where tag == iterator.leader
{
  // want "yield 0..length-1;"
  // but compilerError in length causes a problem because leaders are
  // resolved wherever an iterator is.
  if boundedType == BoundedRangeType.boundedNone then
    halt("iteration over a range with no bounds");

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
      coforall chunk in 0..numChunks-1
      {
        const (lo,hi) = _computeBlock(v, numChunks, chunk, v-1);
        if debugChapelRange then
          writeln("*** RI: tuple = ", tuple(lo..hi));
        yield tuple(lo..hi);
      }
    }
  }
}

iter range.these(param tag: iterator, follower) where tag == iterator.follower
{
  if boundedType == BoundedRangeType.boundedNone then
    halt("iteration over a range with no bounds");
  if follower.size != 1 then
    halt("iteration over a range with multi-dimensional iterator");

  if debugChapelRange then
    writeln("In range follower code: Following ", follower);

  var followThis = follower(1);

  if debugChapelRange then
    writeln("Range = ", followThis);

  // It would be nice to be able to factor out the repeated code below,
  // but what is the type of r?.
  if stridable
  {
    // r is a range which contains the next chunk of values controlled by followThis.
    // The range in followThis usually has a stride of 1 (optimization opportunity?).
    var r = (if _stride > 0 then
         _low + followThis._low*_stride:idxType.._low+followThis._high*_stride:idxType
       else
        _high + followThis._high*_stride:idxType.._high+followThis._low*_stride:idxType
      ) by _stride by followThis._stride;

    if debugChapelRange then
      writeln("Expanded range = ",r);
    
    for i in r
    {
      __primitive("noalias pragma");
      yield i;
    }
  }
  else // ! stridable
  {
    var r = _low+followThis;

    if debugChapelRange then
      writeln("Expanded range = ",r);
    
    for i in r
    {
      __primitive("noalias pragma");
      yield i;
    }
  }
}

//################################################################################
//# Utilities
//#

// Write implementation for ranges
proc range.writeThis(f: Writer)
{
  if hasLowBound() then
    f.write(_low);
  f.write("..");
  if hasHighBound() then
    f.write(_high);
  if _stride != 1 then
    f.write(" by ", _stride);
  if _alignment != 0 then
    f.write(" align ", _alignment);
}

// Return a substring of a string with a range of indices.
pragma "inline" proc string.substring(s: range(?e,?b,?st))
{
  if s.boundedType != BoundedRangeType.bounded then
    compilerError("substring indexing undefined on unbounded ranges");

  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);
}


//################################################################################
//# Internal helper functions.
//#

// TODO: hilde
// This function should be migrated to a more global location.

proc chpl__legalIntCoerce(type t1, type t2) param
{
  if (_isSignedType(t2)) {
    if (_isSignedType(t1)) {
      return (numBits(t1) <= numBits(t2));
    } else {
      return (numBits(t1) < numBits(t2));
    }
  } else {
    if (_isSignedType(t1)) {
      return false;
    } else {
      return (numBits(t1) <= numBits(t2));
    }
  }
}

// 
// Return the number in the range 0 <= result < b that is congruent to a mod b
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
    if tmp < 0 then tmp += modulus;

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
                   subtrahend : minuend.type,
                   in modulus : integral) : minuend.type
{
  type minType = minuend.type;

  modulus = abs(modulus);
  var m = modulus : minType;
  if m : modulus.type != modulus then
    halt("Modulus too large.");

  var minMod = chpl__mod(minuend, m);
  var subMod = chpl__mod(subtrahend, m);
  return if minMod < subMod
    then m - (subMod - minMod)
    else minMod - subMod;
}

proc range.chpl__unTranslate(i: idxType)
{
  return this - i;
}

proc range.chpl__unTranslate(i)
{
  if _isSignedType(i.type) then
    return this - i;
  else
    return this + abs(i);
}

