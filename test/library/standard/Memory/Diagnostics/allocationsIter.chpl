import MemDiagnostics;
use List;

class C {
  param size: int;
  var x: size*uint(8);
}

class D {
  var size: int;
  var x: [1..size] uint(8);
}

var initAllocs = new list(2*uint);

proc main() {
  test();

  var c27 = new unmanaged C(27); // allocates 32 bytes

  test();

  var d45 = new unmanaged D(45); // allocates 45 bytes

  test();


  assert(c27.x[3]==0); // to keep the object alive
  delete c27;

  test();

  assert(d45.x[3]==0); // to keep the object alive
  delete d45;

  test();
}

proc test() {
  var recordInits = false;
  if initAllocs.isEmpty() then recordInits = true;

  writeln("Allocations");
  for (addr,size) in MemDiagnostics.allocations() {
    if recordInits then
      initAllocs.pushBack((addr,size));
    else if !initAllocs.contains((addr,size)) then
      writef("Address: 0x%xu Size: %i\n", addr, size);
  }
  writeln();
}

