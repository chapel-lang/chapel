var A: [1..10] int = [i in 1..10] i;

writeln(A);
writeln("reversing even sized array");
reverse(A);
writeln(A);

var B: [1..9] int = [i in 1..9] i;
writeln(B);
writeln("reversing odd sized array");
reverse(B);
writeln(B);

var C: [11..20] int = [i in 11..20] i;
writeln(C);
writeln("reversing array with higher domain");
reverse(C);
writeln(C);

/* Reverse the order of the values in the array. */
proc reverse(ref A) {
  const lo = A.domain.low,
        mid = A.domain.sizeAs(A.idxType) / 2,
        hi = A.domain.high;
  for i in 0..#mid {
    A[lo + i] <=> A[hi - i];
  }
}
