record foo {
  type t;
  var x : t;
}

var f : foo(foo(int));
writeln(f);

record bar {
  var y : foo(foo(real));
}

var b : bar;
writeln(b);
