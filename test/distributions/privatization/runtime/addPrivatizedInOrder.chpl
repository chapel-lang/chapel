use PrivatizationWrappers;

config const numIters = 10000;

for i in 0..#numIters {
  var newValue = new unmanaged C(i);
  insertPrivatized(newValue, i);
}

for i in 0..#numIters by -1 {
  writeln(getPrivatized(i).i);
}

for i in 0..#numIters {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}

for i in 0..#numIters by -1 {
  assert(getPrivatized(i) == nil);
}
