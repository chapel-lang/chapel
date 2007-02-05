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

  iterator this() : eltType {
    forall x in this
      yield x; 
  }

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
