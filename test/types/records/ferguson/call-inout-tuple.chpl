use myrecord;

proc foo(inout rec: (R,R)) {
  rec(1).verify();
  rec(1).increment();
  rec(1).verify();
  rec(2).verify();
  rec(2).increment();
  rec(2).verify();

}

proc myfunction() {

  var myvar: R;
  myvar.init(x = 20);
  myvar.verify();
  assert(myvar.x == 20);

  var myvar2: R;
  myvar2.init(x = 40);
  myvar2.verify();
  assert(myvar.x == 40);


  foo((myvar,myvar2));

  myvar.verify();
  assert(myvar.x == 21);
  myvar2.verify();
  assert(myvar2.x == 41);
}

myfunction();

verify();
