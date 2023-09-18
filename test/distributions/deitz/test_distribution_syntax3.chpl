use BlockDist;

var Dom1 = {1..5};
var Dom2 = Dom1 dmapped new blockDist(boundingBox={1..5});
writeln(Dom1);
writeln(Dom2);
