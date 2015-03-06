const DBase = {1..10};

var D: [DBase] sparse subdomain(DBase);

for i in 1..10 do
  D(i) += i;

writeln("D is: ", D);
