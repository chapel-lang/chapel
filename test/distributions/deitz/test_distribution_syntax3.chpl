use BlockDist;

var Dom1 = {1..5};
var Dom2 = Dom1 dmapped new dmap(new Block(boundingBox={1..5}));
writeln(Dom1);
writeln(Dom2);
