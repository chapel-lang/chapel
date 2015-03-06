const DBase = {1..10};

var D: [DBase] domain(1);

for i in 1..10 do
  D(i) = {1..i};

writeln("D is: ", D);
