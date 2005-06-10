function #(s1 : seq2, s2 : seq2) {
  return s1._concat(s2);
}

function write(s : seq2) {
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

class seq2 {
  type _elementType;

  var _length : integer;
  var _first : _seqNode;
  var _last : _seqNode;

  function length : integer
    return _length;

  function reverse : seq2 {
    var new_seq : seq2 = seq2(_elementType);
    var tmp = _first;
    while (tmp != nil) {
      new_seq._prepend(tmp._element);
      tmp  = tmp._next;    
    }
    return new_seq;     
  }

  function _append(e : _elementType) : seq2 {
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

  function _prepend(e : _elementType) : seq2 {
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

  function _concat(s : seq2) : seq2 {
    _last._next = s._first;
    _last = s._last;
    _length += s._length;
    return this;
  }

  function _copy() : seq2 {
    var new : seq2 = seq2(_elementType);
    var tmp : _seqNode = _first;
    while tmp != nil {
      new = new._append(tmp._element);
      tmp = tmp._next;
    }
    return new;
  }
}

/*** Need where clauses to get all these working
function #(s : seq2, e : seq2._elementType) {
  return s._append(e);
}

function #(e : seq2._elementType, s : seq2) {
  return s._prepend(e);
}
***/
