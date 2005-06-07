class _chplSeqNode {
  type _chplSeqElementType;

  var _chplSeqElement : _chplSeqElementType;
  var _chplSeqNext : _chplSeqNode;
}

class seq2 {
  type _chplSeqElementType;

  var length : integer;
  var _chplSeqFirst : _chplSeqNode;
  var _chplSeqLast : _chplSeqNode;

  function _chplSeqAppend(e : _chplSeqElementType) : seq2 {
    var new : _chplSeqNode = _chplSeqNode(_chplSeqElementType);
    new._chplSeqElement = e;
    if length > 0 {
      _chplSeqLast._chplSeqNext = new;
      _chplSeqLast = new;
    } else {
      _chplSeqFirst = new;
      _chplSeqLast = new;
    }
    length += 1;
    return this;
  }

  function _chplSeqPrepend(e : _chplSeqElementType) : seq2 {
    var new : _chplSeqNode = _chplSeqNode(_chplSeqElementType);
    new._chplSeqElement = e;
    if length > 0 {
      new._chplSeqNext = _chplSeqFirst;
      _chplSeqFirst = new;
    } else {
      _chplSeqFirst = new;
      _chplSeqLast = new;
    }
    length += 1;
    return this;
  }

  function _chplSeqConcat(s : seq2) : seq2 {
    _chplSeqLast._chplSeqNext = s._chplSeqFirst;
    _chplSeqLast = s._chplSeqLast;
    length += s.length;
    return this;
  }

  function copy() : seq2 {
    var new : seq2 = seq2(_chplSeqElementType);
    var tmp : _chplSeqNode = _chplSeqFirst;
    while tmp != nil {
      new = new._chplSeqAppend(tmp._chplSeqElement);
      tmp = tmp._chplSeqNext;
    }
    return new;
  }
}

/*** Need where clauses to get all these working
function #(s : seq2, e : seq2._chplSeqElementType) {
  return s._chplSeqAppend(e);
}

function #(e : seq2._chplSeqElementType, s : seq2) {
  return s._chplSeqPrepend(e);
}
***/

function #(s1 : seq2, s2 : seq2) {
  return s1._chplSeqConcat(s2);
}

function write(s : seq2) {
  write("(/");
  var tmp = s._chplSeqFirst;
  while tmp != nil {
    write(tmp._chplSeqElement);
    tmp = tmp._chplSeqNext;
    if (tmp != nil) {
      write(", ");
    }
  }
  write("/)");
}
