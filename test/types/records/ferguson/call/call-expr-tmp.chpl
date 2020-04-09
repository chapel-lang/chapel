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
}

proc myfunction() {
  foo(make(1));
}

myfunction();

verify();
