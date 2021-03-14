use myrecord;

config const N=4;

proc mkarray() {
  var a = [new R(1), new R(2)];
  var b = [new R(3), new R(4)];

  return (a, b);
}


proc run() {
  var (a, b) = mkarray();
  a.verify();
  b.verify();
  assert(a[0].x == 1);
  assert(a[1].x == 2);
  assert(b[0].x == 3);
  assert(b[1].x == 4);
}

run();
verify();

