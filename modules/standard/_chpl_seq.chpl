class _chplSeqNode {
  type _chplElementType;

  var _chplElement : _chplElementType;
  var _chplNext : _chplSeqNode;
}

class seq2 {
  type _chplElementType;

  var length : integer;
  var _chplFirst : _chplSeqNode;
  var _chplLast : _chplSeqNode;

  function append(e : _chplElementType) : seq2 {
    var new : _chplSeqNode = _chplSeqNode(_chplElementType);
    new._chplElement = e;
    if length > 0 {
      _chplLast._chplNext = new;
      _chplLast = new;
    } else {
      _chplFirst = new;
      _chplLast = new;
    }
    length += 1;
    return this;
  }

  function prepend(e : _chplElementType) : seq2 {
    var new : _chplSeqNode = _chplSeqNode(_chplElementType);
    new._chplElement = e;
    if length > 0 {
      new._chplNext = _chplFirst;
      _chplFirst = new;
    } else {
      _chplFirst = new;
      _chplLast = new;
    }
    length += 1;
    return this;
  }

  function concat(s : seq2) : seq2 {
    _chplLast._chplNext = s._chplFirst;
    _chplLast = s._chplLast;
    length += s.length;
    return this;
  }

  function copy() : seq2 {
    var new : seq2 = seq2(_chplElementType);
    var tmp : _chplSeqNode = _chplFirst;
    while tmp != nil {
      new = new.append(tmp._chplElement);
      tmp = tmp.next;
    }
    return new;
  }
}

function #(s : seq2, e : seq2._chplElementType) {
  return s.append(e);
}

function #(e : seq2._chplElementType, s : seq2) {
  return s.prepend(e);
}

function #(s1 : seq2, s2 : seq2) {
  return s1.concat(s2);
}

function write(s : seq2) {
  var tmp = s._chplFirst;
  write("(/");
  while tmp != nil {
    write(tmp._chplElement);
    tmp = tmp._chplNext;
    if (tmp != nil) {
      write(", ");
    }
  }
  write("/)");
}
