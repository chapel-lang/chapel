var lasti = 2;
var logN = 4;

for i in lasti+2..logN-1 {
  writeln("i is: ", i);
}

for j in lasti+2..logN-1 by 2 {
  writeln("j is: ", j);
}

var s = lasti+2..logN-1;

writeln("s is: ", s);

var t = lasti+2..logN-1 by 2;

writeln("t is: ", t);

