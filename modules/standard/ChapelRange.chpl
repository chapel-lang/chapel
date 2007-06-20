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
  def stride: eltType return _stride; // should be :int ??

  def this(subrange: range(?eltType, ?boundedness, ?stridable)) {
    if (stride != 1 || _stride != 1) {
      halt("range slicing not supported for strided ranges yet");
    }
    if (boundedness == bounded) {
      if (subrange.low >= low && subrange.high <= high) {
        return range(eltType, bounded, stridable, subrange.low, subrange.high,
                     subrange.stride);
      } else {
        halt("range slice out of bounds: ", subrange);
      }
    } else if (boundedness == boundedLow) {
      return range(eltType, bounded, stridable, subrange.low, _high,
                   subrange.stride);
    } else if (boundedness == boundedHigh) {
      return range(eltType, bounded, stridable, _low, subrange.high, 
                   subrange.stride);
    } else if (boundedness == boundedNone) {
      return range(eltType, bounded, stridable, _low, _high, subrange.stride);
    } else {
      compilerError("unexpected boundedness case in range.this()");
    }
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


def _intersect(a: range, b: range) {
  if (a.boundedType != bounded) | (b.boundedType != bounded) then
    compilerError("insersection not defined on unbounded ranges");
  var g, x: int;
  (g, x) = _extended_euclid(a._stride, b._stride);
  var gg = g:a.eltType;
  var xx = x:a.eltType;
  var as = (a._stride):a.eltType;
  if abs(a._low - b._low) % gg != 0 then
    return 1..0:a.eltType by 1;
  var low = max(a._low, b._low);
  var high = min(a._high, b._high);
  var stride = a._stride * b._stride / g;
  var alignment = a._low + (b._low - a._low) * xx * as / gg;
  if alignment == 0 then
    alignment = stride:a.eltType;
  low = low + low % alignment;
  return low..high by stride;
}

// Extended-Euclid (Knuth Volume 2 --- Section 4.5.2)
// given two non-negative integers u and v
// returns (gcd(u, v), x) where x is set such that u*x + v*y = gcd(u, v)
def _extended_euclid(u: int, v: int) {
  var u1 = 1;
  var u2 = 0;
  var u3 = u;
  var v1 = 0;
  var v2 = 1;
  var v3 = v;
  while v3 != 0 {
    var q = u3 / v3;
    var t1 = u1 - v1 * q;
    var t2 = u2 - v2 * q;
    var t3 = u3 - v3 * q;
    u1 = v1;
    u2 = v2;
    u3 = v3;
    v1 = t1;
    v2 = t2;
    v3 = t3;
  }
  return (u3, u1);
}


def _build_open_interval_upper(r: range) {
  return range(r.eltType, r.boundedType, r.stridable, r._low, r._high-1);
}

def =(a: _domain, b: range) {
  const bdom = _build_domain(b);
  a = bdom;
  return a;
}


def +(r: range, s:int) {
  return range((r._low+s).type, r.boundedType, r.stridable, r._low+s, r._high+s, r._stride);
}

def -(r: range, s:int) {
  return range((r._low-s).type, r.boundedType, r.stridable, r._low-s, r._high-s, r._stride);
}

def *(r: range, s:int) {
  return range((r._low*s).type, r.boundedType, stridable=true, r._low*s, r._high*s, r._stride*s);
}

def /(r: range, s:int) {
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
