use myrecord;

proc foo(out rec: (R,R) ) {
  rec(1).init(x = 20);
  rec(1).verify();
  assert(rec(1).x == 20);

  rec(2).init(x = 40);
  rec(2).verify();
  assert(rec(2).x == 40);
}

proc myfunction() {
  var mytup: (R,R);

  foo(mytup);

  mytup(1).verify();
  assert(mytup(1).x == 20);
  mytup(2).verify();
  assert(mytup(2).x == 40);
}

myfunction();

verify();
