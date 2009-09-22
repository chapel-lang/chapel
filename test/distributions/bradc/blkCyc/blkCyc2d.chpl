use BlockCycDist;

config const m = 25, n = 35;

var MyBlkCyc = new BlockCyclic(rank=2, idxType=int(32), low=(1,0), blk=(4, 8));

var D: domain(2) distributed MyBlkCyc = [1..m, 0..n];

writeln("D is: ", D);
