use myrecord;

proc foo(ref rec: (R,R)) {
  rec(0).verify();
  rec(0).increment();
  rec(0).verify();
  rec(1).verify();
  rec(1).increment();
  rec(1).verify();

}

proc myfunction() {

  var mytup: (R,R);
  mytup(0).setup(x = 20);
  mytup(0).verify();
  assert(mytup(0).x == 20);

  mytup(1).setup(x = 40);
  mytup(1).verify();
  assert(mytup(1).x == 40);

  foo(mytup);

  mytup(0).verify();
  assert(mytup(0).x == 21);
  mytup(1).verify();
  assert(mytup(1).x == 41);
}

myfunction();

verify();
