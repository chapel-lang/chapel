use BlockDist;

const d = {1..10, 1..10};

// okay:
var d1 = d dmapped new blockDist(d);

// okay:
var bd = new blockDist(d);
var d2 = d dmapped bd;

// deprecated:
var d3 = d dmapped new blockDist(d);
