use BlockCycDist;

config const n = 25;

var MyBlkCyc = new BlockCyclic(rank=1, idxType=int(32), low=tuple(1), blk=tuple(4));

var D: domain(1) distributed MyBlkCyc = [1..n];

writeln("D is: ", D);
