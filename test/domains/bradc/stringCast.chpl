use BlockDist, CyclicDist, ReplicatedDist;

const D = {1..10};
const BD = blockDist.createDomain({1..11});
const CD = cyclicDist.createDomain({1..12});
const RD = {1..13} dmapped new replicatedDist();
var SD: sparse subdomain(D);
for i in 1..10 do
  SD += i;

var Ds = D:string;
var BDs = BD:string;
var CDs = CD:string;
var RDs = RD:string;
var SDs = SD:string;

writeln(Ds);
writeln(BDs);
writeln(CDs);
writeln(RDs);
writeln(SDs);
