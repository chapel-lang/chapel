class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procInoutD(inout arg: D) {
  arg.procInD(); // would be undefined if the error were not reported
}

var c = new C();
var d = new D();

procInoutD(c); // error
procInoutD(d); // OK
