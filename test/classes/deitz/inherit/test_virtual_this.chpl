class C {
  var x: int;
  def this() {
    writeln("C: x");
  }
}

class D : C {
  def this() {
    writeln("D: x");
  }
}

var c = new C();
c();
c = new D();
c();
