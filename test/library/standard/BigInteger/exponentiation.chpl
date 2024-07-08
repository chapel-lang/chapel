use BigInteger;

var base = new bigint(-1);
var exp = new bigint(0);

for val in 0..2 {
  exp = val;
  testExp(base, exp);
}
writeln();

base = 1;
for val in 0..2 {
  exp = val;
  testExp(base, exp);
}
writeln();

for b in 5..#5 {
  for e in 10..#5 {
    base = b;
    exp = e;
    testExp(base, exp);
  }
  writeln();
}

for b in 5..#5 {
  for e in 10..#5 {
    base = b;
    exp = e;
    testExpAssign(base, exp);
  }
  writeln();
}

base = 2;
// test exponent that exceeds c_ulong capacity
exp = "99999999999999999999999":bigint;
testExp(base, exp);

proc testExp(const b: bigint, const e: bigint) {
  write(b ** e, " ");
}

proc testExpAssign(const b: bigint, const e: bigint) {
  var c: bigint = b;
  c **= e;
  write(c, " ");
}
