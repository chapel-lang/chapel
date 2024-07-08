// test yielding and destroying arrays by value
// cf. yield-by-value-3.chpl

var ARR = [321, 4321];

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
    var (proc2,proc3) = myProc();
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
