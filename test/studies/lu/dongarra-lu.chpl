// NEEDS:
// * approximate ~= comparison for floats
// * swap assignment operator (<=> ?)
// * Triangular domain/matrix implementation
// * flattening slices
// * tensor-product multiplication
// * complete indefinite ranges for array slicing

// TODOS:
// * sketch out triangular domain/array implementation

config const epsilon = minPositive(float);

def lutx(A: [1..?n, 1..n]) {
  var p: [i in 1..n] = i;

  for k in [1..n) {
    const (r,m) = maxValLoc(abs(A[k..n,k]));
    // no need to convert m from slice-local to global

    // skip elimination if column is zero
    if (abs(A(m, k)) < epsilon) {       // matlab uses ~= -- cute!  I want!

      // swap pivot row
      if (abs(m-k) < epsilon) {         // again, good use for ~=: m ~= k
        const TempRow = A[m,..];       // would like swap assignment here
        A[m,..] = A[k,..];
        A[k,..] = TempRow;

        const tempPerm = p(m);          // and here
        p(m) = p(k);
        p(k) = tempPerm;
      }

      // Compute multipliers
      const i = k+1..n;
      A[i,k] = A[i,k] / A[k,k];

      // Update the remainder of the matrix
      const j = k+1..n;
      A[i,j] -= *[A[i,k], A[k,j]];
    }
  }

  // bool/enum args to select diagonal inclusion, anti-diagonal triangles
  const (L,U) = Triangular.makeLowerUpper(A);  
  L += eye(n,n);  // TODO: not sure what eye is?

  return (L, U, p);
}
