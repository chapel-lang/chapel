use myrecord;

record OtherRecord {
  var a = [i in 1..2] new R(i);
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

