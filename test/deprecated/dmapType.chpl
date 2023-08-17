use BlockDist, CyclicDist;

const Dist: dmap(Block(1)) = new Block({1..10});
const Dist2: dmap(Cyclic(1)) = new Cyclic(1);

writeln(Dist.type:string);
writeln(Dist2.type:string);
