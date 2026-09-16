proc myTup(A) do return (A, A);
proc doit(D): ([] int, [] int) {
  var A: [D] int;
  var tup = myTup(A);
  return tup;
}
writeln(doit({1..10}));
