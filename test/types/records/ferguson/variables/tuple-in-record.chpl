use myrecord;

record OtherRecord {
  var a = (new R(1), new R(2));
}

proc run() {
  var myR = new OtherRecord();
  myR.a[1].verify();
  myR.a[2].verify();
  assert(myR.a[1].x == 1);
  assert(myR.a[2].x == 2);
}

run();
verify();

