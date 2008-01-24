class node {
  type t;
  var element : t;
  var next : node(t);
}

record foo {
  type t;
  var length : int;
  var first : node(t);
  var last : node(t);

  def append(e : t) {
   var anew : node(t) = node(t);
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
}

def foo.writeThis(fp: Writer) {
  fp.write("(/");
  var tmp = first;
  while tmp != nil {
    fp.write(tmp.element);
    tmp = tmp.next;
    if (tmp != nil) {
      fp.write(", ");
    }
  }
  fp.write("/)");
}

var f : foo(int);

f.append(1);
f.append(2);

writeln(f);
