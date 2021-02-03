use LinearAlgebra;
use Math;

config const n = 10;
config const epsilon = 0.000001;

proc sinMatrix(n) {
  var A = Matrix(0..<n);
  const fac0 = 1.0/(n+1.0);
  const fac1 = sqrt(2.0*fac0);
  for (i,j) in {0..<n,0..<n} {
    A[i,j] = fac1*sin((i+1)*(j+1)*pi*fac0);
  }
  return A;
}

var A = sinMatrix(n);

var (LU, ipiv) = lu(A);
writeln(ipiv);
writeln();
writeln(det(LU));
writeln(det(A));
writeln();


var B = Matrix([1.0, 0.0,-5.0,-1.0, 2.0],
               [2.0, 6.0,-2.0, 3.0, 0.0],
               [2.0, 5.0, 2.0, 1.0, 1.0],
               [-2.0,1.0, 2.5, 3.0, 1.0],
               [-1.0,1.0, 0.0, 3.5, 0.5]);

var b: [0..<5] real = 0..<5;
var c: [0..<5] real = 0..<5;

var x = solve(B, b);

for (control, result) in zip(c, dot(B,x)) {
  if abs(control-result) > epsilon {
    halt("Verification failed");
  }
}
writeln("Verification successful");
