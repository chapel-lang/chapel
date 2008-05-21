var y = 3;

class C {
  var x = 2;
}

module M {
  var y = 1;
  def C.foo() {
    writeln(y, x);
  }

  def main() {
    var c = new C();
    c.foo();
  }
}
