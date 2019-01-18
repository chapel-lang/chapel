use myrecord;

proc myfunction() {

  var t: (R,R);

  t(1).setup(x = 20);
  t(1).verify();
  assert(t(1).x == 20);

  t(2).setup(x = 40);
  t(2).verify();
  assert(t(2).x == 40);
}

myfunction();

verify();
