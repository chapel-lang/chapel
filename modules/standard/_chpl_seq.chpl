class _seqNode {
  type elt_type;

  var _element : elt_type;
  var _next : _seqNode of elt_type;
}

pragma "seq"
record seq {
  type elt_type;

  var _length : int;
  var _first : _seqNode of elt_type;
  var _last : _seqNode of elt_type;

  iterator this() : elt_type {
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

  def _append(e : elt_type)
    return this._copy()._append_in_place(e);

  def _prepend(e : elt_type)
    return this._copy()._prepend_in_place(e);

  def _concat(s : seq)
    return this._copy()._concat_in_place(s);

  def _append_in_place(e : elt_type) {
    var tmp = e;
    var new = _seqNode(elt_type, tmp);
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

  def _yield(e: elt_type) {
    this._append_in_place(e);
  } 

  def _prepend_in_place(e : elt_type) {
    var tmp = e;
    var new = _seqNode(elt_type, tmp);
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
    var new : seq of elt_type;
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
    var new : seq of elt_type;
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

def #(s:seq, e) where e.type:s.elt_type {
  return s._append(e);
}

def #(e, s:seq) where e.type:s.elt_type {
  return s._prepend(e);
}

def reverse(s : seq, dim : int = 1) {
  if (dim != 1) {
    halt("reverse(:seq, dim=1) only implemented for dim 1");
  }
  return s._reverse();
}

def fwrite(f : file, s : seq) {
  fwrite(f, "(/");
  var tmp = s._first;
  while tmp != nil {
    fwrite(f, tmp._element);
    tmp = tmp._next;
    if tmp != nil {
      fwrite(f, ", ");
    }
  }
  fwrite(f, "/)");
}

def _reduce(r, s) { // reduce sequence s by reduction r
  for e in s do
    r.accumulate(e);
  return r.generate();  
}

def _scan(r, s) {
  var s2: seq(r.elt_type);
  for e in s {
    r.accumulate(e);
    s2._append_in_place(r.generate());
  }
  return s2;
}


def _to_seq( i) {
  var s: seq (i.getValue(i.getNextCursor(i.getHeadCursor()))).type;
  for x in i {
    s #= x;
  }
  return s;
}


class _sum {
  type elt_type;
  var value : elt_type;   // assume default value is sum identity value
  def accumulate(x) {
    value = value + x;
  }
  def generate() return value;
}

class _prod {
  type elt_type;
  var value : elt_type = _prod_id( elt_type);

  def accumulate(x) {
    value = value * x;
  }
  def generate() return value;
}

class _max {
  type elt_type;
  var value : elt_type = min( elt_type);

  def accumulate(x) {
    value = max(x, value);
  }
  def generate() return value;
}

class _min {
  type elt_type;
  var value : elt_type = max( elt_type);

  def accumulate(x) {
    value = min(x, value);
  }
  def generate() return value;
}

class _land {                 // logical and
  type elt_type;
  var value : elt_type = _land_id( elt_type);

  def accumulate(x) {
    value = value && x;
  }
  def generate() return value;
}

class _lor {                 // logical or
  type elt_type;
  var value : elt_type = _lor_id( elt_type);

  def accumulate(x) {
    value = value || x;
  }
  def generate() return value;
}

class _band {                 // bit-wise and
  type elt_type;
  var value : elt_type = _band_id( elt_type);

  def accumulate(x) {
    value = value & x;
  }
  def generate() return value;
}

class _bor {                 // bit-wise or
  type elt_type;
  var value : elt_type = _bor_id( elt_type);

  def accumulate(x) {
    value = value | x;
  }
  def generate() return value;
}

class _bxor {                // bit-wise xor
  type elt_type;
  var value : elt_type = _bxor_id( elt_type);

  def accumulate(x) {
    value = value ^ x;
  }
  def generate() return value;
}
