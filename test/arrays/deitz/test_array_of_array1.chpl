config var n : integer = 4;

var d : domain(1) = 1..n;

var a : [d] [d] integer;

forall i1 in d {
  forall i2 in d {
    a(i1)(i2) = (i1-1) * n + i2;
  }
}

forall i1 in d {
  forall i2 in d {
    writeln(a(i1)(i2));
  }
}
