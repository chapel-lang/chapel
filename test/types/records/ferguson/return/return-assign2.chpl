use myrecord;

proc make(i:int) : R {
  var ret: R;
  ret.setup(x = i);
  ret.verify();
  return ret;
}

proc myfunction() {

  var rec:R;

  rec = make(1);
  
  rec.verify();
  assert(rec.x == 1);
}

myfunction();

verify();

