use BlockDist;

var dom = newBlockDom({0..10});

var a: [dom] int;
var b: [dom] int;

for i in b.domain {
  b[i] = i;
}

forall i in a.domain {
  a[i] = b[10-i];
}

writeln(a);
