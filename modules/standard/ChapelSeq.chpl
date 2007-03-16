pragma "seq"
record seq {
  type eltType;
  var _data: _ddata(eltType);
  var _length: int;
  var _capacity: int;
  var _promotionType : eltType;

  def initialize() {
    _capacity = 4;
    _length = 0;
    _data = _ddata(eltType, _capacity);
    _data.init();
  }

  def this(i : int) var {
    if i >= 0 {
      if i < 1 || i > length then
        halt("sequence index out of bounds: ", i);
      return _data(i-1);
    } else {
      if -i > length then
        halt("sequence index out of bounds: ", -i);
      return _data(length+i);
    }
  }

  def this(i: int, is: int...?k) var {
    return this(i)(is);
  }

  def this(is: _tuple) var {
    return this((...is));
  }

  def getHeadCursor()
    return 0;

  def getNextCursor(c)
    return c+1;

  def getValue(c)
    return _data(c);

  def isValidCursor?(c)
    return c < length;

  def length : int
    return _length;

  def =length(x) {
    compilerError("illegal assignment to sequence's length");
  }

  def _append(e : eltType)
    return this._copy()._append_in_place(e);

  def _prepend(e : eltType)
    return this._copy()._prepend_in_place(e);

  def _concat(s : seq)
    return this._copy()._concat_in_place(s);

  def _grow() {
    var d: _ddata(eltType);
    d = _ddata(eltType, _capacity*2);
    d.init();
    for i in 0.._capacity-1 do
      d(i) = _data(i);
    _data = d;
    _capacity = _capacity*2;
  }

  def _append_in_place(e : eltType) {
    if _data == nil then // stopgap for sequence temporaries
      initialize();
    if _length == _capacity then
      _grow();
    _data(_length) = e;
    _length = _length + 1;
    return this;
  }

  def _yield(e: eltType) {
    this._append_in_place(e);
  } 

  def _prepend_in_place(e : eltType) {
    if _length == _capacity then
      _grow();
    for i in 1.._length by -1 do
      _data(i) = _data(i-1);
    _data(0) = e;
    _length = _length + 1;
    return this;
  }

  def _concat_in_place(s : seq) {
    for e in s do
      _append_in_place(e);
    return this;
  }

  def _copy() {
    var new : seq of eltType;
    for e in this do
      new._append_in_place(e);
    return new;
  }

  def _delete() {
    initialize();
  }

  def _reverse() {
    var new: seq of eltType;
    for e in this do
      new._prepend_in_place(e);
    return new;     
  }
}

def _copy(s: seq) {
  var ss: s.type;
  ss = s;
  return ss;
}

pragma "seq pass"
def _pass(s: seq) {
  return _copy(s);
}

def =(s1: seq, s2: seq) {
  s1._delete();
  forall e in s2 do
    s1._append_in_place(e);
  return s1;
}

def #(s1:seq, s2) where s1.type == s2.type {
  return s1._concat(s2);
}

def #(s:seq, e) where e.type:s.eltType {
  return s._append(e);
}

def #(e, s:seq) where e.type:s.eltType {
  return s._prepend(e);
}

def reverse(s : seq, dim : int = 1) {
  if (dim != 1) {
    halt("reverse(:seq, dim=1) only implemented for dim 1");
  }
  return s._reverse();
}

def seq.writeThis(f: Writer) {
  f.write("(/");
  for i in 0..length-1 {
    f.write(_data(i));
    if i != length-1 then
      f.write(", ");
  }
  f.write("/)");
}

def _reduce(r, s) { // reduce sequence s by reduction r
  for e in s do
    r.accumulate(e);
  return r.generate();  
}

def _scan(r, s) {
  var s2: seq(r.eltType);
  for e in s {
    r.accumulate(e);
    s2._append_in_place(r.generate());
  }
  return s2;
}


def _to_seq( i) {
  // var s: seq (i.getValue(i.getNextCursor(i.getHeadCursor()))).type;
  var s: seq (i.getElemType());
  for x in i {
    s #= x;
  }
  return s;
}


def _sum_type(type eltType) {
  var x: eltType;
  return x + x;
}

class _sum {
  type eltType;
  var value : _sum_type(eltType).type;
  def accumulate(x) {
    value = value + x;
  }
  def generate() return value;
}

class _prod {
  type eltType;
  var value : eltType = _prod_id( eltType);

  def accumulate(x) {
    value = value * x;
  }
  def generate() return value;
}

class _max {
  type eltType;
  var value : eltType = min( eltType);

  def accumulate(x) {
    value = max(x, value);
  }
  def generate() return value;
}

class _min {
  type eltType;
  var value : eltType = max( eltType);

  def accumulate(x) {
    value = min(x, value);
  }
  def generate() return value;
}

class maxloc {
  type eltType;
  var value: eltType;
  var uninitialized = true;

  def accumulate(x) {
    if uninitialized || x(1) > value(1) then
      value = x;
    uninitialized = false;
  }
  def generate() return value;
}

class minloc {
  type eltType;
  var value: eltType;
  var uninitialized = true;

  def accumulate(x) {
    if uninitialized || x(1) < value(1) then
      value = x;
    uninitialized = false;
  }
  def generate() return value;
}

class _land {                 // logical and
  type eltType;
  var value : eltType = _land_id( eltType);

  def accumulate(x) {
    value = value && x;
  }
  def generate() return value;
}

class _lor {                 // logical or
  type eltType;
  var value : eltType = _lor_id( eltType);

  def accumulate(x) {
    value = value || x;
  }
  def generate() return value;
}

class _band {                 // bit-wise and
  type eltType;
  var value : eltType = _band_id( eltType);

  def accumulate(x) {
    value = value & x;
  }
  def generate() return value;
}

class _bor {                 // bit-wise or
  type eltType;
  var value : eltType = _bor_id( eltType);

  def accumulate(x) {
    value = value | x;
  }
  def generate() return value;
}

class _bxor {                // bit-wise xor
  type eltType;
  var value : eltType = _bxor_id( eltType);

  def accumulate(x) {
    value = value ^ x;
  }
  def generate() return value;
}


//
// syntactic bridge code for bounded ranges
//
def _build_range(param bt: int, low: int, high: int)
  return range(int, bt, low, high);
def _build_range(param bt: int, low: uint, high: uint)
  return range(uint, bt, low, high);
def _build_range(param bt: int, low: int(64), high: int(64))
  return range(int(64), bt, low, high);
def _build_range(param bt: int, low: uint(64), high: uint(64))
  return range(uint(64), bt, low, high);
def _build_range(param bt: int, low, high) {
  compilerError("range bounds are not integral");
}


//
// syntactic bridge code for unbounded ranges
//
def _build_range(param bt: int, bound: int)
  return range(int, bt, bound, bound);
def _build_range(param bt: int, bound: uint)
  return range(uint, bt, bound, bound);
def _build_range(param bt: int, bound: int(64))
  return range(int(64), bt, bound, bound);
def _build_range(param bt: int, bound: uint(64))
  return range(uint(64), bt, bound, bound);
def _build_range(param bt: int, bound) {
  compilerError("range bound is not integral");
}


//
// range type implementation
//
//   paramterized by how it is bounded and by an integral element type
//
// boundedType: (0 = bounded, 1 or 2 = unbounded)
//   0 = lower and upper bounds
//   1 = lower bound only
//   2 = upper bound only
//
record range {
  type eltType;                 // element type
  param boundedType: int;       // bounded or not
  var _low : eltType = 1;       // lower bound
  var _high : eltType = 0;      // upper bound
  var _stride : int = 1;        // integer stride of range
  var _promotionType : eltType; // enables promotion

  def low: eltType return _low;
  def high: eltType return _high;
  def stride: eltType return _stride; // should be :int ??

  def initialize() {
    if _low > _high {
      _low = 1:eltType;
      _high = 0:eltType;
      _stride = 1;
    }
  }

  def getHeadCursor() {
    if boundedType == 1 then
      if _stride < 0 then
        halt("error: unbounded range has negative stride and lower bound");
    if boundedType == 2 then
      if _stride > 0 then
        halt("error: unbounded range has positive stride and upper bound");
    if _stride > 0 then
      return _low;
    else
      return _high;
  }

  def getNextCursor(c)
    return c + _stride:eltType;

  def getValue(c)
    return c;

  def isValidCursor?(c) {
    if boundedType == 0 then
      return _low <= c && c <= _high;
    else
      return true;
  }

  def length {
    if boundedType != 0 then
      compilerError("unable to determine length of unbounded range");
    return
      (if _stride > 0
        then (_high - _low + _stride:eltType) / _stride:eltType
        else (_low - _high + _stride:eltType) / _stride:eltType);
  }
}

def by(s : range, i : int) {
  if i == 0 then
    halt("illegal stride of 0");
  if s._low == 1 && s._high == 0 then
    return range(s.eltType, s.boundedType, s._low, s._high, s._stride);
  var as = range(s.eltType, s.boundedType, s._low, s._high, s._stride * i);
  if as._stride < 0 then
    as._low = as._low + (as._high - as._low) % (-as._stride):as.eltType;
  else
    as._high = as._high - (as._high - as._low) % (as._stride):as.eltType;
  return as;
}

def _in(s : range, i : s.eltType) {
  if s.boundedType != 0 then
    compilerError("_in undefined on unbounded ranges");
  return i >= s._low && i <= s._high &&
    (i - s._low) % abs(s._stride):s.eltType == 0;
}

// really slow --- REWRITE
def _in(s1: range, s2: range) {
  if (s1.boundedType != 0) | (s2.boundedType != 0) then
    compilerError("_in undefined on unbounded ranges");
  for i in s2 do
    if !_in(s1, i) then
      return false;
  return true;
}

def range.writeThis(f: Writer) {
  if (boundedType == 0) | (boundedType == 1) then
    f.write(_low);
  f.write("..");
  if (boundedType == 0) | (boundedType == 2) then
    f.write(_high);
  if _stride != 1 then
    f.write(" by ", _stride);
}

pragma "inline" def string.substring(s: range) {
  if s.boundedType != 0 then
    compilerError("substring indexing undefined on unbounded ranges");
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);
}

def range._translate(i : int) {
  if boundedType != 0 then
    compilerError("translate is not supported on unbounded ranges");
  return _low+i.._high+i by _stride;
}

def range._interior(i : int) {
  if boundedType != 0 then
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
  if boundedType != 0 then
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
  if boundedType != 0 then
    compilerError("expand is not supported on unbounded ranges");
  return _low-i.._high+i by _stride;
}


def _intersect(a: range, b: range) {
  if (a.boundedType != 0) | (b.boundedType != 0) then
    compilerError("insersection not defined on unbounded ranges");
  var g, x: int;
  (g, x) = _extended_euclid(a._stride, b._stride);
  var gg = g:a.eltType;
  var xx = x:a.eltType;
  var as = (a._stride):a.eltType;
  if abs(a._low - b._low) % gg != 0 then
    return 1..0:a.eltType;
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
