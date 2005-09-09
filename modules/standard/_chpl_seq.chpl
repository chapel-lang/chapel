class _seqNode {
  type elt_type;

  var _element : elt_type;
  var _next : _seqNode(elt_type);
}

class seq : value {
  type elt_type;

  var _length : integer;
  var _first : _seqNode of elt_type;
  var _last : _seqNode of elt_type;

  function this(i : integer) var {
    var start_index : integer = 1;
    var end_index : integer = _length;
    if (i >= start_index and i <= end_index) {
      var cntr : integer = i - start_index;
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

  function length : integer
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

  iterator _for() : elt_type {
    var tmp = _first;
    while tmp != nil {
      yield tmp._element;
      tmp = tmp._next;
    }
  }

  iterator _forall() : elt_type {
    var tmp = _first;
    while tmp != nil {
      yield tmp._element;
      tmp = tmp._next;
    }
  }
}

function #(s1 : seq, s2 : seq) {
  return s1._concat(s2);
}

function reverse(s : seq, dim : integer = 1) {
  if (dim != 1) {
    halt("reverse(:seq, dim=1) only implemented for dim 1");
  }
  return s._reverse();
}

function write(s : seq) {
  write("(/");
  var tmp = s._first;
  while tmp != nil {
    write(tmp._element);
    tmp = tmp._next;
    if tmp != nil {
      write(", ");
    }
  }
  write("/)");
}

-- Arithmetic sequence
-- Should this eventually be a subclass of seq(integer)?

record _aseq {
  var _low : integer;
  var _high : integer;
  var _stride : integer;

  iterator _for() : integer {
    var i = _low;
    while i <= _high {
      yield i;
      i = i + _stride;
    }
  }

  iterator _forall() : integer {
    var i = _low;
    while i <= _high {
      yield i;
      i = i + _stride;
    }
  }

  function length : integer
    return
      (if _stride > 0
        then (_high - _low + _stride) / _stride
        else (_low - _high + _stride) / _stride);
}

function by(s : _aseq, i : integer) {
  var tmp = s;
  tmp._stride = tmp._stride * i;
  return tmp;
}

function write(s : _aseq) {
  write(s._low);
  write("..");
  write(s._high);
  if (s._stride > 1) {
    write(" by ");
    write(s._stride);
  }
}

function string.this(s : _aseq) : string
  return _chpl_string_strided_select(this, s._low, s._high, s._stride);
