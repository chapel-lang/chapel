var A: [1..10] int = [i in 1..10] i;

for i in 0..11 by -1 {
  var (found, idx) = A.find(i);
  writeln((found, i, idx, if found then A[idx] else -1));
}
