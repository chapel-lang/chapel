proc foo(paths: string ...?n) : string {
  var ret : string;

  for p in paths do writeln(p);

  return ret;
}

record myRec {
  var x: int;
}

proc bar(paths: myRec ...?n) {
  ref refToArg = paths;

  for p in refToArg do writeln(p);

  return refToArg;
}

proc main() {
  // some dummy strings built up to emphasize this isn't an issue with string
  // literals
  var x, y : string;
  for i in 0..9 do x += i:string;
  for i in 0..9 by -1 do y += i:string;

  foo(x, y);

  var r1, r2: myRec;
  r1 = new myRec(x=10);
  r2 = new myRec(x=20);

  var (ret1, ret2) = bar(r1, r2);

  writeln("After return");
  writeln(ret1, " ", ret2);
  writeln(r1, " ", r2);

  r1.x *= -1;

  writeln("After multiply");
  writeln(ret1, " ", ret2);
  writeln(r1, " ", r2);
}
