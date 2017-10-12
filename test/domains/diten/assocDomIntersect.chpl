config const nTrials = 100;

var D1: domain(int);
var D2: domain(int);

proc setupDoms(ref D1, ref D2) {
  use Random;
  D1.clear();
  D2.clear();
  var rs = makeRandomStream(real);
  for i in 1..1000 {
    var rnd1 = (rs.getNext()*10000):int;
    var rnd2 = (rs.getNext()*10000):int;
    if !D1.member(rnd1) then
      D1 += rnd1;
    if !D2.member(rnd2) then
      D2 += rnd2;
  }
  delete rs;
}

for i in 1..nTrials {
  setupDoms(D1, D2);
  var oldSize = D1.size;
  D1 &= D2;
  assert(oldSize >= D1.size);
}
