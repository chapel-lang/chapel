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

  function append(e : elementType) {
    var new : myseq_node(elementType) = myseq_node(elementType = elementType);
    new.element = e;
    first = new;
    last = new;
    length = 1;
  }

  function print() {
    writeln("sequence contains ", first.element);
  }
}

var s : myseq(integer) = myseq(integer);

s.append(2);

s.print();

s.append(3);

s.print();
