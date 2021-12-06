use ConcurrentMap;
use utilFunctions;

var m = new shared ConcurrentMap(string, string);

var tok = m.getToken();
for i in 1..99 by 3 {
  var number = intToEnglish(i);
  m.add(number, i:string + " value", tok);
}

for i in 1..99 {
  var number = intToEnglish(i);
  if m.contains(number) {
    writeln("map contains ", number );
  }
}
