use LinkedLists;
use infer_field2_common;

proc foo() {
  var c = new unmanaged C()?;
  var cc = c;

  var s : LinkedList(int);

  c = next_foo(c!);

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



  for i in bar() do
    writeln(i);

  var c = new unmanaged C()?;
  var cc = c;

  c = next_foo(c!);

  while c != nil {
    writeln(c!.result);
    c = next_foo(c!);
  }

  delete cc;
}
