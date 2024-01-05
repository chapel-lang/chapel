proc doNothing() {}

proc explicitRef1(ref A) {
  forall i in A.domain /*with (ref A)*/ do A[i] = i;
  [i in A.domain /*with (ref A)*/] A[i] = i;
  coforall i in A.domain /*with (ref A)*/ do A[i] = i;
  begin /*with (ref A)*/ A = A.domain;
  cobegin /*with (ref A)*/ {
    A = A.domain;
    doNothing();
  }
}
proc explicitRef2(ref A) {
  forall i in A.domain /*with (ref A)*/ do writeln(A[i]);
  [i in A.domain /*with (ref A)*/] writeln(A[i]);
  coforall i in A.domain /*with (ref A)*/ do writeln(A[i]);
  begin /*with (ref A)*/ writeln(A);
  cobegin /*with (ref A)*/ {
    writeln(A);
    doNothing();
  }
}


//
// this case produces an error, in a separate file
// proc explicitConst1(const A) {
//   forall i in A.domain /*with (const A)*/ do A[i] = i;
//   [i in A.domain /*with (const A)*/] A[i] = i;
//    coforall i in A.domain /*with (const A)*/ do A[i] = i;
//    begin /*with (const A)*/ A = A.domain;
//    cobegin /*with (const A)*/ {
//      A = A.domain;
//      doNothing();
//    }
// }
//
proc explicitConst2(const A) {
  forall i in A.domain /*with (const A)*/ do writeln(A[i]);
  [i in A.domain /*with (const A)*/] writeln(A[i]);
  coforall i in A.domain /*with (const A)*/ do writeln(A[i]);
  begin /*with (const A)*/ writeln(A);
  cobegin /*with (const A)*/ {
    writeln(A);
    doNothing();
  }
}


//
// in the deprecation period of ref-maybe-const formals, this will just be a
// deprecation warning on the formal, eventually this case will become an error
// if the user takes no action. If the user adds `ref A` to the function, it
// will never be an error. If the user adds `const A` to the function, it will
// become an error immeditaly and require the user to use `ref A` (or something
// else to make the array modifiable)
//
proc blankIntent1(A) {
  forall i in A.domain /*with (const A)*/ do A[i] = i;
  [i in A.domain /*with (const A)*/] A[i] = i;
  coforall i in A.domain /*with (const A)*/ do A[i] = i;
  begin /*with (const A)*/ A = A.domain;
  cobegin /*with (const A)*/ {
    A = A.domain;
    doNothing();
  }
}
// no matter what the user does, this will always work
proc blankIntent2(A) {
  forall i in A.domain /*with (const A)*/ do writeln(A[i]);
  [i in A.domain /*with (const A)*/] writeln(A[i]);
  coforall i in A.domain /*with (const A)*/ do writeln(A[i]);
  begin /*with (const A)*/ writeln(A);
  cobegin /*with (const A)*/ {
    writeln(A);
    doNothing();
  }
}



// this is another case that will warn due to the ref-maybe-const formal, and
// will do the right thing if a user tries to address the warning
proc update(A, i) do A[i] = i;
proc callFunction(A) {
  forall i in A.domain /*with (const A)*/ do update(A, i);
  [i in A.domain /*with (const A)*/] update(A, i);
  [i in A.domain /*with (const A)*/] update(A, i);
  coforall i in A.domain /*with (const A)*/ do update(A, i);
  begin /*with (const A)*/ update(A, 1);
  cobegin /*with (const A)*/ {
    update(A, 1);
    doNothing();
  }
}


//
// argument passing cases
//
{
  var A: [1..10] int;

  explicitRef1(A);
  explicitRef2(A);

  // explicitConst1(A);
  explicitConst2(A);

  blankIntent1(A);
  blankIntent2(A);

  callFunction(A);
}

//
// 'local' array cases
//
{
  var A: [1..10] int;
  forall i in 1..10 /*with (ref A)*/ do A[i] = i;
  [i in 1..10 /*with (ref A)*/] A[i] = i;
  coforall i in A.domain /*with (ref A)*/ do A[i] = i;
  begin /*with (ref A)*/ A = A.domain;
  cobegin /*with (ref A)*/ {
    A = A.domain;
    doNothing();
  }
}

