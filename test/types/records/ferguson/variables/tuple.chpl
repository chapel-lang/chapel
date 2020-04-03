use myrecord;

proc myfunction() {

  var mytup: (R,R);

  mytup(0).setup(x = 20);
  mytup(0).verify();
  assert(mytup(0).x == 20);

  mytup(1).setup(x = 40);
  mytup(1).verify();
  assert(mytup(1).x == 40);
}

myfunction();

verify();
