config const n = 4;

type vec3 = (real, real, real);
type arr33 = (vec3, vec3, vec3);

type arr33b = 3*(real, real, real);
type arr33c = ((real, real, real), (real, real, real), (real, real, real));

var A: [1..n, 1..n] arr33;
var B: [1..n, 1..n] arr33b;
var C: [1..n, 1..n] arr33c;
var D: [1..n, 1..n] (vec3, vec3, vec3);
var E: [1..n, 1..n] 3*(real, real, real);
var F: [1..n, 1..n] ((real, real, real), (real, real, real), (real, real, real));

forall (i,j) in {1..n, 1..n} {
  forall (x,y) in {0..2, 0..2} {
    A(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    B(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    C(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    D(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    E(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
    F(i,j)(x)(y) = (i-1)*9*n + (j-1)*9 + x*3 + y + 1;
  }
}

writeln("A is:\n", A);
writeln("B is:\n", B);
writeln("C is:\n", C);
writeln("D is:\n", D);
writeln("E is:\n", E);
writeln("F is:\n", F);
