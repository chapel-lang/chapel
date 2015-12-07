use myrecord;

proc foo(ref rec: (R,R)) {
  rec(1).verify();
  rec(1).increment();
  rec(1).verify();
  rec(2).verify();
  rec(2).increment();
  rec(2).verify();

}

proc myfunction() {

  var mytup: (R,R);
  mytup(1).init(x = 20);
  mytup(1).verify();
  assert(mytup(1).x == 20);

  mytup(2).init(x = 40);
  mytup(2).verify();
  assert(mytup(2).x == 40);

  foo(mytup);

  mytup(1).verify();
  assert(mytup(1).x == 21);
  mytup(2).verify();
  assert(mytup(2).x == 41);
}

myfunction();

verify();
