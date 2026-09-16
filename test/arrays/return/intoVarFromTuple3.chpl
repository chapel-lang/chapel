proc doit(D): ([] int, [] int) {
  return 3; // error: can't return non-tuple
}

var (A, B) = doit({1..10});
writeln(A);
writeln(B);
