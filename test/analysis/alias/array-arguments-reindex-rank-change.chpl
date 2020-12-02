config const n = 10;

proc main() {
  var A: [1..n] real;
  var AA: [1..n, 1..n] real;

  const ref reindexA = A.reindex(0..n-1);
  var reindexVarA = reindexA;

  const ref rankChangeA = AA[1, ..];
  var rankChangeVarA = rankChangeA;

  testA1(A, reindexA); // aliases
  testA2(AA, rankChangeA); // aliases

  testN3(A, reindexVarA); // does not alias
  testN4(AA, reindexVarA); // does not alias
  testN5(AA, rankChangeVarA); // does not alias
  testN6(A, rankChangeVarA); // does not alias
}

proc testA1(a, b) { }
proc testA2(a, b) { }
proc testN3(a, b) { }
proc testN4(a, b) { }
proc testN5(a, b) { }
proc testN6(a, b) { }
