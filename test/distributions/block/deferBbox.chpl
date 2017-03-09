use BlockDist, Time;

const D = {1..10} dmapped Block(rank=1);
writeln("D is: ", D);

var A: [D] real;

forall a in A do
  a = here.id;

writeln(A);
writeln();

const D2 = {1..4, 1..4} dmapped Block(rank=2);
writeln("D2 is: ", D2);

var A2: [D2] real;

forall a in A2 do
  a = here.id;

writeln(A2);
writeln();

var grid: [0..0, 0..#numLocales] locale;
for i in 0..#numLocales do
  grid[0,i] = Locales[i];

const D3 = {1..4, 1..4} dmapped Block(rank=2, targetLocales=grid);
writeln("D3 is: ", D3);

var A3: [D3] real;

forall a in A3 do
  a = here.id;

writeln(A3);
