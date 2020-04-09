record R {
  var x:int;
  proc init(x:int) {
    this.x = x;
    writeln("init ", x);
  }
  proc init=(r:R) {
    this.x = r.x;
    writeln("copy init ", x);
  }
  proc deinit() {
    writeln("deinit ", x);
  }
}

proc =(ref lhs:R, const ref rhs:R) {
  writeln("assign ", rhs.x);
  lhs.x = rhs.x;
}

proc makeR(x:int) {
  return new R(x);
}

proc addone_in_ret(in arg:R) {
  arg.x += 1;
  return arg;
}
proc addone_const_in_ret(const in arg:R) {
  var tmp = arg;
  tmp.x += 1;
  return tmp;
}

proc addone_in_noret(in arg:R) {
}

proc addone_const_in_noret(const in arg:R) {
}

proc test00() {
  writeln("test00 starting");
  var a = makeR(0);
  writeln("test00 ending");
}
test00();

proc test10() {
  writeln("test10 starting");
  var a:R = new R(10);
  var b = addone_in_ret(a);
  writeln(a);
  writeln(b);
  writeln("test10 ending");
}
test10();

proc test20() {
  writeln("test20 starting");
  var b = addone_in_ret(makeR(20));
  writeln(b);
  writeln("test20 ending");
}
test20();

proc test30() {
  writeln("test30 starting");
  var a:R = new R(30);
  var b = addone_const_in_ret(a);
  writeln(a);
  writeln(b);
  writeln("test30 ending");
}
test30();

proc test40() {
  writeln("test40 starting");
  var b = addone_const_in_ret(makeR(40));
  writeln(b);
  writeln("test40 ending");
}
test40();


proc test50() {
  writeln("test50 starting");
  var a:R = new R(50);
  addone_in_noret(a);
  writeln(a);
  writeln("test50 ending");
}
test50();

proc test60() {
  writeln("test60 starting");
  addone_in_noret(makeR(60));
  writeln("test60 ending");
}
test60();

proc test70() {
  writeln("test70 starting");
  var a:R = new R(70);
  addone_const_in_noret(a);
  writeln(a);
  writeln("test70 ending");
}
test70();

proc test80() {
  writeln("test80 starting");
  addone_const_in_noret(makeR(80));
  writeln("test80 ending");
}
test80();
