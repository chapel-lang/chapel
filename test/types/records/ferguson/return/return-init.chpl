use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}

proc myfunction() {

  var rec = make(1);
  
  rec.verify();
  assert(rec.x == 1);
}

myfunction();

verify();

