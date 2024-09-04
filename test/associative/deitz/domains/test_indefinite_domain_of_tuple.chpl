import Sort;
use printHelp;

type t = (int, int);
var D: domain(t);
D += (1, 2);
D += (3, 4);
writelnSorted(D);
var A: [D] int;
A((1, 2)) = 5;
A((3, 4)) = 6;
for i in Sort.sorted(D) do
  writeln((i, A(i)));
