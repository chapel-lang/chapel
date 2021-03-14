use myrecord;

proc run() {

  var r:R;

  proc inner() {
    r = new R(1);
    assert(r.x == 1);
    r.verify();
  }

  inner();
  assert(r.x == 1);
  r.verify();
}

run();
verify();

