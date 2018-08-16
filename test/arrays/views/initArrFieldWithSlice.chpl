class C {
  var X: [1..3] real;
}

var A: [1..3] real;
var A1: [1..10] real;
var A2: [1..3, 1..3] real;
var A3: [0..2] real;

forall i in A.domain do
  A[i] = i / 10.0;

forall i in A1.domain do
  A1[i] = i / 10.0;

forall (i,j) in A2.domain do
  A2[i,j] = i + j / 10.0;

forall i in A3.domain do
  A3[i] = i / 10.0;


writeln(A);
writeln(A1);
writeln(A2);
writeln(A3);


var myC = new owned C(A);
var myC1 = new owned C(A1[1..3]);
var myC2 = new owned C(A2[2, ..]);
var myC3 = new owned C(A3.reindex(1..3));

writeln("myC is: ", myC);
writeln("myC1 is: ", myC1);
writeln("myC2 is: ", myC2);
writeln("myC3 is: ", myC3);
