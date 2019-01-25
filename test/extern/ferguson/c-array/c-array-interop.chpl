extern type mypair = c_array(c_int, 2);

extern var myglobal: mypair;

extern proc setpair(arg: mypair);

extern record mycomplex {
  var dat: c_array(real(64), 2);
}

extern proc returncomplex():mycomplex;

proc test1() {
  writeln("test1");
  writeln(myglobal[0]);
  writeln(myglobal[1]);
}
test1();

proc test2() {
  writeln("test2");
  myglobal[0] = 50;
  myglobal[1] = 51;
  writeln(myglobal[0]);
  writeln(myglobal[1]);
}
test2();

proc test3() {
  writeln("test3");
  setpair(myglobal);
  writeln(myglobal[0]);
  writeln(myglobal[1]);
}
test3();

proc test4() {
  writeln("test4");
  var loc: mypair;
  writeln(loc);
}
test4();

proc test5() {
  writeln("test5");
  var loc: mypair;
  setpair(loc);
  writeln(loc);
}
test5();

proc test6() {
  writeln("test6");
  var loc: mypair;
  setpair(loc);
  var x = loc;
  writeln(x);
}
test6();

proc test7() {
  writeln("test7");
  var loc: mypair;
  setpair(loc);
  var x: mypair;
  x = loc;
  writeln(x);
}
test7();
