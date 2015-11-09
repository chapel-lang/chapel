use myrecord;

proc myfunction() {

  var mytup: (R,R);

  mytup(1).init(x = 20);
  mytup(1).verify();
  assert(mytup(1).x == 20);

  mytup(2).init(x = 40);
  mytup(2).verify();
  assert(mytup(2).x == 40);
}

myfunction();

verify();
