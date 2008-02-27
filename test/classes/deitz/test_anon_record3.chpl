def foo(type t) {
  var x: t;
  writeln(x);
}

foo(record { var x : int; var y : int; });
