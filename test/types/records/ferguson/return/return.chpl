use myrecord;

proc make(i:int) {
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

