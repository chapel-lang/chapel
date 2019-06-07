use Lists;

config type listType = DummyRecord;
config param listLock = true;
config const testIters = 1;

var createdRecords = 1;
var createdClasses = 1;

record DummyRecord {
  var cid = 0;
  pragma "no auto destroy"
  var cpy = new list(int);

  proc init() {
    this.cid = createdRecords;
    this.cpy = new list(int);
    this.complete();
    createdRecords += 1;
  }

  proc init=(const ref other: DummyRecord) {
    this.cid = createdRecords;
    this.cpy = new list(int);
    this.complete();
    this.cpy.append(other.cid);
    createdRecords += 1;
  }

  proc deinit() {
    writeln("Destructor of DummyRecord:");
    cpy.debugPrintInternalState();
    writeln("cid: " + cid + ", cpy: " + cpy:string);
    writeln("Destroying cpy list");
    chpl__autoDestroy(cpy);
  }
}

proc =(ref lhs: DummyRecord, const ref rhs: DummyRecord) {
  lhs.cpy.append(rhs.cid);
}

class DummyClass {
  var cid = 0;

  proc init() {
    this.cid = createdClasses;
    this.complete();
    createdClasses += 1;
  }

  proc deinit() {
    writeln("cid: " + cid);
  }
}

proc testLoop(type t) {
  pragma "no auto destroy"
  var lst1 = new list(t, listLock);

  for i in 1..testIters {
    var x = new t();
    lst1.append(x);
  }

  pragma "no auto destroy"
  var lst2 = lst1;

  writeln(lst1);
  writeln(lst2);

  assert(lst1._blocks != lst2._blocks);
  writeln(">> Destroying list 1:");
  chpl__autoDestroy(lst1);
  writeln(">> Destroying list 2:");
  chpl__autoDestroy(lst2);
}

writeln("Testing record type destructors...");
testLoop(DummyRecord);

// writeln("Testing class type destructors...");
// testLoop(DummyClass);



