class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procRefD(ref arg: D) {
  arg.procInD(); // would be undefined if the error were not reported
}

var c = new C();
var d = new D();

procRefD(c); // error
procRefD(d); // OK
