
proc test_begin() {

  var A:[1..10] int;
  sync {
    begin with (ref A) {
      A[1] = 1;
    }
  }

  writeln(A[1]);
}


proc test_begin_on() {

  var A:[1..10] int;
  sync {
    begin with (ref A) on Locales[numLocales-1] {
      A[1] = 1;
    }
  }

  writeln(A[1]);
}

proc test_cobegin() {

  var A:[1..10] int;
  cobegin with (ref A) {
    A[1] = 1;
    A[2] = 2;
  }

  writeln(A[1], " ", A[2]);
}

proc test_cobegin_on() {

  var A:[1..10] int;
  cobegin with (ref A) {
    on Locales[numLocales-1] do A[1] = 1;
    on Locales[numLocales-1] do A[2] = 2;
  }

  writeln(A[1], " ", A[2]);
}


proc test_coforall() {

  var A:[1..10] int;
  coforall i in 1..10 with (ref A) {
    A[i] = i;
  }

  writeln(A[1], " ", A[2], " ", A[3]);
}

proc test_coforall_on() {

  var A:[1..10] int;
  coforall i in 1..10 with (ref A) {
    on Locales[numLocales-1] do A[i] = i;
  }

  writeln(A[1], " ", A[2], " ", A[3]);
}




test_begin();
test_begin_on();
test_cobegin();
test_cobegin_on();
test_coforall();
test_coforall_on();


