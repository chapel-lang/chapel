class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutD(out arg: unmanaged D?) {
  arg = new unmanaged D();
}

var c: unmanaged C?;
var d: unmanaged D?;

procOutD(d); // OK
procOutD(c); // allowed; c stores a D
(c:borrowed D?)!.procInD(); // OK
d!.procInD(); // OK

delete c;
delete d;
