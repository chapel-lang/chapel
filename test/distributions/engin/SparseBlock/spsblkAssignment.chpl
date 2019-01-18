use BlockDist;

const space = {1..10};
const par1 = space dmapped Block(space);
const par2 = space dmapped Block(space);

var sps1: sparse subdomain(par1);
var sps2: sparse subdomain(par2);

var arr1: [sps1] int;
var arr2: [sps2] int;

arr1 = arr2;
