use myrecord;

proc make(i:int) {
  var ret: R;
  ret.init(x = i);
  ret.verify();
  return ret;
}

proc myfunction() {

  make(1).verify();
}

myfunction();

