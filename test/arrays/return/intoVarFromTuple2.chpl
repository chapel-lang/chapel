proc doit(D): ([] int, [] int) {
  var A: [D] int = 2;
  var B: [D] int = 3;
  return (A, B);
}
var AB: ([1..10] int, [1..10] int) = doit({1..10});
writeln(AB(0));
writeln(AB(1));
