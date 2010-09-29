use DSIUtil;
config param debugChapelRange = false;

//
// range type
//
//   parameterized by an integral element type, by whether low and/or
//   high bounds exist, and by whether the stride is one or not
//
enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

pragma "range"
record range {
  type idxType = int;                            // element type
  param boundedType: BoundedRangeType = BoundedRangeType.bounded; // bounded or not
  param stridable: bool = false;                 // range can be strided
  var _low: idxType = 1;                         // lower bound
  var _high: idxType = 0;                        // upper bound
  var _stride: chpl__idxTypeToStrType(idxType) = 1;    // signed stride of range
  var _promotionType : idxType;                  // enables promotion

  pragma "inline" def low return _low;       // public getter for low bound
  pragma "inline" def high return _high;     // public getter for high bound
  pragma "inline" def stride return _stride; // public getter for stride
}


////////////////////////////////////////////////////////////////////////////////
// SYNTAX FUNCTIONS

//
// syntax function for bounded ranges
//
pragma "inline" // SS added pragma inline
def _build_range(low: int, high: int)
  return new range(int, BoundedRangeType.bounded, false, low, high);
def _build_range(low: uint, high: uint)
  return new range(uint, BoundedRangeType.bounded, false, low, high);
def _build_range(low: int(64), high: int(64))
  return new range(int(64), BoundedRangeType.bounded, false, low, high);
def _build_range(low: uint(64), high: uint(64))
  return new range(uint(64), BoundedRangeType.bounded, false, low, high);


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
def chpl__byHelp(r : range(?et, ?bt, ?sbl), str : chpl__idxTypeToStrType(et)) {
  if str == 0 then
    halt("range cannot be strided by zero");
  if r.boundedType == BoundedRangeType.boundedNone then
    halt("unbounded range cannot be strided");
  var result = new range(r.idxType, r.boundedType, true, r.low, r.high, r.stride*str);
  if r.low > r.high then
    return result;
  if result.stride < 0 then
    result._alignLow(result.high);
  else
    result._alignHigh(result.low);
  return result;
}

def chpl__legalIntCoerce(type t1, type t2) param {
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

def by(r : range(?), str) {
  type idxType = r.idxType;
  type strType = chpl__idxTypeToStrType(idxType);
  type argType = str.type;
  if (argType == strType) {
    return chpl__byHelp(r, str);
  } else if (argType == idxType || chpl__legalIntCoerce(argType, strType)) {
    return chpl__byHelp(r, str:strType);
  } else {
    compilerError("type mismatch applying 'by' to range(", 
                  typeToString(idxType), ") with ", typeToString(argType));
  }
}

//
// syntax functions for counted ranges
//
def #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedLow {
  type resultType = (r.low + i).type;
  if i < 0 then
    halt("range cannot have a negative number of elements");
  if i == 0 then
    return new range(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = r.low + 1,
                     _high = r.low,
                     _stride = r.stride);

  return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   _low = r.low,
                   _high = r.low + (i-1)*abs(r.stride):resultType,
                   _stride = r.stride);
}

def #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedHigh {
  type resultType = (r.low + i).type;
  if i < 0 then
    halt("range cannot have a negative number of elements");
  if i == 0 then
    return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   _low = r.high,
                   _high = r.high - 1,
                   _stride = r.stride);

  return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   _low = r.high - (i-1)*abs(r.stride):resultType,
                   _high = r.high,
                   _stride = r.stride);
}

def #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.bounded {
  type resultType = (r.low + i).type;
  if i < 0 then
    halt("range cannot have a negative number of elements");
  if i > r.length then
    halt("bounded range is too small to access ", i, " elements");
  if i == 0 then
    return new range(idxType = resultType,
                     boundedType = BoundedRangeType.bounded,
                     stridable = r.stridable,
                     _low = r.high,
                     _high = r.low,
                     _stride = r.stride);

  return new range(idxType = resultType,
                   boundedType = BoundedRangeType.bounded,
                   stridable = r.stridable,
                   _low = if r.stride < 0 then r.high + (i-1)*r.stride:resultType else r.low,
                   _high = if r.stride < 0 then r.high else r.low + (i-1)*r.stride:resultType,
                   _stride = r.stride);
}

def #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedNone {
  halt("cannot use # operator on an unbounded range");
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
def range._alignLow(alignment: idxType) {
  var s = abs(stride):idxType;
  // The following is equivalent to var d = abs(alignment - low) % s;
  // except that it avoids problems with an overflow in the subtraction.
  // It uses the fact that if a1 == b1 mod n and a2 == b2 mod n then
  // a1 - a2 == b1 - b2 mod n and the fact that abs(a-b) is a-b if a > b
  // and is b-a when b >= a.
  var d = if(alignment > low) then
    mod(mod(alignment,s):int - mod(low,s):int, s):idxType
  else
    mod(mod(low,s):int - mod(alignment, s):int, s):idxType;
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
def range._alignHigh(alignment: idxType) {
  var s = abs(stride):idxType;
  // See the note about this declaration in range._alignLow. It is like
  // var d = abs(alignment - high) % s; but avoids overflow problems.
  var d = if(alignment > high) then
    mod(mod(alignment,s):int - mod(high,s):int, s):idxType
  else
    mod(mod(high,s):int - mod(alignment,s):int,s):idxType;
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
  if r1.boundedType != r2.boundedType then
    compilerError("type mismatch in assignment of ranges with different boundedType parameters");
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
def range.this(other: range(?idxType2, ?boundedType, ?stridable)) {
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
  def extendedEuclidHelp(u, v) {
    var zero: u.type = 0;
    var one: u.type = 1;
    var U = (one, zero, u);
    var V = (zero, one, v);
    while V(3) != 0 {
      (U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));
    }
    return (U(3), U(1));
  }
    
  def extendedEuclid(u:int, v:int) {
    return extendedEuclidHelp(u,v);
  }

  def extendedEuclid(u:int(64), v:int(64)) {
    return extendedEuclidHelp(u,v);
  }

  var lo1 = if _hasLow() then this.low:idxType else other.low;
  var hi1 = if _hasHigh() then this.high:idxType else other.high;
  var st1 = abs(this.stride);
  var al1 = if this.stride < 0 then hi1 else lo1;

  var lo2 = if other._hasLow() then other.low else this.low:idxType;
  var hi2 = if other._hasHigh() then other.high else this.high:idxType;
  var st2 = abs(other.stride);
  var al2 = if this.stride < 0 then hi2 else lo2;

  var (g, x) = extendedEuclid(st1, st2);

  var result = new range(idxType,
                         computeBoundedType(this, other),
                         this.stridable | other.stridable,
                         max(lo1, lo2):idxType,
                         min(hi1, hi2):idxType,
                         (st1 * (st2 / g)):chpl__idxTypeToStrType(idxType));
  if lo1 > hi1 || lo2 > hi2 {
    // empty intersection
    if result.low < result.high then
      result._low <=> result._high;
    else if result.high == result.low then
      (result._low, result._high) = (1:idxType, 0:idxType);
    var al = al1 + (al2:idxType - al1) * x:idxType * st1:idxType / g:idxType;
    result._alignLow(al);
    result._alignHigh(al);
  } else if abs(lo1 - lo2) % g:idxType != 0 {
    // empty intersection, return degenerate result
    result._low <=> result._high;
  } else {
    // non-empty intersection

    if other.stride < 0 then
      result._stride = -result.stride;

    var al = al1 + (al2:idxType - al1) * x:idxType * st1:idxType / g:idxType;

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
    if boundedType == BoundedRangeType.boundedNone then
      halt("iteration over a range with no bounds");
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
        i = i + stride:idxType;
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
    var i, last: idxType;
    if stridable {
      if stride > 0 {
        i = low;
        last = if i > high then low else high + stride:idxType;
      } else {
        i = high;
        last = if i < low then high else low + stride:idxType;
      }
      while i != last {
        yield i;
        i = i + stride:idxType;
      }
    } else {
      i = low;
      last = if i > high then low else high + 1;
      while i != last {
        yield i;
        i = i + 1;
      }
    }
  }
}

def range.these(param tag: iterator) where tag == iterator.leader {
  // want "yield 0..length-1;"
  // but compilerError in length causes a problem because leaders are
  // resolved wherever an iterator is.
  if boundedType == BoundedRangeType.boundedNone then
    halt("iteration over a range with no bounds");
  if debugChapelRange then
    writeln("*** In range leader:"); // ", this);

  var v: idxType;
  if stride > 0 then
    v = (high - low) / stride:idxType + 1;
  else
    v = (low - high) / stride:idxType + 1;
  if v < 0 then
    v = 0;

  const numTasks = if dataParTasksPerLocale==0 then here.numCores
                   else dataParTasksPerLocale;
  const ignoreRunning = dataParIgnoreRunningTasks;
  const minIndicesPerTask = dataParMinGranularity;

  var numChunks = _computeNumChunks(numTasks, ignoreRunning,
                                    minIndicesPerTask, v);
  if debugChapelRange then
    writeln("*** RI: length=", v, " numChunks=", numChunks);

  if debugChapelRange then
    writeln("*** RI: Using ", numChunks, " chunk(s)");
  if numChunks == 1 {
    yield tuple(0..v-1);
  } else {
    coforall chunk in 0..numChunks-1 {
      const (lo,hi) = _computeBlock(v, numChunks, chunk, v-1);
      if debugChapelRange then
        writeln("*** RI: tuple = ", tuple(lo..hi));
      yield tuple(lo..hi);
    }
  }
}

def range.these(param tag: iterator, follower) where tag == iterator.follower {
  if boundedType == BoundedRangeType.boundedNone then
    halt("iteration over a range with no bounds");
  if follower.size != 1 then
    halt("iteration over a range with multi-dimensional iterator");
  if debugChapelRange then
    writeln("In range follower code: Following ", follower);
  var followThis = follower(1);
  if stridable {
    var r = if stride > 0 then
        low+followThis.low*stride:idxType..low+followThis.high*stride:idxType by stride by followThis.stride
      else
        high+followThis.high*stride:idxType..high+followThis.low*stride:idxType by stride by followThis.stride;
    for i in r do
      yield i;
  } else {
    var r = low+followThis;
    for i in r do
      yield i;
  }
}


//
// returns the number of elements in this range
//
pragma "inline" // SS:
def range.length {
  if boundedType != BoundedRangeType.bounded then
    compilerError("unbounded range has infinite length");
  if low > high then
    return 0:idxType;
  const retVal = if stride > 0
               then (high - low) / stride:idxType + 1
               else (low - high) / stride:idxType + 1;
  return if (retVal < 0) then 0 else retVal;
}


//
// returns true if i is in this range
//
pragma "inline" // SS:
def range.member(i: idxType) {
  if stridable {
    if boundedType == BoundedRangeType.bounded {
      return i >= low && i <= high && (i - low) % abs(stride):idxType == 0;
    } else if boundedType == BoundedRangeType.boundedLow {
      return i >= low && (i - low) % abs(stride):idxType == 0;
    } else if boundedType == BoundedRangeType.boundedHigh {
      return i <= high && (high - i) % abs(stride):idxType == 0;
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

def range.indexOrder(i: idxType) {
  if (!member(i)) then return (-1):idxType;
  return (i-low)/abs(stride);
}


//
// returns true if other is in bounds of this for all specified
// bounds; these functions are used to determine if an array slice is
// valid.  We break out the boundedNone case in order to permit
// unbounded ranges to slice ranges of various index types -- otherwise
// we get a compiler error in the boundsCheck function.
//
def range.boundsCheck(other: range(?e,?b,?s)) where b == BoundedRangeType.boundedNone {
  return true;
}


def range.boundsCheck(other: range(?e,?b,?s)) {
  var boundedOther: range(idxType,BoundedRangeType.bounded,s||this.stridable);
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

def range.boundsCheck(other: idxType)
  return member(other);


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
def range.translate(i: idxType)
  return this + i;

//
// intended for internal use only:
//
def range.chpl__unTranslate(i: idxType) {
  return this - i;
}


//
// return an interior portion of this range
//
def range.interior(i: idxType) {
  if boundedType != BoundedRangeType.bounded then
    compilerError("interior is not supported on unbounded ranges");
  if i == 0 then
    return this;
  else if i < 0 then
    return new range(idxType, boundedType, stridable, low, low-1-i, stride);
  else
    return new range(idxType, boundedType, stridable, high+1-i, high, stride);
}


//
// return an exterior portion of this range
//
def range.exterior(i: idxType) {
  if boundedType != BoundedRangeType.bounded then
    compilerError("exterior is not supported on unbounded ranges");
  if i == 0 then
    return this;
  else if i < 0 then
    return new range(idxType, boundedType, stridable, low+i, low-1, stride);
  else
    return new range(idxType, boundedType, stridable, high+1, high+i, stride);
}


//
// returns an expanded range, or a contracted range if i < 0
//
def range.expand(i: idxType)
  return new range(idxType, boundedType, stridable, low-i, high+i, stride);


//
// range op scalar and scalar op range arithmetic
//
def +(r: range(?e,?b,?s), i: integral)
  return new range((r.low+i).type, b, s, r.low+i, r.high+i, r.stride);

def +(i:integral, r: range(?e,?b,?s))
  return new range((i+r.low).type, b, s, i+r.low, i+r.high, r.stride);

def -(r: range(?e,?b,?s), i: integral)
  return new range((r.low-i).type, b, s, r.low-i, r.high-i, r.stride);


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

