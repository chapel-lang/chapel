use driver;

const Dom: domain(2) dmapped Dist2D = {1..5, 1..5};

var A: [Dom] [Dom] int;

forall i in Dom do
  forall j in Dom do
    A(i)(j) = 10000 * ((i(1)-1) * 5 + i(2)) + (j(1)-1)*5 + j(2);

for i in Dom do
  writeln(A(i));
