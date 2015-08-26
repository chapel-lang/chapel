use myrecord;

config const verbose = false;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}

iter myiter() : R {
  for i in 1..4 {
    var ret: R;
    ret.init(x = i);
    ret.verify();
    yield ret;
  }
}


proc run() {
  for r in myiter() {
    if verbose then writeln(r);
  }
}

run();
verify();
