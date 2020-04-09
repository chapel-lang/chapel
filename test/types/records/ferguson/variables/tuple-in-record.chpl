use myrecord;

record OtherRecord {
  var a = (new R(1), new R(2));
}

proc run() {
  var myR = new OtherRecord();
  myR.a[0].verify();
  myR.a[1].verify();
  assert(myR.a[0].x == 1);
  assert(myR.a[1].x == 2);
}

run();
verify();

