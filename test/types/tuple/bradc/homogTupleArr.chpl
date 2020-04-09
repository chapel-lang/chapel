config const n = 4;

type vec3 = 3*real;
type arr33 = 3*vec3;

type arr33c = 3*(3*real);

var A: [1..n, 1..n] arr33;
var C: [1..n, 1..n] arr33c;
var D: [1..n, 1..n] 3*vec3;
var F: [1..n, 1..n] 3*(3*real);

forall (i,j) in {1..n, 1..n} {
  forall (x,y) in {0..2, 0..2} {
    A(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    C(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    D(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    F(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
  }
}

writeln("A is:\n", A);
writeln("C is:\n", C);
writeln("D is:\n", D);
writeln("F is:\n", F);
