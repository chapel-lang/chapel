use myrecord;

record OtherRecord {
  var a = [new R(1), new R(2)];
}

proc run() {

  var r:R = new R(1);

  proc inner() {
    assert(r.x == 1);
    r.verify();
  }

  assert(r.x == 1);
  r.verify();
}

run();
verify();

