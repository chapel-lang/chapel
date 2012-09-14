config var n = 9;

var D = {1..n, 1..n};
var A: [D] real;
var Res: [D] (real, index(D));

forall (i,j) in D {
  A(i,j) = i + j/10.0;
}

Res =  minloc scan zip(A, A.domain);
writeln("Res = ", Res);

Res = maxloc scan zip(A, A.domain);
writeln("Res = ", Res);
