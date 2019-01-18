use myrecord;

proc foo(ref rec: R) {
  rec.verify();
  rec.increment();
  rec.verify();
}

proc myfunction() {

  var myvar: R;
  myvar.setup(x = 20);

  myvar.verify();
  assert(myvar.x == 20);

  foo(myvar);

  myvar.verify();
  assert(myvar.x == 21);
}

myfunction();

verify();
