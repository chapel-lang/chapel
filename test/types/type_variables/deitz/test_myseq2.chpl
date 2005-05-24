class myseq_node {
  type elementType;
  var element : elementType;
  var next : myseq_node;
}

class myseq {
  type elementType;
  var length : integer;
  var first : myseq_node;
  var last : myseq_node;

  function append(e : elementType) : myseq {
    var new : myseq_node = myseq_node(elementType);
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

var s : myseq = myseq(integer);

s.append(2);
s.append(3);

s.print();
