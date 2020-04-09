use myrecord;

record OtherRecord {
  var r = new R(1);
}

proc run() {
  var myR = new OtherRecord();
  myR.r.verify();
  assert(myR.r.x == 1);
}

run();
verify();

