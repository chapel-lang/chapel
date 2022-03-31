
var DR1 = {1..999};
var DR2 = {1..5,1..6};
var spsDom1: sparse subdomain(DR1);
var spsDom2: sparse subdomain(DR2);
var idxBuf1 = spsDom1.makeIndexBuffer(1001);
var idxBuf2 = spsDom2.makeIndexBuffer(1002);

spsDom1.bulkAdd([3, 6, 7]);
spsDom2.bulkAdd([(2,2), (3,4), (5,6)]);
compilerError("done");
