use BlockDist;
use LayoutCS;
use List;

var space = {1..10, 1..10};
var parentDomCSR = space dmapped Block(space, sparseLayoutType=CS(compressRows=true));
var spsDomCSR: sparse subdomain(parentDomCSR);
var parentDomCSC = space dmapped Block(space, sparseLayoutType=CS(compressRows=false));
var spsDomCSC: sparse subdomain(parentDomCSC);

var indices: list(2*int);

for i in {1..10 by 3, 1..10 by 3} do
  indices.pushBack(i);

const arr = indices.toArray();

writeln(arr);
spsDomCSR.bulkAdd(arr);
for i in spsDomCSR do write(i, " ");
writeln();
spsDomCSC.bulkAdd(arr);
for i in spsDomCSC do write(i, " ");
writeln();
