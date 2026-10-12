proc doit(D): ([] int, [] int) {
  return 3; // error: can't return non-tuple
}

writeln(doit({1..10}));
