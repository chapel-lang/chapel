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

proc makeOwnedTuple() {
  return (new owned C(1), new owned C(2));
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
