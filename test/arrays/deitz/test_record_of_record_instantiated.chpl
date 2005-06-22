record foo {
  type t;
  var fe : t;
}

record bar {
  type t;
  var be : foo(t);
}

var f : foo(integer);
f.fe = 12;
writeln(f);

var b : bar(integer);
b.be.fe = 14;
writeln(b);
