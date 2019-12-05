use LinkedLists;
use infer_field2_common;

proc foo() {
  var c  = next_foo(new unmanaged C());
  var cc = c;

  var s : LinkedList(c!.result.type);

  while c != nil {
    s.append(c!.result);
    c = next_foo(c!);
  }

  delete cc;

  return s;
}

proc main {
  var l = foo();

  writeln(l);
  writeln(bar());

  l.destroy();
}
