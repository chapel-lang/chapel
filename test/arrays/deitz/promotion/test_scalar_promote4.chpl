proc foo(i : int, j : int) {
  return i**2 + j**2;
}

var s1: [1..4] int = ( 1, 2, 3, 4 );
var s2: [1..4] int = ( 1, 2, 3, 4 );

writeln(foo(s1, 2));
writeln(foo(s1, s2));
