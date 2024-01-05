proc doNothing() {}

// some error cases from array-intent-from-actual.chpl
proc explicitConst1(const A) {
  forall i in A.domain /*with (const A)*/ do A[i] = i;
  [i in A.domain /*with (const A)*/] A[i] = i;
  // coforall i in A.domain /*with (const A)*/ do A[i] = i;
  // begin /*with (const A)*/ A = A.domain;
  // cobegin /*with (const A)*/ {
  //   A = A.domain;
  //   doNothing();
  // }
}

{
  var A: [1..10] int;
  explicitConst1(A);
}

{
  const A: [1..10] int;
  forall i in A.domain /*with (const A)*/ do A[i] = i;
  [i in A.domain /*with (const A)*/] A[i] = i;
  // coforall i in A.domain /*with (const A)*/ do A[i] = i;
  // begin /*with (const A)*/ A = A.domain;
  // cobegin /*with (const A)*/ {
  //   A = A.domain;
  //   doNothing();
  // }
}
