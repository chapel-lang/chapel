writeln();

use BlockDist;

config var flag = true;

var alwaysTrue = true; // to make sure we run the ifs with no elses

var a = newBlockArr(0..10, int);
var b = newBlockArr(0..10, int);

for i in b.domain {
  b[i] = i;
}

writeln("Loop 1 -- expecting source aggregation");
forall i in a.domain {
  if flag {
    a[i] = b[10-i];
  }
  else {
    a[i] = b[10-i];
  }
}
writeln("End Loop 1");

writeln(a);
writeln();

writeln("Loop 2 -- expecting source aggregation");
forall i in a.domain {
  if flag || alwaysTrue {
    a[i] = b[10-i];
  }
}
writeln("End Loop 2");

writeln(a);
writeln();

writeln("Loop 3 -- expecting destination aggregation");
forall i in a.domain {
  if flag {
    b[10-i] = a[i];
  }
  else {
    b[10-i] = a[i];
  }
}
writeln("End Loop 3");

writeln(a);
writeln();

writeln("Loop 4 -- expecting destination aggregation");
forall i in a.domain {
  if flag || alwaysTrue {
    b[10-i] = a[i];
  }
}
writeln("End Loop 4");

writeln(a);
writeln();
