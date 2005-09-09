class myseq_node {
  type elementType;
  var element : elementType;
  var next : myseq_node(elementType);
}

class myseq {
  type elementType;
  var length : integer;
  var first : myseq_node(elementType);
  var last : myseq_node(elementType);

  function append(e : elementType) : myseq {
    var new : myseq_node(elementType) = myseq_node(elementType);
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

  function print() {
    write("Sequence (length = ", length, ") (/");
    var tmp = first;
    while tmp != nil {
      write(tmp.element, " ");
      tmp = tmp.next;
    }
    writeln("/)");
  }
}

var s : myseq(integer) = myseq(integer);

s.append(2);
s.append(3);

s.print();
