var A: [1..10] int = [i in 1..10] i;

for i in 0..11 by -1 {
  var (found, idx) = maxloc reduce zip(A == i, A.domain);
  writeln((found, i, if found then idx else 0, if found then A[idx] else -1));
}
