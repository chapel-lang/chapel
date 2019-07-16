class C {}
class D:C {
  proc procInD() { writeln("got a D"); }
}

proc procOutD(out arg: D?) {
  arg = new D();
}

var c: C?;
var d: D?;

procOutD(d); // OK
procOutD(c); // works; disallowed by the current implementation
c.procInD(); // disallowed; happens to work in this case if it were allowed
d.procInD(); // OK
