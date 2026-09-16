proc doit(D): ([] int, [] int) {
  var A: [D] int = 2;
  var B: [D] int = 3;
  return (A, B);
}
var (A, B) = doit({1..10});
writeln(A);
writeln(B);
