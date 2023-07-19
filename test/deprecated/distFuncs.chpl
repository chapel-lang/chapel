use BlockDist;
use CyclicDist;
var rng = (1..10, 1..10);
var D = {(...rng)};

// Old way:
var BD1 = newBlockDom(D);
var BA1 = newBlockArr(D, int);
var BD2 = newBlockDom((...rng));
var BA2 = newBlockArr((...rng), int);

var CD1 = newCyclicDom(D);
var CA1 = newCyclicArr(D, int);
var CD2 = newCyclicDom((...rng));
var CA2 = newCyclicArr((...rng), int);

// New way:
var newBD1 = Block.createDomain(D);
var newBA1 = Block.createArray(D, int);
var newBD2 = Block.createDomain((...rng));
var newBA2 = Block.createArray((...rng), int);
var newCD1 = Cyclic.createDomain(D);
var newCA1 = Cyclic.createArray(D, int);
var newCD2 = Cyclic.createDomain((...rng));
var newCA2 = Cyclic.createArray((...rng), int);
