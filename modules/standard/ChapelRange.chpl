//
// range type
//
//   parameterized by an integral element type, by whether low and/or
//   high bounds exist, and by whether the stride is one or not
//
enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

pragma "range"
record range {
  type eltType = int;                            // element type
  param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
  param stridable: bool = false;                 // range can be strided
  var _low: eltType = 1;                         // lower bound
  var _high: eltType = 0;                        // upper bound
  var _stride: int = 1;                          // integer stride of range
  var _promotionType : eltType;                  // enables promotion

  pragma "inline" def low return _low;       // public getter for low bound
  pragma "inline" def high return _high;     // public getter for high bound
  pragma "inline" def stride return _stride; // public getter for stride
}


////////////////////////////////////////////////////////////////////////////////
// SYNTAX FUNCTIONS

//
// syntax function for bounded ranges
//
def _build_range(low: int, high: int)
  return new range(int, BoundedRangeType.bounded, false, low, high);
def _build_range(low: uint, high: uint)
  return new range(uint, BoundedRangeType.bounded, false, low, high);
def _build_range(low: int(64), high: int(64))
  return new range(int(64), BoundedRangeType.bounded, false, low, high);
def _build_range(low: uint(64), high: uint(64))
  return new range(uint(64), BoundedRangeType.bounded, false, low, high);

//
// syntax for counted ranges
//
def _build_counted_range(low: int, high: int)
  return new range(int, BoundedRangeType.bounded, false, low, low+high-1);
def _build_counted_range(low: uint, high: uint)
  return new range(uint, BoundedRangeType.bounded, false, low, low+high-1);
def _build_counted_range(low: int(64), high: int(64))
  return new range(int(64), BoundedRangeType.bounded, false, low, low+high-1);
def _build_counted_range(low: uint(64), high: uint(64))
  return new range(uint(64), BoundedRangeType.bounded, false, low, low+high-1);


//
// syntax function for unbounded ranges
//
def _build_range(param bt: BoundedRangeType, bound: int)
  return new range(int, bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType, bound: uint)
  return new range(uint, bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType, bound: int(64))
  return new range(int(64), bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType, bound: uint(64))
  return new range(uint(64), bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType)
  return new range(int, bt, false);


//
// syntax function for by-expressions
//
def by(r : range(?), i : int) {
  if i == 0 then
    halt("range cannot be strided by zero");
  if r.boundedType == BoundedRangeType.boundedNone then
    halt("unbounded range cannot be strided");
  var result = new range(r.eltType, r.boundedType, true, r.low, r.high, r.stride*i);
  if r.low > r.high then
    return result;
  if result.stride < 0 then
    result._alignLow(result.high);
  else
    result._alignHigh(result.low);
  return result;
}


////////////////////////////////////////////////////////////////////////////////
// INTERNAL FUNCTIONS

//
// return true if this range has a low bound
//
def range._hasLow() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedLow;


//
// return true if this range has a high bound
//
def range._hasHigh() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedHigh;


// 
// Return the number in the range 0 <= result < b that is congruent to a (mod b)
//
def mod(a:integral, b:integral) {
  if (b <= 0) then
    halt("modulus divisor must be positive");
  var tmp = a % b:a.type;
  return if tmp < 0 then b:a.type + tmp else tmp;
}


//
// align low bound of this range to an alignment; snap up
//
def range._alignLow(alignment: eltType) {
  var s = abs(stride):eltType;
  // The following is equivalent to var d = abs(alignment - low) % s;
  // except that it avoids problems with an overflow in the subtraction.
  // It uses the fact that if a1 == b1 mod n and a2 == b2 mod n then
  // a1 - a2 == b1 - b2 mod n and the fact that abs(a-b) is a-b if a > b
  // and is b-a when b >= a.
  var d = if(alignment > low) then
    mod(mod(alignment,s):int - mod(low,s):int, s):eltType
  else
    mod(mod(low,s):int - mod(alignment, s):int, s):eltType;
  if d != 0 {
    if low < alignment then
      _low += d;
    else
      _low += s - d;
  }
}


//
// align high bound of this range to an alignment; snap down
//
def range._alignHigh(alignment: eltType) {
  var s = abs(stride):eltType;
  // See the note about this declaration in range._alignLow. It is like
  // var d = abs(alignment - high) % s; but avoids overflow problems.
  var d = if(alignment > high) then
    mod(mod(alignment,s):int - mod(high,s):int, s):eltType
  else
    mod(mod(high,s):int - mod(alignment,s):int,s):eltType;
  if d != 0 {
    if high > alignment then
      _high -= d;
    else
      _high -= s - d;
  }
}


//
// range assignment
//
def =(r1: range(stridable=?s1), r2: range(stridable=?s2)) {
  if !s1 && s2 then
    if r2.stride != 1 then
      halt("non-stridable range assigned non-unit stride");
  r1._low = r2.low;
  r1._high = r2.high;
  r1._stride = r2.stride;
  return r1;
}


////////////////////////////////////////////////////////////////////////////////
// EXTERNAL FUNCTIONS

//
// return the intersection of this and other
//
def range.this(other: range(?eltType, ?boundedType, ?stridable)) {

  //
  // determine boundedType of result
  //
  def computeBoundedType(r1, r2) param {
    param low = r1._hasLow() || r2._hasLow();
    param high = r1._hasHigh() || r2._hasHigh();
    if low && high then
      return BoundedRangeType.bounded;
    else if low then
      return BoundedRangeType.boundedLow;
    else if high then
      return BoundedRangeType.boundedHigh;
    else
      return BoundedRangeType.boundedNone;
  }

  //
  // returns (gcd(u, v), x) where x is set such that u*x + v*y =
  // gcd(u, v) assuming u and v are non-negative
  //
  // source: Knuth Volume 2 --- Section 4.5.2
  //
  def extendedEuclid(u: int, v: int) {
    var U = (1, 0, u);
    var V = (0, 1, v);
    while V(3) != 0 {
      (U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));
    }
    return (U(3), U(1));
  }

  var lo1 = if _hasLow() then this.low:eltType else other.low;
  var hi1 = if _hasHigh() then this.high:eltType else other.high;
  var st1 = abs(this.stride);
  var al1 = if this.stride < 0 then hi1 else lo1;

  var lo2 = if other._hasLow() then other.low else this.low:eltType;
  var hi2 = if other._hasHigh() then other.high else this.high:eltType;
  var st2 = abs(other.stride);
  var al2 = if this.stride < 0 then hi2 else lo2;

  var (g, x) = extendedEuclid(st1, st2);

  var result = new range(eltType,
                         computeBoundedType(this, other),
                         this.stridable | other.stridable,
                         max(lo1, lo2),
                         min(hi1, hi2),
                         st1 * st2 / g);

  if abs(lo1 - lo2) % g:eltType != 0 {
    // empty intersection, return degenerate result
    result._low <=> result._high;
  } else {
    // non-empty intersection

    if other.stride < 0 then
      result._stride = -result.stride;

    var al = al1 + (al2 - al1) * x:eltType * st1:eltType / g:eltType;

    result._alignLow(al);
    result._alignHigh(al);
  }

  return result;
}


//
// default iterator optimized for unit stride
//
def range.these() {
  if boundedType != BoundedRangeType.bounded {
    if stridable {
      if boundedType == BoundedRangeType.boundedLow then
        if stride < 0 then
          halt("iteration over range with negative stride but no high bound");
      if boundedType == BoundedRangeType.boundedHigh then
        if stride > 0 then
          halt("iteration over range with positive stride but no low bound");
      var i = if stride > 0 then low else high;
      while true {
        yield i;
        i = i + stride:eltType;
      }
    } else {
      if boundedType == BoundedRangeType.boundedHigh then
        halt("iteration over range with positive stride but no low bound");
      var i = low;
      while true {
        yield i;
        i = i + 1;
      }
    }
  } else {
    if stridable {
      var i = if stride > 0 then low else high;
      while low <= i && i <= high {
        yield i;
        i = i + stride:eltType;
      }
    } else {
      var i = low;
      while i <= high {
        yield i;
        i = i + 1;
      }
    }
  }
}


//
// returns the number of elements in this range
//
def range.length {
  if boundedType != BoundedRangeType.bounded then
    compilerError("unbounded range has infinite length");
  const retVal = if stride > 0
               then (high - low) / stride:eltType + 1
               else (low - high) / stride:eltType + 1;
  return if (retVal < 0) then 0 else retVal;
}


//
// returns true if i is in this range
//
def range.member(i: eltType) {
  if stridable {
    if boundedType == BoundedRangeType.bounded {
      return i >= low && i <= high && (i - low) % abs(stride):eltType == 0;
    } else if boundedType == BoundedRangeType.boundedLow {
      return i >= low && (i - low) % abs(stride):eltType == 0;
    } else if boundedType == BoundedRangeType.boundedHigh {
      return i <= high && (high - i) % abs(stride):eltType == 0;
    } else {
      return true;
    }
  } else {
    if boundedType == BoundedRangeType.bounded {
      return i >= low && i <= high;
    } else if boundedType == BoundedRangeType.boundedLow {
      return i >= low;
    } else if boundedType == BoundedRangeType.boundedHigh {
      return i <= high;
    } else {
      return true;
    }
  }
}

def range.order(i: eltType) {
  if (!member(i)) then return (-1):eltType;
  return (i-low)/abs(stride);
}


//
// returns true if other is in bounds of this for all specified
// bounds; this function is used to determine if an array slice is
// valid
//
def range.boundsCheck(other: range(?e,?b,?s)) {
  if other.boundedType == BoundedRangeType.boundedNone then
    return true;
  var boundedOther: range(e,BoundedRangeType.bounded,s);
  if other._hasLow() then
    boundedOther._low = other.low;
  else
    boundedOther._low = low;
  if other._hasHigh() then
    boundedOther._high = other.high;
  else
    boundedOther._high = high;
  boundedOther._stride = other.stride;
  return (boundedOther.length == 0) || (this(boundedOther) == boundedOther);
}


//
// returns true if every i in other is in this range
//
def range.member(other: range(?))
  return this(other) == other;


//
// write implementation for ranges
//
def range.writeThis(f: Writer) {
  if _hasLow() then
    f.write(low);
  f.write("..");
  if _hasHigh() then
    f.write(high);
  if stride != 1 then
    f.write(" by ", stride);
}


//
// translate the indices in this range by i
//
def range.translate(i: eltType)
  return this + i;


//
// return an interior portion of this range
//
def range.interior(i: eltType) {
  if boundedType != BoundedRangeType.bounded then
    compilerError("interior is not supported on unbounded ranges");
  if i == 0 then
    return this;
  else if i < 0 then
    return new range(eltType, boundedType, stridable, low, low-1-i, stride);
  else
    return new range(eltType, boundedType, stridable, high+1-i, high, stride);
}


//
// return an exterior portion of this range
//
def range.exterior(i: eltType) {
  if boundedType != BoundedRangeType.bounded then
    compilerError("exterior is not supported on unbounded ranges");
  if i == 0 then
    return this;
  else if i < 0 then
    return new range(eltType, boundedType, stridable, low+i, low-1, stride);
  else
    return new range(eltType, boundedType, stridable, high+1, high+i, stride);
}


//
// returns an expanded range, or a contracted range if i < 0
//
def range.expand(i: eltType)
  return new range(eltType, boundedType, stridable, low-i, high+i, stride);


//
// range op scalar and scalar op range arithmetic
//
def +(r: range(?e,?b,?s), i: integral)
  return new range((r.low+i).type, b, s, r.low+i, r.high+i, r.stride);

def -(r: range(?e,?b,?s), i: integral)
  return new range((r.low-i).type, b, s, r.low-i, r.high-i, r.stride);

def *(r: range(?e,?b,?s), i: integral) {
  if i == 0 then
    halt("multiplication of range by zero");
  return new range((r.low*i).type, b, true, r.low*i, r.high*i, r.stride*i);
}

def /(r: range(?e,?b,?s), i: integral)
  return new range((r.low/i).type, b, true, r.low/i, r.high/i, r.stride/i);

def +(i:integral, r: range(?e,?b,?s))
  return new range((i+r.low).type, b, s, i+r.low, i+r.high, r.stride);

def -(i:integral, r: range(?e,?b,?s))
  return new range((i-r.low).type, b, s, i-r.low, i-r.high, r.stride);

def *(i:integral, r: range(?e,?b,?s)) {
  if i == 0 then
    halt("multiplication of range by zero");
  return new range((i*r.low).type, b, true, i*r.low, i*r.high, i*r.stride);
}

def /(i:integral, r: range(?e,?b,?s))
  return new range((i/r.low).type, b, true, i/r.low, i/r.high, i/r.stride);


//
// return a substring of a string with a range of indices
//
pragma "inline" def string.substring(s: range(?e,?b,?st)) {
  if s.boundedType != BoundedRangeType.bounded then
    compilerError("substring indexing undefined on unbounded ranges");
  if s.stride != 1 then
    return __primitive("string_strided_select", this, s.low, s.high, s.stride);
  else
    return __primitive("string_select", this, s.low, s.high);
}
