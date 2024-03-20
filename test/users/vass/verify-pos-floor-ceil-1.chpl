// Testing divCeilPos() and divFloorPos()
use Math;

var testcount = 0, errcount = 0;

proc test(type T, m: T, n: T) {
  testcount += 1;
  // actual
  const rc = divCeilPos(m,n);
  const rf = divFloorPos(m,n);
  writeln(T:string, "  ", m, ",", n, "  ceil ", rc, "  floor ", rf);
  // expected
  const floor = m/n;
  const ceil = floor + if floor * n == m then 0:T else 1:T;
  // verify
  if rc != ceil {
    errcount += 1;
    writeln(" *** error in divCeilPos:  expected ", ceil, "  got ", rc);
  }
  if rf != floor {
    errcount += 1;
    writeln(" *** error in divFloorPos:  expected ", floor, "  got ", rf);
  }
}

// produce a bunch of numbers for use in testing
iter numbers() {
  yield 1; yield 2;  yield 3;  yield 4;
  yield 5;  yield 7;  yield 11;
  yield 64;  yield 1023;
  yield 16384; yield 32767;
}

proc tests(type TT) {
  const mm = max(TT);

  test(TT, mm, mm);
  test(TT, mm-1, mm);
  test(TT, mm, mm-1);
  test(TT, mm-1, mm-1);
  test(TT, mm, 1);
// divCeilPos() and divFloorPos() do not handle 0
//test(TT, 0:TT, mm);
  for d in numbers() do
    test(TT, mm, d:TT);
  for d in numbers() do
    test(TT, d:TT, mm);
  writeln();
}

tests(int(64));
tests(int(32));
// divCeilPos() and divFloorPos() handle signed ints only
//tests(uint(64));
//tests(uint(32));

//writeln(max(uint(64)));
//writeln(max(int(64)));
//writeln(max(uint(32)));
//writeln(max(int(32)));

writeln("done  ", testcount, " tests");
if errcount != 0 then
  writeln(errcount, " ERRORS");
