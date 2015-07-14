class C {
}

iter type C.myIter() {
  for i in 1..10 do yield i;
}

iter type C.myIterArgs(start, end, stride) {
  for i in start..end by stride do yield i;
}


for i in C.myIter() {
  writeln(i);
}

for i in C.myIterArgs(1, 20, 2) {
  writeln(i);
}
