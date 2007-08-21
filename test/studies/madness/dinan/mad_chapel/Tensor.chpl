def tensorDup(A) {
  var B = A;
  return B;
}

def transpose(A: [] real) where A.rank == 2 {
  for (i, j) in A.domain do
   if i < j then A[i, j] <=> A[j, i];
  return A;
}

// FIXME: not implemented
def transform(in V, in M) { return V; }

// Frobenius Norm -- 2-norm for Vectors
def normf(in A) where A.rank == 1 {
  return sqrt(+ reduce A**2);
}
