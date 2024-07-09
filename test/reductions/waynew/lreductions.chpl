// Test logical AND and OR reductions

param M = 10;

var D: domain(1) = {1..M};
var B: [D] bool;
forall i in D with (ref B) do {
  B(i) = true;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);

forall i in D with (ref B) do {
  B(i) = false;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);

forall i in D with (ref B) do {
  var toggle: bool = i % 2 == 1;
  B(i) = toggle;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);
