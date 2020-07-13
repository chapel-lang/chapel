record r { var x: int = 0; }

proc modifyConstElemViaDefaultIntentArg(tup: (int, r)) {
  tup[1].x = 256;
}

proc test1() {
  const e2 = new r(0);
  writeln(e2);

  // If the default intent for record elements is `ref-if-modified' then this
  // should fail at the callsite. If it is `const ref`, then the assignment
  // in the body should fail.
  modifyConstElemViaDefaultIntentArg((128, e2));
  writeln(e2);
  return;


}
test1();

proc test2() {
  const e2 = new r(0);
  writeln(e2);

  // This one is a bit more contrived. I'm not sure if we have a way of
  // propagating constness to tuple elements. I think in the worst case
  // we may have to generate a new tuple type when an element is const?
  (128, e2)[1].x = 512;

  writeln(e2);
  return;
}
test2();

