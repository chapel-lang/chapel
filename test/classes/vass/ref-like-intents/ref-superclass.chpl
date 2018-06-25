class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procRefD(ref arg: borrowed D) {
  arg.procInD(); // would be undefined if the error were not reported
}

var c = new borrowed C();
var d = new borrowed D();

procRefD(c); // error
procRefD(d); // OK
