use List;
use infer_field2_common;

proc foo() {
  var c = next_foo(new C());
  var cc = c;
  var s : list(c.result.type);
  while c != nil {
    s.append(c.result);
    c = next_foo(c);
  }
  delete cc;
  return s;
}

proc main {
  writeln(foo());
  writeln( bar());
}
