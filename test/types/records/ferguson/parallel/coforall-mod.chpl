use myrecord;

config const verbose = false;

iter myiter() ref :R {
  var A:[1..4] R;

  for i in 1..4 {
    A[i].init(x = i);
    A[i].verify();
    if verbose then writeln(A[i]);
  }

  for i in 1..4 {
    if verbose then writeln(A[i]);
    yield A[i];
  }
}


proc run() {
  var tot:atomic int;
  coforall r in myiter() {
    tot.add(r.x);
    var tmp: R;
    r = tmp;
    if verbose then writeln(r);
  }
  writeln(tot.read());
}

run();
verify();
