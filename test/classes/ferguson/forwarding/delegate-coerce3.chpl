// This test demonstrates a problem when combining delegation
// with coercion.
//
// The current implementation of delegation looks for delegated
// methods only if no other candidates are available. That means
// that methods taking coercions will take precedence.

record Wrapper {
  forwarding var instance; // e.g. some class
  proc deinit() {
    delete instance;
  }
}

class C {
  var field:int;
  proc foo(x:int(64)) { writeln("in C.foo int(64) (", x, ")"); }
  proc foo(x:int(8)) { writeln("in C.foo int(8) (", x, ")"); }
}

config const i8:int(8)    = 8;
config const i64:int(64)  = 64;

var r = new Wrapper(new unmanaged C());

writeln("int(8)");
r.foo(i8);
writeln("int(64)");
r.foo(i64);
