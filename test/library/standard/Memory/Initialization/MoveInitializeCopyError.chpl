use Memory.Initialization;
use TrackingRecord;

proc test1() {
  pragma "no init"
  var r1: r;
  var r2 = new r();
  moveInitialize(r1, r2);
  writeln(r2);
}
test1();

