use myrecord;

proc make(i:int) {
  var ret: R;
  ret.setup(x = i);
  return ret;
}

proc myfunction() {

  var A:[1..10] R = [i in 1..10] make(i);

}

myfunction();

verify();

