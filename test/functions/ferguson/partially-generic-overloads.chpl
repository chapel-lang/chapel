proc t1(A:[]) where true {
  writeln("t1(A:[]");
}
proc t1(A:[] int) {
  writeln("t1(A:[] int)");
}

var A:[1..100] int;

t1(A);

record R {
  var x;
  var y;
}

proc t2(r:R(int, ?)) {
  writeln("t2(r:R(int, ?)");
}
proc t2(r:R) where true {
  writeln("t2(r:R)");
}

var r:R(int, real);
t2(r);

proc t3(r:R(int, ?)) {
  writeln("t3(r:R(int, ?)");
}
proc t3(r) where true {
  writeln("t3(r)");
}

t3(r);

proc t4(r:R) {
  writeln("t4(r:R)");
}
proc t4(r) where true {
  writeln("t4(r)");
}

t4(r);
