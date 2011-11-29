record foo {
  type t;
  var fe : t;
}

record bar {
  var be : foo(int);
}

var b : bar;
b.be.fe = 14;
writeln(b);
