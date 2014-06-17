config var runsegfault = false;

class C {
  var x = 2;
  proc foo() {
    writeln("In C.foo()");
  }
  proc baz(): int {
    return x;
  }
}

class D : C {
  var DsC: C;
  var y: int = DsC.baz();

  proc foo() {
    writeln("In D.foo(), y is ", y);
  }
  proc baz(): int {
    return y;
  }
  proc buildNew() {
    return new D();
  }
}

var myC = new C();
var myD = new D(DsC=myC);
writeln("myC = ", myC);
writeln("myD = ", myD);

if (runsegfault) {
  var myD2 = myD.buildNew();
  writeln("myD2 = ", myD2);
}
