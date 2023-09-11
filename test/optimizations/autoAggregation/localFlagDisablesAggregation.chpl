use BlockDist;

var dom = blockDist.createDomain({0..10});

var a: [dom] int;
var b: [dom] int;

for i in b.domain {
  b[i] = i;
}

forall i in a.domain with (ref a) {
  a[i] = b[10-i];
}

writeln(a);
