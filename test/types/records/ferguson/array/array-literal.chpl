use myrecord;

proc run() {
  var a = [new R(1)];
  a[1].verify();
  assert(a[1].x == 1);
}

run();
verify();

