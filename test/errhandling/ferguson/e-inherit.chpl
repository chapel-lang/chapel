config const cthrow = false;
config const dthrow = false;

use ExampleErrors;

class C {
  proc foo() throws {
    writeln("C.foo()");
    if cthrow then
      throw new owned StringError("test error C");
  }
}

class D : C {
  override proc foo() throws {
    writeln("D.foo()");
    if dthrow then
      throw new owned StringError("test error D");
  }
}

var c = new owned C();
var d = new owned D();
var dc:owned C = new owned D();

try { c.foo(); } catch { writeln("c.foo threw"); }
try { d.foo(); } catch { writeln("d.foo threw"); }
try {dc.foo(); } catch { writeln("dc.foo threw"); }

writeln("Fini");
