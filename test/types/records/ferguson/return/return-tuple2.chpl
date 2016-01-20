use myrecord;

proc make(i:int, j:int) : (R,R) {
  var r1: R;
  r1.init(x = i);
  r1.verify();
  var r2: R;
  r2.init(x = i);
  r2.verify();
  return (r1,r2);
}

proc myfunction() {
  var x = make(1,2);
  x(1).verify();
  x(2).verify();
}

myfunction();

verify();

