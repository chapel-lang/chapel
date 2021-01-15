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

var dummy = 0;
forall i in a.domain with (ref dummy) {
  b[10-i] = a[i];
  dummy = b[10-i]; // should thwart the optimization at normalize
}

writeln(b);

forall (aElem, i) in zip(a, 0..10) {
  b[10-i] = aElem;
}

writeln(b);
