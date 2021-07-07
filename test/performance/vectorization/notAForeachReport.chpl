iter myIter() {
  for i in 1..10 {
    var j = i*2;
    yield j;
  }
}

foreach i in myIter() {
  writeln(i);
}
