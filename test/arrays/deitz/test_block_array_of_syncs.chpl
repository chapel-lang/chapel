use BlockDist;

const D = {1..4} dmapped new blockDist({1..4});

var S: [D] sync int = -1;

for i in D do
  writeln((S(i).readXX(), S(i).isFull));
