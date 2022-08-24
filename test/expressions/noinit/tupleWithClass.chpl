class foo {
  var bar: int;
}

inline proc _defaultOf(type t) where t == (int, borrowed foo?) {
  writeln("I default initialized!");
  var innerRec: borrowed foo?;
  return (0, innerRec);
}

var tup: (int, borrowed foo?) = noinit; // Should not print message

tup(1) = (new owned foo(2)).borrow();
tup(0) = 5;

writeln(tup);

var otherTup: (int, borrowed foo?);     // Should print message

otherTup(1) = (new owned foo(3)).borrow();
writeln(otherTup);
