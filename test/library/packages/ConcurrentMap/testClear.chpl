use ConcurrentMap;
use utilFunctions;

var m = new ConcurrentMap(string, int);

var tok = m.getToken();
for i in 1..99 by 3 {
  m.add(intToEnglish(i), i, tok);
}

m.Clear();
writeln(m);
