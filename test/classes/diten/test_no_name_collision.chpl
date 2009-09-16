class outerClass {
  class aClass {
    var a: int;
  }
  def foo() {
    var c = new aClass(4);
    writeln(c.a);
    delete c;
  }
}

class aClass {
  var b: int;
}

def main() {
  var a = new aClass(3);
  var b = new outerClass();
  b.foo();
  writeln(a.b);
  delete a;
  delete b;
}
