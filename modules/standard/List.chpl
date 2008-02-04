class listNode {
  type eltType;
  var data: eltType;
  var next: listNode(eltType);
}

record list {
  type eltType;
  var first: listNode(eltType);
  var last: listNode(eltType);
  var length: int;

  def these() {
    var tmp = first;
    while tmp != nil {
      yield tmp.data;
      tmp = tmp.next;
    }
  }

  def append(e : eltType) {
    if last {
      last.next = new listNode(eltType, e);
      last = last.next;
    } else {
      first = new listNode(eltType, e);
      last = first;
    }
    length += 1;
  }

  def append(e: eltType, es: eltType ...?k) {
    append(e);
    for param i in 1..k do
      append(es(i));
  }

  def prepend(e : eltType) {
    first = new listNode(eltType, e, first);
    if last == nil then
      last = first;
    length += 1;
  }

  def concat(l: list(eltType)) {
    for e in l do
      append(e);
  }

  def writeThis(f: Writer) {
    var first: bool = true;
    for e in this {
      if !first then
        f.write(" ");
      else
        first = false;
      f.write(e);
    }
  }
}

def makeList(x ...?k) {
  var s: list of x(1).type;
  for param i in 1..k do
    s.append(x(i));
  return s;
}
