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
  var DsC: unmanaged C?;
  var y: int = DsC!.baz();

  override proc foo() {
    writeln("In D.foo(), y is ", y);
  }

  override proc baz(): int {
    return y;
  }

  proc buildNew() {
    return new unmanaged D();
  }
}

var myC = new unmanaged C();
var myD = new unmanaged D(DsC=myC);

writeln("myC = ", myC);
writeln("myD = ", myD);

if (runsegfault) {
  var myD2 = myD.buildNew();

  writeln("myD2 = ", myD2);

  delete myD2;
}

delete myD;
delete myC;

