proc pushPop(base: int, iters: int) {
  var A: [base..base-1] int;
  for i in 1..iters {
    A.push_back(i);
  }
  writeln(A[A.domain.high]);
  for i in 1..iters {
    A.pop_front();
  }
}

for i in -10..10 by 2 {
  pushPop(i, abs(10*i) + 1);
}
