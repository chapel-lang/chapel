pragma "seqNode"
pragma "special free seqNode"
class _seqNode {
  type eltType;

  var _element : eltType;
  var _next : _seqNode of eltType;
}

pragma "seq"
record seq {
  type eltType;

  var _length : int;
  var _first : _seqNode of eltType;
  var _last : _seqNode of eltType;

  var _promotionType : eltType;

  def this(i : int) var {
    if i >= 0 {
      if i < 1 || i > length then
        halt("sequence index out of bounds: ", i);
      var tmp = _first;
      for j in 1..i-1 do
        tmp = tmp._next;
      return tmp._element;
    } else {
      if -i > length then
        halt("sequence index out of bounds: ", -i);
      var tmp = _first;
      for j in 1..length+i do
        tmp = tmp._next;
      return tmp._element;
    }
  }

  def this(i: int, is: int...?k) var {
    return this(i)(is);
  }

  def this(is: _tuple) var {
    return this((...is));
  }

  def getHeadCursor()
    return _first;

  def getNextCursor(c)
    return c._next;

  def getValue(c)
    return c._element;

  def isValidCursor?(c)
    return c != nil;

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

  def _append_in_place(e : eltType) {
    var tmp = e;
    var new = _seqNode(eltType, tmp);
    if _length > 0 {
      _last._next = new;
      _last = new;
    } else {
      _first = new;
      _last = new;
    }
    _length = _length + 1;
    return this;
  }

  def _yield(e: eltType) {
    this._append_in_place(e);
  } 

  def _prepend_in_place(e : eltType) {
    var tmp = e;
    var new = _seqNode(eltType, tmp);
    if _length > 0 {
      new._next = _first;
      _first = new;
    } else {
      _first = new;
      _last = new;
    }
    _length = _length + 1;
    return this;
  }

  def _concat_in_place(s : seq) {
    if _length > 0 {
      _last._next = s._first;
      _last = s._last;
      _length = _length + s._length;
    } else {
      _first = s._first;
      _last = s._last;
      _length = s._length;
    }
    return this;
  }

  def _copy() {
    var new : seq of eltType;
    var tmp = _first;
    while tmp != nil {
      new._append_in_place(tmp._element);
      tmp = tmp._next;
    }
    return new;
  }

  def _delete() {
    _first = nil;
    _last = nil;
    _length = 0;
  }

  def _reverse() {
    var new : seq of eltType;
    var tmp = _first;
    while (tmp != nil) {
      new._prepend_in_place(tmp._element);
      tmp  = tmp._next;    
    }
    return new;     
  }
}

def _copy(s: seq) {
  var ss: s.type;
  ss = s;
  return ss;
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
  var tmp = _first;
  while tmp != nil {
    f.write(tmp._element);
    tmp = tmp._next;
    if tmp != nil {
      f.write(", ");
    }
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


def _aseq._translate(i : int) {
  return _low+i.._high+i by _stride;
}

def _aseq._interior(i : int) {
  var x = _low.._high by _stride;
  if (i < 0) {
    x = _low.._low-1-i by _stride;
  } else if (i > 0) {
    x = _high+1-i.._high by _stride;
  }
  return x;
}

def _aseq._exterior(i : int) {
  var x = _low.._high by _stride;
  if (i < 0) {
    x = _low+i.._low-1 by _stride;
  } else if (i > 0) {
    x = _high+1.._high+i by _stride;
  }
  return x;
}

def _aseq._expand(i : int) {
  return _low-i.._high+i by _stride;
}

// Arithmetic sequence

def _build_aseq(low: int, high: int) return _aseq(int, low, high);
def _build_aseq(low: uint, high: uint) return _aseq(uint, low, high);
def _build_aseq(low: int(64), high: int(64)) return _aseq(int(64), low, high);
def _build_aseq(low: uint(64), high: uint(64)) return _aseq(uint(64), low, high);

def _build_aseq(low, high) {
  compilerError("arithmetic sequence bounds are not of integral type");
}

record _aseq {
  type eltType;
  var _low : eltType;
  var _high : eltType;
  var _stride : int = 1;

  var _promotionType : eltType;

  def low: eltType return _low;
  def high: eltType return _high;
  def stride: eltType return _stride;

  def initialize() {
    if _low > _high {
      _low = 1:eltType;
      _high = 0:eltType;
      _stride = 1;
    }
  }

  def getHeadCursor()
    if _stride > 0 then
      return _low;
    else
      return _high;

  def getNextCursor(c)
    return c + _stride:eltType;

  def getValue(c)
    return c;

  def isValidCursor?(c)
    return _low <= c && c <= _high;

  def length
    return
      (if _stride > 0
        then (_high - _low + _stride:eltType) / _stride:eltType
        else (_low - _high + _stride:eltType) / _stride:eltType);
}

def by(s : _aseq, i : int) {
  if i == 0 then
    halt("illegal stride of 0");
  if s._low == 1 && s._high == 0 then
    return _aseq(s.eltType, s._low, s._high, s._stride);
  var as = _aseq(s.eltType, s._low, s._high, s._stride * i);
  if as._stride < 0 then
    as._low = as._low + (as._high - as._low) % (-as._stride):as.eltType;
  else
    as._high = as._high - (as._high - as._low) % (as._stride):as.eltType;
  return as;
}

def _in(s : _aseq, i : s.eltType)
  return i >= s._low && i <= s._high &&
    (i - s._low) % abs(s._stride):s.eltType == 0;

// really slow --- REWRITE
def _in(s1: _aseq, s2: _aseq) {
  for i in s2 do
    if !_in(s1, i) then
      return false;
  return true;
}

def _aseq.writeThis(f: Writer) {
  f.write(_low, "..", _high);
  if (_stride != 1) then
    f.write(" by ", _stride);
}

pragma "inline" def string.substring(s: _aseq)
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);

// indefinite arithmetic sequence

def _build_iaseq(bound: int, param upper: int)
  return _iaseq(int, upper, bound);
def _build_iaseq(bound: uint, param upper: int)
  return _iaseq(uint, upper, bound);
def _build_iaseq(bound: int(64), param upper: int)
  return _iaseq(int(64), upper, bound);
def _build_iaseq(bound: uint(64), param upper: int)
  return _iaseq(uint(64), upper, bound);

def _build_iaseq(bound, upper) {
  compilerError("arithmetic sequence bound is not of integral type");
}

record _iaseq {
  type eltType;
  param _upper: int; // 0 bound is lower bound, 1 bound is upper bound
  var _bound: eltType;
  var _stride : int = 1;
  var _promotionType : eltType;

  def getHeadCursor() {
    if _upper == 1 && _stride > 0 then
      halt("error: indefinite arithmetic sequence has positive stride and upper bound");
    if _upper == 0 && _stride < 0 then
      halt("error: indefinite arithmetic sequence has negative stride and lower bound");
    return _bound;
  }

  def getNextCursor(c)
    return c + _stride:eltType;

  def getValue(c)
    return c;

  def isValidCursor?(c)
    return true;

  def length {
    halt("error: attempt to determine length of an indefinite arithmetic sequence");
    return 0:eltType;
  }
}

def by(s : _iaseq, i : int) {
  if i == 0 then
    halt("illegal stride of 0");
  return _iaseq(s.eltType, s._upper, s._bound, s._stride * i);
}

def _iaseq.writeThis(f: Writer) {
  if _upper then
    f.write("..", _bound);
  else
    f.write(_bound, "..");
  if (_stride != 1) then
    f.write(" by ", _stride);
}
