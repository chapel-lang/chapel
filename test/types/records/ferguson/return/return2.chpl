use myrecord;

proc make(i:int) : R {
  var ret: R;
  ret.setup(x = i);
  ret.verify();
  return ret;
}

proc myfunction() {

  var x = make(1);
  x.verify();
}

myfunction();

verify();

