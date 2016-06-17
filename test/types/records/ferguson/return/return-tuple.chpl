use myrecord;

proc make(i:int, j:int) {
  var r1: R;
  r1.setup(x = i);
  r1.verify();
  var r2: R;
  r2.setup(x = i);
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

