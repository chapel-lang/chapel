proc myTup(A) do return (A, A);
proc arrayType(D) type do return ([D] int, [D] int);
proc doit(D, D2): arrayType(D) {
  var A: [D2] int;
  return myTup(A); // error: mismatched domain
}
writeln(doit({1..10}, {11..20}));
