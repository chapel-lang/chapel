class mySeqNode {
  type elementType;

  var element : elementType;
  var next : mySeqNode;
}

class mySeq {
  type elementType;

  var length : integer;
  var first : mySeqNode;
  var last : mySeqNode;

  function append(e : elementType) : mySeq {
    var new : mySeqNode = mySeqNode(elementType);
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

  function prepend(e : elementType) : mySeq {
    var new : mySeqNode = mySeqNode(elementType);
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

  function concat(s : mySeq) : mySeq {
    last.next = s.first;
    last = s.last;
    length += s.length;
    return this;
  }

  function copy() : mySeq {
    var new : mySeq;
    new = mySeq(elementType);
    var tmp : mySeqNode = first;
    while tmp != nil {
      new = new.append(tmp.element);
    }
    return new;
  }
}

function #(s : mySeq, e : mySeq.elementType) {
  return s.append(e);
}

function #(e : mySeq.elementType, s : mySeq) {
  return s.prepend(e);
}

function #(s1 : mySeq, s2 : mySeq) {
  return s1.concat(s2);
}

var s : mySeq = mySeq(integer);

s = s # 1;
s = s # 2;
s = s # 3;

writeln(s);

s = 4 # s;
s = 5 # s;
s = 6 # s;

writeln(s);
