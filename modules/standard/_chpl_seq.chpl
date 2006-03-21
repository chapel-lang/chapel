class _seqNode {
  type elt_type;

  var _element : elt_type;
  var _next : _seqNode(elt_type);
}

class seq : value {
  type elt_type;

  var _length : int;
  var _first : _seqNode of elt_type;
  var _last : _seqNode of elt_type;

  iterator this() : elt_type {
    forall x in this
      yield x; 
  }

  function this(i : int) var {
    var start_index : int = 1;
    var end_index : int = _length;
    if (i >= start_index and i <= end_index) {
      var cntr : int = i - start_index;
      var tmp = _first;
      while(cntr != 0) {
        tmp = tmp._next;
        cntr-= 1;
      }
      return tmp._element;
    }
    else
      halt("error: sequence index out-of-bounds, index is ", i);
  }

  function getHeadCursor()
    return _first;

  function getNextCursor(c)
    return c._next;

  function getValue(c)
    return c._element;

  function isValidCursor?(c)
    return c != nil;

  function length : int
    return _length;

  function _append(e : elt_type)
    return this._copy()._append_in_place(e);

  function _prepend(e : elt_type)
    return this._copy()._prepend_in_place(e);

  function _concat(s : seq)
    return this._copy()._concat_in_place(s);

  function _append_in_place(e : elt_type) {
    var new = _seqNode(elt_type);
    new._element = e;
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

  function _yield(e: elt_type) {
    this._append_in_place(e);
  } 

  function _prepend_in_place(e : elt_type) {
    var new = _seqNode(elt_type);
    new._element = e;
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

  function _concat_in_place(s : seq) {
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

  function _copy() {
    var new : seq of elt_type;
    var tmp = _first;
    while tmp != nil {
      new = new._append_in_place(tmp._element);
      tmp = tmp._next;
    }
    return new;
  }

  function _reverse() {
    var new : seq of elt_type;
    var tmp = _first;
    while (tmp != nil) {
      new = new._prepend_in_place(tmp._element);
      tmp  = tmp._next;    
    }
    return new;     
  }
}

//function =(s1: seq, s2) {
//  forall e in s2 do
//    s1 = s1._append_in_place(e);
//
//  return s1;
//}

function #(s1 : seq, s2 : seq) {
  return s1._concat(s2);
}

function reverse(s : seq, dim : int = 1) {
  if (dim != 1) {
    halt("reverse(:seq, dim=1) only implemented for dim 1");
  }
  return s._reverse();
}

function fwrite(f : file, s : seq) {
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

  function getHeadCursor()
    if _stride > 0 then
      return _low;
    else
      return _high;

  function getNextCursor(c)
    return c + _stride;

  function getValue(c)
    return c;

  function isValidCursor?(c)
    return _low <= c and c <= _high;

  function length : int
    return
      (if _stride > 0
        then (_high - _low + _stride) / _stride
        else (_low - _high + _stride) / _stride);
}

function by(s : _aseq, i : int)
  return _aseq(s._low, s._high, s._stride * i);

function _in(s : _aseq, i : int)
  return i >= s._low and i <= s._high and (i - s._low) mod s._stride == 0;

function fwrite(f : file, s : _aseq) {
  fwrite(f, s._low, "..", s._high);
  if (s._stride > 1) then
    fwrite(f, " by ", s._stride);
}

pragma "inline" function string.this(s: _aseq)
  if s._stride != 1 then
    return __primitive("string_strided_select", this, s._low, s._high, s._stride);
  else
    return __primitive("string_select", this, s._low, s._high);

iterator _cross(param rank : int, seqs) : (rank*int) {
  if rank < 2 {
    halt("cross requires tuple of rank of 2 or greater");
  } else if (rank == 2) {
    var result : (rank*int);
    for i2 in seqs(2) {
      for i1 in seqs(1) {
        result(1) = i1;
        result(2) = i2;
        yield result;
      }
    }
  } else {
    var result : (rank*int);
    for i in seqs(rank) {
      for is in _cross(rank-1, seqs) {
        for j in 1..rank-1 do
          result(j) = is(j);
        result(rank) = i;
        yield result;
      }
    }
  }
}
