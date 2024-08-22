// Chapel port of the C matmul benchmark
// See https://chapel-lang.org/
// Please use the preferred configuration of Chapel when benchmarking;
// see https://chapel-lang.org/docs/usingchapel/QUICKSTART.html#using-chapel-in-its-preferred-configuration

// While Chapel is a parallel programming language, the matrix multiplication
// here is not parallel, in order to be comparable with the C version.
// However, the arrays are initialized in parallel in this version.

// To compile, use
//
//   chpl --fast matmul.chpl
//
// To run, use
//
//   ./matmul
//
// or run with a different matrix size, as in:
//
//  ./matmul --n=100


proc mat_gen(n: int) {
  const tmp = 1.0 / n / n;
  const D = {0..<n, 0..<n};
  const mat:[D] real = forall (i,j) in D do tmp * (i - j) * (i + j); 
  return mat;
}

proc mat_mul(n: int, p: int, a:[] real, m: int, b:[] real) {
  var c:[0..<m, 0..<n] real = 0;
  // One would expect a parallel matmul in Chapel.
  // However, this is serial to use the same algorithm.
  for i in 0..<n {
    for k in 0..<p {
      for j in 0..<m {
        c[i,j] += a[i,k] * b[k,j];
      }
    }
  }

  return c;
}

config const n = 1500;

proc main() {
  const a = mat_gen(n);
  const b = mat_gen(n);
  const c = mat_mul(n, n, a, n, b);
  writeln(c[n>>1, n>>1]);
}
