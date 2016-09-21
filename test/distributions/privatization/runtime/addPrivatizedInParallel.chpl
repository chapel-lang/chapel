use PrivatizationWrappers;

config const numIters = 10000;

forall i in 0..#numIters {
  var newValue = new C(i);
  insertPrivatized(newValue, i);
}

for i in 0..#numIters by -1 {
  writeln(getPrivatized(i).i);
}
