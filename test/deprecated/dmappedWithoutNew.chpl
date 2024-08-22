use BlockDist;

const d = {1..10, 1..10};

// okay:
var d1 = d dmapped new blockDist(d);

var bd = new blockDist(d);
var d2 = d dmapped bd;

var d3: domain(2) dmapped new blockDist(d);

// deprecated:
var d4 = d dmapped blockDist(d);
