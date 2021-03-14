config var n : int = 4;

var d : domain(1) = {1..n};

var a : [d] [d] int;

forall i1 in d {
  forall i2 in d {
    a(i1)(i2) = (i1-1) * n + i2;
  }
}

for i1 in d {
  for i2 in d {
    writeln(a(i1)(i2));
  }
}
