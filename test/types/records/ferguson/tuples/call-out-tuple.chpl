use myrecord;

proc foo(out rec: (R,R) ) {
  rec(0).setup(x = 20);
  rec(0).verify();
  assert(rec(0).x == 20);

  rec(1).setup(x = 40);
  rec(1).verify();
  assert(rec(1).x == 40);
}

proc myfunction() {
  var mytup: (R,R);

  foo(mytup);

  mytup(0).verify();
  assert(mytup(0).x == 20);
  mytup(1).verify();
  assert(mytup(1).x == 40);
}

myfunction();

verify();
