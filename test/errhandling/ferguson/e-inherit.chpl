config const cthrow = false;
config const dthrow = false;

use ExampleErrors;

class C {
  proc foo() throws {
    writeln("C.foo()");
    if cthrow then
      throw new StringError("test error C");
  }
}

class D : C {
  proc foo() throws {
    writeln("D.foo()");
    if dthrow then
      throw new StringError("test error D");
  }
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
