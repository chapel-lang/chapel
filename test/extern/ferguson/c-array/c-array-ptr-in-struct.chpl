use CPtr;
extern record mycomplex {
  var dat: c_ptr(real(64)); // a lie we intentionally support
}

extern var myglobalcomplex:mycomplex;

extern proc setcomplex(arg:c_ptr(mycomplex));
extern proc returncomplex():mycomplex;

proc test1() {
  writeln("test1");
  writeln(myglobalcomplex.dat[0]);
  writeln(myglobalcomplex.dat[1]);
}
test1();

proc test2() {
  writeln("test2");
  myglobalcomplex.dat[0] = 50.0;
  myglobalcomplex.dat[1] = 51.0;
  writeln(myglobalcomplex.dat[0]);
  writeln(myglobalcomplex.dat[1]);
}
test2();

proc test3() {
  writeln("test3");
  setcomplex(c_ptrTo(myglobalcomplex));
  writeln(myglobalcomplex.dat[0]);
  writeln(myglobalcomplex.dat[1]);
}
test3();

proc test4() {
  writeln("test4");
  var loc: mycomplex;
  setcomplex(c_ptrTo(loc));
  writeln(loc.dat[0]);
  writeln(loc.dat[1]);
}
test4();

proc test5() {
  writeln("test5");
  var loc = returncomplex();
  writeln(loc.dat[0]);
  writeln(loc.dat[1]);
}
test5();

proc test6() {
  writeln("test6");
  var x = returncomplex();
  var y = x;
  writeln(y.dat[0]);
  writeln(y.dat[1]);
}
test6();

proc test7() {
  writeln("test7");
  var x = returncomplex();
  var z: mycomplex;
  z = x;
  writeln(z.dat[0]);
  writeln(z.dat[1]);
}
test7();
