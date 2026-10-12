use BlockDist;

var domBase = {1..10};
var domMap = new blockDist(domBase);

var dom1 = domBase dmapped domMap;
var dom2: domain(1) dmapped domMap;
