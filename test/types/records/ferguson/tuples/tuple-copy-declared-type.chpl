use myrecord;

proc myfunction() {

  var t: (R,R);

  t(1).setup(x = 20);
  t(1).verify();
  assert(t(1).x == 20);

  t(2).setup(x = 40);
  t(2).verify();
  assert(t(2).x == 40);

  var t2: (R,R) = t;

  t2(1).verify();
  assert(t2(1).x == 20);
  t2(2).verify();
  assert(t2(2).x == 40);

}

myfunction();

verify();
