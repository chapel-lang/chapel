use myrecord;

proc make(i:int) {
  var ret: R;
  ret.setup(x = i);
  ret.verify();
  return ret;
}

iter myiter() : R {
  yield make(1);
}


proc run() {
  var i = 1;
  for r in myiter() {
    assert(r.x == i);
    i += 1;
  }
}

run();
verify();
