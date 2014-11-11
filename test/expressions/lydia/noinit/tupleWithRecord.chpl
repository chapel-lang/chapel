record foo {
  var bar: int;
}

inline proc _defaultOf(type t) where t == (int, foo) {
  writeln("I default initialized!");
  var innerRec: foo;
  return (0, innerRec);
}

var tup: (int, foo) = noinit; // Should not print message
tup(2).bar = 2;
tup(1) = 5;
var otherTup: (int, foo);     // Should print message
writeln(tup);
writeln(otherTup);
