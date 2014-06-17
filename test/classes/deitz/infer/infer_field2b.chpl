// infer return type of iterator with state transform
use List;
use infer_field2_common;

proc foo() {
  var c = new C();
  var cc = c;
  c = next_foo(c);
  var s : list(c.result.type);
  while c != nil {
    s.append(c.result);
    c = next_foo(c);
  }
  delete cc;
  return s;
}

proc main {
  writeln( bar());
  writeln(foo());

  for i in bar() do
    writeln(i);

  var c = new C();
  var cc = c;
  c = next_foo(c);
  while c != nil {
    writeln(c.result);
    c = next_foo(c);
  }
  delete cc;
}
