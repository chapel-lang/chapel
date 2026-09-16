proc myTup(A) do return (A, A);
proc doit(D): (int, ([] int, [] int)) {
  var A: int;
  var tup = myTup(A);
  return (3, tup); // error: not a tuple of arrays
}
writeln(doit({1..10}));
