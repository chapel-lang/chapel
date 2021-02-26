use myrecord;

proc run() {
  var a = [new R(1)];
  a[0].verify();
  assert(a[0].x == 1);
}

run();
verify();

