use Map;

config const lo = 1;
config const hi = 8;

enum LockMode { neither, left, right, both }

proc addToMaps(ref a, ref b) {
  for i in lo..hi {
    if !(i % 2) {
      a.add(i, 0);
    } else {
      b.add(i, 0);
    }
  }
}

proc populateMaps(param mode: LockMode) where mode == LockMode.left {
  var lhs: map(int, int, parSafe=true);
  var rhs: map(int, int);
  addToMaps(lhs, rhs);
  return (lhs, rhs);
}

proc populateMaps(param mode: LockMode) where mode == LockMode.right {
  var lhs: map(int, int);
  var rhs: map(int, int, parSafe=true);
  addToMaps(lhs, rhs);
  return (lhs, rhs);
}

proc populateMaps(param mode: LockMode) where mode == LockMode.both {
  var lhs, rhs: map(int, int, parSafe=true);
  addToMaps(lhs, rhs);
  return (lhs, rhs);
}

proc populateMaps(param mode: LockMode) where mode == LockMode.neither {
  var lhs, rhs: map(int, int);
  addToMaps(lhs, rhs);
  return (lhs, rhs);
}

proc display(op, m) {
  param space = ' ';
  write(op, space, ':', space);
  for i in lo..hi do
    if m.contains(i) then
      write(i, space);
  writeln();
}

proc doTest(param mode, op) {
  var (a, b) = populateMaps(mode);
  const opEqual = op + '=';
  select op {
    when '|' { var c = a | b; display(op, c); a |= b; display(opEqual, a); }
    when '+' { var c = a + b; display(op, c); a += b; display(opEqual, a); }
    when '-' { var c = a - b; display(op, c); a -= b; display(opEqual, a); }
    when '&' { var c = a & b; display(op, c); a &= b; display(opEqual, a); }
    when '^' { var c = a ^ b; display(op, c); a ^= b; display(opEqual, a); }
    otherwise { halt('Unrecognized op: ' + op:string); }
  }
}

proc testMapCompositionOps(param mode) {
  writeln('Which map is \'parSafe=true\': ', mode);
  doTest(mode, '|');
  doTest(mode, '+');
  doTest(mode, '-');
  doTest(mode, '&');
  doTest(mode, '^');
}

proc test1() {
  writeln('T1');
  testMapCompositionOps(LockMode.neither);
  writeln();
}
test1();

proc test2() {
  writeln('T2');
  testMapCompositionOps(LockMode.left);
  writeln();
}
test2();

proc test3() {
  writeln('T3');
  testMapCompositionOps(LockMode.right);
  writeln();
}
test3();

proc test4() {
  writeln('T4');
  testMapCompositionOps(LockMode.both);
  writeln();
}
test4();

