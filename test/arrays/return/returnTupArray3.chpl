proc myTup(A) do return (A, A);
proc arrayType(D) type do return ([D] int, [D] int);
proc doit(D): arrayType(D) {
  var A: [D] int;
  return myTup(A);
}
writeln(doit({1..10}));
