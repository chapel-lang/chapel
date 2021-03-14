class node {
  type t;
  var element : t;
  var next : unmanaged node(t)?;
}

record foo {
  type t;
  var length : int;
  var first : unmanaged node(t)?;
  var last : unmanaged node(t)?;

  proc append(e : t) {
   var anew : unmanaged node(t) = new unmanaged node(t);
    anew.element = e;
    if length > 0 {
      last!.next = anew;
      last = anew;
    } else {
      first = anew;
      last = anew;
    }
    length += 1;
    return this;
  }

  proc cleanup() {
    var cursor: unmanaged node(t)?;
    var next: unmanaged node(t)?;
    cursor = first;
    while (cursor != nil) {
      next = cursor!.next;
      delete cursor;
      cursor = next;
    }
  }
}

proc foo.writeThis(fp) throws {
  fp.write("(/");
  var tmp = first;
  while tmp != nil {
    fp.write(tmp!.element);
    tmp = tmp!.next;
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

var g : foo(string);

g.append("one");
g.append("two");

writeln(g);

g.cleanup();
f.cleanup();
