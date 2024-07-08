use BlockDist;

var a = blockDist.createArray(0..10, int);
var b = blockDist.createArray(0..10, int);

for i in b.domain {
  b[i] = i;
}

forall i in a.domain with (ref a) {
  a[i] = b[10-i] + 2;
}

writeln(a);

proc foo(i) {
  return i*2;
}
forall i in a.domain with (ref a) {
  a[i] = foo(10-i);
}
writeln(a);
