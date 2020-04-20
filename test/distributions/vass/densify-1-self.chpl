// I used to have a separate densifySelf implementation.
// This test was written for it. Now we fake it.
proc densifySelf(arg) {
  const result = densify(arg,arg);
  testUndensify(arg,arg,result);
  return result;
}

proc testUndensify(whole, r, dense) {
  const undense = unDensify(dense, whole);
  if undense != r then
    writeln("testUndensify() error: ", (whole, r, dense, undense));
}

proc t2(arg) {
  const result = densifySelf(arg);
  proc shouldbe(r) return 0:r.size.type..#r.size;
  writeln("densifySelf ", arg, " -> ", result,
          " : ", result.type:string);

  if isDomain(arg) {
    const doublecheck = computeZeroBasedDomain(arg);
    if result != doublecheck then
      writeln("  ERROR on doublecheck");
  } else if isTuple(arg) {
    for param i in 0..arg.size-1 do
      if result(i) != shouldbe(arg(i)) then
        writeln("  ERROR on tuple/", i);
  } else if isRange(arg) {
    if result != shouldbe(arg) then
      writeln("  ERROR on range");
  } else {
    writeln("  result NOT CHECKED");
  }    
}

t2(1:uint(64)..0:uint(64));
t2(1:uint(32)..0:uint(32));
t2(1..1);
t2(1..1 by 3);
t2(1..1 by -1);
t2(1..1 by -3);
t2(1..5);
t2(1..8 by 3);
t2(1..5 by -1);
t2(1..8 by -3);
t2(1..8 by 3 align 2);
t2(1..8 by -3 align 1);
t2((2..5, 3..9));
t2((2..5 by 1, 3..9 by 3));
t2({2..5 by 1, 3..9 by 3});
t2({2:int(64)..5 by 1, 3:int(64)..9 by 3});
