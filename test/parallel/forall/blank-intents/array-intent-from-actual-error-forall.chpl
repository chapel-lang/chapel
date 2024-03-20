proc doNothing() {}

// some error cases from array-intent-from-actual.chpl
proc explicitConst1(const A) {
  // all the following statements have an implicit `with (const A)`
  forall i in A.domain do A[i] = i;
  [i in A.domain] A[i] = i;
  // coforall, begin, and cobegin are tested in array-intent-from-actual-error-task.chpl
}

{
  var A: [1..10] int;
  explicitConst1(A);
}

{
  // all the following statements have an implicit `with (const A)`
  const A: [1..10] int;
  forall i in A.domain do A[i] = i;
  [i in A.domain] A[i] = i;
  // coforall, begin, and cobegin are tested in array-intent-from-actual-error-task.chpl
}
