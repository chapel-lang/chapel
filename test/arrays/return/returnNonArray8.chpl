proc doit(D): [] {
  return 2; // can't coerce to generic array
}

writeln(doit({1..10}));
