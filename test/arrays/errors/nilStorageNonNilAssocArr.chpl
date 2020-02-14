//
// See issue #14367. An associative array with a value type that is a
// non-nilable class must not insert nil as the default value (as it does for
// _nilable_ classes) when a key is added to the array's domain.
//

class C {
  var x: int;
}

type OwnedC = owned C;
type SharedC = shared C;
type UnmanagedC = unmanaged C;

config type TestClass = OwnedC;

proc main() {
  var d: domain(string);
  var a: [d] TestClass;

  // Some sort of error should have been emitted by this point.
  d += "key";

  // This call will emit a nil error (which shouldn't be possible).
  writeln(a);
}

