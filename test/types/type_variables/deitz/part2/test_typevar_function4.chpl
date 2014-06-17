proc foo(i : ?t) {
  writeln("The function foo has been called with actual value ", i);
}

var i : int = 2;

foo(i);

var j : real = 3.2;

foo(j);
