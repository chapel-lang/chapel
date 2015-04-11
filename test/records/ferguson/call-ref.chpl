use myrecord;

proc foo(ref rec: R) {
  rec.verify();
  rec.x += 1;
}

proc myfunction() {

  var myvar: R;
  myvar.init(x = 20);

  myvar.verify();
  assert(myvar.x == 20);

  foo(myvar);

  myvar.verify();
  assert(myvar.x == 21);
}

myfunction();

