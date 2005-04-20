config var n : integer = 4;

var D : domain(1) = (1..n);

var A : [D] seq of integer;

A(1) = (/1, 2, 3/);
A(3) = (/5, 6, 7/);

forall i in D {
  writeln(A(i));
}

A(1) = A(1) # 4;

forall i in D {
  writeln(A(i));
}
