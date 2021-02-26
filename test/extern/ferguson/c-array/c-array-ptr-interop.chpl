use CPtr;
use SysCTypes;

extern type mytriple = c_ptr(c_int); // a lie we intentionally support

extern var myglobal: mytriple;

extern proc setpair(arg: mytriple);

proc test1() {
  writeln("test1");
  writeln(myglobal[0]);
  writeln(myglobal[1]);
  writeln(myglobal[2]);
}
test1();

proc test2() {
  writeln("test2");
  myglobal[0] = 50;
  myglobal[1] = 51;
  myglobal[2] = 52;
  writeln(myglobal[0]);
  writeln(myglobal[1]);
  writeln(myglobal[2]);
}
test2();

proc test3() {
  writeln("test3");
  setpair(myglobal);
  writeln(myglobal[0]);
  writeln(myglobal[1]);
  writeln(myglobal[2]);
}
test3();

proc test4() {
  writeln("test4");
  var loc: mytriple;
  writeln(loc);
}
test4();
