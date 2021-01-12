use BlockDist;

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);

for i in b.domain {
  b[i] = i;
}

forall i in a.domain {
  a[i] = b[10-i];
}

writeln(a);

forall i in a.domain {
  a[i] = b[10-i];
  b[10-i] = 5; // should thwart the optimization
}

writeln(a);
writeln(b);
