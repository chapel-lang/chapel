var A: [1..5] int = [i in 1..5] i;
/*

   // This is the workaround.

proc foo(n:int, X: [0..#n] int) {
  for (i,x) in (X.domain, X) {
    writeln(i, ": ", x);
  }
}


foo(5, A);

*/

proc foo(X: [0..?n] int) {
  for (i,x) in (X.domain, X) {
    writeln(i, ": ", x);
  }
}


foo(A);


