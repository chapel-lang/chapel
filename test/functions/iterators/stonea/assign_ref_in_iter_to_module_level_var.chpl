var counter = 0;

iter myIter() {
  ref ref_counter = counter;
  for myJ in 0..9 {
    ref_counter += 1;
    yield ref_counter;
  }
}

var A: [0..100] int = 1;

for i in myIter() do
  A[i] += A[i+1];
writeln(A);
