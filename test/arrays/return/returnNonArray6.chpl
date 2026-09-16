proc doit(D): ([D], [D] real) {
  return (2, 3); // error: can't coerce individual tuple elements
}

writeln(doit({1..10}));
