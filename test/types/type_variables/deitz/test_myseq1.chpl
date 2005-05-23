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

  function append(e : elementType) {
    var new : myseq_node = myseq_node(elementType = elementType);
    new.element = e;
    first = new;
    last = new;
    length = 1;
  }

  function print() {
    writeln("sequence contains ", first.element);
  }
}

var s : myseq = myseq(integer);

s.append(2);

s.print();

s.append(3);

s.print();
