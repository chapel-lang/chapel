class C {
  var x: int;
  proc this() {
    writeln("C: x");
  }
}

class D : C {
  proc this() {
    writeln("D: x");
  }
}

var c = new C();
c();
delete c;
c = new D();
c();
delete c;
