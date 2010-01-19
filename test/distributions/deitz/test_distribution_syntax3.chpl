use BlockDist;

var Dom1 = [1..5];
var Dom2 = Dom1 distributed new dist(new Block(1,bbox=[1..5]));
writeln(Dom1);
writeln(Dom2);
