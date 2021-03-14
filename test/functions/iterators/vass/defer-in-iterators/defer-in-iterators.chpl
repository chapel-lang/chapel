// Ensure that appropriate iterator completion actions are performed
// even when breaking out from the loop over the iterator.
//
// To simplify the test, we delegate to other tests the testing of
// defer actions that are lexically within a for-loop
// Also to reduce the complexity, we use the same iterator twice
// for zippered loops.

config const doIdx  = 0;
config const doBreak = false;
config const doReturn = false;

record RRR {
  const jj:int;
  proc init(arg:int) { jj=arg; complete(); writeln("RRR.init { ", jj); }
  proc deinit() { writeln("RRR.fini } ", jj); }
}

iter myIter1() {
  defer { writeln("myIter1 defer 300"); }
  var loc1a = new RRR(300);
  ref r1a = loc1a;
  yield new RRR(33);

  defer { writeln("myIter1 defer 400"); }
  var loc1b = new RRR(400);
  ref r1b = loc1b;
  yield new RRR(44);

  writeln("myIter1 exit");
}

iter myIter2() {
  defer { writeln("myIter2 defer 500"); }
  var loc2a = new RRR(500);
  ref r2a = loc2a;
  yield new RRR(55);

  for r2nz in myIter1() {
    writeln("myIter2 ", r2nz.jj);
    if doIdx == 100 + r2nz.jj  {
      if doBreak then break;
      if doReturn then return;
    }
    defer { writeln("myIter2 defer 600"); }
    var loc2b = new RRR(600);
    ref r2b = loc2b;
    yield new RRR(1000 + r2nz.jj);
  }

  for (k2,l2) in zip(myIter1(),myIter1()) {
    writeln("myIter2 ", k2.jj, ",", l2.jj);
    if doIdx == 200 + k2.jj {
      if doBreak then break;
      if doReturn then return;
    }
    defer { writeln("myIter2 defer 700"); }
    var loc2c = new RRR(700);
    ref r2c = loc2c;
    yield new RRR(2000 + k2.jj);
  }

  writeln("myIter2 exit");
}

proc test1nz {
  writeln();  writeln("test1nz start");

  for r1nz in myIter1() {
    writeln("test1nz ", r1nz.jj);
    if doIdx == r1nz.jj {
      if doBreak then break;
      if doReturn then return;
    }
  }
  writeln("test1nz finish");
}

proc test1zip {
  writeln(); writeln("test1zip start");

  for (p1,q1) in zip(myIter1(),myIter1()) {
    writeln("test1zip ", p1.jj, ",", q1.jj);
    if doIdx == p1.jj {
      if doBreak then break;
      if doReturn then return;
    }
  }
  writeln("test1zip finish");
}

proc test2nz {
  writeln(); writeln("test2nz start");

  for r2nz in myIter2() {
    writeln("test2nz ", r2nz.jj);
    if doIdx == r2nz.jj {
      if doBreak then break;
      if doReturn then return;
    }
  }
  writeln("test2nz finish");
}

proc test2zip {
  writeln(); writeln("test2zip start");

  for (p2,q2) in zip(myIter2(),myIter2()) {
    writeln("test2zip ", p2.jj, ",", q2.jj);
    if doIdx == p2.jj {
      if doBreak then break;
      if doReturn then return;
    }
  }
  writeln("test2zip finish");
}

proc main {
  if doIdx == 0 then
    writeln("not breaking/returning");
  else if doBreak then
    writeln("break for ", doIdx);
  else if doReturn then
    writeln("return for ", doIdx);
  else
    writeln("not breaking");

  test1nz;
  test1zip;
  test2nz;
  test2zip;
}
