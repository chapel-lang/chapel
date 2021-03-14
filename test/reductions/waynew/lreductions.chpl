// Test logical AND and OR reductions

param M = 10;

var D: domain(1) = {1..M};
var B: [D] bool;
forall i in D do {
  B(i) = true;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);

forall i in D do {
  B(i) = false;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);

forall i in D do {
  var toggle: bool = i % 2 == 1;
  B(i) = toggle;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);
