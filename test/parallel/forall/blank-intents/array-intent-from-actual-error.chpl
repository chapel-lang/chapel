// error cases from array-intent-from-actual.chpl
proc explicitConst1(const A) {
  forall i in A.domain /*with (const A)*/ do A[i] = i;
}
proc explicitConstBracketLoop1(const A) {
  [i in A.domain /*with (const A)*/] A[i] = i;
}

{
  var A: [1..10] int;
  explicitConst1(A);
  explicitConstBracketLoop1(A);
}

{
  const A: [1..10] int;
  forall i in 1..10 /*with (const A)*/ do A[i] = i; // error
  [i in 1..10 /*with (const A)*/] A[i] = i; // error
}
