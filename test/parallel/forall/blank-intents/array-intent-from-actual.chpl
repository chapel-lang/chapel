proc doNothing() {}

proc explicitRef1(ref A) {
  // all the following statements have an implicit `with (ref A)`
  forall i in A.domain do A[i] = i;
  [i in A.domain] A[i] = i;
  coforall i in A.domain do A[i] = i;
  sync begin A = A.domain;
  sync cobegin {
    A = A.domain;
    doNothing();
  }
}
proc explicitRef2(ref A) {
  // all the following statements have an implicit `with (ref A)`
  forall i in A.domain do writeln(A[i]);
  [i in A.domain] writeln(A[i]);
  coforall i in A.domain do writeln(A[i]);
  sync begin writeln(A);
  sync cobegin {
    writeln(A);
    doNothing();
  }
}


// modifying an explicit const is tested in another file
// array-intent-from-actual-error-[forall|task].chpl
// proc explicitConst2(const A) {
//   ...
// }

proc explicitConst2(const A) {
  // all the following statements have an implicit `with (const A)`
  forall i in A.domain do writeln(A[i]);
  [i in A.domain] writeln(A[i]);
  coforall i in A.domain do writeln(A[i]);
  sync begin writeln(A);
  sync cobegin {
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
  // all the following statements have an implicit `with (const A)`
  forall i in A.domain do A[i] = i;
  [i in A.domain] A[i] = i;
  coforall i in A.domain do A[i] = i;
  sync begin A = A.domain;
  sync cobegin {
    A = A.domain;
    doNothing();
  }
}
// no matter what the user does, this will always work
proc blankIntent2(A) {
  // all the following statements have an implicit `with (const A)`
  forall i in A.domain do writeln(A[i]);
  [i in A.domain] writeln(A[i]);
  coforall i in A.domain do writeln(A[i]);
  sync begin writeln(A);
  sync cobegin {
    writeln(A);
    doNothing();
  }
}



// this is another case that will warn due to the ref-maybe-const formal, and
// will do the right thing if a user tries to address the warning
proc update(A, i) do A[i] = i;
proc callFunction(A) {
  // all the following statements have an implicit `with (const A)`
  forall i in A.domain do update(A, i);
  [i in A.domain] update(A, i);
  [i in A.domain] update(A, i);
  coforall i in A.domain do update(A, i);
  sync begin update(A, 1);
  sync cobegin {
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
  // all the following statements have an implicit `with (ref A)`
  var A: [1..10] int;
  forall i in 1..10 do A[i] = i;
  [i in 1..10] A[i] = i;
  coforall i in A.domain do A[i] = i;
  sync begin A = A.domain;
  sync cobegin {
    A = A.domain;
    doNothing();
  }
}

