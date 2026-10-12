proc doit(D): ([D] real, [] real) {
  var A, B: [D] int;
  return (A, B); // error: can't coerce individual tuple elements
}

writeln(doit({1..10}));
