proc myTup(A) do return (A, A);
proc arrayType(D) type do return ([D] int, [D] int);
proc doit(D): arrayType(D) {
  var A: int;
  return myTup(A); // erorr: not a tuple of arrays
}
writeln(doit({1..10}));
