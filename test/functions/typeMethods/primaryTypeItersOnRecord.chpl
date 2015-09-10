record R {
  iter type myIter() {
    for i in 1..10 do yield i;
  }

  iter type myIterArgs(start, end, stride) {
    for i in start..end by stride do yield i;
  }
}

for i in R.myIter() {
  writeln(i);
}

for i in R.myIterArgs(1, 20, 2) {
  writeln(i);
}
