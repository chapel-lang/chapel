config var runsegfault = false;

class C {
  var x = 2;
  def foo() {
    writeln("In C.foo()");
  }
  def baz() {
    return x;
  }
}

class D : C {
  var DsC: C;
  var y = DsC.baz();

  def foo() {
    writeln("In D.foo(), y is ", y);
  }
  def baz() {
    return y;
  }
  def buildNew() {
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
