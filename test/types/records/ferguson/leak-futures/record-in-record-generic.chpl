use myrecord;

record OtherRecord {
  var r;
}

proc run() {
  var r = new R(1);
  var myR = new OtherRecord(r);
  myR.r.verify();
  assert(myR.r.x == 1);
}

run();
verify();

