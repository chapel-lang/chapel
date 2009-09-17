class C {
  var a: int;
  def C(n: int) {
    a = n;
    writeln("construct C");
  }
}

def foo(x) type {
  var nn = new C(x);
  for i in 1..x do
    writeln(i);
  return nn.type;
}

def main {
  var a: foo(3);
  writeln(typeToString(a.type));
}
