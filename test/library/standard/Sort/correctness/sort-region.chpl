use Sort;

record AbsCompCmp {
  proc compare(a, b) { return abs(a) - abs(b); }
  proc name() { return 'AbsCompCmp'; }
}
record AbsKeyCmp {
  proc key(a) { return abs(a); }
  proc name() { return 'AbsKeyCmp'; }
}


config const smallN = 10;
config const bigN = 100_000;

proc checkComparisonSort(n: int) {
  var A:[1..n] int = 1..n by -1;

  sort(A, new AbsCompCmp(), 2..n/2);

  assert(isSorted(A[2..n/2]));
  assert(A[1] == n);
  assert(A[n] == 1);
  assert(isSorted(A[n/2+1..n], new ReverseComparator()));
}

proc checkKeySort(n: int) {
  var A:[1..n] int = 1..n by -1;

  sort(A, new AbsKeyCmp(), 2..n/2);

  assert(isSorted(A[2..n/2]));
  assert(A[1] == n);
  assert(A[n] == 1);
  assert(isSorted(A[n/2+1..n], new ReverseComparator()));
}

checkComparisonSort(smallN);
checkKeySort(smallN);

checkComparisonSort(bigN);
checkKeySort(bigN);

writeln("OK");
