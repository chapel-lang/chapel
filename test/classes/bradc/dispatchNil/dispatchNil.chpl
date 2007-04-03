config var runsegfault = false;

class C {
  var x = 2;
  def foo() {
    writeln("In C.foo()");
  }
  def baz(): int {
    return x;
  }
}

class D : C {
  var DsC: C;
  var y: int = DsC.baz();

  def foo() {
    writeln("In D.foo(), y is ", y);
  }
  def baz(): int {
    return y;
  }
  def buildNew() {
    return D();
  }
}

var myC = C();
var myD = D(DsC=myC);
writeln("myC = ", myC);
writeln("myD = ", myD);

if (runsegfault) {
  var myD2 = myD.buildNew();
  writeln("myD2 = ", myD2);
}
