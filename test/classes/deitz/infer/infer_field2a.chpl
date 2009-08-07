use List;
use infer_field2_common;

def foo() {
  var c = new C();
  var s : list(int);
  c = next_foo(c);
  while c != nil {
    s.append(c.result);
    c = next_foo(c);
  }
  return s;
}

def main {
  writeln(foo());
  writeln( bar());
}
