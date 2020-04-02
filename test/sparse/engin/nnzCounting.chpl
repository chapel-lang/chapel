use LayoutCS;
use BlockDist;

config const N = 8;
const Space = {0..#N, 0..#N};

var distParent = Space dmapped Block(boundingBox=Space);
var nonDistParent = Space;

var defSps: sparse subdomain(nonDistParent);
var csrSps: sparse subdomain(nonDistParent) dmapped CS();
var blkSps: sparse subdomain(distParent);

const inds = [(1,1),(2,2),(3,3),(4,4),(5,5)];
const diag = [i in Space.dim(1)] (i,i);
const col = [i in Space.dim(1)] (i,3);

defSps += diag;
csrSps += diag;
blkSps += diag;

writeln(defSps.size);
writeln(csrSps.size);
writeln(blkSps.size);

defSps += col;
csrSps += col;
blkSps += col;

writeln(defSps.size);
writeln(csrSps.size);
writeln(blkSps.size);

