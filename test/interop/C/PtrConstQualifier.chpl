use CTypes;

proc internalGenericProc(x) { x; }

proc internalProc(x: c_ptrConst(int(32))) {
  internalGenericProc(x);

  // Test #0
  var n1: int(32) = 8;
  var p1 = c_ptrToConst(n1);
  assert(p1 != x);
  assert(p1.deref() == x.deref());

  // Test #1
  const n2: int(32) = 8;
  var p2 = c_ptrToConst(n2);
  assert(p2 != x && p2.deref() == x.deref());
}

export proc printAndReturn(x: c_ptrConst(int(32))) {
  writeln(x.deref():string);
  internalProc(x);
  return x;
}
