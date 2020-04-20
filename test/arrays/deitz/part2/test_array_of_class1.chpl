config var n : int = 4;

class foo {
  var i : int;
  var f : real;
}

var d : domain(2) = {1..n, 1..n};

var a : [d] unmanaged foo =
  forall (i,j) in d  do makeFoo(i,j);

proc makeFoo(i,j) {
  const f = new unmanaged foo();

  f.i = i*j;
  f.f = 3.5 * i;

  return f;
}

for (i,j) in d {
  writeln(a(i,j).i);
}

for (i,j) in d do delete a(i,j);
