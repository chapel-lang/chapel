function foo(type t) {
  writeln("foo(type t) called");
  var x : t;
  writeln("  ", x);
}

function foo(x : integer) {
  writeln("foo(integer) called");
  writeln("  ", x);
}

foo(2);
foo(3.0);
foo(integer);
