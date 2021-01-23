writeln();

use BlockDist;

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);

for i in a.domain {
  a[i] = i;
}

writeln("Loop 1");
forall i in a.domain {
  b[10-i] = a[i];
}
writeln("End Loop 1");

writeln(b);
writeln();

var dummy = 0;
writeln("Loop 2");
forall i in a.domain with (ref dummy) {
  b[10-i] = a[i];
  dummy = b[10-i]; // should thwart the optimization at normalize
}
writeln("End Loop 2");

writeln(b);
writeln();

writeln("Loop 3");
forall (aElem, i) in zip(a, 0..10) {
  b[10-i] = aElem;  // expect two confirmations: fast follower, slow follower
}
writeln("End Loop 3");

writeln(b);
writeln();
