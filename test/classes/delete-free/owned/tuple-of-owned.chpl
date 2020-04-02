module mymodule {

class C {
  var i: int;
}

proc acceptsv(args...) {
  writeln( (...args) );
}

proc test1() {
  var a = new owned C(1);
  var b = new owned C(1);

  writeln("test1");
  acceptsv(a, b);
  acceptsv(a, b);
}
test1();

proc acceptst(args) {
  writeln(args);
}

proc test2() {
  var a = new owned C(1);
  var b = new owned C(1);

  writeln("test2");
  acceptst( (a, b) );
  acceptst( (a, b) );
}
test2();

proc acceptst_(args:_tuple) {
  writeln(args);
}

proc test2a() {
  var a = new owned C(1);
  var b = new owned C(1);

  writeln("test2a");
  acceptst_( (a, b) );
  acceptst_( (a, b) );
}
test2a();


proc acceptstt(args) {
  acceptst(args);
}

proc test3() {
  var a = new owned C(1);
  var b = new owned C(1);

  writeln("test3");
  acceptstt( (a, b) );
  acceptstt( (a, b) );
}
test3();

proc acceptst2(args: (owned C?, owned C?)) {
  acceptst(args);
}

proc test4() {
  var a: owned C? = new owned C(1);
  var b: owned C? = new owned C(1);

  writeln("test4");
  acceptst2( (a, b) );
  acceptst2( (a, b) );
}
test4();

proc acceptst5(args: (borrowed C, borrowed C)) {
  acceptst(args);
}

proc test5() {
  var a = new owned C(1);
  var b = new owned C(1);

  writeln("test5");
  acceptst5( (a, b) );
  acceptst5( (a, b) ); // should be nil by here
}
test5();

proc acceptsIn6(in t: (int, owned C?)) {
  return;
}

proc test6() {
  var t = (128, new owned C?(256));
  acceptsIn6(t);
  assert(t[2] == nil);
}
test6();

} // end module
