proc doit(D): ([] int, [] int) {
  return 3; // error: can't return non-tuple
}

var AB: ([1..10] int, [1..10] int) = doit({1..10});
writeln(AB(0));
writeln(AB(1));
