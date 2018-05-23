use PrivatizationWrappers;

config const numIters = 10000;

forall i in 0..#numIters {
  var newValue = new unmanaged C(i);
  insertPrivatized(newValue, i);
}

for i in 0..#numIters by -1 {
  writeln(getPrivatized(i).i);
}

// no leaks
for i in 0..#numIters by -1 {
  var c = getPrivatized(i);
  delete c;
  clearPrivatized(i);
}
