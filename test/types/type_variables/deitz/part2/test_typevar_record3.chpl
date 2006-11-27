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
   var new : node(t) = node(t);
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
}

def foo.write(fp: file) {
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
