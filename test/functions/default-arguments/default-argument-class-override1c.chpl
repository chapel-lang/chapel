class C {
  proc foo(x = 10) {
    writeln("In C.foo(",x,")");
  }
}

class D: C {
  override proc foo(x = 20) {
    writeln("In D.foo(",x,")");
  }
}

// This doesn't do the right thing
var myD: C = new D();
myD.foo();

// Unless we also resolve an obvious dispatch to D.foo() (e.g., by
// compiling with -sdoMore=true
config param doMore = false;
config const printMore = false;

if doMore {
  var myD = new D();
  if printMore then
    myD.foo();
}
