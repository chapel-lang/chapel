proc myTup(A) do return (A, A);
proc doit(D): (int, ([] int, [] int)) {
  var A: [D] int;
  var tup = myTup(A);
  return (3, tup);
}
writeln(doit({1..10}));
