proc doNothing() {}

// some error cases from array-intent-from-actual.chpl
proc explicitConst1(const A) {
  // all the following statements have an implicit `with (const A)`
  // forall and [] are tested in array-intent-from-actual-error-forall.chpl
  coforall i in A.domain do A[i] = i;
  sync begin A = A.domain;
  sync cobegin {
    A = A.domain;
    doNothing();
  }
}

{
  var A: [1..10] int;
  explicitConst1(A);
}

{
  // all the following statements have an implicit `with (const A)`
  const A: [1..10] int;
  // forall and [] are tested in array-intent-from-actual-error-forall.chpl
  coforall i in A.domain do A[i] = i;
  sync begin A = A.domain;
  sync cobegin {
    A = A.domain;
    doNothing();
  }
}
