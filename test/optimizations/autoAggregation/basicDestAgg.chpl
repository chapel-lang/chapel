use BlockDist;

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);

for i in a.domain {
  a[i] = i;
}

forall i in a.domain {
  b[10-i] = a[i];
}

writeln(b);

forall i in a.domain {
  b[10-i] = a[i];
  a[10-i] = 5; // should thwart the optimization at normalize
}

writeln(b);

forall (aElem, i) in zip(a, 0..10) {
  b[10-i] = aElem;
}

writeln(b);
