config var n: int = 5;

const D = {0..n+1, 0..n+1};
const S: sparse subdomain(D) = genSparseInds(n);

iter genSparseInds(n) {
  for i in 1..n {
    yield (i,i);
  }
}

var A: [S] real;

for i in 1..n {
  A(i,i) = i;
}

for (i,j) in S {
  writeln("[", i, ", ", j, "]: ", A(i,j));
}
