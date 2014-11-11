class foo {
  var bar: int;
}

inline proc _defaultOf(type t) where t == (int, foo) {
  writeln("I default initialized!");
  var innerRec: foo;
  return (0, innerRec);
}

var tup: (int, foo) = noinit; // Should not print message
var otherTup: (int, foo);     // Should print message
tup(2) = new foo(2);
tup(1) = 5;
otherTup(2) = new foo(3);
writeln(tup);
writeln(otherTup);
