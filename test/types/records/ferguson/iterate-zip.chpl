use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}


var A = for i in 1..4 do make(i);

proc myiter() {
  for r in A do yield r;
}


for (i,r) in zip(1..4, myiter()) {
  assert(r.x == i);
}

verify();
