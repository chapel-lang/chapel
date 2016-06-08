use myrecord;

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
    writeln(r.x);
  }
}

run();
verify();
