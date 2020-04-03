use myrecord;

config const N=4;

proc mkarray() {
  var a = [new R(1), new R(2)];
  var b = [new R(3), new R(4)];

  return (a, b);
}


proc run() {
  var tup = mkarray();
  tup(0).verify();
  tup(1).verify();
  assert(tup(0)[0].x == 1);
  assert(tup(0)[1].x == 2);
  assert(tup(1)[0].x == 3);
  assert(tup(1)[1].x == 4);
}

run();
verify();

