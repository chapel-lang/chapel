use Map;

var m = new map(int, int);

m[0] = 1;

m.getValue(0);

var caughtVal: int;
var sentinelVal: int;

try {
  m.getValue(1);
} catch e {
  caughtVal = 1;
  writeln("caught error");
}

sentinelVal = m.getValue(1, 1);

writeln(caughtVal == sentinelVal);

m.getValue(1);
