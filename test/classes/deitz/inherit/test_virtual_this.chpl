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
delete c;
c = new D();
c();
delete c;
