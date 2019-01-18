class C {
  var x: int;
  proc this() {
    writeln("C: x");
  }
}

class D : C {
  override proc this() {
    writeln("D: x");
  }
}

var c = new unmanaged C();
c();
delete c;
c = new unmanaged D();
c();
delete c;
