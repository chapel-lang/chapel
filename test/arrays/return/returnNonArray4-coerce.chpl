proc doit(D): [D] int {
  return 2; // coerce to array
}

writeln(doit({1..10}));
