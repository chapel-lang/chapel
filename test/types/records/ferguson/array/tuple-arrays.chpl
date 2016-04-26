use myrecord;

config const N=4;

proc mkarray() {
  var a = [new R(1), new R(2)];
  var b = [new R(3), new R(4)];

  return (a, b);
}


proc run() {
  var tup = mkarray();
  tup(1).verify();
  tup(2).verify();
  assert(tup(1)[1].x == 1);
  assert(tup(1)[2].x == 2);
  assert(tup(2)[1].x == 3);
  assert(tup(2)[2].x == 4);
}

run();
verify();

