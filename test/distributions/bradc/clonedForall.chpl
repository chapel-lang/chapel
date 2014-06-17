use BlockDist;

var B = new dmap(new Block(rank=1, boundingBox={1..numLocales:int(64)}));
var D: domain(1, int(64)) dmapped B = {1..numLocales:int(64)};
var x: int;

forall i in D do
  x %= 1;
