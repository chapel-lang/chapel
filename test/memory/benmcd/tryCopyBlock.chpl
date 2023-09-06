use BlockDist;

const dom = Block.createDomain(0..10);

var myArr = dom.tryCreateArray(int);
myArr = 10;

var cp = myArr.tryCopy();

writeln(myArr);
writeln(cp);
