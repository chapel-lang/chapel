use BlockDist;

const d1 = defaultDist;
writeln(d1 == defaultDist);

const d2 = defaultDist;
writeln(d2 != defaultDist);

const d3 = new dmap(new Block({1..10}));
const d4 = new dmap(new Block({1..10}));

writeln(d3==d4);
writeln(d3!=d4);

var dom = {1..1} dmapped Block({1..10});
writeln(d3==dom.dist);
writeln(d3!=dom.dist);

