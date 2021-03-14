use myrecord;

proc foo(in rec: (R,R)) {
  rec(0).verify();
  rec(0).increment();
  rec(0).verify();
  rec(1).verify();
  rec(1).increment();
  rec(1).verify();

}

proc myfunction() {

  var myvar: R;
  myvar.setup(x = 20);
  myvar.verify();
  assert(myvar.x == 20);

  var myvar2: R;
  myvar2.setup(x = 40);
  myvar2.verify();
  assert(myvar2.x == 40);


  foo((myvar,myvar2));

  myvar.verify();
  assert(myvar.x == 20);
  myvar2.verify();
  assert(myvar2.x == 40);
}

myfunction();

verify();
