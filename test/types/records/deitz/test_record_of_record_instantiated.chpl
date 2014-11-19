record foo {
  type t;
  var fe : t;
}

record bar {
  type t;
  var be : foo(t);
}

var f : foo(int);
f.fe = 12;
writeln(f);

var b : bar(int);
b.be.fe = 14;
writeln(b);
