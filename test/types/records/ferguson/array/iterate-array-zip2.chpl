use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}


proc run() {

  var A = for i in 1..4 do make(i);

  iter myiter() : R {
    for r in A do yield r;
  }


  for (i,r) in zip(1..4, myiter()) {
    assert(r.x == i);
  }

}

run();
verify();
