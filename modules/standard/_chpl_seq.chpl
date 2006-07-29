class _seqNode {
  type elt_type;

  var _element : elt_type;
  var _next : _seqNode of elt_type;
}

record seq {
  type elt_type;

  var _length : int;
  var _first : _seqNode of elt_type;
  var _last : _seqNode of elt_type;

  iterator this() : elt_type {
    forall x in this
      yield x; 
  }

  fun this(i : int) var {
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

  fun getHeadCursor()
    return _first;

  fun getNextCursor(c)
    return c._next;

  fun getValue(c)
    return c._element;

  fun isValidCursor?(c)
    return c != nil;

  fun length : int
    return _length;

  fun _append(e : elt_type)
    return this._copy()._append_in_place(e);

  fun _prepend(e : elt_type)
    return this._copy()._prepend_in_place(e);

  fun _concat(s : seq)
    return this._copy()._concat_in_place(s);

  fun _append_in_place(e : elt_type) {
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

  fun _yield(e: elt_type) {
    this._append_in_place(e);
  } 

  fun _prepend_in_place(e : elt_type) {
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

  fun _concat_in_place(s : seq) {
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

  fun _copy() {
    var new : seq of elt_type;
    var tmp = _first;
    while tmp != nil {
      new._append_in_place(tmp._element);
      tmp = tmp._next;
    }
    return new;
  }

  fun _delete() {
    _first = nil;
    _last = nil;
    _length = 0;
  }

  fun _reverse() {
    var new : seq of elt_type;
    var tmp = _first;
    while (tmp != nil) {
      new._prepend_in_place(tmp._element);
      tmp  = tmp._next;    
    }
    return new;     
  }
}

fun =(s1: seq, s2) {
  s1._delete();
  forall e in s2 do
    s1._append_in_place(e);
  return s1;
}

fun #(s1 : seq, s2 : seq) {
  return s1._concat(s2);
}

fun reverse(s : seq, dim : int = 1) {
  if (dim != 1) {
    halt("reverse(:seq, dim=1) only implemented for dim 1");
  }
  return s._reverse();
}

fun fwrite(f : file, s : seq) {
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

-- Arithmetic sequence
-- Should this eventually be a subclass of seq(int)?

record _aseq {
  var _low : int;
  var _high : int;
  var _stride : int;

  iterator this() : int {
    forall x in this
      yield x; 
  }

  fun getHeadCursor()
    if _stride > 0 then
      return _low;
    else
      return _high;

  fun getNextCursor(c)
    return c + _stride;

  fun getValue(c)
    return c;

  fun isValidCursor?(c)
    return _low <= c && c <= _high;

  fun length : int
    return
      (if _stride > 0
        then (_high - _low + _stride) / _stride
        else (_low - _high + _stride) / _stride);
}

fun by(s : _aseq, i : int)
  return _aseq(s._low, s._high, s._stride * i);

fun _in(s : _aseq, i : int)
  return i >= s._low && i <= s._high && (i - s._low) % s._stride == 0;

fun fwrite(f : file, s : _aseq) {
  fwrite(f, s._low, "..", s._high);
  if (s._stride > 1) then
    fwrite(f, " by ", s._stride);
}

pragma "inline" fun string.this(s: _aseq)
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);

class reduction { }

fun _reduce(r, s) { // reduce sequence s by reduction r
  for e in s do
    r.accumulate(e);
  return r.generate();  
}

class sum : reduction {
  type elt_type;
  var value : elt_type;
  var first = false;
  fun accumulate(x) {
    if first then
      value = value + x;
    else {
      first = true;
      value = x;
    }
  }
  fun generate()
    return value;
}

class max : reduction {
  type elt_type;
  var value : elt_type;
  var first = true;
  fun accumulate(x) {
    if first {
      first = false;
      value = x;
    } else {
      if (x > value) {
        value = x;
      }
    }
  }
  fun generate() {
    return value;
  }
}
