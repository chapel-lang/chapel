use Map;
use utilFunctions;

var m = new map(string, int);

for i in 1..99 by 3 {
  m[intToEnglish(i)] = i;
}

m.clear();
writeln(m);
