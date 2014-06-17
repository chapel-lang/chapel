// Test min, max, sum, and product arithmetic reductions

param M = 10;

var D: domain(1) = {1..M};
var I: [D] int;
forall i in D do {
  I(i) = i;
}
writeln( "\nI[D] = ", I);
writeln( "min reduce I[D] = ", min reduce I);
writeln( "max reduce I[D] = ", max reduce I);
writeln( "+ reduce I[D] = ", + reduce I);
writeln( "* reduce I[D] = ", * reduce I);

forall i in D do {
  I(i) = -1 * i;
}
writeln( "\nI[D] = ", I);
writeln( "min reduce I[D] = ", min reduce I);
writeln( "max reduce I[D] = ", max reduce I);
writeln( "+ reduce I[D] = ", + reduce I);
writeln( "* reduce I[D] = ", * reduce I);

var neg: bool = false;
for i in D do {
  neg = !neg;
  if (neg) {
    I(i) = -1 * i;
  } else {
    I(i) = i;
  }
}
writeln( "\nI[D] = ", I);
writeln( "min reduce I[D] = ", min reduce I);
writeln( "max reduce I[D] = ", max reduce I);
writeln( "+ reduce I[D] = ", + reduce I);
writeln( "* reduce I[D] = ", * reduce I);

var UI: [D] int;
forall i in D do {
  UI(i) = i;
}
writeln( "\nUI[D] = ", UI);
writeln( "min UI[D] = ", min reduce UI);
writeln( "max UI[D] = ", max reduce UI);
writeln( "+ reduce UI[D] = ", + reduce UI);
writeln( "* reduce UI[D] = ", * reduce UI);

var F: [D] real;
for i in D do {
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
writeln( "+ reduce F[D] = ", + reduce F);
writeln( "* reduce F[D] = ", * reduce F);
