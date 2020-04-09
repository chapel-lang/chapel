use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}

iter myiter() {
  var r:R;
  r = make(1);
  for i in 1..4 {
    var r2 = r;
    r2.x = i;
    yield r2;
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
