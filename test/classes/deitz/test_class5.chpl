record foo {
  type t;
  var x : t;
}

var f : foo(foo(integer));
writeln(f);

record bar {
  var y : foo(foo(float));
}

var b : bar;
writeln(b);
