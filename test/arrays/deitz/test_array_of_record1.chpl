config var n : integer = 4;

record foo {
  var i : integer;
  var f : float;
}

var d : domain(2) = (1..n, 1..n);

var a : [d] foo;

forall i,j in d {
  a(i,j).i = i*j;
  a(i,j).f = 3.5 * i;
}

forall i,j in d {
  writeln(a(i,j).i);
}
