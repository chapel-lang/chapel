proc doit(D): ([] int, [] int) {
  var A: [D] int;
  var tup = (A, A);
  return tup;
}

writeln(doit({1..10}));
