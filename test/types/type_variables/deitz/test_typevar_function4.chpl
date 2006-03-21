fun foo(i : ?t) {
  writeln("The fun foo has been called with actual value ", i);
}

var i : int = 2;

foo(i);

var j : float = 3.2;

foo(j);
