use myrecord;

proc make(i:int) {
  var ret: R;
  ret.setup(x = i);
  ret.verify();
  return ret;
}

proc foo(rec: R) {
  rec.verify();
  rec.verify();
  return rec;
}

proc myfunction() {
  var rec:R;

  rec = foo(make(1));

  rec.verify();
}

myfunction();

verify();
