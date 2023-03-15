// test yielding and destroying records by value
// This is a copy of yield-by-value-2.chpl with `ARR` changed to a record.

var cnt: atomic int = 1;
proc nextCnt() do return cnt.fetchAdd(1);

record MyRecord {
  const id = nextCnt();
  proc init()       { this.complete(); writeln(id, " init"); }
  proc init=(other) { this.complete(); writeln(id, " init= from ", other.id); }
  proc deinit()     { writeln(id, " deinit"); }
}

var ARR = new MyRecord();

proc myProc() {
  writeln(11444);
  return (ARR, 440);
}
iter myIter() {
  writeln(11555);
  yield (ARR, 550);
}
iter myIter(param tag) {  // standalone
  writeln(11666);
  coforall 1..numLocales {
    writeln(11777);
    yield (ARR, 770);
  }
}

proc main {
  writeln(11110000);
  {
    var proc1 = myProc();
    writeln(proc1);
  }
  writeln(22220000);
  {
    var (proc2,proc3) = myProc(); // currently this copies ARR one extra time
    writeln(proc2);
  }
  writeln(33330000);
  {
    for iter1 in myIter() do
      writeln(iter1);
  }
  writeln(44440000);
  {
    for (iter2, iter3) in myIter() do
      writeln(iter2);
  }
  writeln(55550000);
  {
    forall pter1 in myIter() do
      writeln(pter1);
  }
  writeln(66660000);
  {
    forall (pter2, pter3) in myIter() do
      writeln(pter2);
  }
  writeln(77770000);
  writeln(ARR);
}
