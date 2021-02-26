class C {
  var x: int;
}

proc makeSharedTuple() {
  return (new shared C(1), new shared C(2));
}
proc returnSharedTupleOut(out arg) {
  arg = makeSharedTuple();
}
proc test1() {
  writeln("test1");
  var t: (shared C, shared C);
  returnSharedTupleOut(t);
  writeln(t);
}
test1();

pragma "unsafe"
proc makeOwnedTuple() {
  // it should just be
  //  return (new owned C(1), new owned C(2));
  // but that does not work today (see issue #14942)
  // the below is a workaround so that this test
  // can check split-init and out behavior.
  pragma "no auto destroy"
  var a = new owned C(1);
  pragma "no auto destroy"
  var b = new owned C(2);
  pragma "no init"
  pragma "no auto destroy"
  var t: (owned C, owned C);
  __primitive("=", t(0), a);
  __primitive("=", t(1), b);
  return t;
}
proc returnOwnedTupleOut(out arg) {
  arg = makeOwnedTuple();
}
proc test2() {
  writeln("test2");
  var t: (owned C, owned C);
  returnOwnedTupleOut(t);
  writeln(t);
}
test2();
