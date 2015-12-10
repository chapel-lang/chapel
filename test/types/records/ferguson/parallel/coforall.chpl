use myrecord;

config const verbose = false;

iter myiter() : R {
  for i in 1..4 {
    var ret: R;
    ret.init(x = i);
    ret.verify();
    yield ret;
  }
}


proc run() {
  var tot:atomic int;
  coforall r in myiter() {
    tot.add(r.x);
    if verbose then writeln(r);
  }
  writeln(tot.read());
}

run();
verify();
