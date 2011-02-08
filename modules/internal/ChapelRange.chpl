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

  pragma "inline" proc low return _low;       // public getter for low bound
  pragma "inline" proc high return _high;     // public getter for high bound
  pragma "inline" proc stride return _stride; // public getter for stride

  // Returns the high index, properly aligned.
  proc alignedHigh {
    if stridable {
      // (For stride == 1, high is already aligned.)
      if stride <= 1 || low > high then return high;
      else return high - (high - low) % abs(stride):idxType;
    } else return high; }

  // Returns the low index, properly aligned.
  proc alignedLow {
    if stridable {
      // If the stride is positive then low is already aligned.
      // Otherwise, we align it with the high bound.
      if stride > 0 || low > high then return low;
      return low + (high - low) % abs(stride):idxType;
    } else return low; }

  // Returns the starting index (with minimal checks).
  pragma "inline" proc first {
    if stridable { return if stride > 0 then low else high; }
    else return low; }
  // Returns the ending index (with minimal checks).
  pragma "inline" proc last {
    if (stridable) { return if stride > 0 then alignedHigh else alignedLow; }
    else return high; }
}


////////////////////////////////////////////////////////////////////////////////
// SYNTAX FUNCTIONS

//
// syntax function for bounded ranges
//
// Albert hack to make this inline
pragma "inline"
proc _build_range(low: int, high: int)
  return new range(int, BoundedRangeType.bounded, false, low, high);

pragma "inline"
proc _build_range(low: uint, high: uint)
  return new range(uint, BoundedRangeType.bounded, false, low, high);

pragma "inline"
proc _build_range(low: int(64), high: int(64))
  return new range(int(64), BoundedRangeType.bounded, false, low, high);

pragma "inline"
proc _build_range(low: uint(64), high: uint(64))
  return new range(uint(64), BoundedRangeType.bounded, false, low, high);

//
// syntax function for unbounded ranges
//
proc _build_range(param bt: BoundedRangeType, bound: int)
  return new range(int, bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: uint)
  return new range(uint, bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: int(64))
  return new range(int(64), bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType, bound: uint(64))
  return new range(uint(64), bt, false, bound, bound);
proc _build_range(param bt: BoundedRangeType)
  return new range(int, bt, false);


//
// syntax function for by-expressions
//
proc chpl__byHelp(r : range(?et, ?bt, ?sbl), str : chpl__idxTypeToStrType(et)) {
  if str == 0 then
    halt("range cannot be strided by zero");
  if r.boundedType == BoundedRangeType.boundedNone then
    halt("unbounded range cannot be strided");

  // We want the strided range to be a subset of the original
  // So when the stride multiplier is negative, we have to adjust the end bound.
  // Both of these ifs are optional!
  if str < 0 {
    if r.stride < 0 then r._low = r.alignedLow;
    else r._high = r.alignedHigh;
  }
  return new range(r.idxType, r.boundedType, true, r.low, r.high, r.stride*str);
}

proc chpl__legalIntCoerce(type t1, type t2) param {
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

proc by(r : range(?), str) {
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
proc #(r:range(?), i:integral)
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

proc #(r:range(?), i:integral)
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

proc #(r:range(?), i:integral)
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

proc #(r:range(?), i:integral)
  where r.boundedType == BoundedRangeType.boundedNone {
  compilerError("count operator is not defined for unbounded ranges");
}



////////////////////////////////////////////////////////////////////////////////
// INTERNAL FUNCTIONS

//
// return true if this range has a low bound
//
proc range._hasLow() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedLow;


//
// return true if this range has a high bound
//
proc range._hasHigh() param
  return boundedType == BoundedRangeType.bounded || boundedType == BoundedRangeType.boundedHigh;


// 
// Return the number in the range 0 <= result < b that is congruent to a (mod b)
//
// Albert hack to make this inline
pragma "inline"
proc mod(a:integral, b:integral) {
  if _isSignedType(b.type) {
    if (b <= 0) then
      halt("modulus divisor must be positive");
  }

  // b is positive, so this cast is OK unless b is very large and a is signed.
  var modulus = b:a.type;
  if _isSignedType(a.type) {
    if (modulus < 0) then
      halt("Unsigned modulus too large for signed result.");
  }

  var tmp = a % modulus;

  if _isSignedType(a.type) {
    if tmp < 0 then tmp += modulus;
  }

  return tmp;
}


//
// align low bound of this range to an alignment; snap up
//
// Albert hack to make this inline
pragma "inline"
proc range._alignLow(alignment: idxType) {
  var s = abs(stride):idxType;
  // The following is equivalent to _low += abs(alignment - low) % s;
  // except that it avoids problems with an overflow in the subtraction.
  // It uses the fact that if a1 == b1 mod n and a2 == b2 mod n then
  // (a1 - a2) mod n == (b1 - b2) mod n

  // The RHS is how far we are from the mark in modulo space;
  // it is always in the range [0, s-1].
  _low += mod((alignment % s):int - (low % s):int, s):idxType;
}


//
// align high bound of this range to an alignment; snap down
//
// Albert hack to make this inline
pragma "inline"
proc range._alignHigh(alignment: idxType) {
  var s = abs(stride):idxType;
  // We take the absolute value of s, so it can be cast to idxType even if idxType is unsigned.

  // Each of the modulo expressions can be safely cast to an int.
  // The mod function is applied to bring the result back into the nonnegative.
  // Then it can be cast back into the index type.
  _high -= mod((high % s):int - (alignment % s):int, s):idxType;
}


//
// range assignment
//
proc =(r1: range(stridable=?s1), r2: range(stridable=?s2)) {
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
proc range.this(other: range(?idxType2, ?boundedType, ?stridable)) {
  //
  // determine boundedType of result
  //
  proc computeBoundedType(r1, r2) param {
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
  proc extendedEuclidHelp(u, v) {
    var zero: u.type = 0;
    var one: u.type = 1;
    var U = (one, zero, u);
    var V = (zero, one, v);
    while V(3) != 0 {
      (U, V) = let q = U(3)/V(3) in (V, U - V * (q, q, q));
    }
    return (U(3), U(1));
  }
    
  proc extendedEuclid(u:int, v:int) {
    return extendedEuclidHelp(u,v);
  }

  proc extendedEuclid(u:int(64), v:int(64)) {
    return extendedEuclidHelp(u,v);
  }

  // If this range us unbounded below, we use low from the other range,
  // so that max(lo1, lo2) == lo2.  etc.
  var lo1 = if _hasLow() then this.low else other.low;
  // If the result type is unsigned, don't let the low bound go negative.
  // This is a kludge.  We should really obey type coercion rules. (hilde)
  if (_isUnsignedType(idxType)) {
    if (lo1 < 0) then lo1 = 0;
  }
  var hi1 = if _hasHigh() then this.high else other.high;
  var st1 = abs(this.stride);

  var lo2 = if other._hasLow() then other.low else this.low;
  var hi2 = if other._hasHigh() then other.high else this.high;
  var st2 = abs(other.stride);

  var (g, x) = extendedEuclid(st1, st2);
  var lcm = st1 / g * st2;	// The LCM of the two strides.
  // The division must be done first to prevent overflow.

  var result = new range(idxType,
                         computeBoundedType(this, other),
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

  if (al2 - al1) % g != 0 {
    // empty intersection, return degenerate result
    (result._low, result._high) = (1:idxType, 0:idxType);
  } else {
    // non-empty intersection
	// x and/or the diff may negative, even with a uint source range.
	var offset = (al2 - al1) * x;
	// offset is in the range [-(lcm-1), lcm-1]
	if offset < 0 then offset += lcm;
	// Now offset can be safely cast to idxType.
    var al = al1:idxType + offset:idxType * st1:idxType / g:idxType;

    // We inherit the sign of the stride from the operand.
	// hilde: Why?
    if other.stride < 0 then 
      result._stride = -result._stride;

    if result.stride > 0 then
      result._alignLow(al);
    else
      result._alignHigh(al);
  }

  return result;
}


//
// default iterator optimized for unit stride
//
iter range.these() {
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
      var i = first;
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
    // a bounded range ...
    if high < low then return;	// The degenerate case.

    // If this range is not degenerate, then we know we will yield at least one index. 
    var i = first;
    var end = last;

    if stridable {
      while true {
        yield i;
		if i == end then break;
        i = i + stride:idxType;
      }
    } else {
      while true {
        yield i;
		if i == end then break;
        i = i + 1;
      }
    }
  }
}

iter range.these(param tag: iterator) where tag == iterator.leader {
  // want "yield 0..length-1;"
  // but compilerError in length causes a problem because leaders are
  // resolved wherever an iterator is.
  if boundedType == BoundedRangeType.boundedNone then
    halt("iteration over a range with no bounds");
  if debugChapelRange then
    writeln("*** In range leader:"); // ", this);

  var v = this.length;

  var numChunks = _computeNumChunks(v);
  if debugChapelRange then
    writeln("*** RI: length=", v, " numChunks=", numChunks);

  if debugChapelRange then
    writeln("*** RI: Using ", numChunks, " chunk(s)");
  if (CHPL_TARGET_PLATFORM != "xmt") {
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
  } else {

    var per_stream_i: uint(64) = 0;
    var total_streams_n: uint(64) = 0;

    __primitive_loop("xmt pragma forall i in n", per_stream_i,
                     total_streams_n) {
      const (lo,hi) = _computeBlock(v, total_streams_n, per_stream_i, v-1);
      yield tuple(lo..hi);
    }
  }
}

iter range.these(param tag: iterator, follower) where tag == iterator.follower {
  if boundedType == BoundedRangeType.boundedNone then
    halt("iteration over a range with no bounds");
  if follower.size != 1 then
    halt("iteration over a range with multi-dimensional iterator");
  if debugChapelRange then
    writeln("In range follower code: Following ", follower);
  var followThis = follower(1);
  if stridable {
    // r is a range which contains the next chunk of values controlled by followThis.
    // The range in followThis usually has a stride of 1 (optimization opportunity?).
    var r = if stride > 0 then
        low+followThis.low*stride:idxType..low+followThis.high*stride:idxType by stride by followThis.stride
      else
        high+followThis.high*stride:idxType..high+followThis.low*stride:idxType by stride by followThis.stride;
    if debugChapelRange then
      writeln("r = ",r);
    for i in r {
      __primitive("noalias pragma");
      yield i;
    }
  } else {
    var r = low+followThis;
    for i in r {
      __primitive("noalias pragma");
      yield i;
    }
  }
}


//
// returns the number of elements in this range
//
proc range.length {
  if boundedType != BoundedRangeType.bounded then
    compilerError("unbounded range has infinite length");
  if low > high then
    return 0:idxType;

  // This works because either high or low is aligned.
  const retVal = if stride > 0
               then (high - low) / stride:idxType + 1
               else (low - high) / stride:idxType + 1;
  return if (retVal < 0) then 0:idxType else retVal;
}


//
// returns true if i is in this range
//
proc range.member(i: idxType) {
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

proc range.indexOrder(i: idxType) {
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
proc range.boundsCheck(other: range(?e,?b,?s)) where b == BoundedRangeType.boundedNone {
  return true;
}


proc range.boundsCheck(other: range(?e,?b,?s)) {
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

proc range.boundsCheck(other: idxType)
  return member(other);


//
// returns true if every i in other is in this range
//
proc range.member(other: range(?))
  return this(other) == other;


//
// write implementation for ranges
//
proc range.writeThis(f: Writer) {
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
// REVIEW: hilde
// Should member functions normally return new objects?
proc range.translate(i: idxType) {
  return this + i;
}

//
// intended for internal use only:
//
proc range.chpl__unTranslate(i: idxType) {
  return this - i;
}


//
// return an interior portion of this range
//
proc range.interior(i: idxType) {
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
proc range.exterior(i: idxType) {
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
proc range.expand(i: idxType)
  return new range(idxType, boundedType, stridable, low-i, high+i, stride);


//
// range op scalar and scalar op range arithmetic
//
proc +(r: range(?e,?b,?s), i: integral)
  return new range((r.low+i).type, b, s, r.low+i, r.high+i, r.stride);

proc +(i:integral, r: range(?e,?b,?s))
  return new range((i+r.low).type, b, s, i+r.low, i+r.high, r.stride);

proc -(r: range(?e,?b,?s), i: integral)
  return new range((r.low-i).type, b, s, r.low-i, r.high-i, r.stride);


//
// return a substring of a string with a range of indices
//
pragma "inline" proc string.substring(s: range(?e,?b,?st)) {
  if s.boundedType != BoundedRangeType.bounded then
    compilerError("substring indexing undefined on unbounded ranges");
  if s.stride != 1 then
    return __primitive("string_strided_select", this, s.low, s.high, s.stride);
  else
    return __primitive("string_select", this, s.low, s.high);
}

