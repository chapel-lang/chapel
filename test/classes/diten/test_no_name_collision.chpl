class outerClass {
  class aClass {
    var a: int;
  }
  def foo() {
    var c = aClass(4);
    writeln(c.a);
  }
}

class aClass {
  var b: int;
}

def main() {
  var a = aClass(3);
  var b = outerClass();
  b.foo();
  writeln(a.b);
}
