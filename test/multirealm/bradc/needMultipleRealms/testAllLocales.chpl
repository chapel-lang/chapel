use BlockDist, Random;

coforall loc in AllLocales do on loc do
  writeln("hello from locale ", loc);

const MyBlock = new dist(new Block(rank=1,idxType=int(32),boundingBox=[1..10],targetLocales=AllLocales));

const D: domain(1) distributed MyBlock = [1..10];

var A: [D] real;

forall a in A do
  a = 1.0;

writeln("A is: ", A);

var randlist = new RandomStream(314159265);
randlist.fillRandom(A);

writeln("A is: ", A);
