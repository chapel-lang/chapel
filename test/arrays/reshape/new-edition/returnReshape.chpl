var A = [1, 2, 3, 4];

proc foo() {
  return reshape(A, 1..2, 1..2);
}

proc bar() {
  return reshape(A, {1..2, 1..2});
}

proc baz() {
  return reshape(A, 1..2, 1..2, copy=true);
}

proc bam() {
  return reshape(A, {1..2, 1..2}, copy=true);
}

var B = foo();
//var C = bar();
var D = baz();
var E = bam();

writeln(A);
writeln(B);
//writeln(C);
writeln(D);
writeln(E);


