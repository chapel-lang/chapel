class _chplSeqNode {
  type elementType;

  var element : elementType;
  var next : _chplSeqNode;
}

class seq2 {
  type elementType;

  var length : integer;
  var first : _chplSeqNode;
  var last : _chplSeqNode;

  function append(e : elementType) : seq2 {
    var new : _chplSeqNode = _chplSeqNode(elementType);
    new.element = e;
    if length > 0 {
      last.next = new;
      last = new;
    } else {
      first = new;
      last = new;
    }
    length += 1;
    return this;
  }

  function prepend(e : elementType) : seq2 {
    var new : _chplSeqNode = _chplSeqNode(elementType);
    new.element = e;
    if length > 0 {
      new.next = first;
      first = new;
    } else {
      first = new;
      last = new;
    }
    length += 1;
    return this;
  }

  function concat(s : seq2) : seq2 {
    last.next = s.first;
    last = s.last;
    length += s.length;
    return this;
  }

  function copy() : seq2 {
    var new : seq2;
    new = seq2(elementType);
    var tmp : _chplSeqNode = first;
    while tmp != nil {
      new = new.append(tmp.element);
    }
    return new;
  }
}

function #(s : seq2, e : seq2.elementType) {
  return s.append(e);
}

function #(e : seq2.elementType, s : seq2) {
  return s.prepend(e);
}

function #(s1 : seq2, s2 : seq2) {
  return s1.concat(s2);
}
