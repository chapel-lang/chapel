class C {
  proc foo() throws { writeln("C.foo()");}
}

class D : C {
  proc foo() { writeln("D.foo()"); }
}

var c = new C();
var d = new D();
var dc:C = new D();

try { c.foo(); } catch { writeln("c.foo threw"); }
try { d.foo(); } catch { writeln("d.foo threw"); }
try {dc.foo(); } catch { writeln("dc.foo threw"); }

delete c;
delete d;
delete dc;

writeln("Fini");
