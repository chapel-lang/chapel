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
  var a: [d] r = [new r(), new r(), new r(3), new r(4)];
  writeln(a);
  for i in d.low..<2 do explicitDeinit(a[i]);
  moveInitializeArrayElements(a, d.low, 2, 2);
  writeln(a);
}
test3();
writeln();

proc test4() {
  writeln('T4');
  var d = {0..3};
  var a1: [d] r;
  var a2: [d] r = [new r(1), new r(2), new r(3), new r(4)];
  writeln(a1);
  writeln(a2);
  for i in d do explicitDeinit(a1[i]);
  moveInitializeArrayElements(a1, d.low, a2, d.low, a1.size);
  writeln(a1);
  writeln(a2);
}
test4();

