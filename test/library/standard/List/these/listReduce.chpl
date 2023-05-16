private use List;

config const testIters = 138;

var l: list(int);

for i in 1..testIters do
  l.pushBack(i);

// standalone iterator
var sum1 = + reduce l;
var mm1 = minmax reduce l;

assert(sum1 == testIters*(testIters+1)/2);
assert(mm1(0) == 1);
assert(mm1(1) == testIters);

// leader iterator
var sum2 = + reduce [x in l] x;
var mm2 = minmax reduce [x in l] x;

assert(sum1 == sum2);
assert(mm1 == mm2);


writeln("sum: ", sum1);
writeln("minmax: ", mm1);
