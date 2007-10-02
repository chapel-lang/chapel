//
// range type
//
//   parameterized by an integral element type, by whether low and/or
//   high bounds exist, and by whether the stride is one or not
//
enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

record range {
  type eltType = int;                            // element type
  param boundedType: BoundedRangeType = bounded; // bounded or not
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
  return range(int, bounded, false, low, high);
def _build_range(low: uint, high: uint)
  return range(uint, bounded, false, low, high);
def _build_range(low: int(64), high: int(64))
  return range(int(64), bounded, false, low, high);
def _build_range(low: uint(64), high: uint(64))
  return range(uint(64), bounded, false, low, high);


//
// syntax function for unbounded ranges
//
def _build_range(param bt: BoundedRangeType, bound: int)
  return range(int, bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType, bound: uint)
  return range(uint, bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType, bound: int(64))
  return range(int(64), bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType, bound: uint(64))
  return range(uint(64), bt, false, bound, bound);
def _build_range(param bt: BoundedRangeType)
  return range(int, bt, false);


//
// syntax function for [range)
//
def _build_open_interval_upper(r: range)
  return range(r.eltType, r.boundedType, r.stridable, r.low, r.high-1);


//
// syntax function for by-expressions
//
def by(r : range, i : int) {
  if i == 0 then
    halt("range cannot be strided by zero");
  if r.boundedType == boundedNone then
    halt("unbounded range cannot be strided");
  var result = range(r.eltType, r.boundedType, true, r.low, r.high, r.stride*i);
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
  return boundedType == bounded || boundedType == boundedLow;


//
// return true if this range has a high bound
//
def range._hasHigh() param
  return boundedType == bounded || boundedType == boundedHigh;


//
// align low bound of this range to an alignment; snap up
//
def range._alignLow(alignment: eltType) {
  var s = abs(stride):eltType, d = abs(low-alignment) % s;
  if d != 0 {
    if low - alignment < 0 then
      _low += d;
    else
      _low += s - d;
  }
}


//
// align high bound of this range to an alignment; snap down
//
def range._alignHigh(alignment: eltType) {
  var s = abs(stride):eltType, d = abs(high-alignment) % s;
  if d != 0 {
    if high - alignment > 0 then
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
      return bounded;
    else if low then
      return boundedLow;
    else if high then
      return boundedHigh;
    else
      return boundedNone;
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

  var result = range(eltType,
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
  if boundedType != bounded {
    if stridable {
      if boundedType == boundedLow then
        if stride < 0 then
          halt("iteration over range with negative stride but no high bound");
      if boundedType == boundedHigh then
        if stride > 0 then
          halt("iteration over range with positive stride but no low bound");
      var i = if stride > 0 then low else high;
      while true {
        yield i;
        i = i + stride:eltType;
      }
    } else {
      if boundedType == boundedHigh then
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
      var i: eltType;
      for __primitive("c for loop", i, low, high, 1) {
        yield i;
      }
    }
  }
}


//
// returns the number of elements in this range
//
def range.length {
  if boundedType != bounded then
    compilerError("unbounded range has infinite length");
  if stride > 0 then
    return (high - low) / stride:eltType + 1;
  else
    return (low - high) / stride:eltType + 1;
}


//
// returns true if i is in this range
//
def range.member(i: eltType) {
  if stridable {
    if boundedType == bounded {
      return i >= low && i <= high && (i - low) % abs(stride):eltType == 0;
    } else if boundedType == boundedLow {
      return i >= low && (i - low) % abs(stride):eltType == 0;
    } else if boundedType == boundedHigh {
      return i <= high && (high - i) % abs(stride):eltType == 0;
    } else {
      return true;
    }
  } else {
    if boundedType == bounded {
      return i >= low && i <= high;
    } else if boundedType == boundedLow {
      return i >= low;
    } else if boundedType == boundedHigh {
      return i <= high;
    } else {
      return true;
    }
  }
}


//
// returns true if every i in other is in this range
//
def range.member(other: range(?e,?b,?s))
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
  if boundedType != bounded then
    compilerError("interior is not supported on unbounded ranges");
  if i == 0 then
    return this;
  else if i < 0 then
    return range(eltType, boundedType, stridable, low, low-1-i, stride);
  else
    return range(eltType, boundedType, stridable, high+1-i, high, stride);
}


//
// return an exterior portion of this range
//
def range.exterior(i: eltType) {
  if boundedType != bounded then
    compilerError("exterior is not supported on unbounded ranges");
  if i == 0 then
    return this;
  else if i < 0 then
    return range(eltType, boundedType, stridable, low+i, low-1, stride);
  else
    return range(eltType, boundedType, stridable, high+1, high+i, stride);
}


//
// returns an expanded range, or a contracted range if i < 0
//
def range.expand(i: eltType)
  return range(eltType, boundedType, stridable, low-i, high+i, stride);


//
// range op scalar and scalar op range arithmetic
//
def +(r: range(?e,?b,?s), i: integral)
  return range((r.low+i).type, b, s, r.low+i, r.high+i, r.stride);

def -(r: range(?e,?b,?s), i: integral)
  return range((r.low-i).type, b, s, r.low-i, r.high-i, r.stride);

def *(r: range(?e,?b,?s), i: integral) {
  if i == 0 then
    halt("multiplication of range by zero");
  return range((r.low*i).type, b, true, r.low*i, r.high*i, r.stride*i);
}

def /(r: range(?e,?b,?s), i: integral)
  return range((r.low/i).type, b, true, r.low/i, r.high/i, r.stride/i);

def +(i:integral, r: range(?e,?b,?s))
  return range((i+r.low).type, b, s, i+r.low, i+r.high, r.stride);

def -(i:integral, r: range(?e,?b,?s))
  return range((i-r.low).type, b, s, i-r.low, i-r.high, r.stride);

def *(i:integral, r: range(?e,?b,?s)) {
  if i == 0 then
    halt("multiplication of range by zero");
  return range((i*r.low).type, b, true, i*r.low, i*r.high, i*r.stride);
}

def /(i:integral, r: range(?e,?b,?s))
  return range((i/r.low).type, b, true, i/r.low, i/r.high, i/r.stride);


//
// return a substring of a string with a range of indices
//
pragma "inline" def string.substring(s: range) {
  if s.boundedType != bounded then
    compilerError("substring indexing undefined on unbounded ranges");
  if s.stride != 1 then
    return __primitive("string_strided_select", this, s.low, s.high, s.stride);
  else
    return __primitive("string_select", this, s.low, s.high);
}
