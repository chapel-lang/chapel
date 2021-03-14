class C {
  proc foo() throws { writeln("C.foo()");}
}

class D : C {
  override proc foo() { writeln("D.foo()"); }
}

var c = new shared C();
var d = new shared D();
var dc:shared C = new shared D();

try { c.foo(); } catch { writeln("c.foo threw"); }
try { d.foo(); } catch { writeln("d.foo threw"); }
try {dc.foo(); } catch { writeln("dc.foo threw"); }

writeln("Fini");
