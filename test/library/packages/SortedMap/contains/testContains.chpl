use SortedMap;
use utilFunctions;



var m = new sortedMap(string, string, false, new DefaultComparator());

for i in 1..99 by 3 {
  var number = intToEnglish(i);
  m[number] = i:string + " value";
}

for i in 1..99 {
  var number = intToEnglish(i);
  if m.contains(number) {
    writeln("map contains ", number );
  }
}
