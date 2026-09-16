proc doit(D): [D] int {
  return "2"; // can't coerce string -> int
}

writeln(doit({1..10}));
