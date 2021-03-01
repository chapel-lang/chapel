use Memory.Initialization;
use LowLevelBuffer;
use TrackingRecord;

config const size = 4;

proc test1() {
  writeln('T1');
  var buf = new buffer(int, size);
  for i in 0..<size do moveInitialize(buf[i], i);
  writeln(buf);
}
test1();
writeln();

proc test2() {
  writeln('T2');
  var buf = new buffer(r, size);
  for i in 0..<size do moveInitialize(buf[i], new r());
  writeln(buf);
  for i in 0..<size {
    explicitDeinit(buf[i]);
    moveInitialize(buf[i], new r(i));
  }
  writeln(buf);
  for slot in buf do explicitDeinit(slot);
}
test2();
writeln();

proc test3() {
  writeln('T3');
  var buf = new buffer(r, size);
  for i in 0..<size do moveInitialize(buf[i], new r(i));
  writeln(buf);
  for i in 0..<(size/2) do moveSwap(buf[i], buf[size - i - 1]);
  writeln(buf);
  for slot in buf do explicitDeinit(slot);
}
test3();
writeln();

proc test4() {
  writeln('T4');
  var buf = new buffer(r, size);
  for i in 0..<size do moveInitialize(buf[i], new r(i));
  writeln(buf);
  for i in 0..<size {
    var x = moveToValue(buf[i]);
    writeln(x);
  }
}
test4();

