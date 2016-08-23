use myrecord;

proc myfunction() {

  var t: (R,R);

  t(1).setup(x = 20);
  t(1).verify();
  assert(t(1).x == 20);

  t(2).setup(x = 40);
  t(2).verify();
  assert(t(2).x == 40);

  var r1 = t(1);
  var r2 = t(2);

  r1.verify();
  assert(r1.x == 20);

  r2.verify();
  assert(r2.x == 40);
}

myfunction();

verify();
