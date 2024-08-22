use BlockDist;

var Dom = {1..20, 1..20} by 2;
var Space = Dom dmapped new blockDist(Dom);
writeln(Space);
writeln("boundingBox = ", Space.distribution._value.boundingBox);
