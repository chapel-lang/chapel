proc myTup(A) do return (A, A);
proc doit(D): ([] int, [] int) {
  var A: int;
  return myTup(A); // error: not a tuple of arrays
}

writeln(doit({1..10}));
