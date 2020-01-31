//
// See issue #14367. An associative array with a value type that is
// a non-nilable class must not insert nil as the default value (as
// it does for _nilable_ classes) when a key is added to the array's
// domain.
//

class C {
  var x: int;
}

proc main() {
  var d: domain(string);
  var a: [d] shared C;

  //
  // We should emit some sort of error here, or prevent this action
  // from happening in the first place.
  //
  d += "key";

  // Will emit a nil error (which shouldn't be possible).
  writeln(a);
}

