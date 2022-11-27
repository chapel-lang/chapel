use Memory.Initialization;
use TrackingRecord;

// Single array of integers.
proc test1() {
  writeln('T1');
  var d = {0..3};
  var a: [d] int = [0, 0, 3, 4];
  writeln(a);
  moveInitializeArrayElements(a, d.low, 2, 2);
  writeln(a);
}
test1();
writeln();

proc test2() {
  writeln('T2');
  var d = {0..3};
  var a1: [d] int;
  var a2: [d] int = [1, 2, 3, 4];
  writeln(a1);
  writeln(a2);
  moveInitializeArrayElements(a1, d.low, a2, d.low, a1.size); 
  writeln(a1);
  writeln(a2);
}
test2();
writeln();

proc test3() {
  writeln('T3');
  var d = {0..3};

  var a: [d] r = [i in d] new r(i);
  writeln(a);

  const half = d.size / 2;

  // Clean up the slots we'll be replacing.
  for i in d.low..<half do explicitDeinit(a[i]);

  // Move high slots into low slots.
  moveInitializeArrayElements(a, d.low, half, half);

  // Replace high slots that were moved.
  for i in half..d.high do moveInitialize(a[i], new r());

  writeln(a);
}
test3();
writeln();

proc test4() {
  writeln('T4');
  var d = {0..3};

  // TODO: Use noinit here.
  var a1: [d] r;
  writeln(a1);
  for i in d do explicitDeinit(a1[i]);

  var a2: [d] r = [new r(1), new r(2), new r(3), new r(4)];
  writeln(a2);

  // This consumes the elements in a2.
  moveInitializeArrayElements(a1, d.low, a2, d.low, a1.size);

  writeln(a1);

  // Have to do this before a2 is destroyed.
  for i in d do moveInitialize(a2[i], new r());
  writeln(a2);
}
test4();

