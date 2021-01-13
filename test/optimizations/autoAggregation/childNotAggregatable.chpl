use BlockDist;

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);

for i in b.domain {
  b[i] = i;
}

forall i in a.domain {
  a[i] = b[10-i] + 2;
}

writeln(a);

proc foo(i) {
  return i*2;
}
forall i in a.domain {
  a[i] = foo(10-i);
}
writeln(a);
