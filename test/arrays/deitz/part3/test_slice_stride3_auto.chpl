var x = 1;

proc foo(n: int, m: int, A: [1..n, 1..m] int) {
  for i in A.domain {
    A(i) = x;
    x += 1;
  }
}


var D = {1..6, 1..6};
var A: [D] int;

writeln(A);writeln();
foo(6, 6, =>A);
writeln(A);writeln();
foo(6, 6, =>A(1..6, 1..6));
writeln(A);writeln();
foo(6, 3, =>A(1..6, 1..6 by 2));
writeln(A);writeln();
foo(3, 6, =>A(1..6 by 2, 1..6));
writeln(A);writeln();
foo(3, 3, =>A(1..6 by 2, 1..6 by 2));
writeln(A);writeln();
