proc foo(x: int) {
  writeln(x);
}

proc foo(x: real) {
  writeln(x);
}

proc foo(xy:(int, real)) {
  writeln(xy(0), " ", xy(1));
}

var A = [4, 5, 6];

foo([i in 1..3] i);
foo([j in A] j);
foo([i in 7..9] 1.0*i);
foo([(i,j) in zip(10..12, A)] (i, 2.0*j));
A = [13, 14, 15];
foo([(i,j) in zip(A, 1..3)] (i, 2.0*j));
