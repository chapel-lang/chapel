class myseq_node {
  type elementType;
  var element : elementType;
  var next : myseq_node(elementType);
}

class myseq {
  type elementType;
  var length : int;
  var first : myseq_node(elementType);
  var last : myseq_node(elementType);

  def append(e : elementType) : myseq(elementType) {
    var anew : myseq_node(elementType) = myseq_node(elementType);
    anew.element = e;
    if length > 0 {
      last.next = anew;
      last = anew;
    } else {
      first = anew;
      last = anew;
    }
    length += 1;
    return this;
  }

  def print() {
    write("Sequence (length = ", length, ") (/");
    var tmp = first;
    while tmp != nil {
      write(tmp.element, " ");
      tmp = tmp.next;
    }
    writeln("/)");
  }
}

var s : myseq(int) = myseq(int);

s.append(2);
s.append(3);

s.print();
