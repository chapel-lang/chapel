// Ensure that the underscore in the yield-by-value scenario
// is properly deinitialized.

// Once this is the case, this test can be removed
// and instead the tests yield-by-value-2.chpl and yield-by-value-3.chpl
// can be extended by adding a sibling to `ARR` that is included in
// the tuple yielded by each iterator and by adding an `_` to each
// detupling expression.

var AR1 = [432, 4321];
var AR2 = [987, 9876];

iter myIter() {
  writeln(11555);
  yield (AR1, AR2, 550);
}
iter myIter(param tag) {  // standalone
  writeln(11666);
  coforall 1..numLocales {
    writeln(11777);
    yield (AR1, AR2, 770);
  }
}

proc main {
  writeln(11110000);
  {
    for (iter1, _, iter3) in myIter() do
      writeln(iter1);
  }
  writeln(55550000);
  {
    forall (pter1, _, pter3) in myIter() do
      writeln(pter1);
  }
  writeln(77770000);
  writeln(AR1, AR2);
}
