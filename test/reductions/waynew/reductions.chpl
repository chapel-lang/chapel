// Test min, max, sum, and product arithmetic reductions

param M = 10;

var D: domain(1) = [1..M];
var I: [D] int;
forall i in D do {
  I(i) = i;
}
writeln( "\nI[D] = ", I);
writeln( "min I[D] = ", min reduce I);
writeln( "max I[D] = ", max reduce I);
writeln( "sum I[D] = ", sum reduce I);
writeln( "product I[D] = ", product reduce I);

forall i in D do {
  I(i) = -1 * i;
}
writeln( "\nI[D] = ", I);
writeln( "min I[D] = ", min reduce I);
writeln( "max I[D] = ", max reduce I);
writeln( "sum I[D] = ", sum reduce I);
writeln( "product I[D] = ", product reduce I);

var neg: bool = false;
forall i in D do {
  neg = !neg;
  if (neg) {
    I(i) = -1 * i;
  } else {
    I(i) = i;
  }
}
writeln( "\nI[D] = ", I);
writeln( "min I[D] = ", min reduce I);
writeln( "max I[D] = ", max reduce I);
writeln( "sum I[D] = ", sum reduce I);
writeln( "product I[D] = ", product reduce I);

var UI: [D] int;
forall i in D do {
  UI(i) = i;
}
writeln( "\nUI[D] = ", UI);
writeln( "min UI[D] = ", min reduce UI);
writeln( "max UI[D] = ", max reduce UI);
writeln( "sum I[D] = ", sum reduce UI);
writeln( "product I[D] = ", product reduce UI);

var F: [D] float;
forall i in D do {
  neg = !neg;
  if (neg) {
    F(i) = -1.0 * i;
  } else {
    F(i) = 1.0 * i;
  }
}
writeln( "\nF[D] = ", F);
writeln( "min F[D] = ", min reduce F);
writeln( "max F[D] = ", max reduce F);
writeln( "sum I[D] = ", sum reduce F);
writeln( "product I[D] = ", product reduce F);
