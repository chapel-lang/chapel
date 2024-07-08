var A: [1..10] int = [i in 1..10] i;

for i in 0..11 by -1 {
  var idx = A.find(i);
  writeln((idx != 0, i, idx, if idx != 0 then A[idx] else -1));
}
