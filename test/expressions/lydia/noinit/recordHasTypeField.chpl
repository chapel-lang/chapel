record foo {
  type t;
  var i: t;
}

inline proc _defaultOf (type t) where t == foo(int) {
  writeln("I default initialized!");
  var res: t = noinit;
  res.i = 3;
  return res;
}

var bar: foo(int) = noinit; // Should not print message
bar.i = 7;
writeln(bar);
var baz: foo(int);          // Should print message
writeln(baz);
