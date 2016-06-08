use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}

proc myfunction(opt:bool) {
  if opt {
    return make(1);
  }
  var y = make(2);
  return y;
}

proc test() {
  var one = myfunction(true);
  var two = myfunction(true);
  assert(one.x == 1);
  one.verify();
  assert(two.x == 1);
  two.verify();
}

test();

verify();

