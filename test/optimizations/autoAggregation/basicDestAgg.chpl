use BlockDist;

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);

for i in b.domain {
  b[i] = i;
}

forall i in a.domain {
  b[10-i] = a[i];
}

writeln(a);

forall i in a.domain {
  b[10-i] = a[i];
  b[10-i] = 5; // should thwart the optimization at normalize
}

writeln(a);
writeln(b);
