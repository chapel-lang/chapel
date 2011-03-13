// ChapelRange.chpl
//

use DSIUtil;

enum ChapelRangeMethod { nothing, count, these };

// Turns on range iterator debugging.
config param debugChapelRange = ChapelRangeMethod.nothing;

//
// range type
//
//   parameterized by:
//        1) an integral element type,
//        2) whether low and/or high bounds exist, and
//        3) whether the stride is one or not.
//        4) whether the alignment is nonzero.
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

pragma "range"
record range
{
  type idxType = int;                            // element type
  param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
  param stridable: bool = false;                 // range can be strided
  param aligned: bool = false;
  
  var _low: idxType = 1;                         // lower bound
  var _high: idxType = 0;                        // upper bound
  var _stride: chpl__signedType(idxType) = 1;    // signed stride of range
  var _alignment: idxType = 0;                   // alignment
  
  var _promotionType: idxType;                   // enables promotion
  
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
           _stride : chpl__signedType(idxType) = 1,
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
  if ! this.hasLowBound() then
    halt("alignedLow -- No low bound defined for this range.");
  else
  {
    if ! stridable then return _low;
    else
    {
      if _low > _high then return _low;
      return _low + chpl__mod(_alignment, _stride);
    }
  }
}

// Returns the high index, properly aligned.
proc range.alignedHigh : idxType
{
  if ! this.hasHighBound() then
    halt("alignedHigh -- No high bound defined for this range.");
  else
  {
    if ! stridable then return _high;
    else
    {
      if _low > _high then return _high;

      // I don't think this will work, since we tested boundedType above.
      if this.hasLowBound() then
        return _high - chpl__diffMod(_high, _low + _alignment, _stride);
      else
        return _high - chpl__mod(_alignment, _stride);
    }
  }
}

// Returns the number of elements in this range.
proc range.length
{
  if boundedType != BoundedRangeType.bounded then
    compilerError("Unbounded range has infinite length.");
  else
  {
    if _low > _high then
      return 0:idxType;

    var s = abs(_stride): idxType;
    return (_high - this.alignedLow) / s + 1;
  }
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

proc range.hasFirst()
{
  if stridable
  {
    if _stride > 0
    {
      if ! hasLowBound() then return false;
    }
    else
    {
      if ! hasHighBound() then return false;
    }
  }
  return true;
}
    
proc range.hasLast()
{
  if stridable
  {
    if _stride > 0
    {
      if ! hasHighBound() then return false;
    }
    else
    {
      if ! hasLowBound() then return false;
    }
  }
  return true;
}
    
// Returns true if i is in this range.
proc range.member(i: idxType)
{
  var al : idxType;
  if hasHighBound()
  {
    if i > _high then return false;
    al = this.alignedHigh;
  }
  if hasLowBound()
  {
    if i < _low then return false;
    al = this.alignedLow;
  }
  if stridable
  {
    var s = abs(_stride):idxType;
    // We don't have to use chpl__mod here, because we only care
    // whether the remainder is different from zero.
    if (i % s - al % s) % s != 0
      then return false;
  }
  return true;
}

// Returns true if the other range is contained within this one.
proc range.member(other: range(?))
{
  return other == this(other);
}

// Returns true if this range is equivalent to the other.
// Equivalent ranges produce the same index set.
//
// This routine relies on the assumption that
// the stride of an unstrided range is set to 1.
proc ==(r1: range(?), r2: range(?))
{
  // Cheapest test first!
  if r1._stride != r2._stride then return false;
  if r1.first != r2.first then return false;
  if r1.last != r2.last then return false;
  return true;
}

// This makes use of the fact that the default values for the
// stride and alignment fields are consistent, whether the two ranges
// being compared are stridable or not, aligned or not.
proc range.equiv(other: range(?))
{
  if this._low != other._low then return false;
  if this._high != other._high then return false;
  if this._stride != other._stride then return false;
  if this._alignment != other._alignment then return false;
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
  if i < 0 then
    return new range(idxType, boundedType, false, false,
                     _low, _low- 1 - i);
  if i > 0 then
    return new range(idxType, boundedType, false, false,
                     _high + 1 - i, _high);
  // if i == 0 then
  return new range(idxType, boundedType, false, false,
                   _low, _high);
}

proc range.interior(i: idxType)
{
  if i < 0 then
    return new range(idxType, boundedType, true, true,
                     _low, _low-1-i, _stride, _alignment);
  if i > 0 then
    return new range(idxType, boundedType, true, true,
                     _high + 1 - i, _high, _stride,
                     chpl__diffMod(_low + _alignment, _high + 1 - i, _stride));
  // if i == 0 then
  return new range(idxType, boundedType, true, true,
                  _low, _high, _stride, _alignment);
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
  if i < 0 then
    return new range(idxType, boundedType, false, false,
                     _low+i, _low-1);
  if i > 0 then
    return new range(idxType, boundedType, false, false,
                     _high+1, _high+i);
  // if i == 0 then
  return new range(idxType, boundedType, false, false,
                   _low, _high);
}

proc range.exterior(i: idxType)
{
  if i < 0 then
    return new range(idxType, boundedType, true, true,
                     _low - i, _low - 1, _stride, _alignment - i);
  if i > 0 then
    return new range(idxType, boundedType, true, true,
                     _high + 1, _high + i, _stride,
                     chpl__diffMod(_low + _alignment, _high + 1, _stride));
  // if i == 0 then
  return new range(idxType, boundedType, true, true,
                   _low, _high, _stride, _alignment);
}

proc range.expand(i: idxType)
  where boundedType != BoundedRangeType.bounded
{
  compilerError("expand() is not supported on unbounded ranges");
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
{
  type resultType = (r._low+i).type;
  type strType = chpl__signedType(resultType);

  return new range(resultType, b, s, a,
                   r._low + i, r._high + i,
		   r._stride : strType, r._alignment : resultType);
}

pragma "inline"
proc +(i:integral, r: range(?e,?b,?s,?a))
  return r + i;

proc -(r: range(?e,?b,?s,?a), i: integral)
{
  type resultType = (r._low+i).type;
  type strType = chpl__signedType(resultType);

  return new range(resultType, b, s, a,
                   r._low - i, r._high - i,
		   r._stride : strType, r._alignment : resultType);
}

// This is the syntax processing routine for the "by" keyword.
// The by operator always clobbers an existing alignment.
proc by(r : range(?), str)
{
  proc byHelp(r : range(?e, ?b, ?s, ?a), str : chpl__signedType(e))
  {
    var al : e;
    if (r.hasLowBound()) then al = chpl__diffMod(r.alignedLow, r._low, r._stride);
    else if (b == BoundedRangeType.boundedHigh) then al = chpl__diffMod(r.alignedHigh, r._high, r._stride);
    else al = 0;

    return new range(e, b, true, true,
                     r._low, r._high, r._stride*str, al);
  }
  
  if str == 0 then
    halt("range cannot be strided by zero");

  type idxType = r.idxType;
  type strType = chpl__signedType(idxType);
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
    if this.hasLowBound() then
      this._alignment = algn - _low;
    else if this.boundedType == BoundedRangeType.boundedHigh then
      this._alignment = algn - _high;
    else
      this._alignment = algn;
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
                         lcm:chpl__signedType(idxType));

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
  { // non-empty intersection

    // x and/or the diff may negative, even with a uint source range.
    var offset = (al2 - al1) * x;
    // offset is in the range [-(lcm-1), lcm-1]
    if offset < 0 then offset += lcm;

    // Now offset can be safely cast to idxType.
    var al = al1:idxType + offset:idxType * st1:idxType / g:idxType;


    if result.hasLowBound()
    {
      result._alignment = chpl__diffMod(al, result._low, lcm);
    }
    else if result.boundedType == BoundedRangeType.boundedHigh
    {
      result._alignment = chpl__diffMod(al, result._high, lcm);
    }
    else
      result._alignment = al;

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

proc #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedNone
{
  compilerError("count operator is not defined for unbounded ranges");
}

proc #(r:range(?), i:integral)
{
  type resultType = (r._low+i).type;
  type strType = chpl__signedType(resultType);

  if (i == 0) then
    // Return a degenerate range.
    return new range(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     aligned = r.stridable,
                     _low = 1,
                     _high = 0,
                     _stride = r._stride,
                     _alignment = 0);

  if i > 0 && !r.hasFirst() then
    halt("With a positive count, the range must have a first index.");
  if i < 0 && !r.hasLast() then
    halt("With a negative count, the range must have a last index.");
  if r.boundedType == BoundedRangeType.bounded && i > r.length then
    halt("bounded range is too small to access ", i, " elements");

  if debugChapelRange == ChapelRangeMethod.count then
    writeln((typeToString(resultType), typeToString(strType)));

  // The distance between the first and last indices.
  var s = r._stride : strType;
  var diff = i : strType * s;

  if debugChapelRange == ChapelRangeMethod.count then
    writeln("r = ", r, " # = ", i);

  var lo : resultType =
    if diff > 0 then r.alignedLow
    else r.alignedHigh : resultType - (abs(diff) - abs(s)) : resultType;
  var hi : resultType =
    if diff < 0 then r.alignedHigh
    else r.alignedLow : resultType + (diff - abs(s)) : resultType;
  var al : resultType =
    if diff > 0 then r.alignedLow else r.alignedHigh;

  if debugChapelRange == ChapelRangeMethod.count then
    writeln("lo = ", lo, " hi = ", hi, " al = ", al);

  if r.hasLowBound() && lo < r._low ||
     r.hasHighBound() && hi > r._high then
    halt("Range is too small to count off ", i, " elements.");

  if debugChapelRange == ChapelRangeMethod.count then
    writeln((typeToString(lo.type), typeToString(strType)));

  return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   aligned = r.stridable,
                   _low = lo,
                   _high = hi,
                   _stride = r._stride : strType,
                   _alignment = chpl__diffMod(al, lo, s): resultType);
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

  if debugChapelRange == ChapelRangeMethod.these then
    writeln("*** In range leader:"); // ", this);

  var v = this.length;
  var numChunks = _computeNumChunks(v);

  if debugChapelRange == ChapelRangeMethod.these
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
        if debugChapelRange == ChapelRangeMethod.these then
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

  if debugChapelRange == ChapelRangeMethod.these then
    writeln("In range follower code: Following ", follower);

  var followThis = follower(1);

  if debugChapelRange == ChapelRangeMethod.these then
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

    if debugChapelRange == ChapelRangeMethod.these then
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

    if debugChapelRange == ChapelRangeMethod.these then
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

