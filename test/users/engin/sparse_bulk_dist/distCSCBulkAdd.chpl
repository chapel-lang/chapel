use BlockDist;
use LayoutCS;

var space = {1..10, 1..10};
var parentDomCSR = space dmapped Block(space, sparseLayoutType=CS(compressRows=true));
var spsDomCSR: sparse subdomain(parentDomCSR);
var parentDomCSC = space dmapped Block(space, sparseLayoutType=CS(compressRows=false));
var spsDomCSC: sparse subdomain(parentDomCSC);

var indices: [{1..0}] 2*int;
for i in {1..10 by 3, 1..10 by 3} do
  indices.push_back(i);

writeln(indices);
spsDomCSR.bulkAdd(indices);
for i in spsDomCSR do write(i, " ");
writeln();
spsDomCSC.bulkAdd(indices);
for i in spsDomCSC do write(i, " ");
writeln();
