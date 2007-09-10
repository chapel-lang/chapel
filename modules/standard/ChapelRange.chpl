//
// syntactic bridge code for bounded ranges
//
def _build_range(param bt: BoundedRangeType, low: int, high: int)
  return range(int, bt, false, low, high);
def _build_range(param bt: BoundedRangeType, low: uint, high: uint)
  return range(uint, bt, false, low, high);
def _build_range(param bt: BoundedRangeType, low: int(64), high: int(64))
  return range(int(64), bt, false, low, high);
def _build_range(param bt: BoundedRangeType, low: uint(64), high: uint(64))
  return range(uint(64), bt, false, low, high);
def _build_range(param bt: BoundedRangeType, low, high) {
  compilerError("range bounds are not integral");
}


//
// syntactic bridge code for unbounded ranges
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
def _build_range(param bt: BoundedRangeType, bound) {
  compilerError("range bound is not integral");
}


//
// range type implementation
//
//   paramterized by how it is bounded and by an integral element type
//
record range {
  type eltType = int;                            // element type
  param boundedType: BoundedRangeType = bounded; // bounded or not
  param stridable: bool = false;                 // range can be strided
  var _low : eltType = 1;                        // lower bound
  var _high : eltType = 0;                       // upper bound
  var _stride : int = 1;                         // integer stride of range
  var _promotionType : eltType;                  // enables promotion

  def low: eltType return _low;
  def high: eltType return _high;
  def stride: int return _stride;

  def alignLow(alignment: eltType) {
    var s = abs(_stride):eltType, d = abs(_low-alignment) % s;
    if d != 0 {
      if _low - alignment < 0 then
        _low += d;
      else
        _low += s - d;
    }
  }

  def alignHigh(alignment: eltType) {
    var s = abs(_stride):eltType, d = abs(_high-alignment) % s;
    if d != 0 {
      if _high - alignment > 0 then
        _high -= d;
      else
        _high -= s - d;
    }
  }

  //
  // return the intersection of this and other
  // the type of the returned range is determined by
  //   taking the eltType of other
  //   setting the boundedType appropriately
  //   setting stridable to true if either this or other is stridable
  //
  def this(other: range(?eltType, ?boundedType, ?stridable)) {

    // return true if r has a low bound
    def hasLow(r) param
      return r.boundedType == bounded || r.boundedType == boundedLow;

    // return true if r has a high bound
    def hasHigh(r) param
      return r.boundedType == bounded || r.boundedType == boundedHigh;

    // determine boundedType of result
    def computeBoundedType(r1, r2) param {
      param low = hasLow(r1) || hasLow(r2);
      param high = hasHigh(r1) || hasHigh(r2);
      if low && high then
        return bounded;
      else if low then
        return boundedLow;
      else if high then
        return boundedHigh;
      else
        return boundedNone;
    }

    // Extended-Euclid (Knuth Volume 2 --- Section 4.5.2)
    // given two non-negative integers u and v
    // returns (gcd(u, v), x) where x is set such that u*x + v*y = gcd(u, v)
    def extendedEuclid(u: int, v: int) {
      var U = (1, 0, u);
      var V = (0, 1, v);
      while V(3) != 0 {
        (U, V) = let q = U(3)/V(3) in (V, U - V * q);
      }
      return (U(3), U(1));
    }

    var lo1 = if hasLow(this) then this._low:eltType else other._low;
    var hi1 = if hasHigh(this) then this._high:eltType else other._high;
    var st1 = abs(this.stride);
    var al1 = if this.stride < 0 then hi1 else lo1;

    var lo2 = if hasLow(other) then other._low else this._low:eltType;
    var hi2 = if hasHigh(other) then other._high else this._high:eltType;
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
        result._stride = -result._stride;

      var al = al1 + (al2 - al1) * x:eltType * st1:eltType / g:eltType;

      result.alignLow(al);
      result.alignHigh(al);
    }

    return result;
  }

  def these() {
    if boundedType != bounded {
      if stridable {
        var i = if _stride > 0 then _low else _high;
        while true {
          yield i;
          i = i + _stride:eltType;
        }
      } else {
        var i = _low;
        while true {
          yield i;
          i = i + 1;
        }
      }
    } else {
      if stridable {
        var i = if _stride > 0 then _low else _high;
        while _low <= i && i <= _high {
          yield i;
          i = i + _stride:eltType;
        }
      } else {
        var i: eltType;
        for __primitive("c for loop", i, _low, _high, 1) {
          yield i;
        }
      }
    }
  }

  def length {
    if boundedType != bounded then
      compilerError("unable to determine length of unbounded range");
    return
      (if _stride > 0
        then (_high - _low + _stride:eltType) / _stride:eltType
        else (_low - _high + _stride:eltType) / _stride:eltType);
  }
}

def =(r1: range(stridable=?stridable), r2) {
  if !stridable then
    if r2.stride != 1 then
      halt("non-stridable range assigned non-unit stride");
  r1._low = r2._low;
  r1._high = r2._high;
  r1._stride = r2._stride;
  return r1;
}

def by(s : range, i : int) {
  if i == 0 then
    halt("illegal stride of 0");
  var as = range(s.eltType, s.boundedType, true, s._low, s._high, s._stride * i);
  if s._low > s._high then return as;

  if as._stride < 0 then
    as._low = as._low + (as._high - as._low) % (-as._stride):as.eltType;
  else
    as._high = as._high - (as._high - as._low) % (as._stride):as.eltType;
  if as.boundedType == boundedLow then
    if as._stride < 0 then
      halt("error: unbounded range has negative stride and lower bound");
  if as.boundedType == boundedHigh then
    if as._stride > 0 then
      halt("error: unbounded range has positive stride and upper bound");
  return as;
}

def _in(s : range, i : s.eltType) {
  if s.boundedType != bounded then
    compilerError("_in undefined on unbounded ranges");
  return i >= s._low && i <= s._high &&
    (i - s._low) % abs(s._stride):s.eltType == 0;
}

// really slow --- REWRITE
def _in(s1: range, s2: range) {
  if (s1.boundedType != bounded) | (s2.boundedType != bounded) then
    compilerError("_in undefined on unbounded ranges");
  for i in s2 do
    if !_in(s1, i) then
      return false;
  return true;
}

def range.writeThis(f: Writer) {
  if (boundedType == bounded) | (boundedType == boundedLow) then
    f.write(_low);
  f.write("..");
  if (boundedType == bounded) | (boundedType == boundedHigh) then
    f.write(_high);
  if _stride != 1 then
    f.write(" by ", _stride);
}

pragma "inline" def string.substring(s: range) {
  if s.boundedType != bounded then
    compilerError("substring indexing undefined on unbounded ranges");
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);
}

def range._translate(i : int) {
  if boundedType != bounded then
    compilerError("translate is not supported on unbounded ranges");
  return _low+i.._high+i by _stride;
}

def range._interior(i : int) {
  if boundedType != bounded then
    compilerError("interior is not supported on unbounded ranges");
  var x = _low.._high by _stride;
  if (i < 0) {
    x = _low.._low-1-i by _stride;
  } else if (i > 0) {
    x = _high+1-i.._high by _stride;
  }
  return x;
}

def range._exterior(i : int) {
  if boundedType != bounded then
    compilerError("exterior is not supported on unbounded ranges");
  var x = _low.._high by _stride;
  if (i < 0) {
    x = _low+i.._low-1 by _stride;
  } else if (i > 0) {
    x = _high+1.._high+i by _stride;
  }
  return x;
}

def range._expand(i : int) {
  if boundedType != bounded then
    compilerError("expand is not supported on unbounded ranges");
  return _low-i.._high+i by _stride;
}


def _build_open_interval_upper(r: range) {
  return range(r.eltType, r.boundedType, r.stridable, r._low, r._high-1);
}

def =(a: _domain, b: range) {
  const bdom = _build_domain(b);
  a = bdom;
  return a;
}


/* Would like to define these as follows:

def +(r: range, s: integral) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

but because we don't distinguish "better" functions between promotion
and generics, it doesn't work -- see the arrayVsPromotion and
rangeVsPromotion futures for examples of this.

Alternatively, we might like to do:

def +(r: range, s: r.eltType) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

but this doesn't allow an int(32) range to be added to an int(64)
integer, which seems useful/important/congruous to integer addition.
So, I'm cloning manually. */


def +(r: range, s: int) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

def -(r: range, s: int) {
  return range((r._low-s).type, r.boundedType, r.stridable, r._low-s, r._high-s, r._stride);
}

def *(r: range, s: int) {
  return range((r._low*s).type, r.boundedType, stridable=true, r._low*s, r._high*s, r._stride*s);
}

def /(r: range, s: int) {
  return range((r._low/s).type, r.boundedType, stridable=true, r._low/s, r._high/s, r._stride/s);
}

def +(s:int, r: range) {
  return range((s+r._low).type, r.boundedType, r.stridable, s+r._low, s+r._high, r._stride);
}

def -(s:int, r: range) {
  return range((s-r._low).type, r.boundedType, r.stridable, s-r._low, s-r._high, r._stride);
}

def *(s:int, r: range) {
  return range((s*r._low).type, r.boundedType, stridable=true, s*r._low, s*r._high, s*r._stride);
}

def /(s:int, r: range) {
  return range((s/r._low).type, r.boundedType, stridable=true, s/r._low, s/r._high, s/r._stride);
}



def +(r: range, s: uint) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

def -(r: range, s: uint) {
  return range((r._low-s).type, r.boundedType, r.stridable, r._low-s, r._high-s, r._stride);
}

def *(r: range, s: uint) {
  return range((r._low*s).type, r.boundedType, stridable=true, r._low*s, r._high*s, r._stride*s);
}

def /(r: range, s: uint) {
  return range((r._low/s).type, r.boundedType, stridable=true, r._low/s, r._high/s, r._stride/s);
}

def +(s:uint, r: range) {
  return range((s+r._low).type, r.boundedType, r.stridable, s+r._low, s+r._high, r._stride);
}

def -(s:uint, r: range) {
  return range((s-r._low).type, r.boundedType, r.stridable, s-r._low, s-r._high, r._stride);
}

def *(s:uint, r: range) {
  return range((s*r._low).type, r.boundedType, stridable=true, s*r._low, s*r._high, s*r._stride);
}

def /(s:uint, r: range) {
  return range((s/r._low).type, r.boundedType, stridable=true, s/r._low, s/r._high, s/r._stride);
}



def +(r: range, s: int(64)) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

def -(r: range, s: int(64)) {
  return range((r._low-s).type, r.boundedType, r.stridable, r._low-s, r._high-s, r._stride);
}

def *(r: range, s: int(64)) {
  return range((r._low*s).type, r.boundedType, stridable=true, r._low*s, r._high*s, r._stride*s);
}

def /(r: range, s: int(64)) {
  return range((r._low/s).type, r.boundedType, stridable=true, r._low/s, r._high/s, r._stride/s);
}

def +(s:int(64), r: range) {
  return range((s+r._low).type, r.boundedType, r.stridable, s+r._low, s+r._high, r._stride);
}

def -(s:int(64), r: range) {
  return range((s-r._low).type, r.boundedType, r.stridable, s-r._low, s-r._high, r._stride);
}

def *(s:int(64), r: range) {
  return range((s*r._low).type, r.boundedType, stridable=true, s*r._low, s*r._high, s*r._stride);
}

def /(s:int(64), r: range) {
  return range((s/r._low).type, r.boundedType, stridable=true, s/r._low, s/r._high, s/r._stride);
}



def +(r: range, s: uint(64)) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

def -(r: range, s: uint(64)) {
  return range((r._low-s).type, r.boundedType, r.stridable, r._low-s, r._high-s, r._stride);
}

def *(r: range, s: uint(64)) {
  return range((r._low*s).type, r.boundedType, stridable=true, r._low*s, r._high*s, r._stride*s);
}

def /(r: range, s: uint(64)) {
  return range((r._low/s).type, r.boundedType, stridable=true, r._low/s, r._high/s, r._stride/s);
}

def +(s:uint(64), r: range) {
  return range((s+r._low).type, r.boundedType, r.stridable, s+r._low, s+r._high, r._stride);
}

def -(s:uint(64), r: range) {
  return range((s-r._low).type, r.boundedType, r.stridable, s-r._low, s-r._high, r._stride);
}

def *(s:uint(64), r: range) {
  return range((s*r._low).type, r.boundedType, stridable=true, s*r._low, s*r._high, s*r._stride);
}

def /(s:uint(64), r: range) {
  return range((s/r._low).type, r.boundedType, stridable=true, s/r._low, s/r._high, s/r._stride);
}
