//
// See issue #14367. An associative array with a value type that is a
// non-nilable class must not insert nil as the default value (as it does for
// _nilable_ classes) when a key is added to the array's domain.
//

class C {
  var x: int;
}

proc main() {
  var d: domain(string);
  var a: [d] shared C;

  // Some sort of error should have been emitted by this point.
  d += "key";

  // Will emit a nil error (which shouldn't be possible).
  writeln(a);
}

