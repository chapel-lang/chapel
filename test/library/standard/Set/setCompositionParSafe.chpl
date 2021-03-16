use Set;

config const lo = 1;
config const hi = 8;

type setIntLocked = set(int, parSafe=true);
type setIntNoLock = set(int);

enum LockMode { neither, left, right, both }

proc addToSets(ref a, ref b) {
  for i in lo..hi {
    if !(i % 2) {
      a.add(i);
    } else {
      b.add(i);
    }
  }
}

proc populateSets(param mode: LockMode) where mode == LockMode.left {
  var lhs: set(int, parSafe=true);
  var rhs: set(int);
  addToSets(lhs, rhs);
  return (lhs, rhs);
}

proc populateSets(param mode: LockMode) where mode == LockMode.right {
  var lhs: set(int);
  var rhs: set(int, parSafe=true);
  addToSets(lhs, rhs);
  return (lhs, rhs);
}

proc populateSets(param mode: LockMode) where mode == LockMode.both {
  var lhs, rhs: set(int, parSafe=true);
  addToSets(lhs, rhs);
  return (lhs, rhs);
}

proc populateSets(param mode: LockMode) where mode == LockMode.neither {
  var lhs, rhs: set(int);
  addToSets(lhs, rhs);
  return (lhs, rhs);
}

proc display(op, s) {
  param space = ' ';
  write(op, space, ':', space);
  for i in lo..hi do
    if s.contains(i) then
      write(i, space);
  writeln();
}

proc doTest(param mode, op) {
  var (a, b) = populateSets(mode);
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

proc testSetCompositionOps(param mode) {
  writeln('Which set is \'parSafe=true\': ', mode);
  doTest(mode, '|');
  doTest(mode, '+');
  doTest(mode, '-');
  doTest(mode, '&');
  doTest(mode, '^');
}

proc test1() {
  writeln('T1');
  testSetCompositionOps(LockMode.neither);
  writeln();
}
test1();

proc test2() {
  writeln('T2');
  testSetCompositionOps(LockMode.left);
  writeln();
}
test2();

proc test3() {
  writeln('T3');
  testSetCompositionOps(LockMode.right);
  writeln();
}
test3();

proc test4() {
  writeln('T4');
  testSetCompositionOps(LockMode.both);
  writeln();
}
test4();

