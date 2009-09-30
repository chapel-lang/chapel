// Test logical AND and OR reductions

param M = 10;

var D: domain(1) = [1..M];
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

var toggle: sync bool = false;
forall i in D do {
  var my_toggle = !toggle;
  toggle = my_toggle;
  B(i) = my_toggle;
}
writeln( "\nB[D] = ", B);
writeln( "&& reduce B[D] = ", && reduce B);
writeln( "|| reduce B[D] = ", || reduce B);
