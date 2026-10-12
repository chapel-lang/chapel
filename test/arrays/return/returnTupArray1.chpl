proc myTup(A) do return (A, A);
proc doit(D): ([] int, [] int) {
  var A: [D] int;
  return myTup(A);
}

writeln(doit({1..10}));
