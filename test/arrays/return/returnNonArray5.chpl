proc doit(D): ([D], []) {
  return (2, 3); // error: can't coerce individual tuple elements
}

writeln(doit({1..10}));
