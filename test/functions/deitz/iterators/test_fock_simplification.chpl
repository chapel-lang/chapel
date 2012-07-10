//
// This iterator simplifies an iterator seen in
//   users/shetag/fock/fock-3-blc.chpl
//
iter foo() {
  for (i, j) in {1..3, 1..3} {
    for k in 1..3 {
      yield (i, j, k);
    }
  }
  yield (0, 0, 0);
}

for i in foo() {
  writeln(i);
}
