function by(s : _aseq, i : integer) {
  var tmp = s;
  tmp._stride *= i;
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

function #(s1 : seq, s2 : seq) {
  return s1._concat(s2);
}

function write(s : seq) {
  write("(/");
  var tmp = s._first;
  while tmp != nil {
    write(tmp._element);
    tmp = tmp._next;
    if (tmp != nil) {
      write(", ");
    }
  }
  write("/)");
}

class _seqNode {
  type _elementType;

  var _element : _elementType;
  var _next : _seqNode;
}

record seq {
  type _elementType;

  var _length : integer;
  var _first : _seqNode;
  var _last : _seqNode;

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

  function reverse {
    var new_seq : seq(_elementType);
    var tmp = _first;
    while (tmp != nil) {
      new_seq._prepend(tmp._element);
      tmp  = tmp._next;    
    }
    return new_seq;     
  }

  function _append(e : _elementType) {
    var new : _seqNode = _seqNode(_elementType);
    new._element = e;
    if _length > 0 {
      _last._next = new;
      _last = new;
    } else {
      _first = new;
      _last = new;
    }
    _length += 1;
    return this;
  }

  function _prepend(e : _elementType) {
    var new : _seqNode = _seqNode(_elementType);
    new._element = e;
    if _length > 0 {
      new._next = _first;
      _first = new;
    } else {
      _first = new;
      _last = new;
    }
    _length += 1;
    return this;
  }

  function _concat(s : seq) {
    _last._next = s._first;
    _last = s._last;
    _length += s._length;
    return this;
  }

  function _copy() {
    var new : seq(_elementType);
    var tmp : _seqNode = _first;
    while tmp != nil {
      new = new._append(tmp._element);
      tmp = tmp._next;
    }
    return new;
  }

  iterator _for() {
    var tmp : _seqNode = _first;
    while tmp != nil {
      yield tmp._element;
      tmp = tmp._next;
    }
  }

  iterator _forall() {
    var tmp : _seqNode = _first;
    while tmp != nil {
      yield tmp._element;
      tmp = tmp._next;
    }
  }
}

record _aseq {
  var _low : integer;
  var _high : integer;
  var _stride : integer;

  iterator _for() {
    var i = _low;
    while i <= _high {
      yield i;
      i += _stride;
    }
  }

  iterator _forall() {
    var i = _low;
    while i <= _high {
      yield i;
      i += _stride;
    }
  }
}

/*** Need where clauses to get all these working
function #(s : seq, e : seq._elementType) {
  return s._append(e);
}

function #(e : seq._elementType, s : seq) {
  return s._prepend(e);
}
***/
