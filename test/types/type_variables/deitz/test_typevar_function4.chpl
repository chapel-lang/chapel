function foo(i : ?t) {
  writeln("The function foo has been called with actual value ", i);
}

var i : integer = 2;

foo(i);

var j : float = 3.2;

foo(j);
