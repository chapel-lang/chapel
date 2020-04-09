// Verify that 'in' intent results in task-private shadow variables,
// not iteration-private.
// Ensure that 'const' things can be passed by 'in' intent
// and modified inside the loop.

iter myiter() {
  yield 555;
}

iter myiter(param tag) {
  coforall i in 1..5 {
    // ensure serial execution in the presence of task constructs
    if (i == 3) {
      for j in 1..3 do
        yield j;
    }
  }
}

proc main {
  const myarr = [101, 102];
  const myvar = 103;
  forall jjj in myiter() with (in myarr, in myvar) {
    myvar += 100;
    myarr += 200;
    writef("%t:     %t  %t\n", jjj, myvar, myarr);
  }
  writef("done:  %t  %t\n", myvar, myarr);
}
