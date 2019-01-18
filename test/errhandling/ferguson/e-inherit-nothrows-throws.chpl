class C {
  proc foo() { writeln("C.foo()");}
}

class D : C {
  override proc foo() throws { writeln("D.foo()"); }
}

var c = new unmanaged C();
var d = new unmanaged D();
var dc: unmanaged C = new unmanaged D();

try { c.foo(); } catch { writeln("c.foo threw"); }
try { d.foo(); } catch { writeln("d.foo threw"); }
try {dc.foo(); } catch { writeln("dc.foo threw"); }

delete c;
delete d;
delete dc;

writeln("Fini");
