class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutD(out arg: borrowed D?) {
  arg = new D();
}

var c: borrowed C?;
var d: borrowed D?;

procOutD(d); // OK
procOutD(c); // works; disallowed by the current implementation
c.procInD(); // disallowed; happens to work in this case if it were allowed
d.procInD(); // OK
