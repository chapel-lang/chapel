// This test demonstrates a problem when combining delegation
// with coercion.
//
// The current implementation of delegation looks for delegated
// methods only if no other candidates are available. That means
// that methods taking coercions will take precedence.

record Wrapper {
  forwarding var instance; // e.g. some class
  proc foo(x:int(64)) { writeln("in Wrapper.foo int(64) (", x, ")"); }
}

class C {
  var field:int;
  proc foo(x:int(8)) { writeln("in C.foo int(8) (", x, ")"); }
}

var r = new Wrapper(new unmanaged C());

writeln("int(8)");
r.foo(1:int(8));
