// This test locks in the relative priority
// of argument matches vs where clauses when
// selecting best candidates. It is in particular
// focused on constructs that create arguments
// that are neither fully generic nor fully concrete.

var A:[1..100] int;

proc t0(A:[]) {
  writeln("OK t0(A:[])");
}
proc t0(A) where true {
  writeln("t0(A)");
}

t0(A);

proc t1(A:[] int) {
  writeln("OK t1(A:[] int)");
}
proc t1(A:[]) where true {
  writeln("t1(A:[]");
}

t1(A);

record R {
  var x;
  var y;
}

proc t2(r:R(int, ?)) {
  writeln("OK t2(r:R(int, ?)");
}
proc t2(r:R) where true {
  writeln("t2(r:R)");
}

var r:R(int, real);
t2(r);

proc t3(r:R(int, ?)) {
  writeln("OK t3(r:R(int, ?)");
}
proc t3(r) where true {
  writeln("t3(r)");
}

t3(r);

proc t4(r:R) {
  writeln("OK t4(r:R)");
}
proc t4(r) where true {
  writeln("t4(r)");
}

t4(r);

proc t5(r:R(int, real)) {
  writeln("OK t5(r:R(int, real))");
}
proc t5(r:R(int, ?)) where true {
  writeln("t5(r:R(int, ?))");
}

t5(r);


// tuples

var t = (10, 0.10);

proc t6(t:(int, ?)) {
  writeln("OK t6(t:(int, ?))");
}
proc t6(t) where true {
  writeln("t6(t:(?, ?))");
}

t6(t);

proc t7(t:(int, real)) {
  writeln("OK t7(t:(int, real))");
}
proc t7(t:(int, ?)) where true {
  writeln("t7(t:(int, ?))");
}

t7(t);


proc t8(t:(int, ?)) {
  writeln("OK t8(t:(int, ?))");
}
proc t8(t:_tuple) where true {
  writeln("t8(t:_tuple)");
}

t8(t);

proc t9(t:(?, ?)) {
  writeln("OK t9(t:(?, ?))");
}
proc t9(t:_tuple) where true {
  writeln("t9(t:_tuple)");
}

t9(t);
