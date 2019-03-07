config var n = 9;

var D = {1..n, 1..n};
var A: [D] real;

forall (i,j) in D {
  A(i,j) = i + j/10.0;
}

var Res =  minloc scan zip(A, A.domain);
writeln("Res = ", Res);

var Res2 =  maxloc scan zip(A, A.domain);
writeln("Res2 = ", Res2);
