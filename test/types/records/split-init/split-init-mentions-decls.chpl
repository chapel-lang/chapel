proc test1() {
  var t1: int;
  t1Inner();
  t1 = 1;
  proc t1Inner() {
    writeln(t1);
  }
}
test1();

proc test2() {
  var t2: int;
  proc t2Inner() {
    writeln(t2);
  }
  t2Inner();
  t2 = 2;
}
test2();

var t3: int;
t3Function();
t3 = 3;

proc t3Function() {
  writeln(t3);
}


var t4: int;
proc t4Function() {
  writeln(t4);
}
t4Function();
t4 = 4;

var t5: int;
class t5C {
  var field = t5;
}
var t5object = new t5C();
t5 = 5;

var t6: int;
var t6object = new t6C();
t6 = 6;
class t6C {
  var field = t6;
}

var t7: int;
record t7r {
  var field = t7;
}
var t7rec = new t7r();
t7 = 7;

var t8: int;
var t8rec = new t8r();
writeln(t8rec.field);
t8 = 8;
record t8r {
  var field = t8;
}
