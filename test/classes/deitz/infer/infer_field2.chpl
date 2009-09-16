use List;
use infer_field2_common;

def foo() {
  var c = new C();
  var cc = c;
  var s : list(int);
  c = next_foo(c);
  while c != nil {
    s.append(c.result);
    c = next_foo(c);
  }
  delete cc;
  return s;
}

def main {
  writeln(foo());
  writeln( bar());
}
