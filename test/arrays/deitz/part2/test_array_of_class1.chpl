config var n : int = 4;

class foo {
  var i : int;
  var f : real;
}

var d : domain(2) = {1..n, 1..n};

var a : [d] unmanaged foo;

forall (i,j) in d {
  a(i,j) = new unmanaged foo();

  a(i,j).i = i*j;
  a(i,j).f = 3.5 * i;
}

for (i,j) in d {
  writeln(a(i,j).i);
}

for (i,j) in d do delete a(i,j);
