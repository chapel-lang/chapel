use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}

iter myiter() {
  for i in 1..4 {
    yield make(i);
  }
}

proc run() {
  var i = 1;
  for r in myiter() {
    assert(r.x == i);
    if i == 2 then break;
    i += 1;
  }
}

run();
verify();
