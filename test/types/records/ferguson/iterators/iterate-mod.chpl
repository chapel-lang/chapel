use myrecord;

config const verbose = false;

iter myiter() ref :R {
  var A:[1..4] R;

  for i in 1..4 {
    A[i].setup(x = i);
    A[i].verify();
    if verbose then writeln(A[i]);
  }

  for i in 1..4 {
    if verbose then writeln(A[i]);
    yield A[i];
  }
}


proc run() {
  var tot: int;
  for r in myiter() {
    tot += r.x;
    var tmp: R;
    r = tmp;
    if verbose then writeln(r);
  }
  writeln(tot);
}

run();
verify();
