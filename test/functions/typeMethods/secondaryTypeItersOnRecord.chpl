record R {
}

iter type R.myIter() {
  for i in 1..10 do yield i;
}

iter type R.myIterArgs(start, end, stride) {
  for i in start..end by stride do yield i;
}


for i in R.myIter() {
  writeln(i);
}

for i in R.myIterArgs(1, 20, 2) {
  writeln(i);
}
