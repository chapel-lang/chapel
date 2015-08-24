use myrecord;

proc foo(out rec: R) {
  rec.init(x = 20);
  rec.verify();
  assert(rec.x == 20);
}

proc myfunction() {
  var myvar: R;

  foo(myvar);

  myvar.verify();
  assert(myvar.x == 20);
}

myfunction();

verify();
