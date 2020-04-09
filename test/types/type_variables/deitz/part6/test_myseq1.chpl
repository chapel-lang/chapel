class myseq_node {
  type elementType;
  var element : elementType;
  var next : unmanaged myseq_node(elementType)?;
}

class myseq {
  type elementType;

  var  length : int;
  var  first  : unmanaged myseq_node(elementType)?;
  var  last   : unmanaged myseq_node(elementType)?;

  proc deinit() {
    if first != nil then delete first;
  }

  proc append(e : elementType) {
    var copy : unmanaged myseq_node(elementType) = new unmanaged myseq_node(elementType = elementType);
    copy.element = e;

    if first != nil then delete first;

    first = copy;
    last = copy;
    length = 1;
  }

  proc print() {
    writeln("sequence contains ", first!.element);
  }
}

var s : borrowed myseq(int) = new borrowed myseq(int);

s.append(2);

s.print();

s.append(3);

s.print();
