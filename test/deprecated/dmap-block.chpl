use BlockDist;
var n = 10;
var B = new dmap(new Block({1..n}));
var D = {1..n} dmapped B;
var A: [D] real;
writeln(A);
