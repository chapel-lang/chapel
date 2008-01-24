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

  def append(e : elementType) {
    var copy : myseq_node(elementType) = myseq_node(elementType = elementType);
    copy.element = e;
    first = copy;
    last = copy;
    length = 1;
  }

  def print() {
    writeln("sequence contains ", first.element);
  }
}

var s : myseq(int) = myseq(int);

s.append(2);

s.print();

s.append(3);

s.print();
