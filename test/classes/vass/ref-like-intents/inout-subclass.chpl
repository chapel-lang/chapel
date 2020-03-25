class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procInoutC(inout arg: C?) {
  arg = new C();
}

var c: borrowed C?;
var d: borrowed D?;

procInoutC(c); // OK
procInoutC(d); // error
d.procInD(); // would be undefined if the error were not reported
