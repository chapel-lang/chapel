use myrecord;

record OtherRecord {
  var a = [new R(1), new R(2)];
}

proc run() {

  var r:R;

  proc inner() {
    r = new R(1);
    assert(r.x == 1);
    r.verify();
  }

  assert(r.x == 1);
  r.verify();
}

run();
verify();

