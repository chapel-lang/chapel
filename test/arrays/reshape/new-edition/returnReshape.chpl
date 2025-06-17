var A = [1, 2, 3, 4];

proc foo() {
  return reshape(A, 1..2, 1..2);  // 3032
}

proc bar() {
  return reshape(A, {1..2, 1..2});  // 3094
}

proc baz() {
  return reshape(A, 1..2, 1..2, copy=true);  // 3047
}

proc bam() {
  return reshape(A, {1..2, 1..2}, copy=true);  //  3085
}

var B = foo();
B[1,1] = 5;
var C = bar();
C[1,2] = 6;
var D = baz();
D[2,1] = 7;
var E = bam();
E[2,2] = 8;

writeln(A);
writeln(B);
writeln(C);
writeln(D);
writeln(E);
